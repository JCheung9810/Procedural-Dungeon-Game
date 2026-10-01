#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

struct Player {
    Vector2 position;
    Vector2 center;
    Vector2 playerSize;

    float speed;
    float health;
    float iFrames;
    
    Rectangle hitBox;
    
};

void InitializePlayer(Player& player);

#endif