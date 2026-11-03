#include "../include/utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void utils::loadTxtFile(const char* const acpTxtFilePath, std::string& aLoadedText)
{
	std::ifstream file(acpTxtFilePath, std::ios::binary);
	if (!file)
	{
		throw std::runtime_error("Failed to load file!");
	}

	std::ostringstream fileContents;
	fileContents << file.rdbuf();
	aLoadedText = fileContents.str();
}