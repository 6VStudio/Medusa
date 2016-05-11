// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "TestPreCompiled.h"
#include "TestScene.h"
#include "TestLayer.h"

TestScene::TestScene(StringRef name/*=StringRef::Empty*/, const IEventArg& e /*= IEventArg::Empty*/)
	:UIScene(name,e)
{

}

TestScene::~TestScene(void)
{

}

bool TestScene::Initialize()
{
	RETURN_FALSE_IF_FALSE(UIScene::Initialize());
	PushLayer<TestLayer>();

	return true;
}

MEDUSA_IMPLEMENT_SCENE(TestScene, UIScene, StringRef::Empty, StringRef::Empty);
