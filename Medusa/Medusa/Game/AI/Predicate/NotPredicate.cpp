// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "NotPredicate.h"
#include "PredicateFactory.h"
MEDUSA_BEGIN;

NotPredicate::NotPredicate(const IPredicate* right /*= nullptr*/)
{
	if (right != nullptr)
	{
		Add(right);
	}
}

int NotPredicate::Evaluate(void* p) const
{
	RETURN_OBJECT_IF_EMPTY(mItems,1);
	return !mItems.First()->Evaluate(p);
}

MEDUSA_IMPLEMENT_PREDICATE(NotPredicate);
MEDUSA_END;