#pragma once

#include "assetManager.h"

#include "raylib.h"

#include <string>

using namespace std;

//Load sprite sheet with srcRec once
//Load json for level once, for each wallType, assign a certain id and set hitbox values
//When drawing use the srcRec based on ID
struct Wall {
    Rectangle hitbox;
    SpriteID spriteID;
    
};

struct Door {
    Rectangle hitbox;
    SpriteID spriteID;

    bool active;
};

struct RoomDecor {
    Rectangle hitbox;
    SpriteID spriteID;
    
    bool collision;
    
};

struct RoomBreakable {
    Rectangle hitbox;
    SpriteID spriteID;
    
    float health;
    
};

struct RoomUsable {
    Rectangle hitbox;
    SpriteID spriteID;
    
    bool active;
    
};

struct EnemySpawnInfo {
    const char* name;
    float spawnWeight;
    
};

struct EnemySpawnPoint {
    const char* name[10] = {};
    Vector2 position;
};

struct RoomContent {
    //Enemy spawning
    int numEnemiesSpawn;
    
    int numEnemySpawnPoints;
    EnemySpawnPoint enemySpawnPoint[10] = {};
    
    //Walls
    int numWalls;
    Wall walls[50] = {};
    
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
    
    //Background
    Texture2D backgroundTexture;
    
    Vector2 backgroundSize;
    
    
};

enum RoomState {
    INACTIVE,
    ACTIVE,
    CLEARED
};

struct Room {
    const char* type;     //2DNS, 2DEW, 2DNE, 2DES, 2DSW, 2DNW, 3DNEW, 3DNES, 3DESW, 3DNSW, 4DNESW
    string layoutType;
    
    Vector2 size;
    Vector2 position;
    Vector2 gridPos;
    int distance;
    bool exists = false;
    
    //Room content
    RoomContent roomContent = {};
    
    //State
    RoomState state = RoomState::INACTIVE;
    
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

struct Dungeon {   
    int floor;
    int bestFloor;
    
    int numEnemyTypes;
    EnemySpawnInfo enemySpawnInfo[10] = {};
    
    Room rooms[100] = {};
    
    int currentRooms;
    int numRoomLocs;
    
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

void InitializeDungeon(Dungeon& dungeon, DungeonConfig dungeonConfig);
void SetRoomInfo(Room& room);
bool RoomForBoss(Dungeon& dungeon, const Room& emptyRoom);
bool ValidRoomInDirection(const Room& currentRoom, char direction);
void AddDirection(char directions[], char direction);
const char* TestDeadEnd(Dungeon& dungeon, const Room& currentRoom);
void SortRooms(Dungeon& dungeon);
void AddRoom(Dungeon& dungeon, const char* type, Vector2 position);
void GenerateDungeon(Dungeon& dungeon);
