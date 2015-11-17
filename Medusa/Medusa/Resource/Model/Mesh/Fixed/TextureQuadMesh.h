// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "MedusaPreDeclares.h"
#include "Resource/Model/Mesh/IMesh.h"
#include "Core/Geometry/Point2.h"
#include "Core/Geometry/Point3.h"
#include "Core/Geometry/Color4.h"
#include "Graphics/GraphicsTypes.h"
#include "Core/Collection/Array.h"
#include "Core/Geometry/Rect2.h"

MEDUSA_BEGIN;

/*
We choose separate buffer instead of interleaved, because we want to update only one componet
*/
class TextureQuadMesh :public IMesh
{
public:
	TextureQuadMesh(IEffect* effect = nullptr, IMaterial* material = nullptr, bool isStatic = false);
	virtual ~TextureQuadMesh(void);
public:


	virtual size_t VertexCount()const { return 4; }
	virtual size_t IndexCount()const { return 6; }

	const Array<Point3F, 4>& Vertices()  const { return mVertices; }
	const Array<Point2F, 4>& Texcoords() const { return mTexcoords; }
	const Array<Color4F, 4>& Colors() const { return mColors; }

	Array<Point3F, 4>& MutableVertices() { return mVertices; }
	Array<Point2F, 4>& MutableTexcoords() { return mTexcoords; }


	void SetVertices(const Array<Point3F, 4>& val);
	void SetTexcoords(const Array<Point2F, 4>& val);


	void SetVertex(uint i, const Point3F& val);
	void SetVertexBySize(const Size2F& val);

	void SetTexcoord(uint i, const Point2F& val);
	void SetTexcoordByRect(const Size2U& textureSize, const Rect2F& textureRect);

	void SetColor(uint i, const Color4F& val);
	void SetColorAll(const Color4F& val);

	void Initialize(const Size2U& textureSize, const Rect2F& textureRect = Rect2F::Zero, const Color4F& color = Color4F::White);
	void Initialize(const Rect2F& quadRect, const Rect2F& texcoord, const Color4F& color = Color4F::White);
	void Initialize(const Array<Point3F, 4>& vertices, const Array<Point2F, 4>& texcoords, const Color4F& color = Color4F::White);

	virtual INode* CreateCloneInstance()const;
	bool CopyFrom(const TextureQuadMesh& val);


public:
	virtual void AddToVertexBufferObject(VertexGraphicsBuffer& bufferObject, size_t vertexIndex, const Matrix& matrix)const;
	virtual void AddToTexCoordBufferObject(TexCoordGraphicsBuffer& bufferObject, size_t vertexIndex)const;
	virtual void AddToColorBufferObject(ColorGraphicsBuffer& bufferObject, size_t vertexIndex, const Color4F& parentColor = Color4F::White)const;
	virtual void AddToIndexBufferObject(IndexGraphicsBuffer& bufferObject, size_t vertexIndex, size_t indexIndex)const;
protected:
	Array<Point3F, 4> mVertices;
	Array<Point2F, 4> mTexcoords;
	Array<Color4F, 4> mColors;
};

MEDUSA_END;