#include "draw.h"

#include "player.h"
#include "dungeon.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"

#include <raymath.h>

void DrawGame(Camera2D camera, Player player, Dungeon& dungeon, vector<Enemy>& enemies, Projectile projectiles[], int maxProjectiles, 
            Texture2D heartTexture, Texture2D weaponHudBoxTexture,
            float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            int screenWidth, int screenHeight,
            bool debugHitbox,            
            bool exitWindowRequested){
                
                
    Vector2 mouseScreenPos = GetMousePosition();
    Rectangle cursorRec;
    Rectangle cursorDestination;
    Vector2 cursorOrigin;
                
    BeginDrawing();

    ClearBackground(GRAY);

    if (!exitWindowRequested && player.health > 0.0f){
        
        BeginMode2D(camera);
        
            int fontSize;
            int textWidth;

            //Draw rooms
            for(int roomIt = 0; roomIt < (int)dungeon.rooms.size(); roomIt++){
                Room& room = dungeon.rooms[roomIt];

                if(room.exists == true){

                    float scale = room.roomContent.backgroundSize.x;
                    float tileSize = 16 * scale;

                    DrawTextureEx(
                        room.roomContent.backgroundTexture,
                        room.position,
                        0.0f,
                        scale,
                        WHITE
                    );

                    for(int wallIt = 0; wallIt < (int)room.roomContent.walls.size(); wallIt++){
                        Wall& wall = room.roomContent.walls[wallIt];

                        int tilesX = (int)roundf(wall.hitbox.width / tileSize);
                        int tilesY = (int)roundf(wall.hitbox.height / tileSize);

                        for(int x = 0; x < tilesX; x++){
                            for(int y = 0; y < tilesY; y++){
                                Rectangle dest = {
                                    wall.hitbox.x + x * tileSize,
                                    wall.hitbox.y + y * tileSize,
                                    tileSize,
                                    tileSize
                                };

                                DrawTexturePro(
                                    assets.textures.at("assets/textures/dungeonTileset.png"),
                                    assets.spriteInfo[wall.spriteID].textureSrc,
                                    dest,
                                    Vector2{0, 0},
                                    0.0f,
                                    WHITE
                                );
                            }
                        }
                    }
                    
                    for(int decorIt = 0; decorIt < (int)room.roomContent.decor.size(); decorIt++){
                        RoomDecor& decor = room.roomContent.decor[decorIt];

                        int tilesX = (int)roundf(decor.hitbox.width / tileSize);
                        int tilesY = (int)roundf(decor.hitbox.height / tileSize);

                        for(int x = 0; x < tilesX; x++){
                            for(int y = 0; y < tilesY; y++){
                                Rectangle dest = {
                                    decor.hitbox.x + x * tileSize,
                                    decor.hitbox.y + y * tileSize,
                                    tileSize,
                                    tileSize
                                };

                                DrawTexturePro(
                                    assets.textures.at("assets/textures/dungeonTileset.png"),
                                    assets.spriteInfo[decor.spriteID].textureSrc,
                                    dest,
                                    Vector2{0, 0},
                                    0.0f,
                                    WHITE
                                );
                            }
                        }
                    }
                }
                fontSize = 30;
                //Text Type
                textWidth = MeasureText(dungeon.rooms[roomIt].type, fontSize);
                DrawText(
                    dungeon.rooms[roomIt].type, dungeon.rooms[roomIt].position.x + Vector2{dungeon.rooms[roomIt].size.x/2.0f,0}.x - textWidth/2, 
                    dungeon.rooms[roomIt].position.y + Vector2{0,dungeon.rooms[roomIt].size.y/2.0f}.y - fontSize/2, 
                    fontSize, 
                    BLACK
                );                       
                //Text Dist
                textWidth = MeasureText(TextFormat("Dist: %i", dungeon.rooms[roomIt].distance), fontSize);
                DrawText(
                    TextFormat("Dist: %i", dungeon.rooms[roomIt].distance), dungeon.rooms[roomIt].position.x + Vector2{dungeon.rooms[roomIt].size.x/2.0f,0}.x - textWidth/2, 
                    dungeon.rooms[roomIt].position.y + Vector2{0,dungeon.rooms[roomIt].size.y/2.0f}.y - fontSize/2 + fontSize, 
                    fontSize, 
                    BLACK
                ); 
                //Text Index
                textWidth = MeasureText(TextFormat("Index: %i", roomIt), fontSize);
                DrawText(
                    TextFormat("Index: %i", roomIt), dungeon.rooms[roomIt].position.x + Vector2{dungeon.rooms[roomIt].size.x/2.0f,0}.x - textWidth/2, 
                    dungeon.rooms[roomIt].position.y + Vector2{0,dungeon.rooms[roomIt].size.y/2.0f}.y - fontSize/2 + fontSize * 2, 
                    fontSize, 
                    BLACK
                );   
                //Text Grid Position
                textWidth = MeasureText(TextFormat("Grid Position: {%f,%f}", dungeon.rooms[roomIt].gridPos.x, dungeon.rooms[roomIt].gridPos.y), fontSize);
                DrawText(
                    TextFormat("Grid Position: {%f,%f}", dungeon.rooms[roomIt].gridPos.x, dungeon.rooms[roomIt].gridPos.y), dungeon.rooms[roomIt].position.x + Vector2{dungeon.rooms[roomIt].size.x/2.0f,0}.x - textWidth/2, 
                    dungeon.rooms[roomIt].position.y + Vector2{0,dungeon.rooms[roomIt].size.y/2.0f}.y - fontSize/2 + fontSize * 3, 
                    fontSize, 
                    BLACK
                );
            }

            //Draw player
            DrawPlayer(player, debugHitbox);
            
            //Draw weapon
            DrawWeapon(player, playerToMouseRotation);
            
            //Draw dummy
            for (int i = 0; i < (int)enemies.size(); i++){
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
                    //DrawRectanglePro(projectiles[i].hitbox, Vector2{projectiles[i].size.x / 2.0f, projectiles[i].size.y / 2.0f}, projectiles[i].rotation * RAD2DEG, projectiles[i].color);
                    DrawRectangleRec(projectiles[i].hitbox, YELLOW);
            } 
      
        EndMode2D();
        
        //Draw cursor
        mouseScreenPos = GetMousePosition();
        cursorRec = {0, 0, (float)cursorTexture.width, (float)cursorTexture.height};
        cursorDestination = {mouseScreenPos.x + (float)cursorTexture.width/2.0f - cursorSize/2.0f, mouseScreenPos.y + (float)cursorTexture.height/2.0f - cursorSize/2.0f, cursorSize, cursorSize};

        cursorOrigin = {cursorTexture.width / 2.0f, cursorTexture.height / 2.0f};

        DrawTexturePro(cursorTexture, cursorRec, cursorDestination, cursorOrigin, 0.0f, WHITE);
        
        
        //------------------------------HUD------------------------------
        
        //Draw health
        Vector2 heartSize = {100, 100};
        
        for(int i = 0; i < (player.maxHealth / 2.0f); i++){
            
            Rectangle heartTextureSrc;
            Vector2 heartPos = {25 + (float)i * 100, 25};
            if(player.health >= 2 * (i + 1)){
                heartTextureSrc = {(float)heartTexture.width / 3.0f * 0.0f, 0, (float)heartTexture.width / 3.0f, (float)heartTexture.height};
            } else if(player.health == 2 * (i + 1) - 1){
                heartTextureSrc = {(float)heartTexture.width / 3.0f * 1.0f, 0, (float)heartTexture.width / 3.0f, (float)heartTexture.height};
            } else {
                heartTextureSrc = {(float)heartTexture.width / 3.0f * 2.0f, 0, (float)heartTexture.width / 3.0f, (float)heartTexture.height};
            }
            
            Rectangle heartTextureDest = {heartPos.x,heartPos.y, heartSize.x, heartSize.y};
            DrawTexturePro(heartTexture, heartTextureSrc, heartTextureDest, {0,0}, 0, WHITE);
        }
        
        //Draw weapon hud box
        Vector2 weaponHudBoxSize = {300, 300};
        Vector2 weaponHudBoxPos = {screenWidth-weaponHudBoxSize.x - 30.0f, screenHeight-weaponHudBoxSize.y + 25.0f};
        Rectangle weaponHudBoxTextureSrc = {0,0, (float)weaponHudBoxTexture.width, (float)weaponHudBoxTexture.height};
        Rectangle weaponHudBoxTextureDest = {weaponHudBoxPos.x,weaponHudBoxPos.y,weaponHudBoxSize.x,weaponHudBoxSize.y};
        
        DrawTexturePro(weaponHudBoxTexture, weaponHudBoxTextureSrc, weaponHudBoxTextureDest, {0,0}, 0, WHITE);
        
        //Draw weapon inside hud
        Rectangle weaponHudTextureSrc = {0,0, (float)player.weapons[player.equippedWeaponIndex].hudTexture.width, (float)player.weapons[player.equippedWeaponIndex].hudTexture.height};
        Rectangle weaponHudTextureDest = {weaponHudBoxPos.x, weaponHudBoxPos.y, weaponHudBoxSize.x, weaponHudBoxSize.y};
        
        DrawTexturePro(player.weapons[player.equippedWeaponIndex].hudTexture, weaponHudTextureSrc, weaponHudTextureDest, {0,0}, 0, WHITE);
        
        //Draw ammo counter text
        fontSize = 30;
        
        //Draw slash separater
        textWidth = MeasureText(TextFormat("/"), fontSize);
        DrawText(
            TextFormat("/"), 
            weaponHudBoxPos.x + weaponHudBoxSize.x/2 - textWidth/2, 
            weaponHudBoxPos.y + weaponHudBoxSize.y / 8, 
            fontSize, 
            WHITE
        );
        
        //Draw mag counter
        if(!player.weapons[player.equippedWeaponIndex].reloading){
            textWidth = MeasureText(TextFormat("%i", player.weapons[player.equippedWeaponIndex].mag), fontSize);
            DrawText(
                TextFormat("%i", player.weapons[player.equippedWeaponIndex].mag), 
                weaponHudBoxPos.x + weaponHudBoxSize.x/2 - textWidth - MeasureText(TextFormat("/"), fontSize), 
                weaponHudBoxPos.y + weaponHudBoxSize.y / 8, 
                fontSize, 
                WHITE
            ); 
        } else {
            textWidth = MeasureText(TextFormat("-"), fontSize);
            DrawText(
                TextFormat("-"), 
                weaponHudBoxPos.x + weaponHudBoxSize.x/2 - textWidth - MeasureText(TextFormat("/"), fontSize), 
                weaponHudBoxPos.y + weaponHudBoxSize.y / 8, 
                fontSize, 
                WHITE
            ); 
        }
        
        //Draw magSize counter
        textWidth = MeasureText(TextFormat("%i", player.weapons[player.equippedWeaponIndex].magSize), fontSize);
        DrawText(
            TextFormat("%i", player.weapons[player.equippedWeaponIndex].magSize), 
            weaponHudBoxPos.x + weaponHudBoxSize.x/2 + (MeasureText(TextFormat("/"), fontSize) / 2) + 10, 
            weaponHudBoxPos.y + weaponHudBoxSize.y / 8, 
            fontSize, 
            WHITE
        );
        
    } else if(exitWindowRequested){  
    
        //Exit menu
        DrawRectangle(0, screenHeight/2 - 100, screenWidth, 200, BLACK);
        int fontSize = 30;
        int textWidth = MeasureText("Are you sure you want to exit program? [Y/N]", fontSize);
        DrawText("Are you sure you want to exit program? [Y/N]", screenWidth/2 - textWidth/2, screenHeight/2 - fontSize/2, fontSize, WHITE);
    
    } else if(!exitWindowRequested && player.health <= 0.0f){
        //Respawn menu
        DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
        int fontSize1 = 200;
        int textWidth = MeasureText("YOU DIED", fontSize1);
        DrawText("YOU DIED", screenWidth/2 - textWidth/2, screenHeight/2 - fontSize1/2, fontSize1, RED);
        
        int fontSize2 = 80;
        textWidth = MeasureText("SPACE to Restart", fontSize2);
        DrawText("SPACE to Restart", screenWidth/2 - textWidth/2, screenHeight/2 - fontSize2/2 + fontSize1, fontSize2, WHITE);
    }
    
    EndDrawing();
}

