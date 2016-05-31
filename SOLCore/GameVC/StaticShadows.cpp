/*
 * Static Shadows Limit Adjuster
 * Copyright (C) 2014 Wesser
 * http://gtaforums.com/topic/675664-staticshadow-limit/
 */
#include "../LimitAdjuster.h"
#include "misc\hook.h"

using namespace Hook;

DWORD SSHADS_LIMIT;

//#define SSHADS_LIFETIME		5000
//#define SSHADS_UPDATEFIX

void patch_706ED0();
void patch_706FE2();
void patch_7072C0();
void patch_707391();
void patch_70990A();
void patch_70BB3C();
void patch_70BB60();
void patch_70BB67();
void patch_70BC49();

DWORD ext_706ED0 = 0x706ED0;
DWORD ext_706FEB = 0x706FEB;
DWORD ext_706FF8 = 0x706FF8;
DWORD ext_7072CC = 0x7072CC;
DWORD ext_7072D2 = 0x7072D2;
DWORD ext_70739C = 0x70739C;
DWORD ext_70991E = 0x70991E;
DWORD ext_70BA6F = 0x70BA6F;
DWORD ext_70BB41 = 0x70BB41;
DWORD ext_70BB43 = 0x70BB43;
DWORD ext_70BB65 = 0x70BB65;
DWORD ext_70BB71 = 0x70BB71;
DWORD ext_70BC53 = 0x70BC53;

std::vector<char> asShadowsStored;
std::vector<char> aStaticShadows;
std::vector<char> aPermanentShadows;

void PatchShadowsStored()
{
    AdjustPointer(0x400000+0x3073B0, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30A9EE, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30AA10, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30AA18, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B0EF, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B166, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B17A, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B18D, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B23A, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B2B1, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B2C2, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B2D1, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B38E, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B404, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B40F, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B424, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B4EA, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B569, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B578, &asShadowsStored[0], 0xC40430, 0xC40DF0);
    AdjustPointer(0x400000+0x30B594, &asShadowsStored[0], 0xC40430, 0xC40DF0);

    RedirectJump(0x707391, patch_707391);
    Nop(0x7073AA, 1);								// mov eax, esi
    SetUChar(0x7073AB, 0x8B);
    Nop(0x707439, 1);								// inc esi
    Nop(0x707441, 1);								// mov ds:[0C403DCh], esi
    Nop(0x70A9E0, 1);								// mov ecx, ds:[0C403DCh]
    SetUChar(0x70A9E1, 0x8B);
    Nop(0x70AA3F, 2);								// mov eax, ds:[0C403DCh]
    SetUChar(0x70AA41, 0xA1);
    Nop(0x70ADF9, 1);								// mov ecx, ds:[0C403DCh]
    SetUChar(0x70ADFA, 0x8B);
    Nop(0x70B6C1, 1);								// mov ecx, ds:[0C403DCh]
    SetUChar(0x70B6C2, 0x8B);
    Nop(0x70B71A, 1);								// mov ds:[0C403DCh], ebx
}

