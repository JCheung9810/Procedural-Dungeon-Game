#include "dungeon.h"

#include "assetManager.h"

#include "json.hpp"

#include <fstream>
#include <cstring>
#include <string>

using json = nlohmann::json;
using namespace std;

#include <raymath.h>

void InitializeDungeon(const char* filePath, Dungeon& dungeon) {
    ifstream file(filePath);

    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Could not open dungeon config: %s", filePath);
        return;
    }

    try {
        json data;
        file >> data;

        dungeon.maxRooms = data.at("maxRooms").get<int>();
        dungeon.roomSize = data.at("roomSize").get<int>();

        dungeon.straight = data.at("straight").get<float>();
        dungeon.turn = data.at("turn").get<float>();
        dungeon.threeWay = data.at("threeWay").get<float>();
        dungeon.fourWay = data.at("fourWay").get<float>();
        
        for(const auto& floorInfo : data.at("floorSpawnPool")){

            int i = floorInfo.at("floorNum").get<int>();

            FloorSpawnPool& floorSpawnPool = dungeon.floorSpawnPool[i];

            const auto& enemyPool = floorInfo.at("enemyPool");

            floorSpawnPool.numEnemyTypes = enemyPool.size();

            for(int j = 0; j < floorSpawnPool.numEnemyTypes; j++){

                string name = enemyPool[j].at("name").get<string>();

                floorSpawnPool.enemySpawnInfo[j].name = strdup(name.c_str());

                floorSpawnPool.enemySpawnInfo[j].spawnWeight = enemyPool[j].at("spawnWeight").get<float>();
            }
        }
        
        //Initialize vars
        dungeon.floor = 0;
        dungeon.bestFloor = dungeon.floor;
        dungeon.currentRooms = 0;
        dungeon.numRoomLocs = 0;
        
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Invalid dungeon config: %s", e.what());
        throw;
    }
}

void SetRoomInfo(Room& room) {
    //Reset room contents
    room.roomContent.numWalls = 0;
    room.roomContent.numDoors = 0;
    room.roomContent.numDecor = 0;
    room.roomContent.numBreakables = 0;
    room.roomContent.numUsables = 0;
    room.roomContent.numEnemySpawnPoints = 0;
    room.roomContent.numEnemiesSpawn = 0;
    
    //Load corresponding roomType
    const char* roomPath = TextFormat("ldtkSaves/map/simplified/_%s/data.json", room.layoutType.c_str());
    ifstream file(roomPath);
    
    //Load corresponding background
    const char* backgroundPath = TextFormat("ldtkSaves/map/simplified/_%s/_composite.png", room.layoutType.c_str());
    room.roomContent.backgroundTexture = assets.LoadGameTexture(backgroundPath);

    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Could not open level data");
        return;
    }

    try {
        json data;
        file >> data;

        const auto& entities = data["entities"];
        
        room.roomContent.backgroundSize = (Vector2){room.size.x / data.at("width").get<float>(), room.size.y / data.at("height").get<float>()};
        float scale =  room.roomContent.backgroundSize.x;

        //Go through each wall type
        for (auto i = entities.begin(); i != entities.end(); ++i) {

            //Go through each instance of the entities
            for (const auto& entity : i.value()) {
                
                if(entity["layer"] == "Walls"){

                    if (room.roomContent.numWalls >= 50){
                        TraceLog(LOG_WARNING, "Maximum room walls reached");
                        return;
                    }

                    Wall& wall = room.roomContent.walls[room.roomContent.numWalls];

                    wall.spriteID = GetSpriteID(i.key());

                    wall.hitbox = {
                        entity["x"].get<float>() * scale + room.position.x,
                        entity["y"].get<float>() * scale + room.position.y,
                        entity["width"].get<float>() * scale,
                        entity["height"].get<float>() * scale
                    };

                    room.roomContent.numWalls++;
                    
                } else if(entity["layer"] == "EnemySpawPoint"){
                    if (room.roomContent.numEnemySpawnPoints >= 10){
                        TraceLog(LOG_WARNING, "Maximum enemy spawn points reached");
                        return;
                    }
                    
                    EnemySpawnPoint& enemySpawnPoint = room.roomContent.enemySpawnPoint[room.roomContent.numEnemySpawnPoints];
                    
                    enemySpawnPoint.position = {
                        entity["x"].get<float>() * scale + room.position.x, 
                        entity["y"].get<float>() * scale + room.position.y
                    };
                    
                    room.roomContent.numEnemySpawnPoints++;
                } else if(entity["layer"] == "Decor"){
                    if (room.roomContent.numDecor >= 50){
                        TraceLog(LOG_WARNING, "Maximum room decors reached");
                        return;
                    }

                    RoomDecor& decor = room.roomContent.decor[room.roomContent.numDecor];

                    decor.spriteID = GetSpriteID(i.key());

                    decor.hitbox = {
                        entity["x"].get<float>() * scale + room.position.x,
                        entity["y"].get<float>() * scale + room.position.y,
                        entity["width"].get<float>() * scale,
                        entity["height"].get<float>() * scale
                    };
                    
                    decor.playerCollision = entity["customFields"].at("playerCollision").get<bool>();
                    decor.projCollision = entity["customFields"].at("projCollision").get<bool>();

                    room.roomContent.numDecor++;
                }
            }
        }
    }
    catch (const json::exception& e) {
        TraceLog(LOG_ERROR, "Error loading level data: %s", e.what());
    }
}

