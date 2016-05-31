#ifndef __CORONAS
#define __CORONAS
#include <vector>
#include <map>
#include "General.h"

//#define NUM_CORONAS		20000

class CRegisteredCorona
{
public:
    CVector     Coordinates;            // Where is it exactly.
    DWORD       Identifier;             // Should be unique for each corona. Address or something (0 = empty)
    RwTexture*	pTex;                 // Pointer to the actual texture to be rendered
    float       Size;                   // How big is this fellow
    float       NormalAngle;            // Is corona normal (if relevant) facing the camera?
    float       Range;                  // How far away is this guy still visible
    float       PullTowardsCam;         // How far away is the z value pulled towards camera.
    float       HeightAboveGround;      // Stired so that we don't have to do a ProcessVerticalLine every frame
                                        // The following fields are used for trails behind coronas (glowy lights)
    float       FadeSpeed;              // The speed the corona fades in and out ##SA##
    BYTE        Red, Green, Blue;       // Rendering colour.
    BYTE        Intensity;              // 255 = full
    BYTE        FadedIntensity;         // Intensity that lags behind the given intenisty and fades out if the LOS is blocked
    BYTE        RegisteredThisFrame;    // Has this guy been registered by game code this frame
    BYTE        FlareType;              // What type of flare to render
    BYTE        ReflectionType;         // What type of reflection during wet weather
    BYTE        LOSCheck : 1;           // Do we check the LOS or do we render at the right Z value
    BYTE        OffScreen : 1;          // Set by the rendering code to be used by the update code
    BYTE        JustCreated;            // If this guy has been created this frame we won't delete it (It hasn't had the time to get its OffScreen cleared) ##SA removed from packed byte ##
    BYTE        NeonFade : 1;           // Does the guy fade out when closer to cam
    BYTE        OnlyFromBelow : 1;      // This corona is only visible if the camera is below it. ##SA##
    BYTE        bHasValidHeightAboveGround : 1;
    BYTE        WhiteCore : 1;          // This corona rendered with a small white core.
    BYTE        bIsAttachedToEntity : 1;

    CEntity*	pEntityAttachedTo;

public:
	CRegisteredCorona()
		: Identifier(0), pEntityAttachedTo(nullptr)
	{}

	void		Update();
};

class CCoronasLinkedListNode
{
private:
	CCoronasLinkedListNode*		pNext;
	CCoronasLinkedListNode*		pPrev;
	CRegisteredCorona*			pEntry;

private:
	inline void						Remove()
		{ pNext->pPrev = pPrev; pPrev->pNext = pNext; pNext = nullptr; }

public:
	inline void						Init()
		{ pNext = pPrev = this; }
	inline void						Add(CCoronasLinkedListNode* pHead)
		{ if ( pNext ) Remove();
		  pNext = pHead->pNext; pPrev = pHead; pHead->pNext->pPrev = this; pHead->pNext = this; }
	inline void						SetEntry(CRegisteredCorona* pEnt)
		{ pEntry = pEnt; }
	inline CRegisteredCorona*		GetFrom()
		{ return pEntry; }
	inline CCoronasLinkedListNode*	GetNextNode()
		{ return pNext; }
	inline CCoronasLinkedListNode*	GetPrevNode()
		{ return pPrev; }

	inline CCoronasLinkedListNode*	First()
		{ return pNext == this ? nullptr : pNext; }
};

class CCoronas
{
private:
	static std::map<unsigned int,CCoronasLinkedListNode*>	UsedMap;
	static CCoronasLinkedListNode							FreeList, UsedList;		
    static std::vector<CCoronasLinkedListNode>				aLinkedList;
    static std::vector<CRegisteredCorona>					aCoronas;
	static int&												bChangeBrightnessImmediately;
	static float&											ScreenMult;

public:
	static void							RegisterCorona(unsigned int nID, CEntity* pAttachTo, unsigned char R, unsigned char G, unsigned char B, unsigned char A, const CVector& Position, float Size, float Range, RwTexture* pTex, unsigned char flareType, unsigned char reflectionType, unsigned char LOSCheck, unsigned char unused, float normalAngle, bool bNeonFade, float PullTowardsCam, bool bFadeIntensity, float FadeSpeed, bool bOnlyFromBelow, bool bWhiteCore);
	static void							Update();
	static void							UpdateCoronaCoors(unsigned int nID, const CVector& vecPosition, float fMaxDist, float fNormalAngle);
	static void							Init();

	// Hacky hacky
	/*static inline void					PrepareForSizeChange()
	{ if ( aCoronas.size() == aCoronas.capacity() )
		{ bRemakeReferences = true; InvalidateAllReferences(); } }*/

	//static void							UpdatePointersInCode();
	//static void							InvalidateAllReferences();
	static void								Inject();
};

extern RwTexture** const	gpCoronaTexture;

static_assert(sizeof(CRegisteredCorona) == 0x3C, "CRegisteredCorona has wrong size!");

#endif