#include "main.h"
#include "StdInc.h"

//Patches of this section are loaded through PatchMiscData()


//=======================Loading Game Screen Patches By Elvis============================

/* This patches are subject to change in the future
It Patches how the loading bar should be displayed */

char* GameStateText;
char* loadingStageText;

DWORD dwHookJmp4A6A88 = 0x4A6A88;
void _declspec (naked) GetLoadingScreenMsg(){
	_asm{
		mov eax, dword ptr[esp + 0x70]
			mov GameStateText, eax
			mov eax, dword ptr[esp + 0x74]
			mov loadingStageText, eax
			cmp[esp + 0x70], 0
			jmp dwHookJmp4A6A88
	}
}

wchar_t MsgBuffer[256];
void DisplayLoadingScreenMsg(){
	if (GameStateText){
		CFont::SetPropOn();
		CFont::SetBackgroundOff();
		CFont::SetScale(ResolutionX / 640.0f * 0.4f, ResolutionY / 448.0f * 0.8f);

		CFont::SetRightJustifyOff();
		CFont::SetJustifyOff();
		CFont::SetRightJustifyWrap(999.0f);
		CFont::SetBackGroundOnlyTextOff();
		CFont::SetFontStyle(1);
		CFont::SetCentreOn();
		CFont::SetBackgroundOff();
		CFont::SetColor(&CRGBA(255, 255, 255, 255));
		CFont::AsciiToUnicode(GameStateText, MsgBuffer);
		CFont::PrintString(0.5f * (float)ResolutionX, 335.0f / 480.0f * (float)ResolutionY, MsgBuffer);
		CFont::AsciiToUnicode(loadingStageText, MsgBuffer);
		CFont::PrintString(0.5f * (float)ResolutionX, 365.0f / 480.0f * (float)ResolutionY, MsgBuffer);
		CFont::SetScale(ResolutionX / 640.0f * 0.2f, ResolutionY / 448.0f * 0.8f);
	}

	_asm mov eax, 0x550250
	_asm call eax
}


void PatchLoadingBarDisplayOnly()
{
	*(float*)0x68E70C = 550.0f;  // length (50)
	*(float*)0x68E710 = 5.0f;    // Width (25)
	*(float*)0x68E708 = 115.0f;   // distance from bottom (27)
	*(float*)0x68E704 = 275.0f;  // distance from right (1)
	*(float*)0x68E6FC = 0.65979f;//Progress Step

	//Background color

	CMemory::InstallPatch<byte>(0x4A6B7C, 113); //B
	CMemory::InstallPatch<byte>(0x4A6B7E, 68); //G
	CMemory::InstallPatch<byte>(0x4A6B80, 11); //R


	//Foreground color (call @ 0x4A6C37)
	CMemory::InstallPatch<byte>(0x4A6C29, 171);//B
	CMemory::InstallPatch<byte>(0x4A6C2E, 113);//G
	CMemory::InstallPatch<byte>(0x4A6C33, 47); //R

	CMemory::NoOperation(0x4A6A83, 5);
	CMemory::InstallCallHook(0x4A6A83, &GetLoadingScreenMsg, ASM_JMP);

	CMemory::NoOperation(0x4A6D47, 5);
	CMemory::InstallCallHook(0x4A6D47, &DisplayLoadingScreenMsg, ASM_CALL);
}

//=======================End of Elvis's patches===================================


// This is the pointer to the new CarGenerator Store Buffer
CCarGenerator* pNewCarGeneratorBuffer = NULL;

// ----------------------------------------------------------------
// CTheCarGenerator Array Limit patches
//ToDos :
// Nothing is done for CCarGenerators::LoadAllCarGenerators() 
// Nothing is done for CCarGenerators::SaveAllCarGenerators()
// MayBe make this like SA loads from IPL?

#define ARRLEN(a) (sizeof(a)/sizeof(a[0])) 


// This is the new 4-byte counter for CTheCarGenerators
int CTheCarGenerators__ProcessCounter;

DWORD dwHookJmp5A6CB4 = 0x005A6CB4;
void _declspec (naked) HookCTheCarGenerators4bCounter() {
	_asm{
		inc CTheCarGenerators__ProcessCounter
			cmp CTheCarGenerators__ProcessCounter, 4
			jnz Label_if_not_4
			mov CTheCarGenerators__ProcessCounter, 0
		Label_if_not_4:
		mov ebx, CTheCarGenerators__ProcessCounter
			jmp dwHookJmp5A6CB4
	}
}


