// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma  once

#include "MedusaCorePreDeclares.h"

MEDUSA_BEGIN;
template<typename T>
class Singleton
{
private:
	Singleton(const Singleton&);
	Singleton & operator=(const Singleton&);
public:
	MEDUSA_FORCE_INLINE static T& Instance()
	{
		static T object;
		return object;
	}

	MEDUSA_FORCE_INLINE static T* InstancePtr()
	{
		return &Instance();
	}
protected:
	Singleton() {}
	virtual ~Singleton() {}
};
MEDUSA_END;