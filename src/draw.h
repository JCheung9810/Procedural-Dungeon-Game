#pragma once

#include "player.h"
#include "dungeon.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"

#include "raylib.h"

void DrawGame(Camera2D camera, Player player, Dungeon& dungeon, vector<Enemy>& enemies, vector<Projectile>& projectiles,  
            Texture2D heartTexture, Texture2D weaponHudBoxTexture,
            float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            int screenWidth, int screenHeight, 
            bool debugHurtbox, bool debugHitbox,  
            bool exitWindowRequested
);

void DrawPlayer(const Player& player, bool debugHurtbox, bool debugHitbox);
void DrawEnemies(vector<Enemy>& enemies);
void DrawWeapons(Player& player, float playerToMouseRotation, vector<Enemy>& enemies);
