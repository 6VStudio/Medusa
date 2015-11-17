// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "Node/Control/EditBox/SingleLineEditBox.h"

MEDUSA_BEGIN;


SingleLineEditBox::SingleLineEditBox(StringRef name, const Size2F& size, const FontId& fontId, WStringRef text /*= WStringRef::Empty*/, Alignment alignment /*= Alignment::LeftBottom*/)
	:BaseSingleLineEditBox(name,size, fontId, text, alignment)
{
}

SingleLineEditBox::~SingleLineEditBox( void )
{

}
MEDUSA_IMPLEMENT_RTTI(SingleLineEditBox, BaseSingleLineEditBox);

MEDUSA_END;
