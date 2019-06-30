// dllmain.cpp : Définit le point d'entrée de l'application DLL.


#include "pch.h"
#include "offsets.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#define PI 3.1415927f
#ifndef WINDOW
#define WINDOW
#include <windows.h>
#endif // !window

#pragma warning(disable : 4996)

// Struct def
struct ACPLAYER {
	BYTE    unknown1[0x4];            // +0x0
	FLOAT headPositionx;			// +0x4
	FLOAT headPositiony;
	FLOAT headPositionz;
	BYTE unknown2[0x24];           // +0x10
	FLOAT positionx;                 // +0x34
	FLOAT positiony;
	FLOAT positionz;
	FLOAT viewx;                     // +0x40
	FLOAT viewy;
	FLOAT viewz;
	BYTE    unknown3[0x8];            // +0x58
	INT32   jumpFallSpeed;            // +0x54
	FLOAT   noClip;                   // +0x58
	BYTE    unknown4[0x14];           // +0x5C
	INT32   isImmobile;               // +0x70
	BYTE    unknown5[0xE];            // +0x74
	INT8    state;                    // +0x82
	BYTE    unknown6[0x75];           // +0x83
	INT32   hp;                       // +0xF8
	INT32   armor;                    // +0xFC
	BYTE    unknown7[0xC];            // +0x100
	INT8    dualPistolEnabled;        // +0x10C
	BYTE    unknown8[0x7];            // +0x10D
	INT32   pistolReserveAmmos;       // +0x114
	INT32   carabineReserveAmmos;     // +0x118
	INT32   shotgunReserveAmmos;      // +0x11C
	INT32   smgReserveAmmos;          // +0x120
	INT32   sniperRifleReserveAmmos;  // +0x124
	INT32   assaultRifleReserveAmmos; // +0x128
	BYTE    unknown9[0x8];            // +0x12C
	INT32   doublePistolReserveAmmos; // +0x134
	BYTE    unknown10[0x4];           // +0x138
	INT32   pistolLoadedAmmos;        // +0x13C
	INT32   carabineLoadedAmmos;      // +0x140
	INT32   shotgunLoadedAmmos;       // +0x144
	INT32   smgLoadedAmmos;           // +0x148
	INT32   sniperRifleLoadedAmmos;   // +0x14C
	INT32   assaultRifleLoadedAmmos;  // +0x150
	BYTE    unknown11[0x4];           // +0x154
	INT32   grenades;                 // +0x158
	INT32   doublePistolLoadedAmmos;  // +0x15C
	INT32   knifeSlashDelay;          // +0x160
	INT32   pistolShootDelay;         // +0x164
	INT32   carabineShootDelay;       // +0x168
	INT32   shotgunShootDelay;        // +0x16C
	INT32   smgShootDelay;            // +0x170
	INT32   sniperRifleShootDelay;    // +0x174
	INT32   assaultRifleShootDelay;   // +0x178
	BYTE    unknown12[0x8];           // +0x17C
	INT32   doublePistolShootDelay;   // +0x184
	BYTE    unknown13[0x7C];          // +0x188
	INT32   numberOfDeaths;           // +0x204
	BYTE    unknown14[0x1D];          // +0x208
	CHAR    nickname[16];             // +0x225
	BYTE    unknown15[0xF7];          // +0x235
	INT8    team;                     // +0x32C
};

struct vec3 {
	float x, y, z;
};
// Geometry tools
vec3 Subtract(vec3 src, vec3 dst) {
	vec3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
};

float Magnitude(vec3 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
};

float Distance(vec3 src, vec3 dst) {
	vec3 diff = Subtract(src, dst);
	return Magnitude(diff);
};

vec3 CalcAngle(vec3 src, vec3 dst) {
	vec3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / PI * 180.0f + 180.0f;
	angle.y = asinf((dst.z - src.z) / Distance(src, dst)) * 180.0f / PI;
	angle.z = 0.0f;

	return angle;
};

// 
float getDistance(ACPLAYER* player1, ACPLAYER* player2) {
	vec3 p1{ player1->positionx, player1->positiony, player1->positionz };
	vec3 p2{ player2->positionx, player2->positiony, player2->positionz };
	return Distance(p1,p2);
}

void aimPlayer(ACPLAYER* myPlayer, ACPLAYER* targetPlayer) {
	
	vec3 p1{ myPlayer->positionx, myPlayer->positiony, myPlayer->positionz };
	vec3 p2{ targetPlayer->positionx, targetPlayer->positiony, targetPlayer->positionz };
	vec3 angle = CalcAngle(p1, p2);

	std::cout << "MY POS:" << p1.x << " " << p1.y << " " << p1.z << std::endl;
	std::cout << "HIS POS:" << p2.x << " " << p2.y << " " << p2.z << std::endl;
	// Left Rigth
	float yaw = angle.x;
	// Up Down
	float pitch = angle.y;
	std::cout << "YAW : " << yaw << std::endl;
	std::cout << "PITCH" << pitch << std::endl;
	myPlayer->viewx = FLOAT(yaw);
	myPlayer->viewy = FLOAT(pitch);
}


DWORD WINAPI runBot(LPVOID lpParam) {

	DWORD baseAddress = (DWORD)GetModuleHandle(NULL);
	
	DWORD myPlayerAddress = baseAddress + my_player_base_off;
	DWORD entityListAddressPtr = baseAddress + entity_list_off;
	DWORD playersCountAddress = baseAddress + players_count_off;

	
	while (TRUE)
	{
		ACPLAYER* myPlayer = *(ACPLAYER * *)(myPlayerAddress);
		// Some invulnerability
		myPlayer->hp = 150;
		// Defining some variables to check if the player is the target
		float bestDistance = 9999;
		ACPLAYER* targetPlayer = NULL;
		// The element 0 of entity list is not a player, therefore we take element 1
		DWORD playerAddress = *(DWORD*)(entityListAddressPtr)+0x4;
		ACPLAYER* player;

		// Loop through all others players
		int players_count = *(DWORD*)playersCountAddress;
		for (int i = 0; i < players_count - 1; i++)
		{
			//std::cout << "NEW PLAYER :\n";
			player = *(ACPLAYER * *)(playerAddress);
			// Let's not focus our team
			if (player->team != myPlayer->team) {
				// Check if the player is the target
				float distance = getDistance(player, myPlayer);
/*				
				std::cout << "PLAYER ADRESS" << std::hex << playerAddress << '\n';
				std::cout << "PLAYER NICKNAME :" << player->nickname << std::endl; //go to pointer in the array, dereference it, grab the name from the object it points to
				std::cout << "DISTANCE PLAYER :" << distance << std::endl;
*/
				if (distance < bestDistance) {
					targetPlayer = player;
					distance = bestDistance;
				}
			}
			// going to next player
			playerAddress = playerAddress + 0x4;
		}
		// Target the player
		aimPlayer(myPlayer, targetPlayer);
		std::cout << "SLEEPING\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	
	return 1;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		std::cout << "PROCESS ATTACH\n";
		CreateThread(NULL, 0, &runBot, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