bool RoomForBoss(Dungeon& dungeon, const Room& emptyRoom){
    
    bool available = true;
    
    if(TextIsEqual(emptyRoom.type, "EmptyN")){
        for(int i = 0; i < dungeon.numRoomLocs; i++){
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,0} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,0} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,-1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,-1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,-1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,-2} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,-2} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,-2} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
        }
    } else if(TextIsEqual(emptyRoom.type, "EmptyE")){
        for(int i = 0; i < dungeon.numRoomLocs; i++){
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,-1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,0} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,-1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){2,1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){2,0} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){2,-1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
        }
    } else if(TextIsEqual(emptyRoom.type, "EmptyS")){
        for(int i = 0; i < dungeon.numRoomLocs; i++){
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,0} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,0} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,2} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,2} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){1,2} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
        }
    } else if(TextIsEqual(emptyRoom.type, "EmptyW")){
        for(int i = 0; i < dungeon.numRoomLocs; i++){
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,1} + emptyRoom.gridPos  || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){0,-1} + emptyRoom.gridPos  || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,1} + emptyRoom.gridPos  || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,0} + emptyRoom.gridPos  || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-1,-1} + emptyRoom.gridPos || !dungeon.rooms[i].exists);
            
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-2,1} + emptyRoom.gridPos  || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-2,0} + emptyRoom.gridPos  || !dungeon.rooms[i].exists);
            available = available && (dungeon.rooms[i].gridPos != (Vector2){-2,-1} + emptyRoom.gridPos  || !dungeon.rooms[i].exists);
        }
    }
    
    return available;
}

