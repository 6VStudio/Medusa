// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "Core/Log/ILogger.h"
MEDUSA_BEGIN;

#ifdef MEDUSA_ANDROID

class AndroidTraceLogger :public ILogger
{
public:
	using ILogger::ILogger;

protected:
	virtual void Print(const Share<LogMessage>& message)override;
	virtual void Print(const Share<WLogMessage>& message)override;


};
#endif

MEDUSA_END;
