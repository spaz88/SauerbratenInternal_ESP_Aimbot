#include "Entities.h"

void initializeOffsets() {
	moduleBase = (uintptr_t)GetModuleHandle("sauerbraten.exe");

	entityListBasePtr = *(uintptr_t*)(moduleBase + 0x346C90);

	LocalPLayerEntPtr = *(uintptr_t*)(entityListBasePtr + 0x0);

	LocalPLayerEnt = *(Entity**)(entityListBasePtr + 0x0);

	numOfPlayers = (int*)(moduleBase + 0x346C9C);
	matrix = (float*)(moduleBase + 0x32D040);
	localPlayer = (uintptr_t)(moduleBase + 0x2A5730);

	offsetHealth = 0x178;     // santé dans la structure entité
	entitySpacing = 0x8;
	offsetName = 0x274;

	entityList = *(entlist**)(moduleBase + 0x346C90); // entlist pointer

	gameMode = (int*)(moduleBase + 0x26F6B0);
}