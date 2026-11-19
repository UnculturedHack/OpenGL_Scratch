#pragma once

#include <fstream>
#include "UTILS_API.h"

namespace utils
{
	enum LogType
	{
		LT_INFO,
		LT_WARN,
		LT_ERROR,
	};

	UTILS_API void log(std::string aMsg, LogType aLogType = LT_INFO);
}