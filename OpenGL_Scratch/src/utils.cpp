#include "../include/utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdarg>
#include <string>

using namespace std;

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

void utils::printfDbg(const char* apMsgF, ...)
{
    va_list args;
    va_start(args, apMsgF);

    bool fmtSpecFound = false;
    constexpr char fmtSpecSymbol = '%';
    string fmtMsg = "";
    for (; *apMsgF; ++apMsgF)
    {
        const char c = *apMsgF;
        if (fmtSpecFound)
        {
            if (c == 'd')
            {
                int32_t d = va_arg(args, int32_t);
                fmtMsg.append(to_string(d));
            }
            else if (c == 'f')
            {
                double f = va_arg(args, double);
                fmtMsg.append(to_string(f));
            }
            else if (c == 'c')
            {
                int32_t cInt = va_arg(args, int32_t);
                fmtMsg.push_back(static_cast<char>(va_arg(args, int32_t)));
            }
            else if (c == 's')
            {
                const char* s = va_arg(args, const char*);
                fmtMsg.append(s);
            }
            else
            {
                fmtMsg.push_back(fmtSpecSymbol);
                if (c == fmtSpecSymbol)
                {
                    continue;
                }
                fmtMsg.push_back(c);
            }
            fmtSpecFound = false;
        }
        else
        {
            if (c == fmtSpecSymbol)
            {
                fmtSpecFound = true;
            }
            else
            {
                fmtMsg.push_back(c);
            }
        }
    }

    va_end(args);
    OutputDebugString(fmtMsg.append("\n").c_str());
}

void utils::errMsg(HWND aWinHdl, const char* const apText, const char* const apCaption)
{
    MessageBox(aWinHdl, apText, apCaption, MB_OK | MB_ICONERROR);
}