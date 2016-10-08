// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "RenderingObjectFactory.h"
#include "Resource/TextureAtlas/TextureAtlasFactory.h"
#include "Resource/TextureAtlas/TextureAtlasRegion.h"
#include "Resource/TextureAtlas/TextureAtlasPage.h"
#include "Core/Log/Log.h"
#include "Resource/Material/MaterialFactory.h"
#include "RenderingObject.h"
#include "Resource/Model/Mesh/MeshFactory.h"
#include "Resource/Model/Mesh/Fixed/TextureQuadMesh.h"
#include "Resource/Model/Mesh/Fixed/TextureNineGridMesh.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/Map/FileMapOrderItem.h"

MEDUSA_BEGIN;

RenderingObjectFactory::RenderingObjectFactory()
{

}

RenderingObjectFactory::~RenderingObjectFactory()
{

}

bool RenderingObjectFactory::Initialize()
{
	return true;
}

bool RenderingObjectFactory::Uninitialize()
{
	return true;
}


RenderingObject RenderingObjectFactory::CreateFromTexture(const FileIdRef& textureName, const Rect2F& textureRect /*= Rect2F::Zero*/, const Color4F& color /*= Color4F::White*/)
{
	auto* orderItem = FileSystem::Instance().FindOrderItem(textureName);
	RETURN_NULL_IF_NULL(orderItem);

	if (orderItem->Region() != nullptr)
	{
		return CreateFromTextureAtlasRegion((TextureAtlasRegion*)orderItem->Region());
	}

	if (!orderItem->RegionName().IsEmpty())
	{
		auto fileId = FileId::ParseFrom(orderItem->GetFileEntry()->Path());
		return CreateFromTextureAtlasRegion(orderItem->RegionName(), fileId);
	}

	auto material = MaterialFactory::Instance().CreateSingleTexture(textureName);
	RETURN_OBJECT_IF_NULL(material, RenderingObject::Empty);
	auto mesh = MeshFactory::Instance().CreateTextureQuadMesh(material->FirstTexture()->Size(), textureRect, color);
	RETURN_OBJECT_IF_NULL(mesh, RenderingObject::Empty);

	return RenderingObject(mesh, material);
}
RenderingObject RenderingObjectFactory::CreateFromTexture(const Share<ITexture>& texture, const Rect2F& textureRect /*= Rect2F::Zero*/, const Color4F& color /*= Color4F::White*/)
{
	auto mesh = MeshFactory::Instance().CreateTextureQuadMesh(texture, textureRect, color);
	RETURN_NULL_IF_NULL(mesh);
	auto material = MaterialFactory::Instance().CreateSingleTexture(texture);
	return RenderingObject(mesh, material);
}

RenderingObject RenderingObjectFactory::CreateNineGridTexture(const Size2F& targetSize, const FileIdRef& textureName, const ThicknessF& padding, const Rect2F& textureRect /*= Rect2F::Zero*/)
{
	TextureAtlasRegion* region = nullptr;
	auto* orderItem = FileSystem::Instance().FindOrderItem(textureName);
	if (orderItem->Region() != nullptr)
	{
		region = (TextureAtlasRegion*)orderItem->Region();
		if (region->IsPolygon())
		{
			Log::AssertFailedFormat("Not support polygon region mode on {} when CreateNineGridTexture", textureName);
			return nullptr;
		}
	}
	else if (!orderItem->RegionName().IsEmpty())
	{
		auto fileId = FileId::ParseFrom(orderItem->GetFileEntry()->Path());
		region = TextureAtlasFactory::Instance().CreateAtlasRegion(orderItem->RegionName(), fileId);
		if (region==nullptr)
		{
			Log::AssertFailedFormat("Cannot find region:{} in {}", orderItem->RegionName(), fileId);
			return nullptr;
		}
	}

	if (region!=nullptr)
	{
		auto material = region->CreateMaterial();
		RETURN_NULL_IF_NULL(material);
		auto mesh = MeshFactory::Instance().CreateTextureNineGridMesh(targetSize, material->FirstTexture()->Size(), padding, region->ResultTextureRect(), region->GetRotateDirection());
		RETURN_NULL_IF_NULL(mesh);

		return RenderingObject(mesh, material);
	}


	auto material = MaterialFactory::Instance().CreateSingleTexture(textureName);
	RETURN_NULL_IF_NULL(material);
	auto mesh = MeshFactory::Instance().CreateTextureNineGridMesh(targetSize, material->FirstTexture()->Size(), padding, textureRect);
	RETURN_NULL_IF_NULL(mesh);

	return RenderingObject(mesh, material);

}

