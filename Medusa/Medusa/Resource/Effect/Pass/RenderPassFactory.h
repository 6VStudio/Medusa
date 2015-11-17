// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "Core/String/StringRef.h"
#include "Core/Pattern/Singleton.h"
#include "Resource/BaseResourceFactory.h"
#include "Resource/Effect/Pass/IRenderPass.h"

MEDUSA_BEGIN;

class RenderPassFactory:public Singleton<RenderPassFactory>,public BaseResourceFactory<IRenderPass>
{
	friend class Singleton<RenderPassFactory>;
public:
	RenderPassFactory();
	~RenderPassFactory();
public:
	virtual bool Initialize();
	virtual bool Uninitialize();
public:
	RenderPassDescription* GetRenderPassDescription(StringRef name);
	void RegisterRenderPassDescription(StringRef name, const FileIdRef& vertexShaderFile, const FileIdRef& pixelShaderFile,int index=0,const List<HeapString>* defines=nullptr);
	bool UnregisterRenderPassDescription(StringRef name);

	IRenderPass* CreateRenderPass(const FileIdRef& fileId, const FileIdRef& vertexShaderFile, const FileIdRef& pixelShaderFile,int index=0,const List<HeapString>* defines=nullptr,ResourceShareType shareType = ResourceShareType::Share);
	IRenderPass* CreateRenderPass(const FileIdRef& fileId,IShader* vertexShader,IShader* pixelShader,int index=0,ResourceShareType shareType = ResourceShareType::Share);
	IRenderPass* CreateRenderPass(const RenderPassDescription& description,const List<HeapString>* defines=nullptr,ResourceShareType shareType = ResourceShareType::Share);
	IRenderPass* CreateRenderPass(const FileIdRef& fileId,const List<HeapString>* defines=nullptr,ResourceShareType shareType = ResourceShareType::Share);
	IRenderPass* CreateCustomRenderPass(ResourceShareType shareType = ResourceShareType::Share);
private:
	Dictionary<StringRef,RenderPassDescription*> mRenderPassDescriptions;

};

MEDUSA_END;