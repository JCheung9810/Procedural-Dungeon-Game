#pragma once

#include "player.h"
#include "enemy.h"
#include "dungeon.h"

#include "raylib.h"

struct Projectile {
    Vector2 position;
    Vector2 speed;
    
    Vector2 size;
    float rotation;
    
    float lifeSpan;
    
    bool active;
    
    const char* team;
    const char* type;
    
    float damage;
    
    Color color;
    Texture2D sprite;
    
    Rectangle hitBox;
    
};

void InitializeProjectiles(Projectile projectiles[], int maxProjectiles);
void FireProjectile(Projectile projectiles[], int maxProjectiles, float playerToMouseRotation, const char* team, float projectileLifespan, Player& player, Enemy enemy);
void UpdateProjectiles(Projectile projectiles[], int maxProjectiles, Dungeon dungeon, Enemy enemies[], int numEnemies, Player& player);
