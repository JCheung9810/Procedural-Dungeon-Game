#ifndef PLAYER_H
#define PLAYER_H

#include "room.h"
#include "weapon.h"

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
    
    Weapon weapon;

    float dashTime;
    float dashCD;
    
    
    Texture2D texture;
};

struct PlayerConfig {
    float maxHealth;
    float speed;   
};

PlayerConfig LoadPlayerConfig(const char* filePath);

void InitializePlayer(Player& player, PlayerConfig playerConfig);
void UpdatePlayer(Player& player, Room rooms[], int& numRoomLocs, bool debugSpeed, bool debugWall);

#endif