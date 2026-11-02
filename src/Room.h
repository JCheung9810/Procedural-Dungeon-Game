#pragma once

#include "raylib.h"

struct RoomObjects {
    const char* type;
    
    Vector2 position;
    Vector2 size;
    
    Rectangle hitbox;
    
    bool exists = false;
    
};

struct EnemySpawnInfo {
    const char* type;
    float spawnWeight;
    
};

struct RoomContent {
    int numEnemiesSpawn;
    
    int numEnemyTypes;
    EnemySpawnInfo enemySpawnInfo[10] = {};
    
    int numRoomObjects;
    RoomObjects roomObject[20] = {};
    
};

struct Room {
    const char* type;     //2DNS, 2DEW, 2DNE, 2DES, 2DSW, 2DNW, 3DNEW, 3DNES, 3DESW, 3DNSW, 4DNESW
    Vector2 size;
    Vector2 position;
    Vector2 gridPos;
    int distance;
    bool exists = false;
    
    //Perimeter
    int numWalls;
    Rectangle walls[12];
    int numDoors;
    Rectangle doors[4];
    
    //Interior
    RoomContent roomContent = {};
    
};

struct DungeonConfig {
    int maxRooms;
    int roomSize;
    float wallDepth;
    int doorSize;

    float straight;
    float turn;
    float threeWay;
    float fourWay;
};

DungeonConfig LoadDungeonConfig(const char* filePath);

bool RoomForBoss(Room rooms[], Room emptyRoom, int numRoomLocs);
bool ValidRoomInDirection(Room currentRoom, char direction);
void AddDirection(char directions[], char direction);
const char* TestDeadEnd(Room rooms[], Room currentRoom, int numRoomLocs);
void SortRooms(Room rooms[], int numRoomLocs);
void AddRoom(Room rooms[], const char* type, int& currentRooms, int maxRooms, int& numRoomLocs, Vector2 position, int roomSize, float wallDepth, int doorSize);
void GenerateEndRoom(Room rooms[], int& numRoomLocs);
void GenerateDungeon(Room rooms[], DungeonConfig dungeonConfig, int& currentRooms, int& numRoomLocs);
