#include "Room.h"

#include <raymath.h>

bool RoomForBoss(Room rooms[], Room emptyRoom, int numRoomLocs){
    
    bool available = true;
    
    if(TextIsEqual(emptyRoom.type, "EmptyN")){
        for(int i = 0; i < numRoomLocs; i++){
            available = available && (rooms[i].gridPos != (Vector2){-1,0} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,0} + emptyRoom.gridPos || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){-1,-1} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){0,-1} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,-1} + emptyRoom.gridPos || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){-1,-2} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){0,-2} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,-2} + emptyRoom.gridPos || !rooms[i].exists);
        }
    } else if(TextIsEqual(emptyRoom.type, "EmptyE")){
        for(int i = 0; i < numRoomLocs; i++){
            available = available && (rooms[i].gridPos != (Vector2){0,1} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){0,-1} + emptyRoom.gridPos || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){1,1} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,0} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,-1} + emptyRoom.gridPos || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){2,1} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){2,0} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){2,-1} + emptyRoom.gridPos || !rooms[i].exists);
        }
    } else if(TextIsEqual(emptyRoom.type, "EmptyS")){
        for(int i = 0; i < numRoomLocs; i++){
            available = available && (rooms[i].gridPos != (Vector2){-1,0} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,0} + emptyRoom.gridPos || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){-1,1} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){0,1} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,1} + emptyRoom.gridPos || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){-1,2} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){0,2} + emptyRoom.gridPos || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){1,2} + emptyRoom.gridPos || !rooms[i].exists);
        }
    } else if(TextIsEqual(emptyRoom.type, "EmptyW")){
        for(int i = 0; i < numRoomLocs; i++){
            available = available && (rooms[i].gridPos != (Vector2){0,1} + emptyRoom.gridPos  || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){0,-1} + emptyRoom.gridPos  || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){-1,1} + emptyRoom.gridPos  || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){-1,0} + emptyRoom.gridPos  || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){-1,-1} + emptyRoom.gridPos || !rooms[i].exists);
            
            available = available && (rooms[i].gridPos != (Vector2){-2,1} + emptyRoom.gridPos  || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){-2,0} + emptyRoom.gridPos  || !rooms[i].exists);
            available = available && (rooms[i].gridPos != (Vector2){-2,-1} + emptyRoom.gridPos  || !rooms[i].exists);
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
    
    if(TextIsEqual(currentRoom.type,"4DNESW") || TextIsEqual(currentRoom.type,"Spawn") || isBoss){
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

char* TestDeadEnd(Room rooms[], Room currentRoom, int numRoomLocs){
    
    static char directions[5] = "";
    
    // Reset directions
    directions[0] = '\0';
    
    for(int i = 0; i < numRoomLocs; i++){
        // 2DNS, 2DEW, 2DNE, 2DES, 2DSW, 2DNW,
        // 3DNEW, 3DNES, 3DESW, 3DNSW, 4DNESW
        
        if(TextIsEqual(currentRoom.type, "2DNS")){
            if((rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DEW")){
            if((rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DNE")){
            if((rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DES")){
            if((rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DSW")){
            if((rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "2DNW")){
            if((rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DNEW")){
            if((rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DNES")){
            if((rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DESW")){
            if((rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "3DNSW")){
            if((rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
        
        else if(TextIsEqual(currentRoom.type, "4DNESW")){
            if((rooms[i].gridPos == (Vector2){0,-1} + currentRoom.gridPos) &&
               !ValidRoomInDirection(rooms[i], 'N')){
                AddDirection(directions, 'N');
            }
            if((rooms[i].gridPos == (Vector2){1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'E')){
                AddDirection(directions, 'E');
            }
            if((rooms[i].gridPos == (Vector2){0,1} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'S')){
                AddDirection(directions, 'S');
            }
            if((rooms[i].gridPos == (Vector2){-1,0} + currentRoom.gridPos) &&
                    !ValidRoomInDirection(rooms[i], 'W')){
                AddDirection(directions, 'W');
            }
        }
    }
    
    return directions;
}

void SortRooms(Room rooms[], int numRoomLocs){
    int unsorted = numRoomLocs;
    
    for(int i = 0; i < numRoomLocs - 1; i++){
        for(int j = 0; j < unsorted - 1; j++){
            if(rooms[j].distance > rooms[j+1].distance){
                Room tempRoom = rooms[j+1];
                rooms[j+1] = rooms[j];
                rooms[j] = tempRoom;
                
                
            }
            
        }
        unsorted--;
    }
    
}

void AddRoom(Room rooms[], char* type, int& currentRooms, int maxRooms, int& numRoomLocs, Vector2 position, int roomSize, float wallDepth, int doorSize){   
    
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
    

    //Set room position offset
    roomPositionOffset = (Vector2){-roomSize/2.0f, -roomSize/2.0f};
    
    CurrentRoomIndex = numRoomLocs;
    
    //Set current room up
    rooms[CurrentRoomIndex].gridPos = position;
    rooms[CurrentRoomIndex].type = type; 
    rooms[CurrentRoomIndex].size = (Vector2){(float)roomSize, (float)roomSize};
    if(!isBoss){  
        rooms[CurrentRoomIndex].position = (position * roomSize) + roomPositionOffset;
    } else {
        if(TextIsEqual(rooms[CurrentRoomIndex].type, "BossN")){
                rooms[CurrentRoomIndex].position = (position * roomSize/3.0f) + roomPositionOffset + (Vector2){0,-roomSize/3.0f};
            } else if(TextIsEqual(rooms[CurrentRoomIndex].type, "BossE")){
                rooms[CurrentRoomIndex].position = (position * roomSize/3.0f)+ roomPositionOffset + (Vector2){roomSize/3.0f,0};
            } else if(TextIsEqual(rooms[CurrentRoomIndex].type, "BossS")){
                rooms[CurrentRoomIndex].position = (position * roomSize/3.0f)+ roomPositionOffset + (Vector2){0,roomSize/3.0f};
            } else if(TextIsEqual(rooms[CurrentRoomIndex].type, "BossW")){
                rooms[CurrentRoomIndex].position = (position * roomSize/3.0f)+ roomPositionOffset + (Vector2){-roomSize/3.0f,0};
            }
    }
    rooms[CurrentRoomIndex].distance = abs(rooms[CurrentRoomIndex].gridPos.x) + abs(rooms[CurrentRoomIndex].gridPos.y);  

    //Wall width on each side of the door
    doorSide = (rooms[CurrentRoomIndex].size.x - doorSize) / 2.0f;    
    
    //--------------------------------------------------------------------------
    //WALL RECTANGLES
    
    //Top
    Rectangle topLeft = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y,
        doorSide,
        wallDepth
    };
    
    Rectangle topRight = {
        rooms[CurrentRoomIndex].position.x + doorSide + doorSize,
        rooms[CurrentRoomIndex].position.y,
        doorSide,
        wallDepth
    };
    
    Rectangle topWall = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y,
        rooms[CurrentRoomIndex].size.x,
        wallDepth
    };
    
    //Bottom
    Rectangle bottomLeft = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y + rooms[CurrentRoomIndex].size.y - wallDepth,
        doorSide,
        wallDepth
    };
    
    Rectangle bottomRight = {
        rooms[CurrentRoomIndex].position.x + doorSide + doorSize,
        rooms[CurrentRoomIndex].position.y + rooms[CurrentRoomIndex].size.y - wallDepth,
        doorSide,
        wallDepth
    };
    
    Rectangle bottomWall = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y + rooms[CurrentRoomIndex].size.y - wallDepth,
        rooms[CurrentRoomIndex].size.x,
        wallDepth
    };
    
    //Left
    Rectangle leftTop = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y,
        wallDepth,
        doorSide
    };
    
    Rectangle leftBottom = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y + doorSide + doorSize,
        wallDepth,
        doorSide
    };
    
    Rectangle leftWall = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y,
        wallDepth,
        rooms[CurrentRoomIndex].size.y
    };
    
    //Right
    Rectangle rightTop = {
        rooms[CurrentRoomIndex].position.x + rooms[CurrentRoomIndex].size.x - wallDepth,
        rooms[CurrentRoomIndex].position.y,
        wallDepth,
        doorSide
    };
    
    Rectangle rightBottom = {
        rooms[CurrentRoomIndex].position.x + rooms[CurrentRoomIndex].size.x - wallDepth,
        rooms[CurrentRoomIndex].position.y + doorSide + doorSize,
        wallDepth,
        doorSide
    };
    
    Rectangle rightWall = {
        rooms[CurrentRoomIndex].position.x + rooms[CurrentRoomIndex].size.x - wallDepth,
        rooms[CurrentRoomIndex].position.y,
        wallDepth,
        rooms[CurrentRoomIndex].size.y
    };
    
    //--------------------------------------------------------------------------
    // DOOR RECTANGLES
    
    Rectangle topDoor = {
        rooms[CurrentRoomIndex].position.x + doorSide,
        rooms[CurrentRoomIndex].position.y,
        (float)doorSize,
        wallDepth
    };
    
    Rectangle rightDoor = {
        rooms[CurrentRoomIndex].position.x + rooms[CurrentRoomIndex].size.x - wallDepth,
        rooms[CurrentRoomIndex].position.y + doorSide,
        wallDepth,
        (float)doorSize
    };
    
    Rectangle bottomDoor = {
        rooms[CurrentRoomIndex].position.x + doorSide,
        rooms[CurrentRoomIndex].position.y + rooms[CurrentRoomIndex].size.y - wallDepth,
        (float)doorSize,
        wallDepth
    };
    
    Rectangle leftDoor = {
        rooms[CurrentRoomIndex].position.x,
        rooms[CurrentRoomIndex].position.y + doorSide,
        wallDepth,
        (float)doorSize
    };
    
    //--------------------------------------------------------------------------
    // ROOM TYPE
    
    //Check if empty type
    if(isEmpty){
        
        rooms[CurrentRoomIndex].exists = false;
        
        numRoomLocs++;
        
    } else if(isBoss){ 
        //Add boss room
        rooms[CurrentRoomIndex].exists = true;
        rooms[CurrentRoomIndex].numWalls = 0;
        rooms[CurrentRoomIndex].numDoors = 0;
        
        if(TextIsEqual(type, "BossN")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    rooms[CurrentRoomIndex].gridPos + (Vector2){0,-3}, roomSize/3.0f, wallDepth, doorSize);
                    
        } else if(TextIsEqual(type, "BossE")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    rooms[CurrentRoomIndex].gridPos + (Vector2){3,0}, roomSize/3.0f, wallDepth, doorSize);
        } else if(TextIsEqual(type, "BossS")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    rooms[CurrentRoomIndex].gridPos + (Vector2){0,3}, roomSize/3.0f, wallDepth, doorSize);
                    
        } else if(TextIsEqual(type, "BossW")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    rooms[CurrentRoomIndex].gridPos + (Vector2){-3,0}, roomSize/3.0f, wallDepth, doorSize);
        }
        
    }else if(currentRooms < maxRooms){
        //Add the room 
        rooms[CurrentRoomIndex].exists = true;
        rooms[CurrentRoomIndex].numWalls = 0;
        rooms[CurrentRoomIndex].numDoors = 0;
        
        //--------------------------------------------------------------------------
        // 4 DOORS
        
        if(TextIsEqual(type, "4DNESW") || TextIsEqual(type, "Spawn")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y - 1}, roomSize, wallDepth, doorSize);
            
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y + 1}, roomSize, wallDepth, doorSize);
            
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x - 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/S/W
        
        } 
        else if(TextIsEqual(type, "3DNSW")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y - 1}, roomSize, wallDepth, doorSize);
            
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y + 1}, roomSize, wallDepth, doorSize);
            
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x - 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 3 DOORS - E/S/W
        
        } 
        else if(TextIsEqual(type, "3DESW")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y + 1}, roomSize, wallDepth, doorSize);
            
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x - 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/E/S
        
        } 
        else if(TextIsEqual(type, "3DNES")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y - 1}, roomSize, wallDepth, doorSize);
            
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y + 1}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 3 DOORS - N/E/W
        
        } 
        else if(TextIsEqual(type, "3DNEW")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y - 1}, roomSize, wallDepth, doorSize);
            
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x - 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/W
        
        } 
        else if(TextIsEqual(type, "2DNW")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y - 1}, roomSize, wallDepth, doorSize);
            
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x - 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 2 DOORS - S/W
        
        } 
        else if(TextIsEqual(type, "2DSW")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y + 1}, roomSize, wallDepth, doorSize);
            
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x - 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 2 DOORS - E/S
        
        } 
        else if(TextIsEqual(type, "2DES")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y + 1}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/E
        
        } 
        else if(TextIsEqual(type, "2DNE")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y - 1}, roomSize, wallDepth, doorSize);
            
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 2 DOORS - E/W
        
        } 
        else if(TextIsEqual(type, "2DEW")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topWall;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomWall;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftBottom;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightTop;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightBottom;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = rightDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = leftDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Right
            AddRoom(rooms, (char*)"EmptyE", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
            //Left
            AddRoom(rooms, (char*)"EmptyW", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x - 1, position.y + 0}, roomSize, wallDepth, doorSize);
            
        //--------------------------------------------------------------------------
        // 2 DOORS - N/S
        
        } 
        else if(TextIsEqual(type, "2DNS")){
            
            //TOP
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = topRight;
            
            //BOTTOM
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomLeft;
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = bottomRight;
            
            //LEFT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = leftWall;
            
            //RIGHT
            rooms[CurrentRoomIndex].walls[rooms[CurrentRoomIndex].numWalls++] = rightWall;
            
            //Doors
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = topDoor;
            rooms[CurrentRoomIndex].doors[rooms[CurrentRoomIndex].numDoors++] = bottomDoor;
            
            numRoomLocs++;

            //Add possible room locations
            //Top
            AddRoom(rooms, (char*)"EmptyN", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y - 1}, roomSize, wallDepth, doorSize);
            
            //Bottom
            AddRoom(rooms, (char*)"EmptyS", currentRooms, maxRooms, numRoomLocs,
                    (Vector2){position.x + 0, position.y + 1}, roomSize, wallDepth, doorSize);
        }
        
        currentRooms++;
    }
    
    //Purge dupe locations
    for(int i = 0; i < numRoomLocs; i++){
        for(int j = i + 1; j < numRoomLocs; j++){
            
            if(rooms[i].gridPos == rooms[j].gridPos &&
               (rooms[i].exists == false || rooms[j].exists == false)){
                
                if(rooms[i].exists == false){
                    rooms[i] = rooms[numRoomLocs - 1];
                    numRoomLocs--;
                    
                    //Recheck swapped room
                    i--;
                    break;
                    
                } else {
                    rooms[j] = rooms[numRoomLocs - 1];
                    numRoomLocs--;
                    
                    //Recheck swapped room
                    j--;
                }
            }
        }
    }
    
    SortRooms(rooms, numRoomLocs);   
}

