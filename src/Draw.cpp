#include "draw.h"

#include "player.h"
#include "room.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"

#include <raymath.h>

void DrawGame(Camera2D camera, Player player, Room rooms[], int numRoomLocs, Enemy enemies[], int numEnemies, Projectile projectiles[], int maxProjectiles, 
            Texture2D heartTexture,
            float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            int screenWidth, int screenHeight,
            bool debugHitbox,            
            bool exitWindowRequested){
                
                
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

            //Draw player
            DrawPlayer(player, debugHitbox);
            
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
            gunPos = {player.center.x - cos(playerToMouseRotation) * player.weapon.positionOffset,player.center.y - sin(playerToMouseRotation) * player.weapon.positionOffset};
            gunRec = {0,0,(float)player.weapon.texture.width / 6.0f,(float)player.weapon.texture.height - 30.0f};
            gunDest = {gunPos.x,gunPos.y,player.weapon.size,player.weapon.size};
            gunAngle = playerToMouseRotation * RAD2DEG;
            
            if (gunAngle < -90 || gunAngle > 90){
                gunRec.height = -gunRec.height;
            }
            

            DrawTexturePro(player.weapon.texture, gunRec, gunDest, (Vector2){0, gunDest.height / 2.0f}, gunAngle, WHITE);
            //              texture     source      dest               origin/pivot             rotation  color
      
        EndMode2D();
        
        mouseScreenPos = GetMousePosition();
        cursorRec = {0, 0, (float)cursorTexture.width, (float)cursorTexture.height};
        cursorDestination = {mouseScreenPos.x + (float)cursorTexture.width/2.0f - cursorSize/2.0f, mouseScreenPos.y + (float)cursorTexture.height/2.0f - cursorSize/2.0f, cursorSize, cursorSize};

        cursorOrigin = {cursorTexture.width / 2.0f, cursorTexture.height / 2.0f};

        DrawTexturePro(cursorTexture, cursorRec, cursorDestination, cursorOrigin, 0.0f, WHITE);
        
        Vector2 heartSize = {100,100};
        
        for(int i = 0; i < (player.maxHealth / 2.0f); i++){
            
            Rectangle heartTextureRec;
            Vector2 heartPos = {25 + (float)i * 100, 25};
            if(player.health >= 2 * (i + 1)){
                heartTextureRec = {(float)heartTexture.width / 3.0f * 0.0f, 0, (float)heartTexture.width / 3.0f, (float)heartTexture.height};
            } else if(player.health == 2 * (i + 1) - 1){
                heartTextureRec = {(float)heartTexture.width / 3.0f * 1.0f, 0, (float)heartTexture.width / 3.0f, (float)heartTexture.height};
            } else {
                heartTextureRec = {(float)heartTexture.width / 3.0f * 2.0f, 0, (float)heartTexture.width / 3.0f, (float)heartTexture.height};
            }
            
            Rectangle heartTextureDest = {heartPos.x,heartPos.y, heartSize.x, heartSize.y};
            DrawTexturePro(heartTexture, heartTextureRec, heartTextureDest, {0,0}, 0, WHITE);
        }
    } else {  
    
        //Exit menu
        DrawRectangle(0, screenHeight/2 - 100, screenWidth, 200, BLACK);
        int fontSize = 30;
        int textWidth = MeasureText("Are you sure you want to exit program? [Y/N]", fontSize);
        DrawText("Are you sure you want to exit program? [Y/N]", screenWidth/2 - textWidth/2, screenHeight/2 - fontSize/2, 30, WHITE);
    
    }
    
    EndDrawing();
}

void DrawPlayer(const Player& player, bool debugHitbox){
    
    int fontSize;
    int textWidth;
    
    Rectangle playerDest; 
    float frameWidth = (float)player.texture.width / 4.0f;
    Rectangle playerRec = {frameWidth * player.currentFrame,0,frameWidth,(float)player.texture.height};
    Vector2 playerTextureSize = {80.0f, 80.0f};
    
    //Draw hitbox
    if(debugHitbox){
        DrawRectangleRec(player.hitBox, BLUE);  
    }
    
    //Draw sprite
    playerDest = {player.center.x - playerTextureSize.x/2.0f, player.center.y - playerTextureSize.y/2.0f + 1.5f, playerTextureSize.x, playerTextureSize.y};
    if(player.facingDirection == -1){
        playerRec = {frameWidth * player.currentFrame,0,(float)-player.texture.width / 4.0f,(float)player.texture.height};
    } else if(player.facingDirection == 1){
        playerRec = {frameWidth * player.currentFrame,0,(float)player.texture.width / 4.0f,(float)player.texture.height};
    }
    DrawTexturePro(player.texture, playerRec, playerDest, (Vector2){0, 0}, 0, WHITE);
    
    //Draw stats
    fontSize = 15;
    textWidth = MeasureText("Player", fontSize);
    DrawText(
        "Player", player.center.x - textWidth/2, 
        player.hitBox.y - fontSize*2.2f, 
        fontSize, 
        BLACK
    );   
    textWidth = MeasureText(TextFormat("%0.2f",player.health), fontSize);
    DrawText(
        TextFormat("%0.2f",player.health), player.center.x - textWidth/2, 
        player.hitBox.y - fontSize*1.1f, 
        fontSize, 
        BLACK
    );  
    
    
    
}