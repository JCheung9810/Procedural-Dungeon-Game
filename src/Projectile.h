#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "player.h"
#include "enemy.h"
#include "room.h"

#include "raylib.h"

struct Projectile {
    Vector2 position;
    Vector2 speed;
    
    Vector2 size;
    float rotation;
    
    float lifeSpan;
    
    bool active;
    
    char* team;
    char* type;
    
    float damage;
    
    Color color;
    Texture2D sprite;
    
    Rectangle hitBox;
    //add 8 circles for detection so it can be "rotated" (4 corners 4 sides)
    //detect collision with the array of corners
};

void InitializeProjectiles(Projectile projectiles[], int maxProjectiles);
void FireProjectile(Projectile projectiles[], int maxProjectiles, float playerToMouseRotation, char* team, float projectileLifespan, Player player, Enemy enemy);
void UpdateProjectiles(Projectile projectiles[], int maxProjectiles, Room rooms[], int numRoomLocs, Enemy enemies[], int numEnemies, Player& player);

#endif