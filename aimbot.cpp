#include "aimbot.h"
#include "Entities.h"
#include "geom.h"
#include "iostream"

#define M_PI 3.14159265358979323846

bool IsTeamGame()
{
    if (gameMode == nullptr) return false;

    int validModes[] = { 2, 4, 6, 8, 10, 11, 12, 17, 13, 14, 18, 15, 16, 19, 20, 21, 22 };
    int size = sizeof(validModes) / sizeof(validModes[0]);

    for (int i = 0; i < size; i++) {
        if (*gameMode == validModes[i]) {
            return true;
        }
    }
    return false;
}

bool IsEnemy(Entity* e)
{
    // Check if the entity is valid
    if (!e) 
        return false;

    // Check if it's a team game


    // In team games, enemies are on different teams
    // In non-team games, everyone except the local player is an enemy
    if (IsTeamGame())
    {
        return LocalPLayerEnt->Team != e->Team;
    }
    else
    {
        return e != LocalPLayerEnt;
    }
}

vec CalcAngle(vec src, vec dst)
{
    vec3 angles;

    angles.x = ((float)-atan2(dst.x - src.x, dst.y - src.y)) / M_PI * 180.0f;

    if (angles.x < 0.0f) angles.x += 360.0f;

    angles.y = asinf((dst.z - src.z) / src.Distance(dst)) * 180.0f / M_PI;
    angles.z = 0.0f;

    return angles;
}

Entity* GetBestTarget(entlist* entList)
{

    if (!entList || !entList->entities || !numOfPlayers || !LocalPLayerEnt) return nullptr;

    float oldDistance = FLT_MAX;
    float newDistance = 0;
    Entity* target = nullptr;
    for (int i = 0; i < *numOfPlayers; i++)
    {
        if (entList->entities[i] && IsEnemy(entList->entities[i]))
        {
            const vec3 angleTo = CalcAngle(LocalPLayerEnt->playerPos, entList->entities[i]->playerPos);

            newDistance = LocalPLayerEnt->viewAngles.Distance(angleTo);
            if (newDistance < oldDistance)
            {
                oldDistance = newDistance;
                target = entList->entities[i];
            }
        }
    }
    return target;
}

void Aimbot()
{

	if (GetAsyncKeyState(VK_XBUTTON1))
	{
		const Entity* target = GetBestTarget(entityList);

        if (!target) 
            return;

        const vec3 Angles = CalcAngle(LocalPLayerEnt->playerPos, target->playerPos);

        LocalPLayerEnt->viewAngles = Angles;

        Sleep(1);
	}
}