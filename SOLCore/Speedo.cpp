/*
	file:
		Speedo.cpp
	desc:
		This file contains the dll entry point and initialization code.
	notes:
		Original d3d8.dll exports:
			0x0001  0x000188F0 "ValidatePixelShader"
			0x0002  0x0001A480 "ValidateVertexShader"
			0x0003  0x000501F0 "DebugSetMute"
			0x0004  0x0000D4C0 "Direct3DCreate8"
*/

// Includes
#include "Speedo.h"
#include <stdio.h>

#pragma comment (lib, "d3dx8.lib")

// Functions
/*
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID)	// Entry point.
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			// Save the DLL instance for later.
			hInst = hInstance;

			char strNextDll[MAX_PATH];
			bool bNextDllFound=false;

			FILE* config = fopen("common.cfg", "r");
			if (config)	// If it opened alright...
			{
				char line[255];		// Current line.
				char* line2;			// First token of current line.

				while (fgets(line, 255, config) && !bNextDllFound)	// While we're still reading lines...
				{
					if ((line[0] != '#') && (line[0] != 0))		// If this line isn't a comment...
					{
						line2=strtok(line,";");
						
						if (strcmpi(line2,"sa_hud")==0) {
							fgets(line, 255, config);
							line2=strtok(line,";");

							if (strcmpi(line2, "end") == 0) {
								GetSystemDirectory (strNextDll, sizeof(strNextDll));
								strcat(strNextDll, "\\d3d8.dll");
							} else {
								strcpy(strNextDll,line2);
								strcat(strNextDll,".dll");
							}
							bNextDllFound=true;
						}
					}
				}
			}
			fclose(config);	// Close the config file.

			// Below is a quick and dirty command line parser.
			// Take note, this totally ignores quoted parameters on the command line.
			bWindowedMode = false;				// Windowed mode off by default
			char* cmdline = GetCommandLine();	// Get the current process' command line.
			char* opt = strtok(cmdline, " ");	// Get a space delimited token.
			do
			{
				if (strcmpi(opt, "-window") == 0)	// If current token is "-window"...
				{
					bWindowedMode = true;			// Set windowed mode.
				}
				opt = strtok(NULL, " ");			// Get next token.			
			} while (opt);	// While we've not ran out of tokens...
			 
				// Load the original d3d8.dll
				hOrigDll = LoadLibrary(strNextDll);

				// Get function addresses for all d3d8.dll exports
				OldValidatePixelShader	= GetProcAddress(hOrigDll, "ValidatePixelShader");
				OldValidateVertexShader = GetProcAddress(hOrigDll, "ValidateVertexShader");
				OldDebugSetMute			= GetProcAddress(hOrigDll, "DebugSetMute");
				OldDirect3DCreate8		= (Direct3DCreate8_t)GetProcAddress(hOrigDll, "Direct3DCreate8");

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			if (hOrigDll)	// If the original dll is loaded...
			{
				FreeLibrary(hOrigDll);	// Unload it.
				hOrigDll = NULL;
			}
			break;
		}
	}
	return true;
}
*/

void __declspec(naked) NewValidatePixelShader(void)
{
	__asm jmp OldValidatePixelShader;	// Jump to original ValidatePixelShader()
}

void __declspec(naked) NewValidateVertexShader(void)
{
	__asm jmp OldValidateVertexShader;	// Jump to original ValidateVertexShader()
}

void __declspec(naked) NewDebugSetMute(void)
{
	__asm jmp OldDebugSetMute;			// Jump to original DebugSetMute()
}

IDirect3D8* WINAPI NewDirect3DCreate8(UINT SDKVersion)
{
	pD3D = OldDirect3DCreate8(SDKVersion);	// Create a real IDirect3D8 interface for us.
	pD3DHook = &D3D8Hook;						// Get a pointer to our IDirect3D8Hook interface.
	return pD3DHook;		// Give Vice City our fake IDirect3D8 interface pointer.
}
