// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaCorePreCompiled.h"
#include "Core/Coder/XOR/XORDecoder.h"
#include "Core/Command/EventArg/UserDataEventArg.h"
#include "Core/Coder/CoderFactory.h"
#include "Core/IO/Stream/MemoryStream.h"
#include "Core/Log/Log.h"

MEDUSA_BEGIN;

XORDecoder::XORDecoder(const IEventArg& e) :ICoder(e)
{
	const UserDataEventArg<MemoryByteData>& e2 = (const UserDataEventArg<MemoryByteData>&)e;
	mKey = e2.Data();
}

XORDecoder::XORDecoder(const MemoryByteData& e)
{
	mKey = e;
}

XORDecoder::~XORDecoder()
{

}


size_t XORDecoder::GuessResultSize(const IStream& input) const
{
	return input.LeftLength();
}

size_t XORDecoder::OnCode(const MemoryByteData& input, MemoryByteData& output) const
{
	RETURN_ZERO_IF_EMPTY(input);

	if (output.Size()<input.Size())
	{
		Log::AssertFailedFormat("output size:{} < expected size{}", output.Size(), input.Size());
		return 0;
	}

	const byte* data = input.Data();
	byte* result = output.MutableData();

	size_t keyIndex = 0;
	size_t srcSize = input.Size();
	size_t keySize = mKey.Size();

	if (keySize != srcSize)
	{
		for (size_t i = 0; i < srcSize; ++i)
		{
			result[i] = data[i] ^ mKey[keyIndex];
			if (keyIndex < keySize - 1)
			{
				++keyIndex;
			}
			else
			{
				keyIndex = 0;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < srcSize; ++i)
		{
			result[i] = (char)(data[i] ^ mKey[i]);
		}
	}

	return srcSize;
}


MEDUSA_END;