void PatchCarGeneratorLimit() {
	static uint32_t dwCarGeneratorDataRefs[] =
	{
		0x4537A9, 0x5A69DB, 0x5A69E6, 0x5A69F1, 0x5A69FC, 0x5A6A07, 0x5A6A14, 0x5A6A21,
		0x5A6A2A, 0x5A6A33, 0x5A6A3C, 0x5A6A49, 0x5A6A56, 0x5A6A61, 0x5A6A6C, 0x5A6A79,
		0x5A6A85, 0x5A6B16, 0x5A6B22, 0x5A6B2A, 0x5A6B35, 0x5A6B40, 0x5A6B4C, 0x5A6B5C,
		0x5A6B65, 0x5A6B6E, 0x5A6B77, 0x5A6B85, 0x5A6B95, 0x5A6BA1, 0x5A6BAC, 0x5A6BB8,
		0x5A6BC1, 0x5A6C5F, 0x5A6CC3
	};

	// This is the new CarGenerator Array Buffer
	pNewCarGeneratorBuffer = (CCarGenerator*)malloc(0x2C * MAXPARKEDCARS);

	for (int i = 0; i<ARRLEN(dwCarGeneratorDataRefs); i++){
		DWORD dwPrevProt = CMemory::UnProtect(dwCarGeneratorDataRefs[i], 4);
		*((uint32_t*)dwCarGeneratorDataRefs[i]) -= 0xA0DC94;
		*((uint32_t*)dwCarGeneratorDataRefs[i]) += (uint32_t)pNewCarGeneratorBuffer;
		CMemory::RestoreProtection(dwCarGeneratorDataRefs[i], 4, dwPrevProt);
	}

	// Patch cmp instruction in CreateCarGenerator
	CMemory::InstallPatch<uint32_t>(0x5A6C16, MAXPARKEDCARS);

	// Create a byte array of assembly instruction
	uint8_t HookCCarGeneratorsInitBuffer[] = {
		0xC7, 0x05, 0x90, 0x90, 0x90, 0x90, 0x00, 0x00, 0x00, 0x00,
		0xC7, 0x05, 0xA8, 0xF2, 0x97, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xC3,
	};

	// Patch the byte array to CTheCarGenerators::Init(void) section
	(*(uint32_t*)(HookCCarGeneratorsInitBuffer + 2)) = (uint32_t)&CTheCarGenerators__ProcessCounter;
	CMemory::UnProtect(0x5A6BF1, sizeof(HookCCarGeneratorsInitBuffer));
	memcpy((void*)0x005A6BF1, HookCCarGeneratorsInitBuffer, sizeof(HookCCarGeneratorsInitBuffer));

	CMemory::NoOperation(0x005A6C97, 6);
	CMemory::InstallCallHook(0x005A6C97, &HookCTheCarGenerators4bCounter, ASM_JMP);
}
//-----------------------------------------------------------------------------


