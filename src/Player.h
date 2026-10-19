#ifndef PLAYER_H
#define PLAYER_H

#include "Room.h"

#include "raylib.h"

struct Player {
    Vector2 position;
    Vector2 center;
    Vector2 direction;

    float speed;
    float maxHealth;
    float health;
    float iFrames;

    Rectangle hitBox;

    bool canMove;
    bool dashing;
    Vector2 tempDirection;
    int facingDirection;
    
    char* gunType;
    Texture2D gunTexture;
    float gunSize;
    float gunOffset; 

    float dashTime;
    float dashCD;
    
    
    Texture2D texture;
};

void InitializePlayer(Player& player);
void UpdatePlayer(Player& player, Room rooms[], int& numRoomLocs, bool debugSpeed, bool debugWall);

#endif