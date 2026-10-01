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

bool RoomForBoss(Room rooms[], Room emptyRoom, int numRoomLocs);
bool ValidRoomInDirection(Room currentRoom, char direction);
void AddDirection(char directions[], char direction);
char* TestDeadEnd(Room rooms[], Room currentRoom, int numRoomLocs);
void SortRooms(Room rooms[], int numRoomLocs);
void AddRoom(Room rooms[], char* type, int& currentRooms, int maxRooms, int& numRoomLocs, Vector2 position, int roomSize, float wallDepth, int doorSize);
void GenerateDungeon(Room rooms[], int& currentRooms, int& maxRooms, int& numRoomLocs, int roomSize, float wallDepth, int doorSize, float straight, float turn, float threeWay, float fourWay, int& bossRoomIndex);

#endif