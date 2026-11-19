#pragma once

#if defined(UTILS_EXPORTS)
#define UTILS_API __declspec(dllexport)
#else
#define UTILS_API __declspec(dllimport)
#endif