//Credits: Casualhacker
inline void**& getvtable(void* inst, size_t offset = 0)
{
	return *reinterpret_cast<void***>((size_t)inst + offset);
}
inline const void** getvtable(const void* inst, size_t offset = 0)
{
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}
template< typename Fn >
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

typedef float matrix3x4[3][4];
typedef float matrix4x4[4][4];
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

//SRC SDK VECTORADD
inline void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

//USERCMD OFFSETS
#define USERCMDOFFSET 0xC4
#define VERIFIEDCMDOFFSET 0xC8
#define MULTIPLAYER_BACKUP 90
#define CURRENTCOMMANDOFFSET 0x100C
#define CURRENTPLAYERCOMMANDOFFSET 0x1640
#define PREIDCTIONSEEDOFFSET 0x1A
#define PREDICTIONPLAYEROFFSET 0x2D
#define GLOBALSOFFSET 0x34
#define INPUTOFFSET 0x20
#define GETSPREADOFFSET 0x5E0 // updated
#define GETCONEOFFSET 0x5E4 // updated
#define UPDATEACCURACYPENALTYOFFSET 0x5E8 // updated
#define WEAPONIDOFFSET 0x5A0
#define APPSYSTEMFACTORYOFFSET 0x2A
#define CLIENTFACTORYOFFSET 0x67

//LIFESTATE
#define	LIFE_ALIVE				0 
#define	LIFE_DYING				1 
#define	LIFE_DEAD				2 
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water


//USERCMD BUTTONS
#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

struct mstudiobbox_t
{
	int                                     bone;
	int                                     group;
	Vector									bbmin;
	Vector									bbmax;
	int                                     szhitboxnameindex;
	int                                     unused[8];

	const char* pszHitboxName() const
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	}
};

struct mstudiohitboxset_t
{
	int                                             sznameindex;
	inline char* const              pszName(void) const { return ((char*)this) + sznameindex; }
	int                                             numhitboxes;
	int                                             hitboxindex;
	inline mstudiobbox_t*   pHitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};

struct studiohdr_t
{
	unsigned char           pad[0xAC];
	int                                     numhitboxsets;
	int                                     hitboxsetindex;

	mstudiohitboxset_t* pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((BYTE*)this) + hitboxsetindex) + i;
	};

	inline mstudiobbox_t* pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);

		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	inline int iHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	};
};


class CBaseEntity;
class model_t;

typedef struct
{
	byte r, g, b, a;
} color32;

#include "dt_recv2.h"
#include "netvars.h"
#include "CBaseCombatWeapon.h"
#include "CBaseEntity.h"
#include "ClientClass.h"
#include "HLClient.h"
#include "CEngineClient.h"
#include "ISurface.h"
#include "IPanel.h"
#include "CEntityList.h"
#include "CInput.h"
#include "CPrediction.h"
#include "CGameMovement.h"
#include "CDebugOverlay.h"
#include "CModelInfo.h"
#include "CTrace.h"
#include "ConVar.h"
#include "CGlobalVars.h"
#include "IMoveHelper.h"
#include "IGameEvent.h"
#include "IGameEventListener2.h"
#include "IGameEventManager.h"
#include "MD5.h"

