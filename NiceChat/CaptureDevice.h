#pragma once
#include <Windows.h>


struct CaptureDevice
{
	static const int cbName = 256;
	static const int cbVer = 512;
	WORD wIndex;
	TCHAR lpszName[cbName];
	TCHAR lpszDescription[cbVer];
	//free ?
};