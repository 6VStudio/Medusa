// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "MedusaPreDeclares.h"
#include "IPredicate.h"

MEDUSA_BEGIN;

struct XorPredicate :public IPredicate
{
	MEDUSA_DECLARE_RTTI;
public:

	XorPredicate(const IPredicate* left = nullptr, const IPredicate* right = nullptr);
public:
	virtual int Evaluate(void* p)const override;
};


MEDUSA_END;
