// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "WrapPanel.h"
#include "Graphics/ResolutionAdapter.h"
#include "Node/NodeFactory.h"
MEDUSA_BEGIN;

WrapPanel::WrapPanel(StringRef name/*=StringRef::Empty*/, const IEventArg& e /*= IEventArg::Empty*/)
	:IPanel(name,e),
	mItemFixedSize(Size2F::Zero), 
	mItemOffset(Point2F::Zero)
{
	mSize = ResolutionAdapter::Instance().WinSize();
	//SetStretch(Stretch::Fill);
	//EnableStretchToSize(true);
}

WrapPanel::~WrapPanel(void)
{

}

bool WrapPanel::ArrangeChildren(const Rect2F& limitRect/*=Rect2F::Zero*/, NodeLayoutArrangeFlags arrangeFlags/*=NodeLayoutArrangeFlags::None*/)
{
	Rect2F leftRect = limitRect;
	leftRect.Origin.X += mPadding.Left;
	leftRect.Size.Width -= mPadding.Left + mPadding.Right;
	leftRect.Origin.Y += mPadding.Down;
	leftRect.Size.Height -= mPadding.Down + mPadding.Up;

	Point2F origin = Point2F::Zero;
	Rect2F itemRect = Rect2F::Zero;
	bool hasFixedSize = mItemFixedSize != Size2F::Zero;


	if (mDirection == Direction::Horizontal)
	{
		origin.X = leftRect.Left();
		origin.Y = leftRect.Top();

		for(auto child: mNodes)
		{
			itemRect.Size = hasFixedSize ? mItemFixedSize : child->MeasuredSize();

			origin.Y = Math::Min(origin.Y, leftRect.Top() - itemRect.Size.Height);
			itemRect.Origin = origin;
			child->ArrangeRecursively(itemRect, arrangeFlags);

			origin.X += itemRect.Size.Width;
			if (origin.X > leftRect.Size.Width)
			{
				//change line
				leftRect.Size.Height = origin.Y - mItemOffset.Y;
				origin.X = 0;
			}
			else
			{
				origin.X += mItemOffset.X;
			}
		}
	}
	else
	{
		origin.X = leftRect.Left();
		origin.Y = leftRect.Top();

		for (auto child : mNodes)
		{
			itemRect.Size = hasFixedSize ? mItemFixedSize : child->MeasuredSize();

			origin.Y -= itemRect.Size.Height;
			itemRect.Origin.Y = origin.Y;
			origin.X = Math::Max(origin.X, leftRect.Right() - itemRect.Size.Width);

			child->ArrangeRecursively(itemRect, arrangeFlags);

			if (origin.Y < 0.f)
			{
				//change line
				origin.X += mItemOffset.X;
				leftRect.Size.Width -= (origin.X - leftRect.Origin.X);
				leftRect.Origin.X = origin.X;
				origin.Y = leftRect.Top();
				itemRect.Origin.X = origin.X;
			}
			else
			{
				origin.Y -= mItemOffset.Y;
			}
		}

	}


	return true;
}

MEDUSA_IMPLEMENT_NODE(WrapPanel);

MEDUSA_END;
