// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "SpineSkeletonModel.h"
#include "Resource/Model/Mesh/MeshFactory.h"
#include "Resource/Model/Mesh/Fixed/TextureQuadMesh.h"
#include "Resource/Material/MaterialFactory.h"
#include "Resource/Effect/EffectFactory.h"
#include "Resource/ResourceNames.h"
#include "Resource/Texture/TextureFactory.h"
#include "Core/IO/FileSystem.h"
#include "Resource/TextureAtlas/TextureAtlas.h"
#include "Resource/TextureAtlas/TextureAtlasPage.h"
#include "Resource/TextureAtlas/TextureAtlasFactory.h"

#include "SkeletonBoneModel.h"
#include "SkeletonSlotModel.h"
#include "SkeletonAvatarModel.h"
#include "SkeletonIKModel.h"

#include "SkeletonTriggerModel.h"
#include "Attachment/ISkeletonAttachmentModel.h"
#include "Attachment/SkeletonBoundingBoxAttachmentModel.h"
#include "Attachment/SkeletonMeshAttachmentModel.h"
#include "Attachment/SkeletonRegionAttachmentModel.h"
#include "Attachment/SkeletonSkinnedMeshAttachmentModel.h"

#include "SkeletonAnimationModel.h"
#include "Resource/Timeline/ColorTimelineModel.h"
#include "Resource/Timeline/TextureFileIdTimelineModel.h"

#include "Resource/Timeline/ScaleTimelineModel.h"
#include "Resource/Timeline/RotationTimelineModel.h"
#include "Resource/Timeline/TranslateTimelineModel.h"
#include "Resource/Timeline/VertexTimelineModel.h"
#include "Resource/Timeline/DrawOrderTimelineModel.h"
#include "Resource/Timeline/TriggerTimelineModel.h"
#include "Resource/Timeline/IKTimelineModel.h"
#include "Resource/Timeline/StringTimelineModel.h"
#include "CoreLib/Common/rapidjson.h"

#include "Core/Math/Math.h"
#include "Core/System/BitConverter.h"

MEDUSA_BEGIN;


SpineSkeletonModel::SpineSkeletonModel(const FileIdRef& fileId)
	:ISkeletonModel(fileId)
{

}


SpineSkeletonModel::~SpineSkeletonModel(void)
{
}


bool SpineSkeletonModel::Initialize()
{
	RETURN_FALSE_IF_FALSE(ISkeletonModel::Initialize());

	return true;
}

SpineSkeletonModel* SpineSkeletonModel::CreateFromJsonFile(const FileIdRef& skeletonfileId, const FileIdRef& atlasFileId)
{
	MemoryData skeletonFileData = FileSystem::Instance().ReadAllData(skeletonfileId);
	if (skeletonFileData.IsEmpty())
	{
		return nullptr;
	}

	auto atlas = TextureAtlasFactory::Instance().Create(atlasFileId, TextureAtlasType::Spine);
	return CreateFromJsonData(skeletonfileId, skeletonFileData, atlas);
}

