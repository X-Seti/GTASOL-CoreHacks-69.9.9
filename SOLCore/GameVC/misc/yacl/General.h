#include <math.h>
#ifndef __GENERAL
#define __GENERAL

#define RAD_TO_DEG								(180.0/M_PI)
#define DEG_TO_RAD								(M_PI/180.0)

// TODO: Reverse it
#define FUNC_CPlaceable__GetRotation			0x441DB0

#define FUNC_CEntity__GetBoundCentre				0x534250

struct CBaseModelInfo
{
	int _vmt, m_dwKey;
	short usageCount, m_wTxdIndex;
	char _fC, m_nbCount2dfx;
	short m_w2dfxIndex, m_wObjectInfoIndex, m_wFlags;
	int m_pColModel;
	float m_fDrawDistance;
	int m_pRwObject;
};

struct RwV2D
{
	float x, y;
};

struct RwV3d
{
	float x, y, z;
};

struct RwMatrix
{
	RwV3d right;
	DWORD flags;
	RwV3d up;
	DWORD _pad1;
	RwV3d at;
	DWORD _pad2;
	RwV3d pos;
	DWORD _pad3;
};
struct RwRGBA
{
	char r, g, b, a;
};
struct RwRGBAReal
{
	float red, green, blue, alpha;
};
struct RwObject
{
	char type, subType, flags, privateFlags;
	struct RwFrame *parent;
};
struct RwLLLink
{
	void *next;
	void *prev;
};
struct RwLinkList
{
	RwLLLink link;
};
struct RpClump
{
	RwObject object;
	RwLinkList atomicList, lightList, cameraList;
	RwLLLink inWorldList;
	DWORD callback;
};

struct RwRaster
{
	void *parent;
	char *cpPixels;
	char *palette;
	int width, height, depth, stride;
	short u, v;
	char cType, cFlags, privateFlags, cFormat;
	char *originalPixels;
	int originalWidth, originalHeight, originalStride;
};


struct RwTexDictionary
{
	RwObject object;
	RwLinkList texturesInDict;
	RwLLLink lInInstance;
};

struct RwTexture
{
	RwRaster *raster;
	RwTexDictionary *dict;
	RwLLLink lInDictionary;
	char name[32];
	char mask[32];
	char filterAddressing;
	char _pad[3];
	int refCount;
};

struct RwObjectHasFrame
{
	RwObject object;
	RwLLLink lFrame;
	DWORD sync;
};

struct RwPlane
{
	RwV3d normal;
	float distance;
};

struct RwFrustumPlane
{
	RwPlane plane;
	char closestX, closestY, closestZ, _pad;
};

struct RwBBox
{
	RwV3d sup, inf;
};

struct RwCamera
{
	RwObjectHasFrame object;
	DWORD projectionType, beginUpdate, endUpdate;
	RwMatrix viewMatrix;
	RwRaster *frameBuffer;
	RwRaster *zBuffer;
	RwV2D viewWindow, recipViewWindow, viewOffset;
	float nearPlane, farPlane, fogPlane, zScale, zShift;
	RwFrustumPlane frustrumPlanes[6];
	RwBBox frustrumBoundBox;
	RwV3d frustrumCorners[8];
};

struct CShadowImage
{
	RwCamera *camera;
	RwTexture *texture;
};

struct RpLight
{
	RwObjectHasFrame object;
	float radius;
	RwRGBAReal color;
	float minusCosAngle;
	RwLinkList WorldSectorsInLight;
	RwLLLink InWorld;
	WORD lightFrame, _pad;
};

struct RwSphere
{
	RwV3d center;
	float radius;
};

struct CShadowData
{
	int *owner;
	char isExist, intensity, _pad1[2];
	CShadowImage image;
	char isBlurred, _pad2[3];
	CShadowImage blurredImage;
	int blurLevel;
	char createBlurTypeB, _pad3[3];
	int objectType;
	RpLight *light;
	RwSphere boundingSphere, baseSphere;
};

struct CRegisteredShadow
{
	RwV3d position;
	float x1, y1, x2, y2, zDistance, scale;
	RwTexture *texture;
	CShadowData *shadowData;
	short intensity;
	char type, red, green, blue;
	short flags; /*
				 0001 DRAW_ON_WATER
				 0010 IGNORE_MAP_OBJECTS
				 0100 DRAW_ON_BUILDINGS
				 */
};