void DrawPlayer(const Player& player, bool debugHitbox){
    
    int fontSize;
    int textWidth;
    
    Rectangle playerRec;
    Rectangle playerDest; 
    float frameWidth;
    
    Vector2 playerTextureSize = {80.0f, 80.0f};
    
    if(player.state == State::IDLE || player.state == State::WALKING){
        frameWidth = (float)player.texture.width / player.walkAnimationFrames;
        playerRec = {frameWidth * player.currentFrame,0,frameWidth,(float)player.texture.height};
    } else if(player.state == State::DASHING){
        frameWidth = (float)player.dashTexture.width / player.dashAnimationFrames;
        playerRec = {frameWidth * player.currentFrame,0,frameWidth,(float)player.dashTexture.height};
    }
    
    //Draw hitbox
    if(debugHitbox){
        DrawRectangleRec(player.hitBox, BLUE);  
    }
    
    //Draw sprite
    playerDest = {player.center.x - playerTextureSize.x/2.0f, player.center.y - playerTextureSize.y/2.0f + 1.5f, playerTextureSize.x, playerTextureSize.y};
    if(player.facingDirection == -1){
        playerRec = {frameWidth * player.currentFrame,0,-frameWidth,(float)player.texture.height};
    } else if(player.facingDirection == 1){
        playerRec = {frameWidth * player.currentFrame,0,frameWidth,(float)player.texture.height};
    }
    
    Color opacity;
    
    if(player.iFrames != 0.0f && player.state != State::DASHING && (int)(player.iFrames * 10)% 2 == 0){
        opacity = Color{255, 255, 255, 128};
    } else {
        opacity = Color{WHITE};
    }
    
    if(player.state == State::IDLE || player.state == State::WALKING){
        DrawTexturePro(player.texture, playerRec, playerDest, Vector2{0, 0}, 0, opacity);
    } else if(player.state == State::DASHING){
        DrawTexturePro(player.dashTexture, playerRec, playerDest, Vector2{0, 0}, 0, opacity);
    }
    
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

void DrawWeapon(Player player, float playerToMouseRotation){
    Vector2 gunPos;
    Rectangle gunSrc;
    Rectangle gunDest;
    float gunAngle;
    
    gunPos = {
        player.center.x + cos(playerToMouseRotation) * player.weapons[player.equippedWeaponIndex].positionOffset.x,
        player.center.y + sin(playerToMouseRotation) * player.weapons[player.equippedWeaponIndex].positionOffset.x
    };
    gunSrc = {0,0,(float)player.weapons[player.equippedWeaponIndex].texture.width / player.weapons[player.equippedWeaponIndex].textureFrames,(float)player.weapons[player.equippedWeaponIndex].texture.height - player.weapons[player.equippedWeaponIndex].positionOffset.y};
    gunDest = {gunPos.x,gunPos.y,player.weapons[player.equippedWeaponIndex].size,player.weapons[player.equippedWeaponIndex].size};
    gunAngle = playerToMouseRotation * RAD2DEG;
    
    if (gunAngle < -90 || gunAngle > 90){
        gunSrc.height = -gunSrc.height;
    }
    
    Color opacity;

    if(player.iFrames != 0.0f && player.state != State::DASHING && (int)(player.iFrames * 10)% 2 == 0){
        opacity = Color{255, 255, 255, 128};
    } else {
        opacity = Color{WHITE};
    }
    
    DrawTexturePro(player.weapons[player.equippedWeaponIndex].texture, gunSrc, gunDest, Vector2{0, gunDest.height / 2.0f}, gunAngle, opacity);
    //              texture                                           source   dest               origin/pivot              rotation   color
}
