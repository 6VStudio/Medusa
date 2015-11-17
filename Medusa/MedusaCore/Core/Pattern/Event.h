// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma  once

#include "Core/Pattern/Delegate.h"
#include "Core/Collection/List.h"

MEDUSA_BEGIN;
template<typename FuncSignature>
class Event
{
public:
	using DelegateType = Delegate<FuncSignature>;
	using ContainerType = List<DelegateType>;
	using ResultContainerType = List<typename DelegateType::return_type>;
public:
	Event() = default;
	~Event() = default;
	Event(const Event& other) :mItems(other.mItems) {}
	Event& operator=(const Event& other) { assert(this != &other); mItems = other.mItems; return*this; }
	Event(Event&& other) :mItems(std::move(other.mItems)) {}
	Event& operator=(Event&& other)
	{
		assert(this != &other);
		mItems = std::move(other.mItems); return*this;
	}
public:
	template<typename ...TArgs>
	void Invoke(TArgs&&... args)const
	{
		for (const auto& f : mItems)
		{
			f(std::forward<TArgs>(args)...);
		}
	}
	template<typename ...TArgs>
	ResultContainerType&& InvokeWithResults(TArgs&&... args)const
	{
		ResultContainerType results;
		for (const auto& f : mItems)
		{
			results.EmplaceBack(f(std::forward<TArgs>(args)...));
		}
		return std::move(results);
	}

	template<typename ...TArgs>
	void operator()(TArgs&&... args)const
	{
		Invoke(std::forward<TArgs>(args)...);
	}

	void Clear()
	{
		mItems.Clear();
	}
	bool IsEmpty()const
	{
		return mItems.IsEmpty();
	}
	explicit operator bool()const { return !IsEmpty(); }

	void Add(const DelegateType& func)
	{
		mItems.EmplaceBack(func);
	}
	void Add(DelegateType&& func)
	{
		mItems.EmplaceBack(std::move(func));
	}

	bool Remove(const DelegateType& func)
	{
		return mItems.Remove(func);
	}
	bool Contains(const DelegateType& func)const
	{
		return mItems.Contains(func);
	}
	Event& operator+=(const DelegateType& func)
	{
		Add(func);
		return *this;
	}
	Event& operator+=(DelegateType&& func)
	{
		Add(std::move(func));
		return *this;
	}

	Event& operator-=(const DelegateType& func)
	{
		Remove(func);
		return *this;
	}
private:
	ContainerType mItems;
};


MEDUSA_END;
