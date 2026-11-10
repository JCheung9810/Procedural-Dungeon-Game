#include "camera.h"

#include "dungeon.h"
#include "player.h"

#include <raymath.h>

void InitializeCamera(Camera2D& camera, Player player, int screenHeight, int screenWidth){
    camera.target = player.center;
    camera.offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f};
    camera.zoom = 1.0f;
    
}

void UpdateGameCamera(Dungeon& dungeon, Player player, Camera2D& camera, bool debugCam){
    //Camera
        int playerRoom = -1;

        //Find the room that the player is in
        for (int i = 0; i < (int)dungeon.rooms.size(); i++){
            if (dungeon.rooms[i].exists){
                Rectangle roomRect = {
                    dungeon.rooms[i].position.x,
                    dungeon.rooms[i].position.y,
                    dungeon.rooms[i].size.x,
                    dungeon.rooms[i].size.y
                };

                if (CheckCollisionPointRec(player.center, roomRect)){
                    playerRoom = i;
                    break;
                }
            }
        }
        
        //If found
        if (playerRoom != -1){
            float roomLeft = dungeon.rooms[playerRoom].position.x;
            float roomRight = roomLeft + dungeon.rooms[playerRoom].size.x;

            float roomTop = dungeon.rooms[playerRoom].position.y;
            float roomBottom = roomTop + dungeon.rooms[playerRoom].size.y;

            float halfCameraWidth = GetScreenWidth() / (2.0f * camera.zoom);

            float halfCameraHeight = GetScreenHeight() / (2.0f * camera.zoom);

            //Clamp
            camera.target.x = Clamp(
                player.center.x,
                roomLeft + halfCameraWidth,
                roomRight - halfCameraWidth
            );

            camera.target.y = Clamp(
                player.center.y,
                roomTop + halfCameraHeight,
                roomBottom - halfCameraHeight
            );
        }    

        bool isBoss = false;
        if(playerRoom != -1){
            isBoss =
            (TextIsEqual(dungeon.rooms[playerRoom].type, "BossN") ||
             TextIsEqual(dungeon.rooms[playerRoom].type, "BossE") ||
             TextIsEqual(dungeon.rooms[playerRoom].type, "BossS") ||
             TextIsEqual(dungeon.rooms[playerRoom].type, "BossW"));
        }

        //Camera mode
        if(debugCam){
            camera.target = player.center;
            camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));
        } else {
            if(!isBoss){
                camera.zoom = 1.5f; 
            } else {
                camera.zoom = 1.5f;
            }                
        }
    
}