SpineSkeletonModel* SpineSkeletonModel::CreateFromJsonData(const FileIdRef& fileId, const MemoryData& skeletonFileData, const Share<TextureAtlas>& atlas)
{
	rapidjson::Document root;

	const char* beginDoc = (const char*)skeletonFileData.Data();
	//const char* endDoc = beginDoc + skeletonFileData.Size();
	root.Parse<rapidjson::kParseStopWhenDoneFlag>(beginDoc);
	if (root.HasParseError())
	{
		rapidjson::ParseErrorCode errorCode = root.GetParseError();
		Log::AssertFailedFormat("Invalid json format:{}. ErrorCode:{}", fileId.Name.c_str(), errorCode);
		return nullptr;
	}

	std::unique_ptr<SpineSkeletonModel> model(new SpineSkeletonModel(fileId));
	const rapidjson::Value& skeletonItem = root["skeleton"];
	float width = skeletonItem.GetMember("width", 0.f);
	float height = skeletonItem.GetMember("height", 0.f);

	model->SetBoundingSize(msize(width, height));

	//bones
	const rapidjson::Value& bonesArray = root["bones"];
	for (const auto& bone : bonesArray)
	{
		const char* name = bone.GetMember("name", nullptr);
		const char* parentBoneName = bone.GetMember("parent", nullptr);

		SkeletonBoneModel* boneModel = new SkeletonBoneModel(name, parentBoneName);

		boneModel->SetLength(bone.GetMember("length", 0));

		Point3F pos = Point3F::Zero;
		pos.X = bone.GetMember("x", 0.f);
		pos.Y = bone.GetMember("y", 0.f);
		boneModel->SetPosition(pos);

		Rotation3F rotation = Rotation3F::Zero;
		rotation.Z = Math::WrapToPI(Math::ToRadian(bone.GetMember("rotation", 0.f)));
		boneModel->SetRotation(rotation);


		Scale3F scale = Scale3F::One;
		scale.X = bone.GetMember("scaleX", 1.f);
		scale.Y = bone.GetMember("scaleY", 1.f);
		boneModel->SetScale(scale);

		boneModel->EnableInheritScale(bone.GetMember("inheritScale", true));
		boneModel->EnableInheritRotation(bone.GetMember("inheritRotation", true));

		const char* colorStr = bone.GetMember("color", nullptr);
		Color4F color = Color4F::White;
		if (colorStr != nullptr)
		{
			uint val = StringParser::StringTo<uint32>(colorStr, 16);
			val = BitConverter::ToLittle(val);//0x00ff04ff means rgba in spine,so we have to swap it
			color = Color4F(val);
		}
		boneModel->SetColor(color);

		model->AddBone(boneModel);
	}

	//ik
	const rapidjson::Value* ikArray = root.GetMemberValue("ik");
	if (ikArray != nullptr)
	{
		for (const auto& ik : *ikArray)
		{
			const char* name = ik.GetMember("name", nullptr);
			SkeletonIKModel* ikModel = new SkeletonIKModel(name);

			const rapidjson::Value& boneArray = ik["bones"];
			for (const auto& j : boneArray)
			{
				StringRef boneName = j.GetString();
				SkeletonBoneModel* boneModel = model->FindBone(boneName);
				//add bone to ik
				ikModel->AddBone(boneModel);
			}

			const char* targetName = ik.GetMember("target", nullptr);
			SkeletonBoneModel* targetBoneModel = model->FindBone(targetName);
			ikModel->SetTargetBone(targetBoneModel);
			ikModel->EnableBendPositive(ik.GetMember("bendPositive", 1) == 1);
			ikModel->SetMix(ik.GetMember("mix", 1.f));

			model->AddIK(ikModel);
		}

	}


	const rapidjson::Value& slotArray = root["slots"];
	for (const auto& slot : slotArray)
	{
		const char* name = slot.GetMember("name", nullptr);
		const char* boneName = slot.GetMember("bone", nullptr);
		SkeletonBoneModel* bone = model->FindBone(boneName);
		if (bone == nullptr)
		{
			Log::AssertFailedFormat("Cannot find slot:{} 's bone:{}", name, boneName);
			return nullptr;
		}

		SkeletonSlotModel* slotModel = new SkeletonSlotModel(name);
		bone->AddSlot(slotModel);
		model->AddSlot(slotModel);

		const char* attachmentName = slot.GetMember("attachment", nullptr);
		if (attachmentName != nullptr)
		{
			slotModel->SetAttachmentName(attachmentName);
		}

		const char* colorStr = slot.GetMember("color", nullptr);
		Color4F color = Color4F::White;
		if (colorStr != nullptr)
		{
			uint val = StringParser::StringTo<uint32>(colorStr, 16);
			val = BitConverter::ToLittle(val);//0x00ff04ff means rgba in spine,so we have to swap it
			color = Color4F(val);
		}
		slotModel->SetColor(color);

		bool isAdditiveBlending = slot.GetMember("additive", false);
		slotModel->EnableAdditiveBlending(isAdditiveBlending);

	}


	const rapidjson::Value* skinArray = root.GetMemberValue("skins");
	if (skinArray != nullptr)
	{
		FOR_EACH_JSON(i, *skinArray)
		{
			const rapidjson::Value& skin = i->value;

			SkeletonAvatarModel* avatarModel = new SkeletonAvatarModel(i->name.GetString());
			model->AddAvatar(avatarModel, avatarModel->Name() == "default");
			FOR_EACH_JSON(j, skin)
			{
				const rapidjson::Value& slot = j->value;

				StringRef slotName = j->name.GetString();
				SkeletonSlotModel* slotModel = model->FindSlot(slotName);
				FOR_EACH_JSON(k, slot)
				{
					const rapidjson::Value& attachemnt = k->value;
					const char* attachemntName = k->name.GetString();
					const char* regionName = attachemnt.GetMember("name", attachemntName);
					const char* type = attachemnt.GetMember("type", "region");
					StringRef typeString = type;
					SkeletonAttachmentType outAttachmentType = SkeletonAttachmentType::Region;

					if (!ParseAttachmentType(outAttachmentType, typeString))
					{
						Log::AssertFailedFormat("Unknown attachment type:{}", typeString.c_str());
						return nullptr;
					}

					TextureAtlasRegion* region = nullptr;
					switch (outAttachmentType)
					{
					case SkeletonAttachmentType::Region:
					case SkeletonAttachmentType::Mesh:
					case SkeletonAttachmentType::SkinnedMesh:
					{
						HeapString regionNameCopy(regionName);
						regionNameCopy += FileExtensions::png;
						region = atlas->FindRegion(regionNameCopy);
						if (region == nullptr)
						{
							Log::AssertFailedFormat("Cannot find region:{}", regionNameCopy);
							return nullptr;
						}

						break;
					}
					case SkeletonAttachmentType::BoundingBox:
					default:
						break;
					}


					switch (outAttachmentType)
					{
					case SkeletonAttachmentType::Region:
					{
						SkeletonRegionAttachmentModel* attachemntModel = new SkeletonRegionAttachmentModel(attachemntName, region);
						Point3F pos = Point3F::Zero;
						pos.X = attachemnt.GetMember("x", 0.f);
						pos.Y = attachemnt.GetMember("y", 0.f);
						attachemntModel->SetPosition(pos);

						Rotation3F rotation = Rotation3F::Zero;
						rotation.Z = Math::WrapToPI(Math::ToRadian(attachemnt.GetMember("rotation", 0.f)));
						attachemntModel->SetRotation(rotation);


						Scale3F scale = Scale3F::One;
						scale.X = attachemnt.GetMember("scaleX", 1.f);
						scale.Y = attachemnt.GetMember("scaleY", 1.f);
						attachemntModel->SetScale(scale);

						Size3F size = Size3F::Zero;
						size.Width = attachemnt.GetMember("width", 32.f);
						size.Height = attachemnt.GetMember("height", 32.f);
						attachemntModel->SetSize(size);

						const char* colorStr = attachemnt.GetMember("color", nullptr);
						Color4F color = Color4F::White;
						if (colorStr != nullptr)
						{
							uint val = StringParser::StringTo<uint32>(colorStr, 16);
							val = BitConverter::ToLittle(val);//0x00ff04ff means rgba in spine,so we have to swap it
							color = Color4F(val);
						}
						attachemntModel->SetColor(color);

						attachemntModel->Initialize();
						avatarModel->AddSlotAttachment(slotModel, attachemntModel);
						break;
					}
					case SkeletonAttachmentType::Mesh:
					{
						SkeletonMeshAttachmentModel* attachemntModel = new SkeletonMeshAttachmentModel(attachemntName, region);
						rapidjson::Value::ConstMemberIterator itr = attachemnt.FindMember("vertices");
						if (itr != attachemnt.MemberEnd())
						{
							const rapidjson::Value& vertices = itr->value;
							uint count = vertices.Size();
							for (uint m = 0; m < count; m += 2)
							{
								Point3F& pos = attachemntModel->MutableVertices().NewAdd();
								pos.X = vertices[m].GetFloat();
								pos.Y = vertices[m + 1].GetFloat();
								pos.Z = 0.f;
							}
						}

						itr = attachemnt.FindMember("uvs");
						if (itr != attachemnt.MemberEnd())
						{
							const rapidjson::Value& texcoords = itr->value;

							size_t count = texcoords.Size();
							for (uint m = 0; m < count; m += 2)
							{
								Point2F& texcoord = attachemntModel->MutableTexcoords().NewAdd();
								texcoord.X = texcoords[m].GetFloat();
								texcoord.Y = texcoords[m + 1].GetFloat();
							}
						}

						itr = attachemnt.FindMember("triangles");
						if (itr != attachemnt.MemberEnd())
						{
							const rapidjson::Value& triangles = itr->value;
							for (const auto& m : triangles)
							{
								attachemntModel->AddIndex(m.GetUint());
							}
						}

						const char* colorStr = attachemnt.GetMember("color", nullptr);
						Color4F color = Color4F::White;
						if (colorStr != nullptr)
						{
							uint val = StringParser::StringTo<uint32>(colorStr, 16);
							val = BitConverter::ToLittle(val);//0x00ff04ff means rgba in spine,so we have to swap it
							color = Color4F(val);
						}
						attachemntModel->SetColor(color);

						Size3F size = Size3F::Zero;
						size.Width = attachemnt.GetMember("width", 32.f);
						size.Height = attachemnt.GetMember("height", 32.f);
						attachemntModel->SetSize(size);


						attachemntModel->Initialize();
						avatarModel->AddSlotAttachment(slotModel, attachemntModel);

						break;

					}
					case SkeletonAttachmentType::SkinnedMesh:
					{
						SkeletonSkinnedMeshAttachmentModel* attachemntModel = new SkeletonSkinnedMeshAttachmentModel(attachemntName, region);

						//format: {boneCount:{boneIndex,x,y,weight}}

						rapidjson::Value::ConstMemberIterator itr = attachemnt.FindMember("vertices");
						if (itr != attachemnt.MemberEnd())
						{
							const rapidjson::Value& vertices = itr->value;
							uint count = vertices.Size();
							for (uint m = 0; m < count;)
							{
								uint boneCountOfThisVertex = vertices[m++].GetUint();

								FOR_EACH_SIZE(n, boneCountOfThisVertex)
								{
									VertexWeightInfo& weightInfo = attachemntModel->MutableWeights().NewAdd();
									weightInfo.BoneIndex = vertices[m++].GetUint();
									weightInfo.X = vertices[m++].GetFloat();
									weightInfo.Y = vertices[m++].GetFloat();
									weightInfo.Weight = vertices[m++].GetFloat();
								}
							}

						}


						itr = attachemnt.FindMember("uvs");
						if (itr != attachemnt.MemberEnd())
						{
							const rapidjson::Value& texcoords = itr->value;
							size_t count = texcoords.Size();
							for (uint m = 0; m < count; m += 2)
							{
								Point2F& texcoord = attachemntModel->MutableTexcoords().NewAdd();
								texcoord.X = texcoords[m].GetFloat();
								texcoord.Y = texcoords[m + 1].GetFloat();
							}
						}

						itr = attachemnt.FindMember("triangles");
						if (itr != attachemnt.MemberEnd())
						{
							const rapidjson::Value& triangles = itr->value;
							for (const auto& m : triangles)
							{
								attachemntModel->AddIndex(m.GetUint());
							}
						}

						const char* colorStr = attachemnt.GetMember("color", nullptr);
						Color4F color = Color4F::White;
						if (colorStr != nullptr)
						{
							uint val = StringParser::StringTo<uint32>(colorStr, 16);
							val = BitConverter::ToLittle(val);//0x00ff04ff means rgba in spine,so we have to swap it
							color = Color4F(val);
						}
						attachemntModel->SetColor(color);

						Size3F size = Size3F::Zero;
						size.Width = attachemnt.GetMember("width", 32.f);
						size.Height = attachemnt.GetMember("height", 32.f);
						attachemntModel->SetSize(size);

						attachemntModel->Initialize();
						avatarModel->AddSlotAttachment(slotModel, attachemntModel);
					}
					break;
					case SkeletonAttachmentType::BoundingBox:
					{
						SkeletonBoundingBoxAttachmentModel* attachemntModel = new SkeletonBoundingBoxAttachmentModel(attachemntName);

						rapidjson::Value::ConstMemberIterator itr = attachemnt.FindMember("vertices");
						if (itr != attachemnt.MemberEnd())
						{
							const rapidjson::Value& vertices = itr->value;
							uint count = vertices.Size();
							for (uint m = 0; m < count; m += 2)
							{
								Point2F& pos = attachemntModel->MutablePolygon().NewVertex();
								pos.X = vertices[m].GetFloat();
								pos.Y = vertices[m + 1].GetFloat();
							}

						}

						attachemntModel->Initialize();
						avatarModel->AddSlotAttachment(slotModel, attachemntModel);

					}
					break;
					default:
						break;
					}
				}

			}

		}

	}


	const rapidjson::Value* eventArray = root.GetMemberValue("events");
	if (eventArray != nullptr)
	{
		FOR_EACH_JSON(i, *eventArray)
		{
			const rapidjson::Value& eventData = i->value;
			SkeletonTriggerModel* triggerModel = new SkeletonTriggerModel(i->name.GetString());
			TriggerEventArg& eventArg = triggerModel->MutableEventArg();
			eventArg.SetInt(eventData.GetMember("int", 0));
			eventArg.SetFloat(eventData.GetMember("float", 0.f));
			const char* stringJson = eventData.GetMember("string", nullptr);
			eventArg.SetString(stringJson);
			model->AddTrigger(triggerModel);
		}

	}

	const rapidjson::Value* animationArray = root.GetMemberValue("animations");
	if (animationArray != nullptr)
	{
		FOR_EACH_JSON(i, *animationArray)
		{
			SkeletonAnimationModel* animationModel = new SkeletonAnimationModel(i->name.GetString());
			model->AddAnimation(animationModel);

			//slot
			const rapidjson::Value* slotArray2 = i->value.GetMemberValue("slots");
			if (slotArray2 != nullptr)
			{
				FOR_EACH_JSON(j, *slotArray2)
				{
					const rapidjson::Value& slot = j->value;
					StringRef slotName = j->name.GetString();
					SkeletonSlotModel* slotModel = model->FindSlot(slotName);

					FOR_EACH_JSON(k, slot)
					{
						const rapidjson::Value& timeline = k->value;
						StringRef timelineType = k->name.GetString();
						if (timelineType == "color")
						{
							Share<ColorTimelineModel> timelineModel = new ColorTimelineModel(FileIdRef::Empty);

							for (const auto& frame : timeline)
							{
								float time = frame.GetMember("time", 0.f);

								const char* colorStr = frame.GetMember("color", nullptr);
								Color4F color = Color4F::White;
								if (colorStr != nullptr)
								{
									uint val = StringParser::StringTo<uint32>(colorStr, 16);
									val = BitConverter::ToLittle(val);//0x00ff04ff means rgba in spine,so we have to swap it
									color = Color4F(val);
								}

								timelineModel->AddColor(time, color);
							}
							timelineModel->Initialize();
							animationModel->AddColorTimeline(slotModel, timelineModel);
						}
						else if (timelineType == "attachment")
						{
							Share<StringTimelineModel> timelineModel = new StringTimelineModel(FileIdRef::Empty);

							for (const auto& frame : timeline)
							{
								float time = frame.GetMember("time", 0.f);
								//if (timelineModel->FrameCount()==0&&!Math::IsZero(time))
								//{
								//	//add first string
								//	//timelineModel->AddString(0.f, slotModel->AttachmentName());
								//}

								const char* name = frame.GetMember("name", nullptr);
								StringRef textureName = name;
								timelineModel->AddString(time, textureName);
							}
							timelineModel->Initialize();
							animationModel->AddTextureTimeline(slotModel, timelineModel);

						}
						else
						{
							Log::AssertFailedFormat("Invalid timeline type:{} for slot:{}", timelineType.c_str(), slotName.c_str());
							return nullptr;
						}

					}

				}
			}

			//bones
			const rapidjson::Value* boneArray = i->value.GetMemberValue("bones");
			if (boneArray != nullptr)
			{
				FOR_EACH_JSON(j, *boneArray)
				{
					const rapidjson::Value& bone = j->value;
					StringRef boneName = j->name.GetString();
					SkeletonBoneModel* boneModel = model->FindBone(boneName);

					FOR_EACH_JSON(k, bone)
					{
						const rapidjson::Value& timeline = k->value;
						StringRef timelineType = k->name.GetString();
						if (timelineType == "rotate")
						{
							Share<RotationTimelineModel> timelineModel = new RotationTimelineModel(FileIdRef::Empty);
							for (const auto& frame : timeline)
							{
								float time = frame.GetMember("time", 0.f);
								float angle = frame.GetMember("angle", 0.f);
								Math::TweenType outTweenType;
								List<float> outTweenArgs;
								ParseCurve(frame, outTweenType, outTweenArgs);
								float radian = Math::ToRadian(angle);
								float adjustRadian = Math::WrapToPI(radian);
								timelineModel->AddRotation(time, Rotation3F(0.f, 0.f, adjustRadian), outTweenType, outTweenArgs);
							}
							timelineModel->Initialize();
							animationModel->AddRotationTimeline(boneModel, timelineModel);

						}
						else if (timelineType == "scale")
						{
							Share<ScaleTimelineModel> timelineModel = new ScaleTimelineModel(FileIdRef::Empty);
							for (const auto& frame : timeline)
							{
								float time = frame.GetMember("time", 0.f);
								Scale3F scale = Scale3F::One;
								scale.X = frame.GetMember("x", 0.f);
								scale.Y = frame.GetMember("y", 0.f);

								Math::TweenType outTweenType;
								List<float> outTweenArgs;
								ParseCurve(frame, outTweenType, outTweenArgs);
								timelineModel->AddScale(time, scale, outTweenType, outTweenArgs);
							}

							timelineModel->Initialize();
							animationModel->AddScaleTimeline(boneModel, timelineModel);
						}
						else if (timelineType == "translate")
						{
							Share<TranslateTimelineModel> timelineModel = new TranslateTimelineModel(FileIdRef::Empty);
							for (const auto& frame : timeline)
							{
								float time = frame.GetMember("time", 0.f);
								Point3F pos = Point3F::Zero;
								pos.X = frame.GetMember("x", 0.f);
								pos.Y = frame.GetMember("y", 0.f);

								Math::TweenType outTweenType;
								List<float> outTweenArgs;
								ParseCurve(frame, outTweenType, outTweenArgs);
								timelineModel->AddTranslate(time, pos, outTweenType, outTweenArgs);
							}

							timelineModel->Initialize();
							animationModel->AddTranslateTimeline(boneModel, timelineModel);
						}
						else
						{
							Log::AssertFailedFormat("Invalid timeline type:{} for bone:{}", timelineType.c_str(), boneName.c_str());
							return nullptr;
						}
					}

				}
			}

			//ik
			const rapidjson::Value* ikArray2 = i->value.GetMemberValue("ik");
			if (ikArray2 != nullptr)
			{
				FOR_EACH_JSON(j, *ikArray2)
				{
					StringRef ikName = j->name.GetString();
					SkeletonIKModel* ikModel = model->FindIK(ikName);
					const rapidjson::Value& timeline = j->value;

					Share<IKTimelineModel> timelineModel = new IKTimelineModel(FileIdRef::Empty);
					for (const auto& frame : timeline)
					{
						float time = frame.GetMember("time", 0.f);
						float mix = frame.GetMember("mix", 0.f);
						bool isBendPositive = frame.GetMember("bendPositive", 1) == 1;

						Math::TweenType outTweenType;
						List<float> outTweenArgs;
						ParseCurve(frame, outTweenType, outTweenArgs);

						timelineModel->AddData(time, mix, isBendPositive, outTweenType, outTweenArgs);
					}

					timelineModel->Initialize();
					animationModel->AddIKTimeline(ikModel, timelineModel);
				}

			}

			const rapidjson::Value* ffdArray = i->value.GetMemberValue("ffd");
			if (ffdArray != nullptr)
			{

				FOR_EACH_JSON(j, *ffdArray)
				{
					const rapidjson::Value& ffd = j->value;
					StringRef skinName = j->name.GetString();
					SkeletonAvatarModel* avatarModel = model->FindAvatar(skinName);

					FOR_EACH_JSON(k, ffd)
					{
						const rapidjson::Value& slot = k->value;
						StringRef slotName = k->name.GetString();
						SkeletonSlotModel* slotModel = model->FindSlot(slotName);

						FOR_EACH_JSON(m, slot)
						{
							const rapidjson::Value& timeline = m->value;;

							StringRef attachmentName = m->name.GetString();
							ISkeletonAttachmentModel* attachmentModel = avatarModel->FindAttachment(slotModel, attachmentName);
							if (animationModel == nullptr)
							{
								Log::AssertFailedFormat("Cannot find attachemnt:{} ", attachmentName.c_str());
								return nullptr;
							}

							Share<VertexTimelineModel> timelineModel = new VertexTimelineModel(FileIdRef::Empty);

							for (const auto& frame : timeline)
							{
								float time = frame.GetMember("time", 0.f);
								rapidjson::Value::ConstMemberIterator itr = frame.FindMember("vertices");
								List<Point3F>& vertices = timelineModel->NewAddVertexList();

								if (itr != frame.MemberEnd())
								{
									const rapidjson::Value& verticesJson = itr->value;

									int offset = frame.GetMember("offset", 0);
									vertices.Add(Point3F::Zero, offset / 2);
									size_t count = verticesJson.Size();
									for (uint p = 0; p < count; p += 2)
									{
										Point3F pos = Point3F::Zero;
										pos.X = verticesJson[p].GetFloat();
										pos.Y = verticesJson[p + 1].GetFloat();
										vertices.Add(pos);
									}

									intp leftCount = 0;
									if (attachmentModel->Type() == SkeletonAttachmentType::Mesh)
									{
										//pre add?
										const List<Point3F>& attachmentVertexes = ((SkeletonMeshAttachmentModel*)attachmentModel)->Vertices();
										FOR_EACH_SIZE(p, vertices.Count())
										{
											vertices[p] += attachmentVertexes[p];
										}

										leftCount = attachmentVertexes.Count() - vertices.Count();
										vertices.AppendRange(attachmentVertexes.Items() + vertices.Count(), leftCount);
									}
									else if (attachmentModel->Type() == SkeletonAttachmentType::SkinnedMesh)
									{
										leftCount = ((SkeletonSkinnedMeshAttachmentModel*)attachmentModel)->Weights().Count() - vertices.Count();
										vertices.Add(Point3F::Zero, leftCount);

									}


								}
								else
								{
									if (attachmentModel->Type() == SkeletonAttachmentType::Mesh)
									{
										vertices = ((SkeletonMeshAttachmentModel*)attachmentModel)->Vertices();
									}
									else if (attachmentModel->Type() == SkeletonAttachmentType::SkinnedMesh)
									{

									}
								}




								Math::TweenType outTweenType;
								List<float> outTweenArgs;
								ParseCurve(frame, outTweenType, outTweenArgs);
								timelineModel->AddVertexFrame(time, outTweenType, outTweenArgs);
							}
							timelineModel->Initialize();
							animationModel->AddVertexTimeline(KeyValuePair<SkeletonSlotModel*, ISkeletonAttachmentModel*>(slotModel, attachmentModel), timelineModel);

						}


					}

				}

			}


			const rapidjson::Value* drawOrderArray = i->value.GetMemberValue("drawOrder");
			if (drawOrderArray != nullptr)
			{
				uint drawOrderCount = drawOrderArray->Size();
				if (drawOrderCount > 0)
				{
					Share<DrawOrderTimelineModel> drawOrderTimelineModel = new DrawOrderTimelineModel(FileIdRef::Empty);
					size_t slotCount = model->Slots().Count();
					List<uint> setupDrawOrders;
					FOR_EACH_SIZE(j, slotCount)
					{
						setupDrawOrders.Add((uint)j);
					}

					{
						//at setup draw order list at 0.f time
						List<uint>& drawOrders = drawOrderTimelineModel->NewDrawOrderList();
						drawOrders = setupDrawOrders;
						drawOrderTimelineModel->AddDrawOrderFrame(0.f);
					}

					for (const auto& drawOrder : *drawOrderArray)
					{
						float time = drawOrder.GetMember("time", 0.f);
						const rapidjson::Value* offsets = drawOrder.GetMemberValue("offsets");

						if (offsets != nullptr)
						{
							List<uint>& drawOrders = drawOrderTimelineModel->NewDrawOrderList();
							drawOrders = setupDrawOrders;

							for (const auto& offset : *offsets)
							{
								const char* slotName = offset.GetMember("slot", "");
								SkeletonSlotModel* slotModel = model->FindSlot(slotName);
								intp orginalIndex = model->FindSlotIndex(slotModel);
								int offsetValue = offset.GetMember("offset", 0);
								drawOrders.Offset(orginalIndex, offsetValue);
								//drawOrders[orginalIndex + offsetValue] = (uint)orginalIndex;

							}
						}

						drawOrderTimelineModel->AddDrawOrderFrame(time);

					}
					drawOrderTimelineModel->Initialize();
					animationModel->SetDrawOrderTimeLine(drawOrderTimelineModel);
				}
			}



			const rapidjson::Value* triggerArray = i->value.GetMemberValue("events");
			if (triggerArray != nullptr)
			{
				Share<TriggerTimelineModel> triggerTimelineModel = new TriggerTimelineModel(FileIdRef::Empty);
				for (const auto& frame : *triggerArray)
				{
					float time = frame.GetMember("time", 0.f);

					const char* name = frame.GetMember("name", nullptr);
					StringRef triggerName = name;
					SkeletonTriggerModel* triggerModel = model->FindTrigger(triggerName);
					TriggerEventArg eventArg;
					eventArg.SetName(triggerName);
					eventArg.SetInt(frame.GetMember("int", triggerModel->EventArg().Int()));
					eventArg.SetFloat(frame.GetMember("float", triggerModel->EventArg().Float()));
					eventArg.SetString(frame.GetMember("string", triggerModel->EventArg().String().c_str()));

					triggerTimelineModel->AddItem(time, eventArg);
				}
				triggerTimelineModel->Initialize();
				animationModel->SetTriggerTimeline(triggerTimelineModel);

			}


			animationModel->Initialize();
		}

	}

	model->Initialize();
	return model.release();
}


