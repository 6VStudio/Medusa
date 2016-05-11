// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "NineGridSprite.h"
#include "Resource/Model/Mesh/Fixed/TextureNineGridMesh.h"
#include "Resource/Material/IMaterial.h"
#include "Resource/Texture/ITexture.h"

MEDUSA_BEGIN;

NineGridSprite::NineGridSprite(StringRef name/*=StringRef::Empty*/)
	:INode(name),
	mTexturePadding(ThicknessF::Zero)
{
	SetSizeToContent(SizeToContent::WidthAndHeight);
}

NineGridSprite::~NineGridSprite( void )
{

}

bool NineGridSprite::Initialize()
{
	return true;
}


void NineGridSprite::EnableNineGrid(bool val)
{
	mNineGridEnabled = val;
}

void NineGridSprite::OnMoveableDirty(MoveableChangedFlags changedFlags)
{
	INode::OnMoveableDirty(changedFlags);

	if (MEDUSA_FLAG_HAS(changedFlags,MoveableChangedFlags::SizeChanged))
	{
		OnUpdateMesh();
	}
}

void NineGridSprite::OnUpdateMesh()
{
	TextureNineGridMesh* mesh=(TextureNineGridMesh*)mRenderingObject.Mesh();
	RETURN_IF_NULL(mesh);
	mesh->UpdateToNewTargetSize(mSize.To2D());
	
}

MEDUSA_IMPLEMENT_RTTI(NineGridSprite, INode);


MEDUSA_END;
