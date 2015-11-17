// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "MedusaCorePreDeclares.h"
#include "IPredicate.h"

MEDUSA_BEGIN;

struct TruePredicate :public IPredicate
{
	MEDUSA_DECLARE_RTTI;
public:
	virtual int Evaluate(void*)const override { return 1; }
};


MEDUSA_END;