bool ValidRoomInDirection(const Room& currentRoom, char direction){
    bool valid = false;
    bool isBoss;
    bool isEnd;
    
    isBoss =
        (TextIsEqual(currentRoom.type, "BossN") ||
         TextIsEqual(currentRoom.type, "BossE") ||
         TextIsEqual(currentRoom.type, "BossS") ||
         TextIsEqual(currentRoom.type, "BossW"));
         
    isEnd =
        (TextIsEqual(currentRoom.type, "EndN") ||
         TextIsEqual(currentRoom.type, "EndE") ||
         TextIsEqual(currentRoom.type, "EndS") ||
         TextIsEqual(currentRoom.type, "EndW"));
    
    if(TextIsEqual(currentRoom.type,"4DNESW") || TextIsEqual(currentRoom.type,"Spawn") || isBoss || isEnd){
        return true;
    }
    
    if(direction == 'N'){
        valid = ( 
            TextIsEqual(currentRoom.type,"2DNS") || 
            TextIsEqual(currentRoom.type,"2DES") || 
            TextIsEqual(currentRoom.type,"2DSW") || 
            TextIsEqual(currentRoom.type,"3DESW") || 
            TextIsEqual(currentRoom.type,"3DNES") || 
            TextIsEqual(currentRoom.type,"3DNSW"));
    } else if(direction == 'E'){
        valid = (
            TextIsEqual(currentRoom.type,"2DEW") || 
            TextIsEqual(currentRoom.type,"2DSW") || 
            TextIsEqual(currentRoom.type,"2DNW") || 
            TextIsEqual(currentRoom.type,"3DNSW") || 
            TextIsEqual(currentRoom.type,"3DESW") || 
            TextIsEqual(currentRoom.type,"3DNEW"));
    } else if(direction == 'S'){
        valid = (
            TextIsEqual(currentRoom.type,"2DNS") || 
            TextIsEqual(currentRoom.type,"2DNW") || 
            TextIsEqual(currentRoom.type,"2DNE") || 
            TextIsEqual(currentRoom.type,"3DNEW") || 
            TextIsEqual(currentRoom.type,"3DNSW") || 
            TextIsEqual(currentRoom.type,"3DNES"));
    } else if(direction == 'W'){
        valid = (
            TextIsEqual(currentRoom.type,"2DEW") || 
            TextIsEqual(currentRoom.type,"2DNE") || 
            TextIsEqual(currentRoom.type,"2DES") || 
            TextIsEqual(currentRoom.type,"3DNES") || 
            TextIsEqual(currentRoom.type,"3DNEW") || 
            TextIsEqual(currentRoom.type,"3DESW"));
    }
    
    return valid;
}

void AddDirection(char directions[], char direction){
    int len = TextLength(directions);
    directions[len] = direction;
    directions[len + 1] = '\0';
}

const char* TestDeadEnd(Dungeon& dungeon, const Room& currentRoom){
    
    static char directions[5] = "";
    
    // Reset directions
    directions[0] = '\0';
    
    for(int i = 0; i < dungeon.numRoomLocs; i++){
        // 2DNS, 2DEW, 2DNE, 2DES, 2DSW, 2DNW,
        // 3DNEW, 3DNES, 3DESW, 3DNSW, 4DNESW
        
        if(TextIsEqual(currentRoom.type, "2DNS")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DEW")){
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DNE")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DES")){
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DSW")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DNW")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DNEW")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DNES")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DESW")){
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DNSW")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "4DNESW")){
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "End")){
            //Find boss gridPos
            Vector2 bossGridPos;
            for(int j = 0; j < dungeon.numRoomLocs; j++){
                bool isBoss =
                (TextIsEqual(dungeon.rooms[j].type, "BossN") ||
                 TextIsEqual(dungeon.rooms[j].type, "BossE") ||
                 TextIsEqual(dungeon.rooms[j].type, "BossS") ||
                 TextIsEqual(dungeon.rooms[j].type, "BossW"));
                 
                if(isBoss){
                    bossGridPos = dungeon.rooms[j].gridPos;
                }
                
            }
            
            if((dungeon.rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(dungeon.rooms[i], 'N') && (bossGridPos != currentRoom.gridPos + (Vector2){0,-3})){
                AddDirection(directions, 'N');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'E') && (bossGridPos != currentRoom.gridPos + (Vector2){3,0})){
                AddDirection(directions, 'E');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'S') && (bossGridPos != currentRoom.gridPos + (Vector2){0,3})){
                AddDirection(directions, 'S');
            }
            if((dungeon.rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(dungeon.rooms[i], 'W') && (bossGridPos != currentRoom.gridPos + (Vector2){-3,0})){
                AddDirection(directions, 'W');
            }
        }
    }
    
    return directions;
}

