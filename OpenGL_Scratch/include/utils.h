#pragma once

#include <iosfwd>

namespace utils
{
	void loadTxtFile(const char* const acpTxtFilePath, std::string& aLoadedText);
}