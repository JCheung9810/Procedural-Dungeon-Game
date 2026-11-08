#pragma once

#include "player.h"
#include "dungeon.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"

#include "raylib.h"

void DrawGame(Camera2D camera, Player player, Dungeon& dungeon, Enemy enemies[], int numEnemies, Projectile projectiles[], int maxProjectiles,  
            Texture2D heartTexture, Texture2D weaponHudBoxTexture,
            float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            int screenWidth, int screenHeight, 
            bool debugHitbox,
            bool exitWindowRequested
);

void DrawPlayer(const Player& player, bool debugHitbox);
void DrawWeapon(Player player, float playerToMouseRotation);
