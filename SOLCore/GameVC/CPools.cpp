/*
 *  Dynamic Pools Limit Adjuster
 *  Copyright (C) 2013 ThirteenAG <thirteenag@gmail.com>
 *  Copyright (C) 2014 LINK/2012 <dma_2012@hotmail.com>
 *  Licensed under the MIT License (http://opensource.org/licenses/MIT)
 */
#include "../StdInc.h"
#include "../LimitAdjuster.h"
#include "utility/PoolAdjuster.hpp"
#include "utility/PoolAdjusterDynamic.hpp"
using namespace injector;



/*
 *  Key: [...]
 *  Value: Integer
 *  Game: III, VC, SA
 */



// Pools initialization on each game happens at:
// 3  at 0x4A1770
// VC at 0x4C0270
// SA at 0x550F10

enum
{
	PtrNode,            // GTA3 / VC
	PtrNodeSingle,      // SA
	PtrNodeDouble,      // SA
	EntryInfoNode,
	Peds,
	Vehicles,
	Buildings,
	Objects,
	Dummys,
	ColModel,           // VC / SA
	
	// VC-only below
	Treadables,
	AudioScriptObj,

	// SA-only below
	Task,
	Event,
	PointRoute,
	PatrolRoute,
	NodeRoute,
	TaskAllocator,
	PedIntelligence,
	PedAttractors,
	
	MAX_POOLS
};



/*
 *  Base for pools adjusters here
 *  Provides an ChangeLimit() and GetUsage() interface by taking the PoolAdjusterBase* (NOT >Pools<AdjusterBase) from the limit user data
 *  Use DEFINE_POOL_LIMIT on inherited objects to define a proper Limit with proper user data
 */
struct PoolsAdjusterBase : public Adjuster
{
    PoolAdjusterBase* GetUserData(int id)
    {
        for(auto* p = GetLimits(); p->name; ++p)
            if(p->id == id) return p->GetUserData<PoolAdjusterBase*>();
        return nullptr;
    }

	// Sets the limit
	void ChangeLimit(int id, const std::string& value)
	{
   		if (id == PtrNode || id == Buildings)
		injector::ReadMemory<int>((0x4C0284) > 50000 || injector::ReadMemory<int>(0x4C0309) > 50000);    // Vice Cry 1.8 compat

        if(IsUnlimited(value))
            return GetUserData(id)->MakeUnlimited();
        else
            return GetUserData(id)->ChangeLimit(std::stoul(value));
	}

    bool GetUsage(int id, std::string& usagebuf)
    {
        return GetUserData(id)->GetUsage(usagebuf);
    }
};

#define DEFINE_POOL_LIMIT(limit) DEFINE_LIMIT_U(limit, (PoolAdjusterBase*) &m_##limit)

/*
 *  Pool Adjuster for GTA VC
 */
struct PoolsAdjusterVC : public PoolsAdjusterBase
{
    PoolAdjuster<0x4C0288> m_PtrNode;
    PoolAdjuster<0x4C02AA> m_EntryInfoNode;
    PoolAdjuster<0x4C02CC> m_Peds;
    PoolAdjuster<0x4C02EB> m_Vehicles;
    PoolAdjuster<0x4C030D> m_Buildings;
    PoolAdjuster<0x4C034E> m_Objects;
    PoolAdjuster<0x4C0370> m_Dummys;
    PoolAdjuster<0x4C03B4> m_ColModel;
    PoolAdjuster<0x4C032C> m_Treadables;
    PoolAdjuster<0x4C0392> m_AudioScriptObj;

	// Limit Names
	const Limit* GetLimits()
	{
		static Limit lm[] =
			{
				DEFINE_POOL_LIMIT(PtrNode),
				DEFINE_POOL_LIMIT(EntryInfoNode),
				DEFINE_POOL_LIMIT(Peds),
				DEFINE_POOL_LIMIT(Vehicles),
				DEFINE_POOL_LIMIT(Buildings),
				DEFINE_POOL_LIMIT(Objects),
				DEFINE_POOL_LIMIT(Dummys),
				DEFINE_POOL_LIMIT(ColModel),
				DEFINE_POOL_LIMIT(Treadables),
				DEFINE_POOL_LIMIT(AudioScriptObj),
				FINISH_LIMITS()
			};
			return lm;
		return nullptr;
	}

// Instantiate the adjuster on the global scope
} adjuster_Pools_VC;


