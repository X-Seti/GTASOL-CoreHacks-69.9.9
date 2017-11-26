#pragma once

#include "StdInc.h"
#include <Windows.h>
//#include "CIMGAdjuster.h"

/*
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitInstance(hModule);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		ExitInstance();
		break;
	}
	return TRUE;
}
*/

//Game Limits here
#define MAXPARKEDCARS 2100

#define ResolutionX *(DWORD*)0x9B48E4
#define ResolutionY *(DWORD*)0x9B48E8