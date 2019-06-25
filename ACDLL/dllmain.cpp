// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include "offsets.h"
#include <iostream>

#ifndef WINDOW
#define WINDOW
#include <windows.h>
#endif // !window

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




DWORD WINAPI runBot(LPVOID lpParam) {
	
	// D
	/*
	* Charger la structure du joueur ne marche pas
	* Probablement un probleme avec les adresses...

	DWORD baseAdress = (DWORD)GetModuleHandle(NULL);
	DWORD myplayerAddress = baseAdress + my_player_base_off;
	ACPLAYER myPLayer = *(ACPLAYER*)myplayerAddress;
	myPLayer.assaultRifleLoadedAmmos = 30;
*/
	//int ammo = *((int*)0x0088A188);
	
	// Test d'ecriture direct pour changer les ammo (sans passer par le pointeur)
	// Fonctionne
	*((int*)0x00A3A188) = 30;
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
		std::cout << "PROCESS ATTACH";
		CreateThread(NULL, 0, &runBot, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