void GenerateDungeon(Room rooms[], int& currentRooms, int& maxRooms, int& numRoomLocs, int roomSize, float wallDepth, int doorSize, float straight, float turn, float threeWay, float fourWay, int& bossRoomIndex){
    
    char* deadEnds;
    int chosenEmptyRoom;
    int chosenRoomType;
    
    currentRooms = 0;
    numRoomLocs = 0;
    
    Vector2 straightChance = (Vector2){1,straight};
    Vector2 turnChance = (Vector2){straightChance.y + 1,straightChance.y + turn};
    Vector2 threeWayChance = (Vector2){turnChance.y + 1,turnChance.y + threeWay};
    Vector2 fourWayChance = (Vector2){threeWayChance.y + 1,threeWayChance.y + fourWay};
    
    bool generateDungeon = true;
    
    
    AddRoom(rooms, (char*)"Spawn", currentRooms, maxRooms, numRoomLocs, (Vector2){0,0}, roomSize, wallDepth, doorSize);
    
    while(generateDungeon){
        
        while(currentRooms < maxRooms){
            
            chosenEmptyRoom = -1;
            
            //Choose closest empty room
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == false){
                    chosenEmptyRoom = i;
                    break;
                }
                
            }
            
            //If not stuck
            if(chosenEmptyRoom != -1){
                chosenRoomType = GetRandomValue(1, 100);
                
                //Spawn corresponding room
                if(TextIsEqual(rooms[chosenEmptyRoom].type,"EmptyN")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //Straight
                        AddRoom(rooms, (char*)"2DNS", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - turn / 2){
                            //Right
                            AddRoom(rooms, (char*)"2DES", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Left
                            AddRoom(rooms, (char*)"2DSW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * threeWay / 3){
                            //Split Left Right
                            AddRoom(rooms, (char*)"3DESW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - threeWay / 3){
                            //Split Front Right
                            AddRoom(rooms, (char*)"3DNES", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Split Front Left
                            AddRoom(rooms, (char*)"3DNSW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    }
                } else if(TextIsEqual(rooms[chosenEmptyRoom].type,"EmptyE")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //Straight
                        AddRoom(rooms, (char*)"2DEW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - turn / 2){
                            //Right
                            AddRoom(rooms, (char*)"2DSW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Left
                            AddRoom(rooms, (char*)"2DNW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * threeWay / 3){
                            //Split Left Right
                            AddRoom(rooms, (char*)"3DNSW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - threeWay / 3){
                            //Split Front Right
                            AddRoom(rooms, (char*)"3DESW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Split Front Left
                            AddRoom(rooms, (char*)"3DNEW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    }
                } else if(TextIsEqual(rooms[chosenEmptyRoom].type,"EmptyS")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //Straight
                        AddRoom(rooms, (char*)"2DNS", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - turn / 2){
                            //Right
                            AddRoom(rooms, (char*)"2DNW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Left
                            AddRoom(rooms, (char*)"2DNE", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * threeWay / 3){
                            //Split Left Right
                            AddRoom(rooms, (char*)"3DNEW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - threeWay / 3){
                            //Split Front Right
                            AddRoom(rooms, (char*)"3DNSW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Split Front Left
                            AddRoom(rooms, (char*)"3DNES", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    }
                } else if(TextIsEqual(rooms[chosenEmptyRoom].type,"EmptyW")){    
                    if(chosenRoomType >= straightChance.x && chosenRoomType <= straightChance.y){
                        //Straight
                        AddRoom(rooms, (char*)"2DEW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);                    
                    } else if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y){
                        if(chosenRoomType >= turnChance.x && chosenRoomType <= turnChance.y - turn / 2){
                            //Right
                            AddRoom(rooms, (char*)"2DNE", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Left
                            AddRoom(rooms, (char*)"2DES", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y){
                        if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - 2 * threeWay / 3){
                            //Split Left Right
                            AddRoom(rooms, (char*)"3DNES", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else if(chosenRoomType >= threeWayChance.x && chosenRoomType <= threeWayChance.y - threeWay / 3){
                            //Split Front Right
                            AddRoom(rooms, (char*)"3DNEW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        } else {
                            //Split Front Left
                            AddRoom(rooms, (char*)"3DESW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);
                        }
                    }
                }
                
                
                //Wall empty rooms can have the same 4 way
                if(chosenRoomType >= fourWayChance.x && chosenRoomType <= fourWayChance.y){
                    //4 Way intersection
                    AddRoom(rooms, (char*)"4DNESW", currentRooms, maxRooms, numRoomLocs, rooms[chosenEmptyRoom].gridPos, roomSize, wallDepth, doorSize);                    
                }
                
            } else {
                
                //Reset generation because we are stuck
                currentRooms = 0;
                numRoomLocs = 0;
                AddRoom(rooms, (char*)"Spawn", currentRooms, maxRooms, numRoomLocs, (Vector2){0,0}, roomSize, wallDepth, doorSize);
                break;
                
            }            
        }   
        
        bossRoomIndex = -1;
    
        //Choose boss room
        for(int i = numRoomLocs - 1; i > -1; i--){
            if(rooms[i].exists == false && RoomForBoss(rooms, rooms[i], numRoomLocs)){
                bossRoomIndex = i;
                break;
            }
            
        }
        
        //If valid
        if(bossRoomIndex != -1){
            generateDungeon = false;  
            if(TextIsEqual(rooms[bossRoomIndex].type, "EmptyN")){
                AddRoom(rooms, (char*)"BossN", currentRooms, maxRooms, numRoomLocs, rooms[bossRoomIndex].gridPos, roomSize * 3, wallDepth * 10, doorSize);
            } else if(TextIsEqual(rooms[bossRoomIndex].type, "EmptyE")){
                AddRoom(rooms, (char*)"BossE", currentRooms, maxRooms, numRoomLocs, rooms[bossRoomIndex].gridPos, roomSize * 3, wallDepth * 10, doorSize);
            } else if(TextIsEqual(rooms[bossRoomIndex].type, "EmptyS")){
                AddRoom(rooms, (char*)"BossS", currentRooms, maxRooms, numRoomLocs, rooms[bossRoomIndex].gridPos, roomSize * 3, wallDepth * 10, doorSize);
            } else if(TextIsEqual(rooms[bossRoomIndex].type, "EmptyW")){
                AddRoom(rooms, (char*)"BossW", currentRooms, maxRooms, numRoomLocs, rooms[bossRoomIndex].gridPos, roomSize * 3, wallDepth * 10, doorSize);
            }
        } else {
            //Reset generation because we cant spawn boss room
                currentRooms = 0;
                numRoomLocs = 0;
                AddRoom(rooms, (char*)"Spawn", currentRooms, maxRooms, numRoomLocs, (Vector2){0,0}, roomSize, wallDepth, doorSize);
                break;
        }
    }
    
    //Make dead ends    
    for(int i = 0; i < numRoomLocs; i++){
        //Wall width on each side of the door
        float doorSide = (rooms[i].size.x - doorSize) / 2.0f; 
    
        Rectangle topWall = {
            rooms[i].position.x + doorSide,
            rooms[i].position.y,
            (float)doorSize,
            wallDepth
        };
    
        Rectangle rightWall = {
            rooms[i].position.x + rooms[i].size.x - wallDepth,
            rooms[i].position.y + doorSide,
            wallDepth,
            (float)doorSize
        };
        
        Rectangle bottomWall = {
            rooms[i].position.x + doorSide,
            rooms[i].position.y + rooms[i].size.y - wallDepth,
            (float)doorSize,
            wallDepth
        };
        
        Rectangle leftWall = {
            rooms[i].position.x,
            rooms[i].position.y + doorSide,
            wallDepth,
            (float)doorSize
        };
        
        
    
        deadEnds = TestDeadEnd(rooms, rooms[i], numRoomLocs);
        for(int j = 0; j < (int)TextLength(deadEnds); j++){
            if(deadEnds[j] == 'N'){
                rooms[i].numWalls++;
                rooms[i].walls[rooms[i].numWalls - 1] = topWall;
            }
            if(deadEnds[j] == 'E'){
                rooms[i].numWalls++;
                rooms[i].walls[rooms[i].numWalls - 1] = rightWall;
            }
            if(deadEnds[j] == 'S'){
                rooms[i].numWalls++;
                rooms[i].walls[rooms[i].numWalls - 1] = bottomWall;
            }
            if(deadEnds[j] == 'W'){
                rooms[i].numWalls++;
                rooms[i].walls[rooms[i].numWalls - 1] = leftWall;
            }
            
        }
    }
}
