#pragma once

#include "assetManager.h"

#include "raylib.h"

#include <vector>
#include <string>

using namespace std;

struct Player;
struct Enemy;

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
    
    bool projCollision;
    bool playerCollision;
    
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

struct FloorSpawnPool {
    int floorNum;
    int minEnemiesPerRoom;
    int maxEnemiesPerRoom;
    int minEnemiesPerWave;
    int maxEnemiesPerWave;

    vector<EnemySpawnInfo> enemySpawnInfo;
    
};

struct EnemySpawnPoint {
    vector<const char*> names;
    Vector2 position;
};

struct RoomContent {
    //Room hitbox
    Rectangle hitbox;

    //Enemy spawning
    int numEnemiesToSpawn;
    int numEnemiesPerWave;
    
    vector<EnemySpawnPoint> enemySpawnPoints;
    
    //Walls
    vector<Wall> walls;
    
    //Doors
    vector<Door> doors;
    
    //Decorations
    vector<RoomDecor> decor;

    //Breakable objects
    vector<RoomBreakable> breakables;

    //Usable objects
    vector<RoomUsable> usables;
    
    //Background
    Texture2D backgroundTexture;
    
    Vector2 backgroundSize;
    
    
};

enum RoomState {
    INACTIVE,
    DISCOVERED,
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

struct Dungeon {   
    int floor;
    int bestFloor;
    
    vector<FloorSpawnPool> floorSpawnPools;
    
    vector<Room> rooms;
    
    int currentRooms;   //number of existing rooms (not empty)
    
    int maxRooms;
    int roomSize;

    float straight;
    float turn;
    float threeWay;
    float fourWay;
  
};

void InitializeDungeon(const char* filePath, Dungeon& dungeon);

void SetRoomInfo(Dungeon& dungeon, Room& room);

void UpdateDungeon(Dungeon& dungeon, Player& player, vector<Enemy>& enemies);

bool RoomForBoss(Dungeon& dungeon, const Room& emptyRoom);
bool ValidRoomInDirection(const Room& currentRoom, char direction);
void AddDirection(char directions[], char direction);
const char* TestDeadEnd(Dungeon& dungeon, const Room& currentRoom);
void SortRooms(Dungeon& dungeon);
void AddRoom(Dungeon& dungeon, const char* type, Vector2 position);
void GenerateDungeon(Dungeon& dungeon);
