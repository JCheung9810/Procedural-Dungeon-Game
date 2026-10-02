#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

struct Enemy {
    char* name;
    
    Vector2 size;
    Vector2 position;
    
    float health;
    
    Rectangle hitBox;
    
    bool active;
    Texture2D sprite;
    
};

void AddEnemy(Enemy enemies[], char* name, int& numEnemies);

#endif