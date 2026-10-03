#include "Camera.h"

#include "Room.h"
#include "Player.h"

#include <raymath.h>

void UpdateGameCamera(Room rooms[], int numRoomLocs, Player player, Camera2D& camera, bool debugCam){
    //Camera
        int playerRoom = -1;

        //Find the room that the player is in
        for (int i = 0; i < numRoomLocs; i++){
            if (rooms[i].exists){
                Rectangle roomRect = {
                    rooms[i].position.x,
                    rooms[i].position.y,
                    rooms[i].size.x,
                    rooms[i].size.y
                };

                if (CheckCollisionPointRec(player.center, roomRect)){
                    playerRoom = i;
                    break;
                }
            }
        }
        
        //If found
        if (playerRoom != -1){
            float roomLeft = rooms[playerRoom].position.x;
            float roomRight = roomLeft + rooms[playerRoom].size.x;

            float roomTop = rooms[playerRoom].position.y;
            float roomBottom = roomTop + rooms[playerRoom].size.y;

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
            (TextIsEqual(rooms[playerRoom].type, "BossN") ||
             TextIsEqual(rooms[playerRoom].type, "BossE") ||
             TextIsEqual(rooms[playerRoom].type, "BossS") ||
             TextIsEqual(rooms[playerRoom].type, "BossW"));
        }

        //Camera mode
        if(debugCam){
            camera.target = player.center;
            camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));
        } else {
            if(!isBoss){
                camera.zoom = 1.5f; 
            } else {
                camera.zoom = 0.5f;
            }                
        }
    
}