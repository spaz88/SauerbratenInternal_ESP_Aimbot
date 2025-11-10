#pragma once
#include "Windows.h"
#include "geom.h"

class Entity {
public:
    Vector3 playerPos;        // 0x0000
    Vector3 camVelocity;      // 0x000C
    char pad_0018[36];        // 0x0018
    Vector3 viewAngles;       // 0x003C
    Vector3 playerPos2;       // 0x0048
    float yaw;                // 0x0054
    float pitch;              // 0x0058
    char pad_005C[284];       // 0x005C
    int32_t health;           // 0x0178
    char pad_017C[4];         // 0x017C
    int32_t armor;            // 0x0180
    char pad_0184[240];       // 0x0184
    char Name[8];             // 0x0274
    char pad_027C[252];       // 0x027C
    char Team[4];             // 0x0378
};


class entlist
{
public:
    Entity* entities[32]; //0x0000
};

inline uintptr_t moduleBase;
inline uintptr_t entityListBasePtr;
inline uintptr_t LocalPLayerEntPtr;
inline uintptr_t localPlayer;

inline Entity* LocalPLayerEnt;

inline int* numOfPlayers;
inline int* gameMode;
inline float* matrix;

inline uintptr_t offsetHealth;     // santé dans la structure entité
inline size_t entitySpacing;
inline uintptr_t offsetName;

inline entlist* entityList; // entlist pointer

void initializeOffsets();