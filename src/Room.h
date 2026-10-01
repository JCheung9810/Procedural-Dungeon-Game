#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"

struct Room {
    char* type;     //2DNS, 2DEW, 2DNE, 2DES, 2DSW, 2DNW, 3DNEW, 3DNES, 3DESW, 3DNSW, 4DNESW
    Vector2 size;
    Vector2 position;
    Vector2 gridPos;
    int distance;
    bool exists = false;
    
    int numWalls;
    Rectangle walls[12];
    int numDoors;
    Rectangle doors[4];
    
};

#endif