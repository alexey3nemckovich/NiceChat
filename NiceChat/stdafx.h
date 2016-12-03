// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

//camera defines
#define CAM_FRAME_WIDTH 640
#define CAM_FRAME_HEIGHT 480

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <memory>
#include <tchar.h>
#include <exception>
#include <iostream>
#include <vector>

LPCWSTR PCharToLPCWSTR(char* str);

// TODO: reference additional headers your program requires here