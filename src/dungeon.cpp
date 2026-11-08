#include "dungeon.h"

#include "assetManager.h"

#include "json.hpp"
#include <fstream>

using json = nlohmann::json;
using namespace std;

#include <raymath.h>

DungeonConfig LoadDungeonConfig(const char* filePath) {
    ifstream file(filePath);

    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Could not open dungeon config: %s", filePath);
        return {};
    }

    try {
        json data;
        file >> data;

        DungeonConfig config;

        config.maxRooms = data.at("maxRooms").get<int>();
        config.roomSize = data.at("roomSize").get<int>();
        config.wallDepth = data.at("wallDepth").get<float>();
        config.doorSize = data.at("doorSize").get<int>();

        config.straight = data.at("straight").get<float>();
        config.turn = data.at("turn").get<float>();
        config.threeWay = data.at("threeWay").get<float>();
        config.fourWay = data.at("fourWay").get<float>();

        return config;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Invalid dungeon config: %s", e.what());
        throw;
    }
}

void InitializeDungeon(Dungeon& dungeon, DungeonConfig dungeonConfig){
    
    dungeon.maxRooms = dungeonConfig.maxRooms;
    dungeon.roomSize = dungeonConfig.roomSize;
    dungeon.wallDepth = dungeonConfig.wallDepth;
    dungeon.doorSize = dungeonConfig.doorSize;

    dungeon.straight = dungeonConfig.straight;
    dungeon.turn = dungeonConfig.turn;
    dungeon.threeWay = dungeonConfig.threeWay;
    dungeon.fourWay = dungeonConfig.fourWay;
    
    dungeon.floor = 0;
    dungeon.bestFloor = dungeon.floor;
    dungeon.currentRooms = 0;
    dungeon.numRoomLocs = 0;
    
}

void SetRoomInfo(Room& room) {
    ifstream file("ldtkSaves/map/simplified/Level_0/data.json");
    
    room.roomContent.numWalls = 0;
    room.roomContent.backgroundTexture = assets.LoadGameTexture("ldtkSaves/map/simplified/Level_0/_composite.png");

    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Could not open level data");
        return;
    }

    try {
        json data;
        file >> data;

        const auto& entities = data["entities"];
        
        float scale =  room.size.x / 256.0f;;

        // Go through each wall type
        for (auto i = entities.begin(); i != entities.end(); ++i) {

            SpriteID spriteID = GetSpriteID(i.key());

            // Go through each instance of that wall type
            for (const auto& entity : i.value()) {

                if (room.roomContent.numWalls >= 50) {
                    TraceLog(LOG_WARNING, "Maximum room walls reached");
                    return;
                }

                Wall& wall = room.roomContent.walls[room.roomContent.numWalls];

                wall.spriteID = spriteID;

                wall.hitbox = {
                    entity["x"].get<float>() * scale + room.position.x,
                    entity["y"].get<float>() * scale + room.position.y,
                    entity["width"].get<float>() * scale,
                    entity["height"].get<float>() * scale
                };

                room.roomContent.numWalls++;
            }
        }
    }
    catch (const json::exception& e) {
        TraceLog(LOG_ERROR, "Error loading level data: %s", e.what());
    }
}

