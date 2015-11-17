// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "MedusaPreDeclares.h"
#include "BaseMultipleBatchRenderQueue.h"

MEDUSA_BEGIN;


class UIRenderQueue :public BaseMultipleBatchRenderQueue
{
public:
	UIRenderQueue(IRenderTarget* renderTarget = nullptr, Camera* camera = nullptr);
	~UIRenderQueue(void);
};

MEDUSA_END;
