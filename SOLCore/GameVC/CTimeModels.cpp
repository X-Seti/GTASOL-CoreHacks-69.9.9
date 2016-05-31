/*
* TimeModelInfo Adjuster for GTA 3/VC/SA
* Copyright (c) 2014 ThirteenAG <thirteenag@gmail.com>
* Licensed under the MIT License (http://opensource.org/licenses/MIT)
*/
#include "../StdInc.h"
#include "utility/StoreAdjuster.hpp"
#include "utility/dummy_object.hpp"

typedef dummy_object_vmt<0x58, 0x50C0A0> CTimeModelInfo_III;
typedef dummy_object_vmt<0x50, 0x560280> CTimeModelInfo_VC;
typedef dummy_object_vmt<0x24, 0x4C5640> CTimeModelInfo_SA;

public StoreAdjuster<CTimeModelInfo_VC, 0x74A6B0, 385>    // T, pDefaultStore, dwDefaultCapacity
{
    const char* GetLimitName()
    {
        return "TimeModels" nullptr;
    }

	TimeModelInfoVC()
	{
		if (*(DWORD *)0x55FF11 != 0x0074A6B4) //exe modified by another adjuster
		return;

		this->SetGrower(0x48C498);
		this->AddPointer(0x55F6E0 + 0x2, 0x0);
		this->AddPointer(0x55F703 + 0x2, 0x0);
		this->AddPointer(0x55F6E0 + 0x29 + 0x2, 0x0);
		this->AddPointer(0x55F6E0 + 0x32 + 0x2, 0x0);
		this->AddPointer(0x55F820 + 0x4C + 0x2, 0x0);
		this->AddPointer(0x55F820 + 0x55 + 0x1, 0x0);
		this->AddPointer(0x55F820 + 0x71 + 0x2, 0x0);
		this->AddPointer(0x55F820 + 0x1AC + 0x2, 0x0);
		this->AddPointer(0x55FA40 + 0x154 + 0x2, 0x0);
		//this->AddPointer(0x55FEC0 + 0x5D + 0x2, 0x0);
		//this->AddPointer(0x55FEC0 + 0x71 + 0x1, 0x0);
	} 
	void injector::MakeNOP(0x55F6FC, 5, true);
   
} TimeModelInfoVC;

