// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma  once

#include "MedusaPreDeclares.h"
#include "Core/Pattern/Object/MapObjectFactory.h"
#include "Resource/Effect/Shader/IShader.h"

MEDUSA_BEGIN;

class ShaderCreater:public MapObjectFactory<StringRef,IShader*(const FileIdRef& fileId)>
{
public:
	using MapObjectFactory<StringRef,IShader*(const FileIdRef& fileId)>::Create;
private:
	ShaderCreater();
	~ShaderCreater(){}
public:
	static ShaderCreater& Instance()
	{
		static ShaderCreater creater;
		return creater;
	}

	template<typename T>
	IShader* Create(const FileIdRef& fileId)
	{
		return Create(typename T::ClassNameStatic(), fileId);
	}
};


#define MEDUSA_DECLARE_SHADER_CREATER() 				   \
private:													   \
	const static StaticConstructor mStaticConstructor;		   \
	static void RegisterShader();

#define MEDUSA_IMPLEMENT_SHADER_CREATER(className) 																 \
	const StaticConstructor className::mStaticConstructor(RegisterShader);						 \
	void className::RegisterShader(){ShaderCreater::Instance().Register<className>(#className);}				 

MEDUSA_END;