// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "TestPreDeclares.h"
#include "Game/BaseCaseLayer.h"


class TiledMapLayer :public BaseCaseLayer
{
	MEDUSA_NODE(TiledMapLayer, BaseCaseLayer);
public:
	TiledMapLayer(StringRef name=StringRef::Empty,const IEventArg& e=IEventArg::Empty);
	virtual ~TiledMapLayer(void);
	virtual bool Initialize()override;
protected:
	GridMap mMap;
};

MEDUSA_ENABLE_STATIC_CONSTRUCTOR(TiledMapLayer);
