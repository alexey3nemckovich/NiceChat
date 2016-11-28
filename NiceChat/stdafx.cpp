// stdafx.cpp : source file that includes just the standard includes
// NiceChat.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "stdafx.h"

LPCWSTR PCharToLPCWSTR(char* str)
{
	wchar_t* wtext = (wchar_t*)malloc((strlen(str)+1)*2);
	mbstowcs(wtext, str, strlen(str) + 1);
	LPCWSTR ptr = wtext;
	return ptr;
}

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
