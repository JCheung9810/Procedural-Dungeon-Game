#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"
#include "room.h"

#include "raylib.h"

struct Enemy {
    char* name;
    
    Vector2 size;
    Vector2 position;
    Vector2 center;
    
    float health;
    float speed;
    
    Rectangle hitBox;
    
    float attackRate;
    float attackCD;
    
    char* state;    //idle, walking
    
    bool active;
    Texture2D texture;
    
};

void AddEnemy(Enemy enemies[], char* name, int& numEnemies);
void UpdateEnemies(Enemy enemies[], int& numEnemies, Player player, Room rooms[], int numRoomLocs);

#endif