void SortRooms(Dungeon& dungeon){
    int unsorted = dungeon.numRoomLocs;
    
    for(int i = 0; i < dungeon.numRoomLocs - 1; i++){
        for(int j = 0; j < unsorted - 1; j++){
            if(dungeon.rooms[j].distance > dungeon.rooms[j+1].distance){
                Room tempRoom = dungeon.rooms[j+1];
                dungeon.rooms[j+1] = dungeon.rooms[j];
                dungeon.rooms[j] = tempRoom;
                
                
            }
            
        }
        unsorted--;
    }
    
}

void AddRoom(Dungeon& dungeon, const char* type, Vector2 position){   
    
    Vector2 roomPositionOffset;
    int CurrentRoomIndex;
    bool isEmpty;
    bool isBoss;
    bool isEnd;
    
    isEmpty =
        (TextIsEqual(type, "EmptyN") ||
         TextIsEqual(type, "EmptyE") ||
         TextIsEqual(type, "EmptyS") ||
         TextIsEqual(type, "EmptyW"));
         
    isBoss =
        (TextIsEqual(type, "BossN") ||
         TextIsEqual(type, "BossE") ||
         TextIsEqual(type, "BossS") ||
         TextIsEqual(type, "BossW"));
        
    isEnd =
        (TextIsEqual(type, "EndN") ||
         TextIsEqual(type, "EndE") ||
         TextIsEqual(type, "EndS") ||
         TextIsEqual(type, "EndW"));
       
       
    if(dungeon.numRoomLocs >= 100){
        TraceLog(LOG_ERROR, "Dungeon room array capacity exceeded");
        return;
    }
    
    CurrentRoomIndex = dungeon.numRoomLocs;
    
    //Set current room up
    dungeon.rooms[CurrentRoomIndex].gridPos = position;
    dungeon.rooms[CurrentRoomIndex].type = type; 
    dungeon.rooms[CurrentRoomIndex].layoutType = type; 

    if(!isBoss){  
        roomPositionOffset = (Vector2){-dungeon.roomSize/2.0f, -dungeon.roomSize/2.0f};
        dungeon.rooms[CurrentRoomIndex].position = (position * dungeon.roomSize) + roomPositionOffset;
        dungeon.rooms[CurrentRoomIndex].size = (Vector2){(float)dungeon.roomSize, (float)dungeon.roomSize};
    } else {
        roomPositionOffset = (Vector2){-dungeon.roomSize * 3.0f / 2.0f, -dungeon.roomSize * 3.0f / 2.0f};
        dungeon.rooms[CurrentRoomIndex].size = (Vector2){(float)dungeon.roomSize * 3.0f, (float)dungeon.roomSize * 3.0f};
        if (TextIsEqual(dungeon.rooms[CurrentRoomIndex].type, "BossN")) {
            dungeon.rooms[CurrentRoomIndex].position = (position * (float)dungeon.roomSize) + roomPositionOffset + (Vector2){0, -(float)dungeon.roomSize};
        } else if (TextIsEqual(dungeon.rooms[CurrentRoomIndex].type, "BossE")) {
            dungeon.rooms[CurrentRoomIndex].position = (position * (float)dungeon.roomSize) + roomPositionOffset + (Vector2){(float)dungeon.roomSize, 0};
        } else if (TextIsEqual(dungeon.rooms[CurrentRoomIndex].type, "BossS")) {
            dungeon.rooms[CurrentRoomIndex].position = (position * (float)dungeon.roomSize) + roomPositionOffset + (Vector2){0, (float)dungeon.roomSize};
        } else if (TextIsEqual(dungeon.rooms[CurrentRoomIndex].type, "BossW")) {
            dungeon.rooms[CurrentRoomIndex].position = (position * (float)dungeon.roomSize) + roomPositionOffset + (Vector2){-(float)dungeon.roomSize, 0};
        }
    }
    dungeon.rooms[CurrentRoomIndex].distance = abs(dungeon.rooms[CurrentRoomIndex].gridPos.x) + abs(dungeon.rooms[CurrentRoomIndex].gridPos.y); 
    
    //--------------------------------------------------------------------------
    // ROOM TYPE
    
    //Check if empty type
    if(isEmpty){
        
        dungeon.rooms[CurrentRoomIndex].exists = false;
        
        dungeon.numRoomLocs++;
        
    //Check if boss type
    } else if(isBoss){ 
        //Add boss room
        dungeon.rooms[CurrentRoomIndex].exists = true;
        dungeon.numRoomLocs++;
        
        if(TextIsEqual(type, "BossN")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){0,-3});
                    
        } else if(TextIsEqual(type, "BossE")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyE", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){3,0});
            
        } else if(TextIsEqual(type, "BossS")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyS", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){0,3});
                    
        } else if(TextIsEqual(type, "BossW")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyW", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){-3,0});
        }
        
        dungeon.currentRooms++;
        
    }else if(dungeon.currentRooms < dungeon.maxRooms){
        //Add the room 
        dungeon.rooms[CurrentRoomIndex].exists = true;
        dungeon.numRoomLocs++;
        
        //--------------------------------------------------------------------------
        // 4 DOORS
        
        if(TextIsEqual(type, "4DNESW") || TextIsEqual(type, "Spawn") || isEnd){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/S/W
        
        } 
        else if(TextIsEqual(type, "3DNSW")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - E/S/W
        
        } 
        else if(TextIsEqual(type, "3DESW")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/E/S
        
        } 
        else if(TextIsEqual(type, "3DNES")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/E/W
        
        } 
        else if(TextIsEqual(type, "3DNEW")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/W
        
        } 
        else if(TextIsEqual(type, "2DNW")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - S/W
        
        } 
        else if(TextIsEqual(type, "2DSW")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - E/S
        
        } 
        else if(TextIsEqual(type, "2DES")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/E
        
        } 
        else if(TextIsEqual(type, "2DNE")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - E/W
        
        } 
        else if(TextIsEqual(type, "2DEW")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/S
        
        } 
        else if(TextIsEqual(type, "2DNS")){
            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
        }
        
        dungeon.currentRooms++;
    }
    
    //Purge dupe locations
    for(int i = 0; i < dungeon.numRoomLocs; i++){
        for(int j = i + 1; j < dungeon.numRoomLocs; j++){
            
            if(dungeon.rooms[i].gridPos == dungeon.rooms[j].gridPos &&
               (dungeon.rooms[i].exists == false || dungeon.rooms[j].exists == false)){
                
                if(dungeon.rooms[i].exists == false){
                    dungeon.rooms[i] = dungeon.rooms[dungeon.numRoomLocs - 1];
                    dungeon.numRoomLocs--;
                    
                    //Recheck swapped room
                    i--;
                    break;
                    
                } else {
                    dungeon.rooms[j] = dungeon.rooms[dungeon.numRoomLocs - 1];
                    dungeon.numRoomLocs--;
                    
                    //Recheck swapped room
                    j--;
                }
            }
        }
    }
    
    SortRooms(dungeon);   
}