void PatchMiscData(){
	PatchLoadingBarDisplayOnly();
	PatchCarGeneratorLimit();

	// Change the fade-out color to black upon death/busted.
	CMemory::InstallPatch<BYTE>(0x42BC1A, 0);
	CMemory::InstallPatch<BYTE>(0x42BC1F, 0);
	CMemory::InstallPatch<BYTE>(0x42BC24, 0);

	// Backward face culling.
	CMemory::InstallPatch<byte>(0x4C9E5F, 0);
	CMemory::InstallPatch<byte>(0x4C9F08, 0);
	CMemory::InstallPatch<byte>(0x4C9F5D, 0);
	CMemory::InstallPatch<byte>(0x4CA157, 0);
	CMemory::InstallPatch<byte>(0x4CA199, 0);
	CMemory::InstallPatch<byte>(0x4E0146, 0);
	
	// Initialize the lod/default limit for the detail distance (infusions). 
	CMemory::InstallPatch<float>(0x69022C, 800.0f); //Default 
	CMemory::InstallPatch<float>(0x698FE0, 800.0f); //LOD

	// Initialize the draw limit for the draw distance.
	CMemory::InstallPatch<float>(0x86964C, 30.0);
	CMemory::InstallPatch<float>(0x690220, 30.0);

	// Apply the streaming memory patch.
	CMemory::InstallPatch<DWORD>(0x94DD54, 1024 * 1024 * 1024);

	// These patches allow us to have a custom horizon, replacing the "ugly grey" color. 
	CMemory::NoOperation(0x53F4AC, 0x05);
	CMemory::NoOperation(0x53F4DE, 0x05);
	CMemory::NoOperation(0x53F514, 0x05);
	CMemory::NoOperation(0x53F519, 0x07);
	CMemory::InstallPatch<DWORD>(0x978574, 0xFF646464);

	//  Collision Limits. default 44000
	static char colbuffer[250000]; // increase bytes :)

	CMemory::InstallPatch<char*>(0x48A84D, colbuffer);
	CMemory::InstallPatch<char*>(0x48A8CB, colbuffer);
	CMemory::InstallPatch<char*>(0x48A987, colbuffer);
	CMemory::InstallPatch<char*>(0x48AA1D, colbuffer);
	CMemory::InstallPatch<char*>(0x48AA55, colbuffer);
	CMemory::InstallPatch<char*>(0x48AAFC, colbuffer);
	CMemory::InstallPatch<char*>(0x48AB31, colbuffer);
	CMemory::InstallPatch<char*>(0x48AB61, colbuffer);

	// Misc Water hacks. Z level negative underwater.
	//  CMemory::InstallPatch<float>(0x6912DC, 240.0); // Z depth level.
	//	CMemory::InstallPatch<float>(0x78D658, 6.0); //Visible tiles.
	//	CMemory::InstallPatch<float>(0x78D65C, 6.0); //Physical tiles.

	// Initialize Gravity, (float). 0.008f is normal.
	CMemory::InstallPatch<float>(0x68f5f0, 0.008f);

	//   6F 12 83 3B	0.004	half gravity
	//	 6F 12 03 3B	0.002	quater gravity
	//	 6F 12 83 3C	0.016	double gravity
	//	 6F 12 03 3D	0.032	fourfold gravity
	//	 6F 12 03 BC  - 0.008	negative gravity
	//	 00 00 80 3F	1.0	    very high gravity
	// 	 17 B7 D1 38	0.0001	very low gravity

	// Initialize Subtitles, byte (0 off 1 on).
	CMemory::InstallPatch<byte>(0x869650, 0);

	// Patch Alpha Objects Limit
	// needs to be added, seeking permission to use the code.

	// Initialize the default limit for the detail distance (Trilogy).
	//CMemory::InstallPatch<float>(0x69022C, 300.0f);

	// Initialize the lod limit for the detail distance (Trilogy).
	//CMemory::InstallPatch<float>(0x698FE0, 300.0f);

	// Initialize Frame Limiter, byte (0 off 1 on).
	CMemory::InstallPatch<byte>(0x869655, 1);

	// Initialize Hud Mode, byte (0 off 1 on).
	//CMemory::InstallPatch<byte>(0x86963A, 0);
	//CMemory::InstallPatch<byte>(0xA10B39, 1);

	// Initialize Police chopper chasing player, byte (0 off 1 on).
	CMemory::InstallPatch<byte>(0xA10ADB, 1);

	// Initialize Tick rate, need currect values between low and fast. 
	//CMemory::InstallPatch<byte>(0x69102E, 0);

	// police debug code = 0/1 

	//static char policeflag[1];
	//CMemory::InstallPatch<char*>(0x602E00, policeflag);
	//Debug log = effects all other vehicles. so we are on the right track.

	// All cops have Chainsaws, (float). 4 is default.
	//CMemory::InstallPatch<byte>(0x4ED772, 11);
	//CMemory::InstallPatch<byte>(0x4EC21D, 11);
	//CMemory::InstallPatch<byte>(0x4EC228, 11);

	// Initialize spawned taxi light status, byte (0 off 1 on).
	//CMemory::InstallPatch<byte>(0xA10ABB, 1);

	// Police Car light corona. 
	//CMemory::InstallPatch<float>(0x69a633, 600.0);
	//CMemory::InstallPatch<float>(0x69a65c, 600.0);

	/* work in progress */
	/*
	static char policeweap[11];
	CMemory::InstallPatch<char*>(0x4ec1e7, policeweap);
	CMemory::InstallPatch<char*>(0x4ee108, policeweap);
	CMemory::InstallPatch<char*>(0x4ec21d, policeweap);
	CMemory::InstallPatch<char*>(0x4ec228, policeweap);
	CMemory::InstallPatch<char*>(0x4ec21d, policeweap);
	CMemory::InstallPatch<char*>(0x4ec228, policeweap);
	*/

	/*  to be checked later.

	//Police Weapons:
	0x4EC21D
	0x4EC228

	//Swat Weapons:
	0x4ed7d1
	0x4ed7dc

	//Vicechee Weapons
	0x4ed8bc
	0x4ed8c3

	//Fbi weapons
	0x4ed820
	0x4ed827

	//Army weapons
	0x4ed842
	0x4ed84d

	//The skins
	Police
	0x4ed762
	0x4ed76b

	//Swat
	0x4ed7c3

	//Fbi
	0x4ed812

	//Army
	0x4ed834

	CMemory::RAND_Random();
	{0, -Unarmed
	1, -BrassKnuckle
	2, -ScrewDriver
	3, -GolfClub
	4, -NightStick
	5, -Knife
	6, -BaseballBat
	7, -Hammer
	8, -Cleaver
	9, -Machete
	10, -Katana
	11, -Chainsaw
	};
	*/

	// Initialize Radar Width (byte)
	//CMemory::InstallPatch<byte>(0x68FD16, 1);
	// only radar ring visible
	//CMemory::InstallPatch<byte>(0x68FD17, 1);

	// Radar Size (XY)
	//CMemory::InstallPatch<float>(0x68FD28, 20.0);
	//CMemory::InstallPatch<float>(0x68FD2C, 20.0); //X pos
	//CMemory::InstallPatch<float>(0x68FD34, 20.0); //Y Pos
	//CMemory::InstallPatch<float>(0x68FD38, 4.0); //X shape
	//CMemory::InstallPatch<float>(0x68FD3C, 4.0); //Y shape

	// Initialize Game engine clock speed (float)
	CMemory::InstallPatch<float>(0x68F1F0, 60.0f); // set 50.0 default
	CMemory::InstallPatch<float>(0x68F1F4, 6.0f); // multiplier 5.0 default
	CMemory::InstallPatch<float>(0x68F1F8, 60.0f); // set 50.0 default
	CMemory::InstallPatch<float>(0x68F1FC, 6.0f); // multiplier 5.0 default

	// Controls removing cars that are far away, needs looking at.
	//CMemory::InstallPatch<byte>(0x426640, 0);

	// Vehicle speed multiplier 
	CMemory::InstallPatch<float>(0x821F7C, 4.0f);

	// Disable the calls to the default ingame menu. (works)
	//CMemory::NoOperation(0x49A062, 0x7);
	//CMemory::NoOperation(0x499BEC, 0x7);
	//CMemory::NoOperation(0x4994FA, 0x7);
	//CMemory::NoOperation(0x497378, 0x7);

	// Igorx's Memu hacks.
	//0x4A2831 0x4A292B 0x4A2A34 0x4A2DB9 0x4A2EB3 0x4A2FC2 0x4A30D1 calls bar functions in menu

	DWORD _old;

	VirtualProtect((LPVOID)0x4A2831, 5, PAGE_READWRITE, &_old);
	memset((PVOID)0x4A2831, 0x90, 5);
	VirtualProtect((LPVOID)0x4A292B, 5, PAGE_READWRITE, &_old);
	memset((PVOID)0x4A292B, 0x90, 5);
	VirtualProtect((LPVOID)0x4A2A34, 5, PAGE_READWRITE, &_old);
	memset((PVOID)0x4A2A34, 0x90, 5);
	VirtualProtect((LPVOID)0x4A2DB9, 5, PAGE_READWRITE, &_old);
	memset((PVOID)0x4A2DB9, 0x90, 5);
	VirtualProtect((LPVOID)0x4A2EB3, 5, PAGE_READWRITE, &_old);
	memset((PVOID)0x4A2EB3, 0x90, 5);
	VirtualProtect((LPVOID)0x4A2FC2, 5, PAGE_READWRITE, &_old);
	memset((PVOID)0x4A2FC2, 0x90, 5);
	VirtualProtect((LPVOID)0x4A30D1, 5, PAGE_READWRITE, &_old);
	memset((PVOID)0x4A30D1, 0x90, 5);

	//0x90 is nop. It replaces the call function to figure out the black bars (2d effects) on the game menu.

	// Initialize Set Actor, byte (0 off 1 on).

	// Initialize Translucent Text Box OnScreen, byte (0 off 1 on).
	//CMemory::InstallPatch<byte>(0xA10B83, 0);

	// Initialize ambient lighting.
	//CMemory::InstallPatch<float>(0x698A8C, 20.0);

	// Initialize Wide Screen, byte (0 off 1 on).
	//CMemory::InstallPatch<byte>(0x869652, 1); // needs looking at.
	//CMemory::InstallPatch<byte>(0x7E46F5, 1); // cinamatic borders.

	// AlphaEntityList objects -- still needs patching..??? saved for tomorrow, unless you know the addresses.


	//typedef dummy_object<0x8>;
	/*
	static char alphabuffer[40000];
	CMemory::InstallPatch<char*>(0x5828DB + 1, alphabuffer);

	if (*(DWORD*)0x97F30C == 0x4CBEBC20)
	{
	CMemory::InstallPatch<char*>(0x582870, alphabuffer);
	CMemory::InstallPatch<char*>(0x5828A0, alphabuffer);
	}

	*/
}