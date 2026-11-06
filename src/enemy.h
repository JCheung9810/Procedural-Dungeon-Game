#pragma once

#include "player.h"
#include "dungeon.h"

#include "raylib.h"

struct Enemy {
    const char* name;
    
    Vector2 size;
    Vector2 position;
    Vector2 center;
    
    float health;
    float speed;
    
    Rectangle hitBox;
    
    float attackRate;
    float attackCD;
    
    const char* state;    //idle, walking
    
    bool active;
    Texture2D texture;
    
};

void AddEnemy(Enemy enemies[], const char* name, int& numEnemies);
void UpdateEnemies(Enemy enemies[], int& numEnemies, Player player, Room rooms[], int numRoomLocs);
