#ifndef DRAW_H
#define DRAW_H

#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Camera.h"

#include "raylib.h"

void DrawGame(Camera2D camera, Player player, Room rooms[], int numRoomLocs, Enemy enemies[], int numEnemies, Projectile projectiles[], int maxProjectiles, 
            Texture2D gunTexture, float gunSize, float gunOffset, 
            Texture2D heartTexture,
            float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            int screenWidth, int screenHeight, bool exitWindowRequested
);

#endif