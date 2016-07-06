// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once

#include "Core/IO/FileDefines.h"

#include "Core/IO/FileInfo.h"
#include "Core/Memory/Memory.h"
#include "Core/IO/Stream/FileStream.h"

MEDUSA_BEGIN;


class File
{
public:
	File(void) = delete;
	~File(void) = delete;

	static bool Exists(StringRef filePath, int permission = 0);
	static bool Delete(StringRef filePath);
	static bool GetFileMode(StringRef filePath, FileMode& outFileMode);

	static bool Copy(StringRef srcFilePath, StringRef destFilePath);
	static bool Move(StringRef srcFilePath, StringRef destFilePath);
	static bool Rename(StringRef oldName, StringRef newName);



	static bool ReadAllTextTo(StringRef filePath, HeapString& outString);
	static bool ReadAllTextTo(StringRef filePath, WHeapString& outString);

	static HeapString ReadAllText(StringRef filePath);

	static bool WriteAllText(StringRef filePath, StringRef str);
	static bool WriteAllText(StringRef filePath, WStringRef str);


	static bool ReadAllLines(StringRef filePath, List<HeapString>& outLines, size_t maxCount = 1024, bool isTrim = true, bool ignoreEmptyLine = true);

	static MemoryData ReadAllData(StringRef filePath);
	static bool WriteAllData(StringRef filePath, MemoryData data);

	static MemoryData ReadAllDataPriority(StringRef firstPath, StringRef secondPath);

	static std::unique_ptr<FileStream> OpenTextReader(StringRef filePath);
	static std::unique_ptr<FileStream> OpenBinaryReader(StringRef filePath);

	static std::unique_ptr<FileStream> OpenTextWriter(StringRef filePath);
	static std::unique_ptr<FileStream> OpenBinaryWriter(StringRef filePath);

};

MEDUSA_END;

