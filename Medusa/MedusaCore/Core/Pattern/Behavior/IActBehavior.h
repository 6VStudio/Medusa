// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "MedusaCorePreDeclares.h"
#include "Core/Pattern/Behavior/IBehavior.h"

MEDUSA_BEGIN;


class IActBehavior:public IBehavior
{
	MEDUSA_DECLARE_RTTI;
public:
	IActBehavior();
	virtual ~IActBehavior(void);
	virtual const IBehavior* Behave(IBrainBody& brainBody, void* sender)const override;

	virtual bool LoadFromXmlNode(pugi::xml_node node)override;

};

MEDUSA_END;
