// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "Core/String/StringRef.h"
#include "Core/Pattern/Singleton.h"
#include "Geometry/Rect2.h"
#include "Geometry/Color4.h"
#include "Geometry/Thickness.h"
#include "Geometry/Size2.h"
#include "Resource/TextureAtlas/TextureAtlasDefines.h"
#include "Core/Collection/SortedDictionary.h"
#include "Geometry/GeometryDefines.h"

MEDUSA_BEGIN;

class MeshFactory :public Singleton<MeshFactory>
{
	friend class Singleton<MeshFactory>;
public:
	MeshFactory();
	~MeshFactory();
public:
	virtual bool Initialize();
	virtual bool Uninitialize();
public:
	Share<ShapeQuadMesh> CreateShapeQuadMesh(const Size2F& rectSize, const Color4F& color);
	Share<ShapeQuadMesh> CreateShapeQuadMesh(const Rect2F& rect, const Color4F& color);

	Share<ShapeTriangleMesh> CreateShapeTriangleMesh(const Point3F& p1, const Point3F& p2, const Point3F& p3, const Color4F& color);
	Share<ShapeTriangleMesh> CreateShapeTriangleMesh(float width, float height, const Color4F& color);
	Share<ShapeGeneralMesh> CreateShapeCircleMesh(float radius,float precision, const Color4F& color);
	Share<ShapeGeneralMesh> CreateLineMesh(const Point3F& from, const Point3F& to, const Color4F& color);

	Share<TextureQuadMesh> CreateTextureQuadMesh(const Size2F& textureSize, const Rect2F& textureRect = Rect2F::Zero, const Color4F& color = Color4F::White);
	Share<TextureQuadMesh> CreateTextureQuadMesh(const FileIdRef& textureName, const FileMapOrderItem& orderItem, const Rect2F& textureRect = Rect2F::Zero, const Color4F& color = Color4F::White);
	Share<TextureQuadMesh> CreateTextureQuadMesh(const Share<ITexture>& texture, const Rect2F& textureRect = Rect2F::Zero, const Color4F& color = Color4F::White);


	Share<IMesh> CreateTextureAtlasRegionMesh(TextureAtlasRegion* region, const Color4F& color = Color4F::White);



	Share<TextureNineGridMesh> CreateTextureNineGridMesh(const Size2F& targetSize, const Size2F& textureSize, const ThicknessF& padding, const Rect2F& textureRect = Rect2F::Zero, RotateDirection rotation = RotateDirection::None, const Color4F& color = Color4F::White);
	

	Share<TextureGeneralMesh> CreateTextureGeneralMesh(const Share<ITexture>& texture, bool isStatic = false);



};

MEDUSA_END;