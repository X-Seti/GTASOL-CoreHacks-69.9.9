/*
	file:
		GameScripting.h
	desc:
		This is the header file for GameScripting.cpp.
*/

#ifndef GAMESCRIPTING_H
#define GAMESCRIPTING_H

#include "stdinc.h"
#include <Windows.h>
#include "GameDefines.h"	// Include GameScripting.h and get the game defines too.

#define MAX_SCRIPT_VARS	16	// Size of our variable saving array
#define MAX_SCRIPT_SIZE	255	// Size of ScriptBuf - Max is really (2+(13*5))

struct GAME_SCRIPT_THREAD	// 0x88 bytes total.
{							// - Credit to CyQ & PatrickW
	void* pNext;			// 0x00
	void* pPrev;			// 0x04
	char strName[8];		// 0x08
	DWORD dwScriptIP;		// 0x10
	DWORD dwReturnStack[6];	// 0x14
	DWORD dwStackPointer;	// 0x2C
	DWORD dwLocalVar[18];	// 0x30
	BYTE bStartNewScript;	// 0x78
	BYTE bJumpFlag;			// 0x79
	BYTE bWorky;			// 0x7A
	BYTE bAwake;			// 0x7B
	DWORD dwWakeTime;		// 0x7C
	WORD wIfParam;			// 0x80
	BYTE bNotFlag;			// 0x82
	BYTE bWastedBustedCheck;// 0x83
	BYTE bWastedBustedFlag;	// 0x84
	BYTE bMissionThread;	// 0x85
	BYTE pad[2];			// 0x86
};

struct SCRIPT_COMMAND		//	Params
{							//		i = integer
	WORD OpCode;			//		f = float
	char Params[13];		//		v = variable
};							//		s = string

struct VEHICLE_STRUCT		// 0x5DC bytes total.
{

};

struct ACTOR_STRUCT			// 0x6D8 bytes total.
{
	BYTE Ukn00[0x14];		// 0x00
	float fVelocityX;		// 0x14
	float fVelocityY;		// 0x18
	float fVelocityZ;		// 0x1C
	BYTE Ukn20[0x14];		// 0x20
	float fPositionX;		// 0x34
	float fPositionY;		// 0x38
	float fPositionZ;		// 0x3C
	BYTE Ukn40[0x204];		// 0x40
	DWORD dwAnimation;		// 0x244
	BYTE Ukn248[0x10C];		// 0x248
	float fHealth;			// 0x354
	float fArmor;			// 0x358
	BYTE Ukn35C[0x4C];		// 0x35C
	DWORD dwCarID;			// 0x3A8
	BYTE bInCar;			// 0x3AC
	BYTE Ukn3AB[0x32D];		// 0x3AB
};

struct OBJECT_STRUCT
{

};

typedef OBJECT_STRUCT* (*GameGetObject_t)(int ObjectID);		// Typedef for game GetObjectPointer function.
typedef VEHICLE_STRUCT* (*GameGetVehicle_t)(int VehicleID);		// Typedef for game GetVehiclePointer function.
typedef ACTOR_STRUCT* (*GameGetActor_t)(int ActorID);			// Typedef for game GetActorPointer function.

int ScriptCommand(const SCRIPT_COMMAND* ScriptCommand, ...);	// The main scripting function. See notes.

/*	Below are some opcodes i've defined, but it's still missing
	about 95% of them ;) See http://vc-db.webtools4you.net/		*/
const SCRIPT_COMMAND request_model				= { 0x0247, "i" };		// (CAR_*|BIKE_*|BOAT_*|WEAPON_*|OBJECT_*)
const SCRIPT_COMMAND load_requested_models		= { 0x038B, "" };		// -/-
const SCRIPT_COMMAND create_car					= { 0x00A5, "ifffv" };	// (CAR_*|BIKE_*|BOAT_*), x, y, z, var_car
const SCRIPT_COMMAND load_special_actor			= { 0x023C, "is" };		// SPECIAL_*, MODEL_*
const SCRIPT_COMMAND create_actor				= { 0x009A, "iifffv" };	// PEDTYPE_*, #MODEL, x, y, z, var_actor
const SCRIPT_COMMAND destroy_actor_fading		= { 0x034F, "v" };		// var_actor
const SCRIPT_COMMAND kill_actor					= { 0x0321, "v" };		// var_actor
const SCRIPT_COMMAND kill_player				= { 0x0322, "v" };		// player_char
const SCRIPT_COMMAND force_weather				= { 0x01B5, "i" };		// WEATHER_*
const SCRIPT_COMMAND set_weather				= { 0x01B6, "i" };		// WEATHER_*
const SCRIPT_COMMAND release_weather			= { 0x01B7, "" };		// -/-
const SCRIPT_COMMAND set_fade_color				= { 0x0169, "iii" };	// Red(0-255), Green(0-255), Blue(0-255)
const SCRIPT_COMMAND fade						= { 0x016A, "ii" };		// (time in ms), FADE_*
const SCRIPT_COMMAND is_fading					= { 0x016B, "" };		// -/-
const SCRIPT_COMMAND set_actor_immunities		= { 0x02AB, "viiiii" };	// var_actor, Bullet(1/0), Fire(1/0), Explosion(1/0), Damage(1/0), Water(1/0)
const SCRIPT_COMMAND freeze_player				= { 0x01B4, "vi" };		// PLAYER_CHAR, Freeze(1/0)
const SCRIPT_COMMAND set_max_wanted_level		= { 0x01F0, "i" };		// MaxLevel
const SCRIPT_COMMAND set_wasted_busted_check	= { 0x0111, "i" };		// Check(1/0)
const SCRIPT_COMMAND set_current_time			= { 0x00C0, "ii" };		// Hours, Minutes
const SCRIPT_COMMAND set_camera					= { 0x03CB, "fff" };	// x, y, z
const SCRIPT_COMMAND create_player				= { 0x0053, "ifffv" };	// 0, x, y, z, PLAYER_CHAR
const SCRIPT_COMMAND create_actor_from_player	= { 0x01F5, "vv" };		// PLAYER_CHAR, PLAYER_ACTOR
const SCRIPT_COMMAND set_actor_skin				= { 0x0352, "vs" };		// var_actor, MODEL_*
const SCRIPT_COMMAND refresh_actor_skin			= { 0x0353, "v" };		// var_actor
const SCRIPT_COMMAND select_interior			= { 0x04BB, "i" };		// INTERIOR_*
const SCRIPT_COMMAND player_in_car				= { 0x0443, "v" };		// PLAYER_CHAR
const SCRIPT_COMMAND get_player_car				= { 0x03C1, "vv" };		// PLAYER_CHAR, var_car
const SCRIPT_COMMAND car_stopped				= { 0x01C1, "v" };		// var_car
const SCRIPT_COMMAND get_car_speed				= { 0x02E3, "vv" };		// var_car var_speed
const SCRIPT_COMMAND get_player_money			= { 0x010B, "vv" };		// PLAYER_CHAR, money
const SCRIPT_COMMAND get_current_time			= { 0x00BF, "vv" };		// hours, minutes
const SCRIPT_COMMAND get_player_health			= { 0x0225, "vv" };		// PLAYER_CHAR, health
const SCRIPT_COMMAND get_player_armor			= { 0x04DD, "vv" };		// PLAYER_ACTOR, armor
const SCRIPT_COMMAND get_player_wanted_level	= { 0x01C0, "vv" };		// PLAYER_CHAR, wantedlevel

#endif