bool RoomForBoss(Dungeon dungeon, Room emptyRoom){
    
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

bool ValidRoomInDirection(Room currentRoom, char direction){
    bool valid = false;
    bool isBoss;
    
    isBoss =
        (TextIsEqual(currentRoom.type, "BossN") ||
         TextIsEqual(currentRoom.type, "BossE") ||
         TextIsEqual(currentRoom.type, "BossS") ||
         TextIsEqual(currentRoom.type, "BossW"));
    
    if(TextIsEqual(currentRoom.type,"4DNESW") || TextIsEqual(currentRoom.type,"Spawn") || isBoss || TextIsEqual(currentRoom.type,"End")){
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

const char* TestDeadEnd(Dungeon dungeon, Room currentRoom){
    
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
    float doorSide;
    bool isEmpty;
    bool isBoss;
    
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
    
    CurrentRoomIndex = dungeon.numRoomLocs;
    
    //Set current room up
    dungeon.rooms[CurrentRoomIndex].gridPos = position;
    dungeon.rooms[CurrentRoomIndex].type = type; 

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
    
    //Set room info
    SetRoomInfo(dungeon.rooms[CurrentRoomIndex]);

    //Wall width on each side of the door
    doorSide = (dungeon.rooms[CurrentRoomIndex].size.x - dungeon.doorSize) / 2.0f;    
    
    //--------------------------------------------------------------------------
    //WALL RECTANGLES
    
    //Top
    Rectangle topLeft = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y,
        doorSide,
        dungeon.wallDepth
    };
    
    Rectangle topRight = {
        dungeon.rooms[CurrentRoomIndex].position.x + doorSide + dungeon.doorSize,
        dungeon.rooms[CurrentRoomIndex].position.y,
        doorSide,
        dungeon.wallDepth
    };
    
    Rectangle topWall = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y,
        dungeon.rooms[CurrentRoomIndex].size.x,
        dungeon.wallDepth
    };
    
    //Bottom
    Rectangle bottomLeft = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y + dungeon.rooms[CurrentRoomIndex].size.y - dungeon.wallDepth,
        doorSide,
        dungeon.wallDepth
    };
    
    Rectangle bottomRight = {
        dungeon.rooms[CurrentRoomIndex].position.x + doorSide + dungeon.doorSize,
        dungeon.rooms[CurrentRoomIndex].position.y + dungeon.rooms[CurrentRoomIndex].size.y - dungeon.wallDepth,
        doorSide,
        dungeon.wallDepth
    };
    
    Rectangle bottomWall = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y + dungeon.rooms[CurrentRoomIndex].size.y - dungeon.wallDepth,
        dungeon.rooms[CurrentRoomIndex].size.x,
        dungeon.wallDepth
    };
    
    //Left
    Rectangle leftTop = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y,
        dungeon.wallDepth,
        doorSide
    };
    
    Rectangle leftBottom = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y + doorSide + dungeon.doorSize,
        dungeon.wallDepth,
        doorSide
    };
    
    Rectangle leftWall = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y,
        dungeon.wallDepth,
        dungeon.rooms[CurrentRoomIndex].size.y
    };
    
    //Right
    Rectangle rightTop = {
        dungeon.rooms[CurrentRoomIndex].position.x + dungeon.rooms[CurrentRoomIndex].size.x - dungeon.wallDepth,
        dungeon.rooms[CurrentRoomIndex].position.y,
        dungeon.wallDepth,
        doorSide
    };
    
    Rectangle rightBottom = {
        dungeon.rooms[CurrentRoomIndex].position.x + dungeon.rooms[CurrentRoomIndex].size.x - dungeon.wallDepth,
        dungeon.rooms[CurrentRoomIndex].position.y + doorSide + dungeon.doorSize,
        dungeon.wallDepth,
        doorSide
    };
    
    Rectangle rightWall = {
        dungeon.rooms[CurrentRoomIndex].position.x + dungeon.rooms[CurrentRoomIndex].size.x - dungeon.wallDepth,
        dungeon.rooms[CurrentRoomIndex].position.y,
        dungeon.wallDepth,
        dungeon.rooms[CurrentRoomIndex].size.y
    };
    
    //--------------------------------------------------------------------------
    // DOOR RECTANGLES
    
    Rectangle topDoor = {
        dungeon.rooms[CurrentRoomIndex].position.x + doorSide,
        dungeon.rooms[CurrentRoomIndex].position.y,
        (float)dungeon.doorSize,
        dungeon.wallDepth
    };
    
    Rectangle rightDoor = {
        dungeon.rooms[CurrentRoomIndex].position.x + dungeon.rooms[CurrentRoomIndex].size.x - dungeon.wallDepth,
        dungeon.rooms[CurrentRoomIndex].position.y + doorSide,
        dungeon.wallDepth,
        (float)dungeon.doorSize
    };
    
    Rectangle bottomDoor = {
        dungeon.rooms[CurrentRoomIndex].position.x + doorSide,
        dungeon.rooms[CurrentRoomIndex].position.y + dungeon.rooms[CurrentRoomIndex].size.y - dungeon.wallDepth,
        (float)dungeon.doorSize,
        dungeon.wallDepth
    };
    
    Rectangle leftDoor = {
        dungeon.rooms[CurrentRoomIndex].position.x,
        dungeon.rooms[CurrentRoomIndex].position.y + doorSide,
        dungeon.wallDepth,
        (float)dungeon.doorSize
    };
    
    //--------------------------------------------------------------------------
    // ROOM TYPE
    
    //Check if empty type
    if(isEmpty){
        
        dungeon.rooms[CurrentRoomIndex].exists = false;
        
        dungeon.numRoomLocs++;
        
    } else if(isBoss){ 
        //Add boss room
        dungeon.rooms[CurrentRoomIndex].exists = true;
        dungeon.rooms[CurrentRoomIndex].numWalls = 0;
        dungeon.rooms[CurrentRoomIndex].numDoors = 0;
        
        if(TextIsEqual(type, "BossN")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(dungeon, "EmptyN", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){0,-3});
                    
        } else if(TextIsEqual(type, "BossE")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            //Right
            AddRoom(dungeon, "EmptyE", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){3,0});
        } else if(TextIsEqual(type, "BossS")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            //Bottom
            AddRoom(dungeon, "EmptyS", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){0,3});
                    
        } else if(TextIsEqual(type, "BossW")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            //Left
            AddRoom(dungeon, "EmptyW", dungeon.rooms[CurrentRoomIndex].gridPos + (Vector2){-3,0});
        }
        
        dungeon.currentRooms++;
        
    }else if(dungeon.currentRooms < dungeon.maxRooms){
        //Add the room 
        dungeon.rooms[CurrentRoomIndex].exists = true;
        dungeon.rooms[CurrentRoomIndex].numWalls = 0;
        dungeon.rooms[CurrentRoomIndex].numDoors = 0;
        
        //--------------------------------------------------------------------------
        // 4 DOORS
        
        if(TextIsEqual(type, "4DNESW") || TextIsEqual(type, "Spawn") || TextIsEqual(type, "End")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/S/W
        
        } 
        else if(TextIsEqual(type, "3DNSW")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - E/S/W
        
        } 
        else if(TextIsEqual(type, "3DESW")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/E/S
        
        } 
        else if(TextIsEqual(type, "3DNES")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/E/W
        
        } 
        else if(TextIsEqual(type, "3DNEW")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/W
        
        } 
        else if(TextIsEqual(type, "2DNW")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - S/W
        
        } 
        else if(TextIsEqual(type, "2DSW")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - E/S
        
        } 
        else if(TextIsEqual(type, "2DES")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyS", (Vector2){position.x, position.y + 1});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/E
        
        } 
        else if(TextIsEqual(type, "2DNE")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyN", (Vector2){position.x, position.y - 1});
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - E/W
        
        } 
        else if(TextIsEqual(type, "2DEW")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightTop;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            dungeon.numRoomLocs++;

            //Add possible room locations
            AddRoom(dungeon, "EmptyE", (Vector2){position.x + 1, position.y});
            AddRoom(dungeon, "EmptyW", (Vector2){position.x - 1, position.y});
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/S
        
        } 
        else if(TextIsEqual(type, "2DNS")){
            
            //TOP
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            dungeon.rooms[CurrentRoomIndex].walls[dungeon.rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = topDoor;
            dungeon.rooms[CurrentRoomIndex].doors[dungeon.rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            
            dungeon.numRoomLocs++;

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
                AddRoom(dungeon, "BossN", dungeon.rooms[bossRoomIndex].gridPos);
            } else if(TextIsEqual(dungeon.rooms[bossRoomIndex].type, "EmptyE")){
                AddRoom(dungeon, "BossE", dungeon.rooms[bossRoomIndex].gridPos);
            } else if(TextIsEqual(dungeon.rooms[bossRoomIndex].type, "EmptyS")){
                AddRoom(dungeon, "BossS", dungeon.rooms[bossRoomIndex].gridPos);
            } else if(TextIsEqual(dungeon.rooms[bossRoomIndex].type, "EmptyW")){
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
            AddRoom(dungeon, "End", dungeon.rooms[endRoomIndex].gridPos);
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
        //Wall width on each side of the door
        float doorSide = (dungeon.rooms[i].size.x - dungeon.doorSize) / 2.0f; 
    
        Rectangle topWall = {
            dungeon.rooms[i].position.x + doorSide,
            dungeon.rooms[i].position.y,
            (float)dungeon.doorSize,
            dungeon.wallDepth
        };
    
        Rectangle rightWall = {
            dungeon.rooms[i].position.x + dungeon.rooms[i].size.x - dungeon.wallDepth,
            dungeon.rooms[i].position.y + doorSide,
            dungeon.wallDepth,
            (float)dungeon.doorSize
        };
        
        Rectangle bottomWall = {
            dungeon.rooms[i].position.x + doorSide,
            dungeon.rooms[i].position.y + dungeon.rooms[i].size.y - dungeon.wallDepth,
            (float)dungeon.doorSize,
            dungeon.wallDepth
        };
        
        Rectangle leftWall = {
            dungeon.rooms[i].position.x,
            dungeon.rooms[i].position.y + doorSide,
            dungeon.wallDepth,
            (float)dungeon.doorSize
        };
        
        
    
        deadEnds = TestDeadEnd(dungeon, dungeon.rooms[i]);
        for(int j = 0; j < (int)TextLength(deadEnds); j++){
            if(deadEnds[j] == 'N'){
                dungeon.rooms[i].numWalls++;
                dungeon.rooms[i].walls[dungeon.rooms[i].numWalls - 1] = topWall;
            }
            if(deadEnds[j] == 'E'){
                dungeon.rooms[i].numWalls++;
                dungeon.rooms[i].walls[dungeon.rooms[i].numWalls - 1] = rightWall;
            }
            if(deadEnds[j] == 'S'){
                dungeon.rooms[i].numWalls++;
                dungeon.rooms[i].walls[dungeon.rooms[i].numWalls - 1] = bottomWall;
            }
            if(deadEnds[j] == 'W'){
                dungeon.rooms[i].numWalls++;
                dungeon.rooms[i].walls[dungeon.rooms[i].numWalls - 1] = leftWall;
            }
            
        }
    }
}
