#pragma once

#include "raylib.h"

struct Wall {
    Rectangle hitbox;
    Texture2D texture;
    
};

struct Door {
    Rectangle hitbox;
    Texture2D texture;

    bool active;
};

struct RoomDecor {
    Rectangle hitbox;
    Texture2D texture;
    
};

struct RoomBreakable {
    Rectangle hitbox;
    Texture2D texture;
    
    float health;
    
};

struct RoomUsable {
    Rectangle hitbox;
    Texture2D texture;
    
};

struct EnemySpawnInfo {
    const char* type;
    float spawnWeight;
    
};

struct RoomContent {
    //Enemy spawning
    int numEnemiesSpawn;
    
    int numEnemyTypes;
    EnemySpawnInfo enemySpawnInfo[10] = {};
    
    //Walls
    int numWalls;
    Wall walls[20] = {};
    
    //Doors
    int numDoors;
    Door doors[4] = {};
    
    //Decorations
    int numDecor;
    RoomDecor decor[20] = {};

    //Breakable objects
    int numBreakables;
    RoomBreakable breakables[20] = {};

    //Usable objects
    int numUsables;
    RoomUsable usables[20] = {};
    
    
};

enum RoomState {
    INACTIVE,
    ACTIVE,
    CLEARED
};

struct Room {
    const char* type;     //2DNS, 2DEW, 2DNE, 2DES, 2DSW, 2DNW, 3DNEW, 3DNES, 3DESW, 3DNSW, 4DNESW
    Vector2 size;
    Vector2 position;
    Vector2 gridPos;
    int distance;
    bool exists = false;
    
    //Perimeter (temp)
    int numWalls;
    Rectangle walls[12];
    int numDoors;
    Rectangle doors[4];
    
    //Room content
    RoomContent roomContent = {};
    
    //State
    RoomState state;
    
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