void GenerateDungeon(Dungeon& dungeon){
    
    const char* deadEnds;
    int chosenEmptyRoom;
    int chosenRoomType;
    
    dungeon.currentRooms = 0;
    dungeon.numRoomLocs = 0;
    
    Vector2 straightChance = (Vector2){1,dungeon.straight};
    Vector2 turnChance = (Vector2){straightChance.y + 1,straightChance.y + dungeon.turn};
    Vector2 threeWayChance = (Vector2){turnChance.y + 1,turnChance.y + dungeon.threeWay};
    Vector2 fourWayChance = (Vector2){threeWayChance.y + 1,threeWayChance.y + dungeon.fourWay};
    
    bool generateDungeon = true;
    
    
    AddRoom(dungeon, "Spawn", (Vector2){0,0});
    
    while(generateDungeon){
        
        while(dungeon.currentRooms < dungeon.maxRooms - 2){     //account for boss and end room
            
            chosenEmptyRoom = -1;
            
            //Choose closest empty room
            for(int i = 0; i < dungeon.numRoomLocs; i++){
                if(dungeon.rooms[i].exists == false){
                    chosenEmptyRoom = i;
                    break;
                }
                
            }
            
            //If not stuck
            if(chosenEmptyRoom != -1){
                chosenRoomType = GetRandomValue(1, 100);
                
                //Spawn corresponding room
                if(TextIsEqual(dungeon.rooms[chosenEmptyRoom].type,"EmptyN")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //dungeon.straight
                        AddRoom(dungeon, "2DNS", dungeon.rooms[chosenEmptyRoom].gridPos);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - dungeon.turn / 2){
                            //Right
                            AddRoom(dungeon, "2DES", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Left
                            AddRoom(dungeon, "2DSW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * dungeon.threeWay / 3){
                            //Split Left Right
                            AddRoom(dungeon, "3DESW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - dungeon.threeWay / 3){
                            //Split Front Right
                            AddRoom(dungeon, "3DNES", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Split Front Left
                            AddRoom(dungeon, "3DNSW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    }
                } else if(TextIsEqual(dungeon.rooms[chosenEmptyRoom].type,"EmptyE")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //dungeon.straight
                        AddRoom(dungeon, "2DEW", dungeon.rooms[chosenEmptyRoom].gridPos);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - dungeon.turn / 2){
                            //Right
                            AddRoom(dungeon, "2DSW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Left
                            AddRoom(dungeon, "2DNW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * dungeon.threeWay / 3){
                            //Split Left Right
                            AddRoom(dungeon, "3DNSW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - dungeon.threeWay / 3){
                            //Split Front Right
                            AddRoom(dungeon, "3DESW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Split Front Left
                            AddRoom(dungeon, "3DNEW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    }
                } else if(TextIsEqual(dungeon.rooms[chosenEmptyRoom].type,"EmptyS")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //dungeon.straight
                        AddRoom(dungeon, "2DNS", dungeon.rooms[chosenEmptyRoom].gridPos);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - dungeon.turn / 2){
                            //Right
                            AddRoom(dungeon, "2DNW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Left
                            AddRoom(dungeon, "2DNE", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * dungeon.threeWay / 3){
                            //Split Left Right
                            AddRoom(dungeon, "3DNEW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - dungeon.threeWay / 3){
                            //Split Front Right
                            AddRoom(dungeon, "3DNSW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Split Front Left
                            AddRoom(dungeon, "3DNES", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    }
                } else if(TextIsEqual(dungeon.rooms[chosenEmptyRoom].type,"EmptyW")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //dungeon.straight
                        AddRoom(dungeon, "2DEW", dungeon.rooms[chosenEmptyRoom].gridPos);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - dungeon.turn / 2){
                            //Right
                            AddRoom(dungeon, "2DNE", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Left
                            AddRoom(dungeon, "2DES", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * dungeon.threeWay / 3){
                            //Split Left Right
                            AddRoom(dungeon, "3DNES", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - dungeon.threeWay / 3){
                            //Split Front Right
                            AddRoom(dungeon, "3DNEW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        } else {
                            //Split Front Left
                            AddRoom(dungeon, "3DESW", dungeon.rooms[chosenEmptyRoom].gridPos);
                        }
                    }
                }
                
                
                //Wall empty dungeon.rooms can have the same 4 way
                if(chosenRoomType >= fourWayChance.x && chosenRoomType <= fourWayChance.y){
                    //4 Way intersection
                    AddRoom(dungeon, "4DNESW", dungeon.rooms[chosenEmptyRoom].gridPos);                    
                }
                
            } else {
                
                //Reset generation because we are stuck
                dungeon.currentRooms = 0;
                dungeon.numRoomLocs = 0;
                AddRoom(dungeon, "Spawn", (Vector2){0,0});
                break;
                
            }            
        }   
        
        int bossRoomIndex = -1;
        const char* bossRoomOpening;
    
        //Choose boss room
        for(int i = dungeon.numRoomLocs - 1; i > -1; i--){
            if(dungeon.rooms[i].exists == false && RoomForBoss(dungeon, dungeon.rooms[i])){
                bossRoomIndex = i;
                break;
            }
            
        }
        
        //If valid
        if(bossRoomIndex != -1){
            generateDungeon = false;  
            if(TextIsEqual(dungeon.rooms[bossRoomIndex].type, "EmptyN")){
                bossRoomOpening = "S";
                AddRoom(dungeon, "BossN", dungeon.rooms[bossRoomIndex].gridPos);
            } else if(TextIsEqual(dungeon.rooms[bossRoomIndex].type, "EmptyE")){
                bossRoomOpening = "W";
                AddRoom(dungeon, "BossE", dungeon.rooms[bossRoomIndex].gridPos);
            } else if(TextIsEqual(dungeon.rooms[bossRoomIndex].type, "EmptyS")){
                bossRoomOpening = "N";
                AddRoom(dungeon, "BossS", dungeon.rooms[bossRoomIndex].gridPos);
            } else if(TextIsEqual(dungeon.rooms[bossRoomIndex].type, "EmptyW")){
                bossRoomOpening = "E";
                AddRoom(dungeon, "BossW", dungeon.rooms[bossRoomIndex].gridPos);
            }
        } else {
            //Reset generation because we cant spawn boss room
            dungeon.currentRooms = 0;
            dungeon.numRoomLocs = 0;
            AddRoom(dungeon, "Spawn", (Vector2){0,0});
            break;
        }
        
        int endRoomIndex = -1;
    
        //Choose End Room
        for(int i = dungeon.numRoomLocs - 1; i > -1; i--){
            if(dungeon.rooms[i].exists == false){
                endRoomIndex = i;
                break;
            }
            
        }
        
        //If valid
        if(endRoomIndex != -1){
            const char* endType;
            
            if(TextIsEqual(bossRoomOpening, "N")){
                endType = "EndN";
            } else if(TextIsEqual(bossRoomOpening, "E")){
                endType = "EndE";
            } if(TextIsEqual(bossRoomOpening, "S")){
                endType = "EndS";
            } if(TextIsEqual(bossRoomOpening, "W")){
                endType = "EndW";
            }

            AddRoom(dungeon, endType, dungeon.rooms[endRoomIndex].gridPos);
        } else {
            //Reset generation because we cant spawn end room
            dungeon.currentRooms = 0;
            dungeon.numRoomLocs = 0;
            AddRoom(dungeon, "Spawn", (Vector2){0,0});
            break;
        }
    }

    
    //Make dead ends    
    for(int i = 0; i < dungeon.numRoomLocs; i++){
        if(dungeon.rooms[i].exists){
        
            deadEnds = TestDeadEnd(dungeon, dungeon.rooms[i]);
            
            string newType = dungeon.rooms[i].type;

            for (int j = 0; deadEnds[j] != '\0'; j++) {
                size_t pos = newType.find(deadEnds[j]);

                if (pos != std::string::npos) {
                    newType.erase(pos, 1);
                    newType[0]--;
                }
            }
            
            dungeon.rooms[i].layoutType = newType;
            SetRoomInfo(dungeon.rooms[i]);
        }
    }
}
