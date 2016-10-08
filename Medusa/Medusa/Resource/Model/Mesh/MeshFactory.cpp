// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "MeshFactory.h"
#include "Resource/Font/FontFactory.h"
#include "Resource/Material/MaterialFactory.h"
#include "Resource/ResourceNames.h"
#include "Resource/Effect/EffectFactory.h"
#include "Resource/Model/Mesh/Fixed/ShapeQuadMesh.h"
#include "Resource/Model/Mesh/Fixed/ShapeTriangleMesh.h"
#include "Resource/Model/Mesh/General/ShapeGeneralMesh.h"
#include "Resource/Model/Mesh/Fixed/TextureQuadMesh.h"
#include "Resource/Model/Mesh/Fixed/TextureNineGridMesh.h"
#include "Resource/Model/Mesh/General/TextureGeneralMesh.h"
#include "Resource/TextureAtlas/TextureAtlasFactory.h"
#include "Resource/TextureAtlas/TextureAtlasRegion.h"
#include "Resource/TextureAtlas/TextureAtlasPage.h"
#include "Resource/Texture/ITexture.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/Map/FileMapOrderItem.h"
#include "Resource/Texture/TextureFactory.h"
#include "Resource/Texture/ImageTexture.h"
#include "Core/Log/Log.h"


MEDUSA_BEGIN;

MeshFactory::MeshFactory()
{

}

MeshFactory::~MeshFactory()
{

}

bool MeshFactory::Initialize()
{
	return true;
}

bool MeshFactory::Uninitialize()
{
	return true;
}


Share<ShapeQuadMesh> MeshFactory::CreateShapeQuadMesh(const Size2F& rectSize, const Color4F& color)
{
	Share<ShapeQuadMesh> mesh = new ShapeQuadMesh();
	mesh->Initialize(rectSize, color);
	return mesh;
}

Share<ShapeQuadMesh> MeshFactory::CreateShapeQuadMesh(const Rect2F& rect, const Color4F& color)
{
	Share<ShapeQuadMesh> mesh = new ShapeQuadMesh();
	mesh->Initialize(rect, color);
	return mesh;
}


Share<ShapeTriangleMesh> MeshFactory::CreateShapeTriangleMesh(const Point3F& p1, const Point3F& p2, const Point3F& p3, const Color4F& color)
{
	Share<ShapeTriangleMesh> mesh = new ShapeTriangleMesh();
	mesh->Initialize(p1, p2, p3, color);
	return mesh;
}

Share<ShapeTriangleMesh> MeshFactory::CreateShapeTriangleMesh(float width, float height, const Color4F& color)
{
	Share<ShapeTriangleMesh> mesh = new ShapeTriangleMesh();
	mesh->Initialize(width, height, color);
	return mesh;
}



Share<ShapeGeneralMesh> MeshFactory::CreateShapeCircleMesh(float radius, float precision, const Color4F& color)
{
	//material->SetDrawMode(GraphicsDrawMode::TriangleFan);
	Share<ShapeGeneralMesh> mesh = new ShapeGeneralMesh();
	uint count = (uint)Math::Ceil(Math::PI2 / precision);
	Point3F center(radius, radius);
	mesh->AppendVertex(center);
	mesh->AppendIndex(0);
	mesh->SetSize(msize3(2 * radius, 2 * radius, 0.f));

	FOR_EACH_UINT32(i, count)
	{
		float a = i*precision;
		Point3F pos(radius + radius*Math::Cos(a), radius + radius*Math::Sin(a));
		mesh->AppendVertex(pos);
		mesh->AppendIndex(i + 1);
	}
	mesh->AppendIndex(1);
	mesh->SetColorAll(color);

	return mesh;
}


Share<ShapeGeneralMesh> MeshFactory::CreateLineMesh(const Point3F& from, const Point3F& to, const Color4F& color)
{
	Share<ShapeGeneralMesh> mesh = new ShapeGeneralMesh();
	mesh->AppendVertexAndIndex(from);
	mesh->AppendVertexAndIndex(to);
	mesh->SetColorAll(color);
	return mesh;
}

Share<TextureQuadMesh> MeshFactory::CreateTextureQuadMesh(const Size2F& textureSize, const Rect2F& textureRect/*=Rect2F::Zero*/, const Color4F& color/*=Color4F::White*/)
{
	Share<TextureQuadMesh> mesh = new TextureQuadMesh();
	mesh->Initialize(textureSize, textureRect, color);
	return mesh;
}


Share<TextureQuadMesh> MeshFactory::CreateTextureQuadMesh(const FileIdRef& textureName, const FileMapOrderItem& orderItem, const Rect2F& textureRect /*= Rect2F::Zero*/, const Color4F& color /*= Color4F::White*/)
{
	auto texture = TextureFactory::Instance().CreateFromOrderItem(textureName, orderItem, ShaderSamplerNames::Texture);
	return CreateTextureQuadMesh(texture, textureRect, color);
}

Share<TextureQuadMesh> MeshFactory::CreateTextureQuadMesh(const Share<ITexture>& texture, const Rect2F& textureRect /*= Rect2F::Zero*/, const Color4F& color /*= Color4F::White*/)
{
	const Size2U& textureSize = texture->Size();
	Share<TextureQuadMesh> mesh = new TextureQuadMesh();

	mesh->Initialize(textureSize, textureRect, color);

	return mesh;
}

Share<TextureNineGridMesh> MeshFactory::CreateTextureNineGridMesh(const Size2F& targetSize, const Size2F& textureSize, const ThicknessF& padding, const Rect2F& textureRect/*=Rect2F::Zero*/, RotateDirection rotation /*= RotateDirection::None*/, const Color4F& color/*=Color4F::White*/)
{
	Share<TextureNineGridMesh> mesh = new TextureNineGridMesh();
	mesh->Initialize(targetSize, textureSize, padding, textureRect, rotation, color);
	return mesh;
}

Share<TextureGeneralMesh> MeshFactory::CreateTextureGeneralMesh(const Share<ITexture>& texture, bool isStatic/*=false*/)
{
	Share<TextureGeneralMesh> mesh = new TextureGeneralMesh(isStatic);
	return mesh;
}


Share<IMesh> MeshFactory::CreateTextureAtlasRegionMesh(TextureAtlasRegion* region, const Color4F& color /*= Color4F::White*/)
{
	RETURN_NULL_IF_NULL(region);

	region->AssertMeshLoaded();
	if (region->IsPolygon())
	{
		Share<TextureGeneralMesh> mesh = new TextureGeneralMesh();
		mesh->MutableVertices() = region->Vertices();
		mesh->MutableTexcoords() = region->Texcoords();
		mesh->MutableIndices() = region->Indices();
		mesh->AppendColor(color, (uint)region->Vertices().Count());
		mesh->SetSize(region->OriginalSize());

		return mesh;
	}
	else
	{
		Share<TextureQuadMesh> mesh = new TextureQuadMesh();
		mesh->Initialize(region->Vertices(), region->Texcoords(), color);
		mesh->SetSize(region->OriginalSize());

		return mesh;
	}


}




MEDUSA_END;