Share<TextureGeneralMesh> SpineSkeletonModel::CreateMesh(size_t atlasPageIndex/*=0*/)
{
	RETURN_NULL_IF_NULL(mAtlas);
	TextureAtlasPage* page = mAtlas->GetPage(atlasPageIndex);
	auto texture = page->LoadTexture();
	RETURN_NULL_IF_NULL(texture);

	return MeshFactory::Instance().CreateTextureGeneralMesh(texture);
}

size_t SpineSkeletonModel::GetAtlasPagesCount() const
{
	RETURN_ZERO_IF_NULL(mAtlas);
	return mAtlas->PageCount();
}

bool SpineSkeletonModel::HasSingleAtlasPage() const
{
	return GetAtlasPagesCount() == 1;
}

bool SpineSkeletonModel::CheckBlendFunc(bool& outIsAdditiveBlending) const
{
	return true;
}

bool SpineSkeletonModel::ParseAttachmentType(SkeletonAttachmentType& outAttachmentType, const StringRef& val)
{
	if (val.Compare("Region", true) == 0)
	{
		outAttachmentType = SkeletonAttachmentType::Region;
		return true;
	}
	else if (val.Compare("Mesh", true) == 0)
	{
		outAttachmentType = SkeletonAttachmentType::Mesh;
		return true;
	}
	else if (val.Compare("SkinnedMesh", true) == 0)
	{
		outAttachmentType = SkeletonAttachmentType::SkinnedMesh;
		return true;
	}
	else if (val.Compare("BoundingBox", true) == 0)
	{
		outAttachmentType = SkeletonAttachmentType::BoundingBox;
		return true;
	}
	return false;
}

void SpineSkeletonModel::ParseCurve(const rapidjson::Value &frame, Math::TweenType& outTweenType, List<float>& outTweenArgs)
{
	outTweenType = Math::TweenType::Linear;
	outTweenArgs.Clear();
	const rapidjson::Value* curve = frame.GetMemberValue("curve");
	RETURN_IF_NULL(curve);

	if (curve->IsString())
	{
		StringRef curveType = curve->GetString();
		if (curveType == "stepped")
		{
			outTweenType = Math::TweenType::None;
		}
	}
	else if (curve->IsArray() && curve->Size() == 4)
	{
		outTweenType = Math::TweenType::BezierCurve3ZeroToOneWithPrecomputeArgs;
		float cx1 = (*curve)[0].GetFloat();
		float cy1 = (*curve)[1].GetFloat();
		float cx2 = (*curve)[2].GetFloat();
		float cy2 = (*curve)[3].GetFloat();

		outTweenArgs.ReserveSize(7);
		Math::Tween::PrecomputeBezierCurve3ZeroToOne(cx1, cy1, cx2, cy2, 10, outTweenArgs.MutableItems());
		outTweenArgs.ForceSetCount(7);
	}
}



MEDUSA_END;