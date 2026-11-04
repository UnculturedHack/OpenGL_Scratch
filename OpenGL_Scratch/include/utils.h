#pragma once

#include <string>
#include <windows.h>

namespace utils
{
	void loadTxtFile(const char* const acpTxtFilePath, std::string& aLoadedText);
	void printfDbg(const char* apMsgF, ...);
	void errMsg(HWND aWinHdl, const char* const apText, const char* const apCaption);
}