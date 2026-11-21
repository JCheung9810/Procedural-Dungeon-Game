#pragma once

#include "player.h"
#include "dungeon.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"

#include "raylib.h"

void DrawGame(Camera2D camera, Player player, Dungeon& dungeon, vector<Enemy>& enemies, vector<Projectile>& projectiles,  
            Texture2D heartTexture, Texture2D weaponHudBoxTexture, Texture2D weaponHudBoxBackgroundTexture, Font font,
            Vector2 mouseScreenPos, float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            bool minimap, Vector2 minimapPos, float minimapScale,
            int screenWidth, int screenHeight,  
            bool exitWindowRequested
);

void DrawPlayer(const Player& player);
void DrawEnemies(vector<Enemy>& enemies);
void DrawWeapons(Player& player, float playerToMouseRotation, vector<Enemy>& enemies);
void DrawMinimap(bool minimap, Vector2 minimapPos, float minimapScale, Dungeon& dungeon, int screenWidth, int screenHeight);
