#pragma once

#include "framework.h"
#include "UTILS_API.h"

namespace utils
{
	UTILS_API void loadTxtFile(const char* const acpTxtFilePath, std::string& aLoadedText);
	UTILS_API void printfDbg(const char* apMsgF, ...);
	UTILS_API void errMsg(HWND aWinHdl, const char* const apText, const char* const apCaption);
}