#pragma once

#include "dungeon.h"
#include "player.h"

#include "raylib.h"

void InitializeCamera(Camera2D& camera, Player player, int screenHeight, int screenWidth);
void UpdateGameCamera(Dungeon dungeon, Player player, Camera2D& camera, bool debugCam);
