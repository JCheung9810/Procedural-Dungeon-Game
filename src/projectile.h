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
    
    const char* team;
    const char* type;
    
    float damage;
    
    Color color;
    Texture2D sprite;
    
    Rectangle hitbox;
    
};

void InitializeProjectile(Projectile& projectile);
void FireProjectile(vector<Projectile>& projectiles, float playerToMouseRotation, const char* team, float projectileLifespan, Player& player, Enemy* enemy);
void UpdateProjectiles(vector<Projectile>& projectiles, Dungeon& dungeon, vector<Enemy>& enemies, Player& player);
