#include "Draw.h"

#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Camera.h"

#include <raymath.h>

void DrawGame(Camera2D camera, Player player, Room rooms[], int numRoomLocs, Enemy enemies[], int numEnemies, Projectile projectiles[], int maxProjectiles, 
            Texture2D gunTexture, float gunSize, float gunOffset, float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, int screenWidth, int screenHeight, bool exitWindowRequested){
                
                
    Vector2 gunPos;
    Rectangle gunRec;
    Rectangle gunDest;
    float gunAngle;
    Vector2 mouseScreenPos = GetMousePosition();
    Rectangle cursorRec;
    Rectangle cursorDestination;
    Vector2 cursorOrigin;
                
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!exitWindowRequested){
        
        BeginMode2D(camera);
        
            int fontSize;
            int textWidth;
        
            //Draw rooms
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == true){
                    //Draw doors
                    for(int j = 0; j < rooms[i].numDoors; j++){
                        DrawRectangleRec(rooms[i].doors[j], MAROON);                                 
                    }
                    //Draw walls
                    for(int j = 0; j < rooms[i].numWalls; j++){
                        DrawRectangleRec(rooms[i].walls[j], BLACK);                                 
                    }
                }
                fontSize = 30;
                //Text Type
                textWidth = MeasureText(rooms[i].type, fontSize);
                DrawText(
                    rooms[i].type, rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                    rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2, 
                    fontSize, 
                    BLACK
                );                       
                //Text Dist
                textWidth = MeasureText(TextFormat("Dist: %i", rooms[i].distance), fontSize);
                DrawText(
                    TextFormat("Dist: %i", rooms[i].distance), rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                    rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2 + fontSize, 
                    fontSize, 
                    BLACK
                ); 
                //Text Index
                textWidth = MeasureText(TextFormat("Index: %i", i), fontSize);
                DrawText(
                    TextFormat("Index: %i", i), rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                    rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2 + fontSize * 2, 
                    fontSize, 
                    BLACK
                );   
                //Text Grid Position
                textWidth = MeasureText(TextFormat("Grid Position: {%f,%f}", rooms[i].gridPos.x, rooms[i].gridPos.y), fontSize);
                DrawText(
                    TextFormat("Grid Position: {%f,%f}", rooms[i].gridPos.x, rooms[i].gridPos.y), rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                    rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2 + fontSize * 3, 
                    fontSize, 
                    BLACK
                ); 
            }

            DrawPlayer(player);
            
            //Draw dummy
            for (int i = 0; i < numEnemies; i++){
                if (enemies[i].active){
                    fontSize = 15;
                    textWidth = MeasureText(enemies[i].name, fontSize);
                    DrawText(
                        enemies[i].name, enemies[i].position.x + enemies[i].size.x/2.0f - textWidth/2, 
                        enemies[i].position.y - fontSize*2.2f, 
                        fontSize, 
                        BLACK
                    );   
                    textWidth = MeasureText(TextFormat("%0.2f",enemies[i].health), fontSize);
                    DrawText(
                        TextFormat("%0.2f",enemies[i].health), enemies[i].position.x + enemies[i].size.x/2.0f - textWidth/2, 
                        enemies[i].position.y - fontSize*1.1f, 
                        fontSize, 
                        BLACK
                    );   
                    DrawRectangleRec(enemies[i].hitBox, RED);
                }
            }
            
            //Draw projectiles
            for (int i = 0; i < maxProjectiles; i++){
                if (projectiles[i].active) 
                    //DrawRectanglePro(projectiles[i].hitBox, (Vector2){projectiles[i].size.x / 2.0f, projectiles[i].size.y / 2.0f}, projectiles[i].rotation * RAD2DEG, projectiles[i].color);
                    DrawRectangleRec(projectiles[i].hitBox, YELLOW);
            } 
            
            //Draw gun
            gunPos = {player.center.x - cos(playerToMouseRotation) * gunOffset,player.center.y - sin(playerToMouseRotation) * gunOffset};
            gunRec = {0,0,(float)gunTexture.width / 6.0f,(float)gunTexture.height - 30.0f};
            gunDest = {gunPos.x,gunPos.y,gunSize,gunSize};
            gunAngle = playerToMouseRotation * RAD2DEG;
            
            if (gunAngle < -90 || gunAngle > 90){
                gunRec.height = -gunRec.height;
            }
            

            DrawTexturePro(gunTexture, gunRec, gunDest, (Vector2){0, gunDest.height / 2.0f}, gunAngle, WHITE);
            //              texture     source      dest               origin/pivot             rotation  color
      
        EndMode2D();
        
        mouseScreenPos = GetMousePosition();
        cursorRec = {0, 0, (float)cursorTexture.width, (float)cursorTexture.height};
        cursorDestination = {mouseScreenPos.x + (float)cursorTexture.width/2.0f - cursorSize/2.0f, mouseScreenPos.y + (float)cursorTexture.height/2.0f - cursorSize/2.0f, cursorSize, cursorSize};

        cursorOrigin = {cursorTexture.width / 2.0f, cursorTexture.height / 2.0f};

        DrawTexturePro(cursorTexture, cursorRec, cursorDestination, cursorOrigin, 0.0f, WHITE);
    } else {  
    
        //Exit menu
        DrawRectangle(0, screenHeight/2 - 100, screenWidth, 200, BLACK);
        int fontSize = 30;
        int textWidth = MeasureText("Are you sure you want to exit program? [Y/N]", fontSize);
        DrawText("Are you sure you want to exit program? [Y/N]", screenWidth/2 - textWidth/2, screenHeight/2 - fontSize/2, 30, WHITE);
    
    }
    
    EndDrawing();
}