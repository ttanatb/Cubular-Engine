#pragma once

#ifdef _DEBUG
#define DEBUG_PRINT(a, ...) printf("%s: %d(): " a "\n", __FILE__, __LINE__, __VA_ARGS__);
#else
#define DEBUG_PRINT(a, ...)
#endif