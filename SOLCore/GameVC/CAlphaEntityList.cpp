/*
* Alpha Entity List Adjuster
* Copyright (c) 2014 ThirteenAG <thirteenag@gmail.com>
* Copyright (c) 2014 Silent <zdanio95@gmail.com>
* Copyright (c) 2014 LINK/2012 <dma_2012@hotmail.com>
* Licensed under the MIT License (http://opensource.org/licenses/MIT)
*/
#include "../stdinc.h"
#include "../LimitAdjuster.h"
#include "utility/LinkListAdjuster.hpp"
#include "utility/dummy_object.hpp"
int Adjuster;

class AlphaEntityList : public Adjuster
{
public:
	void ChangeLimit(int, const std::string& value)
	{
		injector::WriteMemory(0x5828DB + 1, std::stoi(value), true);

		if (*(DWORD*)0x97F30C == 0x4CBEBC20)
		{
			auto CVisibilityPlugins__Shutdown = (void(__cdecl *)()) 0x582870;
			auto CVisibilityPlugins__Initialise = (void(__cdecl *)()) 0x5828A0;

			CVisibilityPlugins__Shutdown();
			CVisibilityPlugins__Initialise();
		}
	}
};