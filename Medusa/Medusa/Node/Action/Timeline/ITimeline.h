// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "Core/Action/BaseFiniteRepeatableCountDownDelayAction.h"

MEDUSA_BEGIN;

class ITimeline :public BaseFiniteRepeatableCountDownDelayAction
{
public:
	ITimeline(ITimelineModel* model, intp repeatCount, float beforeDelay = 0.f, float repeatDuration = 0.f, float afterDelay = 0.f, const StringRef& name = StringRef::Empty);
	ITimeline(ITimelineModel* model, bool isRepeatForever, float beforeDelay = 0.f, float repeatDuration = 0.f, float afterDelay = 0.f, const StringRef& name = StringRef::Empty);

	virtual ~ITimeline();

	ITimelineModel* Model() const { return mModel; }
	void SetModel(ITimelineModel* val) { mModel = val; }

	int CurrentFrameIndex()const;
protected:
	ITimelineModel* mModel;

};
MEDUSA_END;
