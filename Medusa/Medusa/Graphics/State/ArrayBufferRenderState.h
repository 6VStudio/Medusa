// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma  once
#include "Graphics/State/BaseBufferRenderState.h"

MEDUSA_BEGIN;

class ArrayBufferRenderState:public BaseBufferRenderState
{
	MEDUSA_RTTI(ArrayBufferRenderState,BaseBufferRenderState);
public:
	ArrayBufferRenderState(uint buffer=0);
	virtual ~ArrayBufferRenderState();
	virtual void Apply()const override;

	virtual GraphicsBufferType BufferType() const override { return GraphicsBufferType::Array; }

	virtual RenderStateType Type()const override {return GetTypeIdStatic();}
	
	static RenderStateType GetTypeIdStatic(){return RenderStateType::Array;}

	Share<ArrayBufferRenderState> Clone()const;
	virtual void CopyFrom(const IRenderState& other)override;

	static Share<ArrayBufferRenderState> Current();
};


MEDUSA_END;