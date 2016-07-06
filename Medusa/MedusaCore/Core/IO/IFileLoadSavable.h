// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma  once
#include "MedusaCorePreDeclares.h"
#include "Core/String/StringRef.h"
#include "Core/Memory/MemoryData.h"

MEDUSA_BEGIN;

class IFileLoadSavable
{
public:
	virtual ~IFileLoadSavable() { Unload(); }
	virtual bool LoadFromFileSystem(const FileIdRef& fileId, uint format = (uint)-1);
	virtual bool LoadFromData(const FileIdRef& fileId, const MemoryData& data, uint format = (uint)-1);	//format used to indicate some other control option
	virtual void Unload();
public:
	virtual bool Save(const StringRef& path, uint format = (uint)-1);
protected:
	virtual uint OnCheckFormat(StringRef path)const { return 0; }
	virtual bool OnSave(const StringRef& path,IStream& stream, uint format = (uint)-1);
};

MEDUSA_END;