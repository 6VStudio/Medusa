// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "TestPreCompiled.h"
#include "CocosFeatureLayer.h"
#include "CocosHelloLayer.h"

CocosFeatureLayer::CocosFeatureLayer(StringRef name/*=StringRef::Empty*/, const IEventArg& e/*=IEventArg::Empty*/) :BaseFeatureLayer(name, e)
{
	Register<CocosHelloLayer>();
}

CocosFeatureLayer::~CocosFeatureLayer(void)
{
}


MEDUSA_IMPLEMENT_LAYER(CocosFeatureLayer, BaseFeatureLayer, StringRef::Empty, StringRef::Empty);

