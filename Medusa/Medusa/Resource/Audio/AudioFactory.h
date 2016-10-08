// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "MedusaPreDeclares.h"
#ifdef MEDUSA_AL
#include "Core/Pattern/Singleton.h"
#include "Core/Log/Log.h"
#include "Resource/BaseResourceFactory.h"
#include "Resource/Audio/IAudio.h"

MEDUSA_BEGIN;

class AudioFactory :public Singleton<AudioFactory>, public BaseResourceFactory<IAudio>
{
	friend class Singleton<AudioFactory>;
protected:
	AudioFactory(void);
	~AudioFactory(void);

public:
	virtual StringRef Name()const override { return "AudioFactory"; }
	virtual bool Initialize();
	virtual bool Uninitialize();

	Share<IAudio> CreateFromRaw(const FileIdRef& fileId, AudioFileType audioFileType, const MemoryData& data,uint sampleCount, uint channelCount, uint samplerRate, uint bitsPerSample, ResourceShareType shareType = ResourceShareType::Share);
	Share<IAudio> CreateFromFile(const FileIdRef& fileId, ResourceShareType shareType = ResourceShareType::Share);
	Share<IAudio> CreateFromMemory(const FileIdRef& fileId, const MemoryData& data,  ResourceShareType shareType = ResourceShareType::Share);

};

MEDUSA_END;
#endif