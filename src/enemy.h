#pragma once

#include "raylib.h"

#include <vector>

using namespace std;

struct Player;
struct Dungeon;

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

void AddEnemy(vector<Enemy>& enemies, const char* name, Vector2 position);
void UpdateEnemies(vector<Enemy>& enemies, Player& player, Dungeon& dungeon);
