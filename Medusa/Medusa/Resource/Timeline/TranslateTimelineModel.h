// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "Resource/Timeline/ITimelineModel.h"
#include "Core/Geometry/Point3.h"

MEDUSA_BEGIN;

class TranslateTimelineModel:public ITimelineModel
{
public:
	TranslateTimelineModel(const FileIdRef& fileId, float duration = 0.f);
	virtual ~TranslateTimelineModel(void);

	void AddTranslate(float time, const Point3F& translate, Math::TweenType tweenType, const List<float>& args);
	Point3F GetTranslate(float time)const;

	virtual void RemovePreCalculated()override;
protected:
	virtual void OnPreCalculateBegin()override;
	virtual void AddPreCalcuatedItem(bool isFound, uint prevFrameIndex, uint nextFrameIndex, float percent) override;
protected:
	List<Point3F> mTranslates;
	List<Point3F> mPreCalculatedTranslates;

};

MEDUSA_END;
