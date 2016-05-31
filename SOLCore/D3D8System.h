#ifndef D3D8HOOKINIT_H
#define D3D8HOOKINIT_H

#define POINTER_64 __ptr64

#include "StdInc.h"
#include <Windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <stdio.h>

void CustomInitialize(IDirect3DDevice8* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
void CustomRender(IDirect3DDevice8* pDevice);
void CustomReset();

typedef IDirect3D8* (WINAPI *Direct3DCreate8_t)(UINT SDKVersion);

#endif