RenderingObject RenderingObjectFactory::CreateFromTextureAtlasRegion(TextureAtlasRegion* region, const Color4F& color /*= Color4F::White*/)
{
	auto mesh = MeshFactory::Instance().CreateTextureAtlasRegionMesh(region);
	RETURN_NULL_IF_NULL(mesh);

	return RenderingObject(mesh, region->CreateMaterial());
}

RenderingObject RenderingObjectFactory::CreateFromTextureAtlasRegion(StringRef regionName, const FileIdRef& atlasFileId, TextureAtlasType fileFormat /*= TextureAtlasFileFormat::Spine*/)
{
	TextureAtlasRegion* region = TextureAtlasFactory::Instance().CreateAtlasRegion(regionName, atlasFileId, fileFormat);
	return CreateFromTextureAtlasRegion(region);
}



RenderingObject RenderingObjectFactory::CreateFromSingleTextureMaterial(const Share<IMaterial>& material, const Rect2F& textureRect /*= Rect2F::Zero*/, const Color4F& color /*= Color4F::White*/)
{
	auto mesh = MeshFactory::Instance().CreateTextureQuadMesh(material->FirstTexture()->Size(), textureRect, color);
	RETURN_OBJECT_IF_NULL(mesh, RenderingObject::Empty);

	return RenderingObject(mesh, material);
}

bool RenderingObjectFactory::CreateFromTextureAtlas(SortedDictionary<uint, RenderingObject>& outObjects, const StringRef& regionPattern, const FileIdRef& atlasFileId, TextureAtlasType fileFormat /*= TextureAtlasFileFormat::Spine*/, const Color4F& color /*= Color4F::White*/)
{
	auto atlas = TextureAtlasFactory::Instance().Create(atlasFileId, fileFormat);
	if (atlas == nullptr)
	{
		Log::AssertFailedFormat("Cannot find texture atlas:{}", atlasFileId.Name.c_str());
		return false;
	}

	if (regionPattern.IsEmpty())
	{
		const List<TextureAtlasPage*>& pages = atlas->Pages();
		for (auto page : pages)
		{
			auto material = MaterialFactory::Instance().CreateSingleTexture(page->LoadTexture());
			const List<TextureAtlasRegion*>& regions = page->Regions();
			for (auto region : regions)
			{
				StringRef regionName = region->Name();
				uint outOrder = 0;
				StringParser::TryExtractUInt(regionName, outOrder);
				auto mesh = MeshFactory::Instance().CreateTextureAtlasRegionMesh(region, color);

				outObjects.Add(outOrder, RenderingObject(mesh, material));
			}
		}
	}
	else
	{
		const List<TextureAtlasPage*>& pages = atlas->Pages();
		for (auto page : pages)
		{
			auto material = MaterialFactory::Instance().CreateSingleTexture(page->LoadTexture());

			const List<TextureAtlasRegion*>& regions = page->Regions();
			for (auto region : regions)
			{
				StringRef regionName = region->Name();
				if (regionName.BeginWith(regionPattern))
				{
					regionName.ForceSetBuffer(regionName.Buffer() + regionPattern.Length());	//skip start
					regionName.ForceSetLength(regionName.Length() - regionPattern.Length());
					uint outOrder = 0;
					StringParser::TryExtractUInt(regionName, outOrder);
					auto mesh = MeshFactory::Instance().CreateTextureAtlasRegionMesh(region, color);
					outObjects.Add(outOrder, RenderingObject(mesh, material));

				}

			}
		}
	}



	return true;

}



bool RenderingObjectFactory::CreateFromTextures(List<RenderingObject>& outObjects, const StringRef& textureNamePattern, const Color4F& color /*= Color4F::White*/)
{
	List<const FileMapOrderItem*> outOrderItems;
	if (!FileSystem::Instance().TryGetOrderItems(textureNamePattern, outOrderItems))
	{
		return false;
	}

	for (const FileMapOrderItem* orderItem : outOrderItems)
	{
		outObjects.NewAdd() = CreateFromTexture(FileIdRef(textureNamePattern, orderItem->Order()));
	}
	return !outObjects.IsEmpty();
}


MEDUSA_END;