// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma  once
#include "MedusaCorePreDeclares.h"
#include "Core/Pattern/RTTI/RTTIObject.h"
#include "Core/Pattern/StaticConstructor.h"
#include "Core/Pattern/Runnable/DefaultRunnable.h"

MEDUSA_BEGIN;

class IComponent :public RTTIObject, public DefaultRunnable
{
	MEDUSA_DECLARE_RTTI_ROOT;
public:
	IComponent(const StringRef& name = StringRef::Empty, int priority = 0, void* userData = nullptr);

	virtual ~IComponent() {}
public:
	StringRef Name() const { return mName; }
	void SetName(const StringRef& val) { mName = val; }

	int Priority() const { return mPriority; }
	void SetPriority(int val) { mPriority = val; }

	void* UserData() const { return mUserData; }
	void SetUserData(void* val) { mUserData = val; }

	IEntity* Entity() const { return mEntity; }
	void SetEntity(IEntity* val) { mEntity = val; }

	int Compare(const IComponent& val)const { return mPriority - val.Priority(); }
public:
	virtual bool Update(float dt);
	virtual bool AcceptEvent(IEventArg& eventArg) { return true; }
protected:
	HeapString mName;
	int mPriority;
	void* mUserData;
	IEntity* mEntity;
	

};

#define MEDUSA_DECLARE_COMPONENT 				   \
	MEDUSA_DECLARE_RTTI;	\
private:													   \
	const static StaticConstructor mStaticConstructor;		   \
	static void RegisterComponent();

#define MEDUSA_IMPLEMENT_COMPONENT(className,baseClassName) 																 \
	MEDUSA_IMPLEMENT_RTTI(className,baseClassName);\
	const StaticConstructor className::mStaticConstructor(RegisterComponent);						 \
	void className::RegisterComponent(){ComponentFactory::Instance().Register<className>(#className);}		

#define MEDUSA_IMPLEMENT_COMPONENT_ROOT(className) 																 \
	MEDUSA_IMPLEMENT_RTTI_ROOT(className);\
	const StaticConstructor className::mStaticConstructor(RegisterComponent);						 \
	void className::RegisterComponent(){ComponentFactory::Instance().Register<className>(#className);}		

MEDUSA_END;