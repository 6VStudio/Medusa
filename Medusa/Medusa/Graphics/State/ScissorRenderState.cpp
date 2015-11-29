// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "Graphics/State/ScissorRenderState.h"
#include "Graphics/Render/Render.h"
#include "Resource/Effect/Shader/Parameter/ShaderConstant.h"
#include "Rendering/RenderingContext.h"
#include "Resource/Effect/IEffect.h"
#include "Resource/ResourceNames.h"
#include "Graphics/ResolutionAdapter.h"
#include "Core/Geometry/Matrix4.h"
MEDUSA_BEGIN;

ScissorRenderState::ScissorRenderState(const Rect2F& scissorBox/*=Rect2F::Zero*/, bool isScissorEnabled/*=false*/)
	:mScissorBox(scissorBox), mEnabled(isScissorEnabled)
{

}

ScissorRenderState::~ScissorRenderState()
{

}

void ScissorRenderState::Apply()const
{
	Render::Instance().EnableFeature(GraphicsFeatures::ScissorTest, mEnabled);
	Render::Instance().SetScissorBox(mScissorBox);
}

ScissorRenderState* ScissorRenderState::Clone() const
{
	ScissorRenderState* state = new ScissorRenderState(mScissorBox, mEnabled);
	return state;
}

void ScissorRenderState::CopyFrom(const IRenderState& other)
{
	MEDUSA_ASSERT(other.Type() == Type(), "Cannot copy render state with different type");
	ScissorRenderState& val = (ScissorRenderState&)other;
	mEnabled = val.mEnabled;
	mScissorBox = val.mScissorBox;
}

bool ScissorRenderState::Equals(const IRenderState& state) const
{
	RETURN_FALSE_IF_FALSE(IRenderState::Equals(state));
	const ScissorRenderState& val = (const ScissorRenderState&)state;
	return mEnabled == val.IsEnabled() && mScissorBox == val.ScissorBox();
}

ScissorRenderState* ScissorRenderState::Current()
{
	IRender& render = Render::Instance();
	ScissorRenderState* state = new ScissorRenderState();

	state->Enable(render.GetBoolean(GraphicsBooleanName::ScissorTest));

	Rect2I scissorBox;
	render.GetIntegerArray(GraphicsIntegerArrayName::ScissorBox, scissorBox.GetBuffer());
	state->SetScissorBox(Rect2F(scissorBox.Origin.X, scissorBox.Origin.Y, scissorBox.Size.Width, scissorBox.Size.Height));

	return state;
}


void ScissorRenderState::Tansform(const Matrix4& matrix)
{
	mScissorBox = matrix.Transform(mScissorBox);
}

bool ScissorRenderState::Enable(bool val)
{
	RETURN_FALSE_IF_EQUAL(mEnabled, val);
	mEnabled = val; 
	OnStateChanged();
	return true;
}

bool ScissorRenderState::SetScissorBox(const Rect2F& val)
{
	RETURN_FALSE_IF_EQUAL(mScissorBox, val);

	mScissorBox = val; 
	OnStateChanged();
	return true;

}

intp ScissorRenderState::HashCode() const
{
	if (mEnabled)
	{
		return mScissorBox.HashCode();
	}
	return 0;
}


void ScissorRenderState::UpdateWorldState(const IRenderState* selfRenderState, const IRenderState* parentRenderState, const Matrix4& selfWorldMatrix)
{
	if (selfRenderState != nullptr&&parentRenderState==nullptr)
	{
		//use self first
		MEDUSA_ASSERT(selfRenderState->Type() == Type(), "Cannot copy render state with different type");

		CopyFrom(*selfRenderState);
		Tansform(selfWorldMatrix);
	}
	else if (parentRenderState != nullptr&&selfRenderState == nullptr)
	{
		MEDUSA_ASSERT(parentRenderState->Type() == Type(), "Cannot copy render state with different type");

		CopyFrom(*parentRenderState);
		Tansform(selfWorldMatrix);
	}
	else if (selfRenderState != nullptr&&parentRenderState != nullptr)
	{
		MEDUSA_ASSERT(selfRenderState->Type() == Type(), "Cannot copy render state with different type");
		MEDUSA_ASSERT(parentRenderState->Type() == Type(), "Cannot copy render state with different type");


		CopyFrom(*selfRenderState);
		Tansform(selfWorldMatrix);

		ScissorRenderState* parent = (ScissorRenderState*)parentRenderState;

		if (parent->IsEnabled())
		{
			mScissorBox = Rect2F::Intersect(mScissorBox, parent->mScissorBox);
		}
	}
}

MEDUSA_IMPLEMENT_RTTI(ScissorRenderState, IRenderState);
MEDUSA_END;