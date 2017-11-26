//Fastman92 Limit Adjuster
#include "./windows.h"
//#include <SDKDDKVer.h>
#include "../StdInc.h"
#include "CIMGAdjuster.h"


SALimitAdjuster::tCDimageName* SALimitAdjuster::CDIMAGE_NAMES = NULL;
HANDLE* SALimitAdjuster::CDIMAGES_HANDLES = NULL;

void printf_MessageBox(const wchar_t * format, ...)
{
	va_list vl;
	va_start(vl, format);	

	wchar_t buffer[4096];

	_vsnwprintf_s(buffer, _countof(buffer), _countof(buffer)-1, format, vl);

	MessageBoxW(NULL, buffer, L"MessageBox", MB_OK);
}

void _patch(DWORD dwAddress,BYTE* bData,int iSize)
{
	DWORD dwProtect[2];
	VirtualProtect((PVOID)dwAddress,iSize,PAGE_EXECUTE_READWRITE,&dwProtect[0]);
	memcpy((PVOID)dwAddress,bData,iSize);
	VirtualProtect((PVOID)dwAddress,iSize,dwProtect[0],&dwProtect[1]);
}

void PatchDWORD(DWORD dwAddress, DWORD to)
{
	char* cBytes = (char*)&to;
	BYTE bData[] = { cBytes[0], cBytes[1], cBytes[2], cBytes[3] };

	patch(dwAddress, bData);
}

void PatchPointer(DWORD dwAddress, void* to)
{
	PatchDWORD(dwAddress, (DWORD)to);
}

void _RedirectFunction(DWORD dwAddress, void* to)
{
	DWORD movement = (DWORD)to - dwAddress - 5;

	char* cBytes = (char*)&movement;

	BYTE bData[] = { 0xE9, cBytes[0], cBytes[1], cBytes[2], cBytes[3] };

	patch(dwAddress, bData);
}

// Clears memory
void SALimitAdjuster::ReleaseMemory()
{	
	delete CDIMAGES_HANDLES;
	delete CDIMAGE_NAMES;
}

	// IMG : max number of IMG archives
void SALimitAdjuster::SetMaxNumberOfIMGarchives(int iIMGarchives)
{
	// printf_MessageBox(L"IMG archives: %d", iIMGarchives);

	delete CDIMAGES_HANDLES;
	delete CDIMAGE_NAMES;

	CDIMAGES_HANDLES = new HANDLE[iIMGarchives];
	CDIMAGE_NAMES = new tCDimageName[iIMGarchives];

	///// Patch references to CDIMAGES_HANDLES
	// CStreaming::CloseCdImages
	PatchPointer(0x408188 + 3, CDIMAGES_HANDLES);
	PatchPointer(0x408196 + 3, CDIMAGES_HANDLES);

	// CStreaming::GetCDImageName
	PatchPointer(0x4081C4 + 3, CDIMAGES_HANDLES);

	// CStreaming::AddCdimage
	PatchPointer(0x408212 + 3, CDIMAGES_HANDLES);
	PatchPointer(0x40821E + 3, CDIMAGES_HANDLES);

	// sub_4084F0	
	PatchPointer(0x408518 + 3, CDIMAGES_HANDLES);

	// CStreaming::GetCDImageSize
	PatchPointer(0x4086E0 + 1, CDIMAGES_HANDLES);

	// CStreaming::Init
	PatchPointer(0x4087DF + 3, CDIMAGES_HANDLES);
	PatchPointer(0x4087EB + 3, CDIMAGES_HANDLES);
	PatchPointer(0x408858 + 3, CDIMAGES_HANDLES);
	PatchPointer(0x408866 + 3, CDIMAGES_HANDLES);
	
	///// Patch references to CDIMAGES_NAMES
	// CStreaming::GetCDImageName
	PatchPointer(0x4081D1 + 1, CDIMAGE_NAMES);

	// CStreaming::AddCdimage
	PatchPointer(0x408239 + 1, CDIMAGE_NAMES);
	PatchPointer(0x4087FF + 2, CDIMAGE_NAMES);
}


char cPath[_MAX_PATH];
const char relativeConfigFilename[_MAX_PATH] = "IMGlimitAdjuster.ini";

char GTAVCexePath[MAX_PATH];

void InitInstance(HANDLE hModule) {
	char cTmp[100];

	GetFullPathNameA(relativeConfigFilename, _countof(cPath), cPath, NULL);

	
	// IPL: inst
	GetPrivateProfileStringA("STATIC LIMITS", "IMG Archives", NULL, cTmp, sizeof(cTmp), cPath);

	if(strlen(cTmp))
		SALimitAdjuster::SetMaxNumberOfIMGarchives(atoi(cTmp));
}

void ExitInstance()
{
	SALimitAdjuster::ReleaseMemory();
}