void PatchStaticShadows()
{
    AdjustPointer(0x400000+0x306E7A, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x306E88, &aStaticShadows[0], 0xC4A030, 0xC4AC70, 64*48, aStaticShadows.size());
    AdjustPointer(0x400000+0x307F41, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x307F96, &aStaticShadows[0], 0xC4A030, 0xC4AC70, 64*48, aStaticShadows.size());
    AdjustPointer(0x400000+0x308361, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x308376, &aStaticShadows[0], 0xC4A030, 0xC4AC70, 64*48, aStaticShadows.size());
    AdjustPointer(0x400000+0x30837D, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x3083BC, &aStaticShadows[0], 0xC4A030, 0xC4AC70, 64*48, aStaticShadows.size());
    AdjustPointer(0x400000+0x308614, &aStaticShadows[0], 0xC4A030, 0xC4AC70, 64*48, aStaticShadows.size());
    AdjustPointer(0x400000+0x30866E, &aStaticShadows[0], 0xC4A030, 0xC4AC70, 64*48, aStaticShadows.size());
    AdjustPointer(0x400000+0x30B73E, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B746, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B751, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B76B, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B77A, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B931, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B937, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B966, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B974, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B97B, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B98A, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B991, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30B998, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BB26, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BB2E, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BB56, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BB84, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BB8E, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BB98, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBA3, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBAD, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBB7, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBBD, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBC3, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBC9, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBD2, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBF2, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BBFC, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC06, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC13, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC19, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC20, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC26, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC31, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC5C, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC72, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BC99, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BCB1, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BCCA, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BCDB, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BCEC, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BCFD, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD0A, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD24, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD2E, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD39, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD43, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD4E, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD59, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD63, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD6F, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD7A, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD80, &aStaticShadows[0], 0xC4A030, 0xC4AC30);
    AdjustPointer(0x400000+0x30BD87, &aStaticShadows[0], 0xC4A030, 0xC4AC30);

    RedirectOffset(0x53CA26, patch_706ED0);
    SetPointer(0x706E95, nullptr);
    RedirectShortJump(0x706E99, 0x706EB5);
    RedirectJump(0x70990A, patch_70990A);
    Nop(0x70B736, 1);								// mov esi, [esp+3Ch]
    SetUChar(0x70B737, 0x8B);
    Nop(0x70BB20, 1);								// mov eax, edi
    SetUChar(0x70BB21, 0x8B);
    RedirectJump(0x70BB3C, patch_70BB3C);
    Nop(0x70BB50, 1);								// mov ecx, edi
    SetUChar(0x70BB51, 0x8B);
    RedirectJump(0x70BB60, patch_70BB60);
    RedirectJump(0x70BB67, patch_70BB67);
    Nop(0x70BB7E, 1);								// mov esi, edi
    SetUChar(0x70BB7F, 0x8B);
    RedirectJump(0x70BC49, patch_70BC49);
    Nop(0x70BC56, 1);								// mov ecx, edi
    SetUChar(0x70BC57, 0x8B);

#ifdef SSHADS_LIFETIME
#if !SSHADS_LIFETIME
    RedirectShortJump(0x707F56, 0x707F90);
#elif SSHADS_LIFETIME != 5000
    SetUInt(0x707F60, SSHADS_LIFETIME);
#endif
#endif

    // Always update static shadows of specific moving thingys.
#ifdef SSHADS_UPDATEFIX
    // Low detailed vehicle shadow (default distance: 0.1f).
    SetPointer(0x70C2B8, 0x858B50);
    SetFloat(0x70C2D5, 0.0f);
    // Vehicle headlight/s (default distance: 0.4f).
    SetPointer(0x70C681, 0x858B50);
    SetFloat(0x70C6A9, 0.0f);
#endif
}

void PatchPermanentShadows()
{
    AdjustPointer(0x400000+0x306EB5, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x306EC5, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x306F64, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x306FBA, &aPermanentShadows[0], 0xC4AC30, 0xC4B720, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x306FF3, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x3072DF, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x3072E5, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x3072F1, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30730D, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307317, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307321, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30732C, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307336, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307341, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30734B, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307355, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30735F, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307369, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307375, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30737B, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x3074F0, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30753E, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x3075AF, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307613, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x307631, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30763D, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x307770, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30777B, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x30C957, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CA84, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x30CAB0, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CAC5, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CACC, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CAD2, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CAE2, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CAF1, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CAFA, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CB03, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CB3D, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x30CB5E, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CB80, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CB8C, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CB94, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CB9A, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CBA3, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CBBB, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CBC4, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CBCD, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CBFE, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CC09, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CC2C, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());
    AdjustPointer(0x400000+0x30CC50, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CC5D, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CC90, &aPermanentShadows[0], 0xC4AC30, 0xC4B6B0);
    AdjustPointer(0x400000+0x30CCA0, &aPermanentShadows[0], 0xC4AC30, 0xC4B6E8, 56*48, aPermanentShadows.size());

    RedirectJump(0x706FE2, patch_706FE2);
    RedirectJump(0x7072C0, patch_7072C0);
    SetUInt(0x70CC86, aPermanentShadows.size());
}

void __declspec(naked) patch_706ED0()
{
    __asm
    {
        push esi
        push edi
        mov edi, ds:[0C403D8h]
        test edi, edi
        jz loc_706ED0
        align 16
walk_free_list:
        mov esi, [edi+54h]
        push edi
        call dword ptr free
        add esp, 4
        mov edi, esi
        test edi, edi
        jnz walk_free_list
loc_706ED0:
        pop edi
        pop esi
        jmp ext_706ED0
    }
}

void __declspec(naked) patch_706FE2()
{
    __asm
    {
        cmp edi, SSHADS_LIMIT
        jl loc_7072D2
        jmp ext_706FEB
loc_7072D2:
        jmp ext_7072D2
    }
}

