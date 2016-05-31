/*
	file:
		Speedo.h
	desc:
		This is the header file for Speedo.cpp, the entrypoint.
*/

#ifndef SPEEDO_H
#define SPEEDO_H

#define WIN32_LEAN_AND_MEAN

#include "stdinc.h"
#include <windows.h>
#include <d3d8.h>
#include "IDirect3DDevice8Hook.h"
#include "IDirect3D8Hook.h"

HMODULE hOrigDll;
HINSTANCE hInst;
extern bool bWindowedMode;

IDirect3D8Hook D3D8Hook;

IDirect3D8 *pD3D;
IDirect3D8Hook *pD3DHook;
IDirect3DDevice8 *pD3DDevice;
IDirect3DDevice8Hook *pD3DDeviceHook;

//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

typedef IDirect3D8* (WINAPI *Direct3DCreate8_t)(UINT SDKVersion);

FARPROC OldValidatePixelShader;
FARPROC OldValidateVertexShader;
FARPROC OldDebugSetMute;
Direct3DCreate8_t OldDirect3DCreate8;

#endif