class CVector
{
public:
	float	x, y, z;

	CVector()
	{}

	CVector(float fX, float fY, float fZ=0.0f)
		: x(fX), y(fY), z(fZ)
	{}

	CVector(const RwV3d& rwVec)
		: x(rwVec.x), y(rwVec.y), z(rwVec.z)
	{}

	CVector&		operator+=(const CVector& vec)
			{ x += vec.x; y += vec.y; z += vec.z;
			return *this; }

	inline float	Magnitude()
		{ return sqrt(x * x + y * y + z * z); }

	friend inline float DotProduct(const CVector& vec1, const CVector& vec2)
		{ return vec1.x * vec2.x + vec1.x * vec2.y + vec1.z * vec2.z; }

	friend inline CVector operator*(const CVector& in, float fMul)
		{ return CVector(in.x * fMul, in.y * fMul, in.z * fMul); }
	friend inline CVector operator+(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend inline CVector operator-(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
};

class CVector2D
{
public:
	float	x, y;

	CVector2D()
	{}

	CVector2D(float fX, float fY)
		: x(fX), y(fY)
	{}
};

class CMatrix
{
public:
	RwMatrix	matrix;
	RwMatrix*	pMatrix;
	BOOL		haveRwMatrix;

public:
	CMatrix()
		: pMatrix(nullptr), haveRwMatrix(FALSE)
	{}

	CMatrix(RwMatrix* rwMatrix, bool bAttach=false)
		: matrix(*rwMatrix), haveRwMatrix(bAttach), pMatrix(bAttach ? rwMatrix : nullptr)
	{}

	CMatrix(const CVector& vecRight, const CVector& vecUp, const CVector& vecAt, const CVector& vecPos)
	{
		matrix.right.x = vecRight.x;
		matrix.right.y = vecRight.y;
		matrix.right.z = vecRight.z;

		matrix.up.x = vecUp.x;
		matrix.up.y = vecUp.y;
		matrix.up.z = vecUp.z;

		matrix.at.x = vecAt.x;
		matrix.at.y = vecAt.y;
		matrix.at.z = vecAt.z;

		matrix.pos.x = vecPos.x;
		matrix.pos.y = vecPos.y;
		matrix.pos.z = vecPos.z;
	}

	friend inline CMatrix operator*(const CMatrix& Rot1, const CMatrix& Rot2)
		{ return CMatrix(	CVector(Rot1.matrix.right.x * Rot2.matrix.right.x + Rot1.matrix.right.y * Rot2.matrix.up.x + Rot1.matrix.right.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.right.x * Rot2.matrix.right.y + Rot1.matrix.right.y * Rot2.matrix.up.y + Rot1.matrix.right.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.right.x * Rot2.matrix.right.z + Rot1.matrix.right.y * Rot2.matrix.up.z + Rot1.matrix.right.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.up.x * Rot2.matrix.right.x + Rot1.matrix.up.y * Rot2.matrix.up.x + Rot1.matrix.up.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.up.x * Rot2.matrix.right.y + Rot1.matrix.up.y * Rot2.matrix.up.y + Rot1.matrix.up.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.up.x * Rot2.matrix.right.z + Rot1.matrix.up.y * Rot2.matrix.up.z + Rot1.matrix.up.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.at.x * Rot2.matrix.right.x + Rot1.matrix.at.y * Rot2.matrix.up.x + Rot1.matrix.at.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.at.x * Rot2.matrix.right.y + Rot1.matrix.at.y * Rot2.matrix.up.y + Rot1.matrix.at.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.at.x * Rot2.matrix.right.z + Rot1.matrix.at.y * Rot2.matrix.up.z + Rot1.matrix.at.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.pos.x * Rot2.matrix.right.x + Rot1.matrix.pos.y * Rot2.matrix.up.x + Rot1.matrix.pos.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.pos.x * Rot2.matrix.right.y + Rot1.matrix.pos.y * Rot2.matrix.up.y + Rot1.matrix.pos.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.pos.x * Rot2.matrix.right.z + Rot1.matrix.pos.y * Rot2.matrix.up.z + Rot1.matrix.pos.z * Rot2.matrix.at.z + Rot2.matrix.pos.z)); };

	friend inline CVector operator*(const CMatrix& matrix, const CVector& vec)
			{ return CVector(matrix.matrix.up.x * vec.y + matrix.matrix.right.x * vec.x + matrix.matrix.at.x * vec.z + matrix.matrix.pos.x,
								matrix.matrix.up.y * vec.y + matrix.matrix.right.y * vec.x + matrix.matrix.at.y * vec.z + matrix.matrix.pos.y,
								matrix.matrix.up.z * vec.y + matrix.matrix.right.z * vec.x + matrix.matrix.at.z * vec.z + matrix.matrix.pos.z); };

	inline CVector*	GetUp()
		{ return reinterpret_cast<CVector*>(&matrix.up); }

	inline CVector*	GetAt()
		{ return reinterpret_cast<CVector*>(&matrix.at); }

	inline CVector* GetPos()
		{ return reinterpret_cast<CVector*>(&matrix.pos); }

	inline void		SetTranslateOnly(float fX, float fY, float fZ)
		{ matrix.pos.x = fX; matrix.pos.y = fY; matrix.pos.z = fZ; }

	void			SetRotateXOnly(float fAngle);
	void			SetRotateYOnly(float fAngle);
	void			SetRotateZOnly(float fAngle);

	void			SetRotateOnly(float fAngleX, float fAngleY, float fAngleZ);
};

class CSimpleTransform
{
public:
    CVector                         m_translate;
    float                           m_heading;
};

class CRGBA
{
public:
	BYTE r, g, b, a;

	inline CRGBA() {}

	inline CRGBA(const CRGBA& in)
		: r(in.r), g(in.g), b(in.b), a(in.a)
	{}

	inline CRGBA(const CRGBA& in, BYTE alpha)
		: r(in.r), g(in.g), b(in.b), a(alpha)
	{}


	inline CRGBA(BYTE red, BYTE green, BYTE blue, BYTE alpha = 255)
		: r(red), g(green), b(blue), a(alpha)
	{}

	void	BaseColors__Constructor();
};

class CRect
{
public:
	float x1, y1;
	float x2, y2;

	inline CRect() {}
	inline CRect(float a, float b, float c, float d)
		: x1(a), y1(b), x2(c), y2(d)
	{}
};


class CPlaceable
{
private:
    CSimpleTransform				m_transform;
    CMatrix*						m_pCoords;

public:
	// Line up the VMTs
	virtual ~CPlaceable() {}

	inline CPlaceable() {}

	explicit inline CPlaceable(int dummy)
	{
		// Dummy ctor
		UNREFERENCED_PARAMETER(dummy);
	}

	inline CVector*					GetCoords()
		{ return m_pCoords ? reinterpret_cast<CVector*>(&m_pCoords->matrix.pos) : &m_transform.m_translate; }
	inline CMatrix*					GetMatrix()
		{ return m_pCoords; }
	inline CSimpleTransform&		GetTransform()
		{ return m_transform; }

	inline void						SetCoords(const CVector& pos)
	{	if ( m_pCoords ) { m_pCoords->matrix.pos.x = pos.x; m_pCoords->matrix.pos.y = pos.y; m_pCoords->matrix.pos.z = pos.z; }
		else m_transform.m_translate = pos; }
	inline void						SetHeading(float fHeading)
		{ if ( m_pCoords ) m_pCoords->SetRotateZOnly(fHeading); else m_transform.m_heading = fHeading; }
};

// TODO: May not be the best place to put it?
class __declspec(novtable) CEntity : public CPlaceable
{
public:
	/********** BEGIN VTBL **************/
    virtual void	Add_CRect();
    virtual void	Add();
    virtual void	Remove();
    virtual void	SetIsStatic(bool);
    virtual void	SetModelIndex(int nModelIndex);
    virtual void	SetModelIndexNoCreate(int nModelIndex);
    virtual void	CreateRwObject();
    virtual void	DeleteRwObject();
    virtual CRect	GetBoundRect();
    virtual void	ProcessControl();
    virtual void	ProcessCollision();
    virtual void	ProcessShift();
    virtual void	TestCollision();
    virtual void	Teleport();
    virtual void	SpecialEntityPreCollisionStuff();
    virtual void	SpecialEntityCalcCollisionSteps();
    virtual void	PreRender();
    virtual void	Render();
    virtual void	SetupLighting();
    virtual void	RemoveLighting();
    virtual void	FlagToDestroyWhenNextProcessed();

//private:
	RpClump*		m_pRwObject;						// 0x18

    /********** BEGIN CFLAGS (0x1C) **************/
    unsigned long	bUsesCollision : 1;				// does entity use collision
    unsigned long	bCollisionProcessed : 1;			// has object been processed by a ProcessEntityCollision function
    unsigned long	bIsStatic : 1;					// is entity static
    unsigned long	bHasContacted : 1;				// has entity processed some contact forces
    unsigned long	bIsStuck : 1;						// is entity stuck
    unsigned long	bIsInSafePosition : 1;			// is entity in a collision free safe position
    unsigned long	bWasPostponed : 1;				// was entity control processing postponed
    unsigned long	bIsVisible : 1;					//is the entity visible

    unsigned long	bIsBIGBuilding : 1;				// Set if this entity is a big building
    unsigned long	bRenderDamaged : 1;				// use damaged LOD models for objects with applicable damage
    unsigned long	bStreamingDontDelete : 1;			// Dont let the streaming remove this
    unsigned long	bRemoveFromWorld : 1;				// remove this entity next time it should be processed
    unsigned long	bHasHitWall : 1;					// has collided with a building (changes subsequent collisions)
    unsigned long	bImBeingRendered : 1;				// don't delete me because I'm being rendered
    unsigned long	bDrawLast :1;						// draw object last
    unsigned long	bDistanceFade :1;					// Fade entity because it is far away

    unsigned long	bDontCastShadowsOn : 1;			// Dont cast shadows on this object
    unsigned long	bOffscreen : 1;					// offscreen flag. This can only be trusted when it is set to true
    unsigned long	bIsStaticWaitingForCollision : 1; // this is used by script created entities - they are static until the collision is loaded below them
    unsigned long	bDontStream : 1;					// tell the streaming not to stream me
    unsigned long	bUnderwater : 1;					// this object is underwater change drawing order
    unsigned long	bHasPreRenderEffects : 1;			// Object has a prerender effects attached to it
    unsigned long	bIsTempBuilding : 1;				// whether or not the building is temporary (i.e. can be created and deleted more than once)
    unsigned long	bDontUpdateHierarchy : 1;			// Don't update the aniamtion hierarchy this frame

    unsigned long	bHasRoadsignText : 1;				// entity is roadsign and has some 2deffect text stuff to be rendered
    unsigned long	bDisplayedSuperLowLOD : 1;
    unsigned long	bIsProcObject : 1;				// set object has been generate by procedural object generator
    unsigned long	bBackfaceCulled : 1;				// has backface culling on
    unsigned long	bLightObject : 1;					// light object with directional lights
    unsigned long	bUnimportantStream : 1;			// set that this object is unimportant, if streaming is having problems
    unsigned long	bTunnel : 1;						// Is this model part of a tunnel
    unsigned long	bTunnelTransition : 1;			// This model should be rendered from within and outside of the tunnel
    /********** END CFLAGS **************/

    WORD			RandomSeed;					// 0x20
    short			m_nModelIndex;				// 0x22
    void*			pReferences;				// 0x24
    void*			m_pLastRenderedLink;		// 0x28
    WORD			m_nScanCode;				// 0x2C
    BYTE			m_iplIndex;					// 0x2E
    BYTE			m_areaCode;					// 0x2F
    CEntity*		m_pLod;					// 0x30
    BYTE			numLodChildren;				// 0x34
    char			numLodChildrenRendered;		// 0x35

    //********* BEGIN CEntityInfo **********//
    BYTE			nType : 3;							// what type is the entity	// 0x36 (2 == Vehicle)
    BYTE			nStatus : 5;						// control status			// 0x36
    //********* END CEntityInfo ************//

public:
	explicit inline CEntity(int dummy)
		: CPlaceable(dummy)
	{
		// Dummy ctor
	}

	inline short&	ModelIndex()
						{ return m_nModelIndex; };
	inline short	GetModelIndex()
					{ return m_nModelIndex; }

	void			UpdateRW();
	void			RegisterReference(CEntity** pAddress);
	void			CleanUpOldReference(CEntity** pAddress);
};

class __declspec(novtable) CPhysical : public CEntity
{
private:
    float			pad1; // 56
    int				__pad2; // 60

    unsigned int	b0x01 : 1; // 64
    unsigned int	bApplyGravity : 1;
    unsigned int	bDisableFriction : 1;
    unsigned int	bCollidable : 1;
    unsigned int	b0x10 : 1;
    unsigned int	bDisableMovement : 1;
    unsigned int	b0x40 : 1;
    unsigned int	b0x80 : 1;

    unsigned int	bSubmergedInWater : 1; // 65
    unsigned int	bOnSolidSurface : 1;
    unsigned int	bBroken : 1;
    unsigned int	b0x800 : 1; // ref @ 0x6F5CF0
    unsigned int	b0x1000 : 1;//
    unsigned int	b0x2000 : 1;//
    unsigned int	b0x4000 : 1;//
    unsigned int	b0x8000 : 1;//

    unsigned int	b0x10000 : 1; // 66
    unsigned int	b0x20000 : 1; // ref @ CPhysical__processCollision
    unsigned int	bBulletProof : 1;
    unsigned int	bFireProof : 1;
    unsigned int	bCollisionProof : 1;
    unsigned int	bMeeleProof : 1;
    unsigned int	bInvulnerable : 1;
    unsigned int	bExplosionProof : 1;

    unsigned int	b0x1000000 : 1; // 67
    unsigned int	bAttachedToEntity : 1;
    unsigned int	b0x4000000 : 1;
    unsigned int	bTouchingWater : 1;
    unsigned int	bEnableCollision : 1;
    unsigned int	bDestroyed : 1;
    unsigned int	b0x40000000 : 1;
    unsigned int	b0x80000000 : 1;

    CVector			m_vecLinearVelocity; // 68
    CVector			m_vecAngularVelocity; // 80
    CVector			m_vecCollisionLinearVelocity; // 92
    CVector			m_vecCollisionAngularVelocity; // 104
    CVector			m_vForce;							// 0x74
    CVector			m_vTorque;							// 0x80
	float			fMass;								// 0x8C
    float			fTurnMass;							// 0x90
    float			m_fVelocityFrequency;					// 0x94
    float			m_fAirResistance;						// 0x98
    float			m_fElasticity;						// 0x9C
    float			m_fBuoyancyConstant;					// 0xA0

    CVector			vecCenterOfMass;					// 0xA4
    DWORD			dwUnk1;								// 0xB0
    void*			unkCPtrNodeDoubleLink;				// 0xB4
    BYTE			byUnk: 8;								// 0xB8
    BYTE			byCollisionRecords: 8;					// 0xB9
    BYTE			byUnk2: 8;								// 0xBA (Baracus)
    BYTE			byUnk3: 8;								// 0xBB

    float			pad4[6];								// 0xBC

    float			fDistanceTravelled;					// 0xD4
    float			fDamageImpulseMagnitude;				// 0xD8
    CEntity*		damageEntity;						// 0xDC
    BYTE			pad2[28];								// 0xE0
    CEntity*		pAttachedEntity;					// 0xFC
    CVector			m_vAttachedPosition;				// 0x100
    CVector			m_vAttachedRotation;				// 0x10C
    BYTE			pad3[20];								// 0x118
    float			fLighting;							// 0x12C col lighting? CPhysical::GetLightingFromCol
    float			fLighting_2;							// 0x130 added to col lighting in CPhysical::GetTotalLighting
    BYTE			pad3a[4];								// 0x134

public:
	// Temp
	CPhysical()
	: CEntity(0) {}
};

// TODO: Move it away
class CKeyGen
{
public:
	static unsigned int		GetUppercaseKey(const char* pEntry);
};

// TODO: Move away?
class CGame
{
private:
	static bool&			bMissionPackGame;

public:
	static inline bool		IsMissionPackGame()
		{ return bMissionPackGame; }
};


bool CalcScreenCoors(const CVector& vecIn, CVector* vecOut);
void LoadingScreenLoadingFile(const char* pText);

#endif