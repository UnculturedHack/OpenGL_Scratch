#include "pch.h"
#include "logger.h"
#include <filesystem>
#include <stdexcept>

using namespace std;

constexpr const char* cpLogsDirPath = "logs";
constexpr const char* cpGenLogFilePath = "logs/general_log.txt";

void utils::log(string aMsg, LogType aLogType /*= LT_INFO*/)
{
	filesystem::create_directories(cpLogsDirPath);

	// use std::ios::app to open the file and append to it rather than truncating the contents.
	ofstream fileStream = ofstream(cpGenLogFilePath, ios::app);
	SYSTEMTIME now;
	GetLocalTime(&now);
	
	fileStream << "[" << now.wMonth << "/" << now.wDay << "/" << now.wYear << " " << now.wHour
		<< ":" << now.wMinute << ":" << now.wSecond << "] " << aMsg << endl;
	fileStream.close();
}