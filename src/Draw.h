#ifndef DRAW_H
#define DRAW_H

#include "player.h"
#include "room.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"

#include "raylib.h"

void DrawGame(Camera2D camera, Player player, Room rooms[], int numRoomLocs, Enemy enemies[], int numEnemies, Projectile projectiles[], int maxProjectiles,  
            Texture2D heartTexture,
            float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            int screenWidth, int screenHeight, bool exitWindowRequested
);

void DrawPlayer(const Player& player);

#endif