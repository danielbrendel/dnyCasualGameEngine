#pragma once

/*
	Casual Game Engine (dnyCasualGameEngine) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

#include "shared.h"

/* Utility environment */
namespace Utils {
	std::wstring ConvertToWideString(const std::string& szString);
	std::string ConvertToAnsiString(const std::wstring& wszString);
	std::string ExtractFileExt(const std::string& szFile);
	std::wstring ExtractFileExt(const std::wstring& wszFile);
	std::wstring ExtractFileName(const std::wstring& wszFile);
	std::wstring ExtractFilePath(const std::wstring& wszFile);
	bool FileExists(const std::wstring& wszFileName);
	bool DirExists(const std::wstring& wszDirName);
	inline void CharReplace(std::string& szStr, const char old, const char _new);
	byte* ReadEntireFile(const std::wstring& wszFile, size_t& uiSizeOut, bool bTreatAsString = false);
	std::vector<std::wstring> ReadFileLines(const std::wstring& wszFile);
	std::vector<std::string> Split(const std::string& szInput, const std::string& szSplit);
	std::vector<std::wstring> SplitW(const std::wstring& wszInput, const std::wstring& wszSplit);
	std::vector<std::wstring> ListFilesByExt(const std::wstring& wszBaseDir, const wchar_t** pFileList, const size_t uiListLen);
	bool RemoveEntireDirectory(const std::wstring& wszDirectory);
	bool CopyEntireDirectory(const std::wstring& wszFrom, const std::wstring& wszTo);
}