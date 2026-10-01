#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#include "Room.h"

struct Player {
    Vector2 position;
    Vector2 center;
    Vector2 direction;

    float speed;
    float health;
    float iFrames;

    Rectangle hitBox;

    bool canMove;
    bool dashing;
    Vector2 tempDirection;

    float dashTime;
    float dashCD;
};

void InitializePlayer(Player& player);
void UpdatePlayer(Player& player, Room rooms[], int& numRoomLocs, bool debugSpeed, bool debugWall);
void DrawPlayer(const Player& player);

#endif