void __declspec(naked) patch_7072C0()
{
    __asm
    {
        cmp eax, SSHADS_LIMIT
        jl loc_706FF8
        cmp edi, SSHADS_LIMIT
        jmp ext_7072CC
loc_706FF8:
        jmp ext_706FF8
    }
}

void __declspec(naked) patch_707391()
{
    __asm
    {
        mov esi, ds:[0C403DCh]
        cmp esi, SSHADS_LIMIT
        jmp ext_70739C
    }
}

void __declspec(naked) patch_70990A()
{
    __asm
    {
        jnz free_list
        push 104				// sizeof(CPolyBunch)
        call dword ptr malloc
        add esp, 4
        mov ebx, eax
        jmp loc_70991E
free_list:
        mov ecx, [ebx+54h]
        mov ds:[0C403D8h], ecx
loc_70991E:
        lea eax, [ebx+54h]
        test ebp, ebp
        jmp ext_70991E
    }
}

void __declspec(naked) patch_70BB3C()
{
    __asm
    {
        inc edi
        cmp edi, SSHADS_LIMIT
        jmp ext_70BB41
    }
}

void __declspec(naked) patch_70BB60()
{
    __asm
    {
        inc edi
        cmp edi, SSHADS_LIMIT
        jmp ext_70BB65
    }
}

void __declspec(naked) patch_70BB67()
{
    __asm
    {
        cmp edi, SSHADS_LIMIT
        je loc_70BA6F
        jmp ext_70BB71
loc_70BA6F:
        jmp ext_70BA6F
    }
}

void __declspec(naked) patch_70BC49()
{
    __asm
    {
        cmp edi, SSHADS_LIMIT
        jge loc_70BB43
        jmp ext_70BC53
loc_70BB43:
        jmp ext_70BB43
    }
}


DWORD _EAX;
WORD _EBX;
void patch_56E6C0();
void patch_56967E();
void patch_56E924();
void patch_56E93B();
void patch_56EB34();
void patch_56EB43();
void patch_56F09B();
void patch_56F24A();
void patch_56C71B();
void patch_56C7BC();
void patch_56CBA8();
void patch_56CC10();
void patch_568F17();

void PatchStaticShadowsVC()
{
        aStaticShadows.resize(SSHADS_LIMIT * 64);

        AdjustPointer(0x56966E, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C56B, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56E92C, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB1D, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB71, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EFFD, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F2AF, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x569618, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x569664, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C73C, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C7E6, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56E934, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EAEA, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB13, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB3C, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EC24, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F007, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x569634, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EADD, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EC19, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C3D3, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56E94F, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56E998, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBBD, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C3E1, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56E972, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56E9C3, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBC6, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C3EF, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56E9EE, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBCF, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C387, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C5E1, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C632, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA1A, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBD9, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C3B9, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C5DB, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C62C, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA32, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBE3, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C371, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C5D5, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C626, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA4A, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBED, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C3A3, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C5C9, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C620, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA62, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBFB, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C5BB, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C612, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EAB7, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBAD, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C5B5, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C60C, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EAC6, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBB5, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C7AA, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C814, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C81E, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA87, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB81, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C931, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA93, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB8D, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C757, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C7F8, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C802, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA7D, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB77, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C9C2, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EAA1, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EB97, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C9B7, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EAAB, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBA1, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C9D0, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EAB1, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EBA7, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x569622, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x569679, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EA76, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EC05, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C6E5, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C749, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56CB9A, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56962B, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EAD0, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56EC0C, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F011, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F01B, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C6EC, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F025, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F02F, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C6F3, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F039, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F043, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C6FA, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F04D, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F057, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C701, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F061, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F06B, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C708, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F075, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F07F, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C70F, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F089, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56F093, &aStaticShadows[0], 0x8615A0, 0x8621A0);
        AdjustPointer(0x56C716, &aStaticShadows[0], 0x8615A0, 0x8621A0);


    RedirectJump(0x56E6C0, patch_56E6C0);
    RedirectJump(0x56967E, patch_56967E);
    RedirectJump(0x56E924, patch_56E924);
    RedirectJump(0x56E93B, patch_56E93B);
    RedirectJump(0x56EB34, patch_56EB34);
    RedirectJump(0x56EB43, patch_56EB43);
    RedirectJump(0x56F09B, patch_56F09B);
    RedirectJump(0x56F24A, patch_56F24A);
    RedirectJump(0x56C71B, patch_56C71B);
    RedirectJump(0x56C7BC, patch_56C7BC);
    RedirectJump(0x56CBA8, patch_56CBA8);


    RedirectJump(0x56CC10, patch_56CC10);
}

