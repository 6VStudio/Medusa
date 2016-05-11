// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "MedusaPreDeclares.h"
#include "Core/String/StringRef.h"
#include "Resource/IResource.h"
#include "Geometry/Size2.h"
#include "Core/Pattern/TuplePattern.h"
#include "Core/Collection/SortedList.h"
#include "Core/Collection/Array.h"

MEDUSA_BEGIN;

struct TimelineFrame
{
	float Time;
	uint Index;
	Math::TweenType TweenType;	//tween to next frame
	List<float> TweenArgs;

	TimelineFrame() {}
	TimelineFrame(float time, uint index = 0, Math::TweenType tweenType = Math::TweenType::Linear) :Time(time), Index(index), TweenType(tweenType) {}
	TimelineFrame(float time, uint index, Math::TweenType tweenType, const List<float>& args) :Time(time), Index(index), TweenType(tweenType), TweenArgs(args) {}

	bool operator==(const TimelineFrame& val)const { return Math::IsEqual(Time, val.Time); }
	bool operator<(const TimelineFrame& val)const { return Time < val.Time; }
	bool operator<=(const TimelineFrame& val)const { return Time <= val.Time; }
	bool operator>(const TimelineFrame& val)const { return Time > val.Time; }
	bool operator>=(const TimelineFrame& val)const { return Time >= val.Time; }

};


class ITimelineModel :public IResource
{

public:
	ITimelineModel(const FileIdRef& fileId, float duration = 0.f);
	virtual ~ITimelineModel();
	virtual bool Initialize()override;

	virtual ResourceType Type()const override{ return ResourceType::Timeline; }
	static ResourceType ClassGetResourceType() { return ResourceType::Timeline; }

	virtual void Clear();
	uintp FrameCount()const { return mFrameCount; }
	float Duration() const { return mDuration; }
	void SetDuration(float val) { mDuration = val; }

	//************************************
	// Method:    TryGetFrame
	// Returns:   bool
	// Parameter: float time	
	// Parameter: uint & outPrevFrameIndex	
	// Parameter: uint & outNextFrameIndex
	// Parameter: float & outPercent	0.f~1.f
	//************************************
	bool TryGetFrame(float time, uint& outPrevFrameIndex, uint& outNextFrameIndex, float& outPercent)const;

	intp GetSteppedFrameIndex(float time)const;

	void PreCalculate(float fps);
	virtual void RemovePreCalculated();
protected:
	void AddFrame(float time, uint index, Math::TweenType tweenType = Math::TweenType::Linear);
	void AddFrame(float time, uint index, Math::TweenType tweenType, const List<float>& args);

	intp GetPreCalculatedIndex(float time)const;

	bool TryGetFrameHelper(float time, uint& outPrevFrameIndex, uint& outNextFrameIndex, float& outPercent)const;
	void AddFrameWithInterval(float frameInterval, uint index, Math::TweenType tweenType = Math::TweenType::Linear);

	virtual void OnPreCalculateBegin() {}
	virtual void AddPreCalcuatedItem(bool isFound, uint prevFrameIndex, uint nextFrameIndex, float percent) {}
	virtual void OnPreCalculateEnd() {}

protected:
	SortedList<TimelineFrame> mFrames;	//frame time-data index
	float mDuration;

	bool mIsPreCalculated;
	float mFPS;
	uintp mFrameCount;

};

MEDUSA_END;
