#include "esp.h"

void esp::Draw(GL::Font& font, int* numOfPlayers, entlist* listplayer, float* matrix)
{
	glGetIntegerv(GL_VIEWPORT, viewport);

	for (int i = 0; i < (*numOfPlayers); i++)
	{

		Entity* e = listplayer->entities[i];
		vec3 center = e->playerPos;
		center.z = center.z - EYE_HEIGHT + PLAYER_HEIGHT / 2;

		vec3 screenCoords;

		if (WorldToScreen(center, screenCoords, matrix, viewport[2], viewport[3]))
		{
			DrawESPBox(e, screenCoords, font);
		}
	}
}

void esp::DrawESPBox(Entity* e, vec3 screen, GL::Font& font)
{
	const GLubyte* color = nullptr;

	if (IsTeamMode() && !IsEnemy(e)) {
		color = rgb::green;
	}
	else color = rgb::red;

	float dist = LocalPLayerEnt->playerPos.Distance(e->playerPos);

	float scale = (GAME_UNIT_MAGIC / dist) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);
	float x = screen.x - scale;
	float y = screen.y - scale * PLAYER_ASPECT_RATIO;
	float width = scale * 2;
	float height = scale * PLAYER_ASPECT_RATIO * 2;

	GL::DrawOutline(x, y, width, height, 2.0f, color);
	
	float textX = font.centerText(x, width, strlen(e->Name) * ESP_FONT_WIDTH);
	float textY = y - ESP_FONT_HEIGHT / 2;
	font.Print(textX, textY, color, "%s", e->Name);
}

bool esp::IsTeamMode() {
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

bool esp::IsEnemy(Entity* e)
{
	if (LocalPLayerEnt->Team == e->Team)
		return false;
	else return true;
}