DWORD ext_56E6C8 = 0x56E6C8;
void __declspec(naked) patch_56E6C0()
{
    __asm
    {
        mov _EBX, bx
        mov eax, SSHADS_LIMIT
        cmp word ptr ds : [0xA10AAC], bx
        mov bx, _EBX
        jmp ext_56E6C8
    }
}

DWORD ext_569684 = 0x569684;
void __declspec(naked) patch_56967E()
{
    __asm
    {
        add     ebx, 40h
        cmp     ecx, SSHADS_LIMIT
        jmp ext_569684
    }
}

DWORD ext_56E92A = 0x56E92A;
DWORD ext_56E93B = 0x56E93B;
void __declspec(naked) patch_56E924()
{
    __asm
    {
        cmp     esi, SSHADS_LIMIT
        jge     loc_56E93B
        jmp ext_56E92A
    loc_56E93B:
        jmp ext_56E93B
    }
}

DWORD ext_56E948 = 0x56E948;
DWORD ext_56EB27 = 0x56EB27;
void __declspec(naked) patch_56E93B()
{
    __asm
    {
        cmp     esi, SSHADS_LIMIT
        jge     loc_56EB27
        mov   ebp, esi
        jmp ext_56E948
    loc_56EB27:
        jmp ext_56EB27
    }
}

DWORD ext_56EB3A = 0x56EB3A;
DWORD ext_56EB43 = 0x56EB43;
void __declspec(naked) patch_56EB34()
{
    __asm
    {
        cmp     esi, SSHADS_LIMIT
        jge      loc_56EB43
        jmp ext_56EB3A
    loc_56EB43:
        jmp ext_56EB43
    }
}

DWORD ext_56EB49 = 0x56EB49;
DWORD ext_56EB60 = 0x56EB60;
void __declspec(naked) patch_56EB43()
{
    __asm
    {
        cmp     esi, SSHADS_LIMIT
        jnz     loc_56EB60
        jmp ext_56EB49
    loc_56EB60:
        jmp ext_56EB60
    }
}

DWORD ext_56F0A4 = 0x56F0A4;
void __declspec(naked) patch_56F09B()
{
    __asm
    {
        add     edx, 200h
        cmp     eax, SSHADS_LIMIT
        jmp ext_56F0A4
    }
}

DWORD ext_56F24F = 0x56F24F;
void __declspec(naked) patch_56F24A()
{
    __asm
    {
        sub     edx, ebx
        cmp     eax, SSHADS_LIMIT
        jmp ext_56F24F
    }
}

DWORD ext_56C724 = 0x56C724;
void __declspec(naked) patch_56C71B()
{
    __asm
    {
        add     ebp, 200h
        cmp     eax, SSHADS_LIMIT
        jmp ext_56C724
    }
}


DWORD ext_56C7C1 = 0x56C7C1;
void __declspec(naked) patch_56C7BC()
{
    __asm
    {
        pop     ecx
        cmp     eax, SSHADS_LIMIT
        pop     ecx
        jmp ext_56C7C1
    }
}

DWORD ext_56CBAD = 0x56CBAD;
void __declspec(naked) patch_56CBA8()
{
    __asm
    {
        mov _EAX, eax
        mov eax, SSHADS_LIMIT
        cmp     dword ptr ds:[esp + 20h], eax
        mov eax, _EAX
        jmp ext_56CBAD
    }
}

DWORD ext_56CC15 = 0x56CC15;
void __declspec(naked) patch_56CC10()
{
    __asm
    {
        mov _EAX, eax
        mov eax, SSHADS_LIMIT
        cmp     dword ptr ds : [esp + 28h], eax
        mov eax, _EAX
        jmp ext_56CC15
    }
}

DWORD ext_568F1C = 0x568F1C;
void __declspec(naked) patch_568F17()
{
    __asm
    {
        sub     edx, ebx
        cmp     eax, SSHADS_LIMIT
        jmp ext_568F1C
    }
}


class StaticShadowsVC : public SimpleAdjuster
{
public:
    const char* GetLimitName() { return nullptr; }
    void ChangeLimit(int, const std::string& value)
    {
        SSHADS_LIMIT = std::stoi(value);
        PatchStaticShadowsVC();
    }
} StaticShadowsVC;
