#include "draw.h"

#include "debug.h"
#include "player.h"
#include "dungeon.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"

#include <raymath.h>

extern Debug debug;

void DrawGame(Camera2D camera, Player player, Dungeon& dungeon, vector<Enemy>& enemies, vector<Projectile>& projectiles, 
            Texture2D heartTexture, Texture2D weaponHudBoxTexture, Texture2D weaponHudBoxBackgroundTexture, Font font, 
            Vector2 mouseScreenPos, float playerToMouseRotation, Texture2D cursorTexture, float cursorSize, 
            bool minimap, Vector2 minimapPos, float minimapScale,
            int screenWidth, int screenHeight,          
            bool exitWindowRequested){
                
                
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

                    float tileSize;
                    float scale = room.roomContent.roomUnscaledSize.x;

                    //Draw background
                    DrawTextureEx(
                        room.roomContent.backgroundTexture,
                        room.position,
                        0.0f,
                        scale,
                        WHITE
                    );

                    //Draw doors
                    for(int doorIt = 0; doorIt < (int)room.roomContent.doors.size(); doorIt++){
                        //Temporary: Only draw active doors
                        if(!room.roomContent.doors[doorIt].active){
                            continue;
                        }

                        RoomDoor& door = room.roomContent.doors[doorIt];

                        tileSize = 16 * scale;
                        int tilesX = (int)roundf(door.hitbox.width / tileSize);
                        int tilesY = (int)roundf(door.hitbox.height / tileSize);

                        for(int x = 0; x < tilesX; x++){
                            for(int y = 0; y < tilesY; y++){
                                Rectangle dest = {
                                    door.hitbox.x + x * tileSize,
                                    door.hitbox.y + y * tileSize,
                                    tileSize,
                                    tileSize
                                };

                                DrawTexturePro(
                                    assets.textures.at("assets/textures/environment/dungeonTileset.png"),
                                    assets.spriteInfo[door.spriteID].textureSrc,
                                    dest,
                                    Vector2{0, 0},
                                    0.0f,
                                    WHITE
                                );
                            }
                        }
                    }
                    
                    //Draw decor
                    for(int decorIt = 0; decorIt < (int)room.roomContent.decor.size(); decorIt++){
                        RoomDecor& decor = room.roomContent.decor[decorIt];

                        if(decor.hitbox.height + decor.hitbox.y < player.hitbox.y + player.hitbox.height || decor.alwaysOnBottom){

                            tileSize = 16 * scale;
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
                                        assets.textures.at("assets/textures/environment/dungeonTileset.png"),
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

                    //Draw usables
                    for(int usableIt = 0; usableIt < (int)room.roomContent.usables.size(); usableIt++){
                        RoomUsable& usable = room.roomContent.usables[usableIt];

                        if(usable.visible){
                            if(usable.yLevel < player.hitbox.y + player.hitbox.height){

                                if(usable.type == "RoomClearRewardSpawn"){
                                    tileSize = 24 * scale;
                                } else if(usable.type == "Staircase"){
                                    tileSize = 32 * scale;
                                }
                                int tilesX = (int)roundf(usable.hitbox.width / tileSize);
                                int tilesY = (int)roundf(usable.hitbox.height / tileSize);

                                int spriteRow;
                                if(CheckCollisionRecs(usable.hitbox, player.hurtbox)){
                                    spriteRow = 1;
                                } else {
                                    spriteRow = 0;
                                }

                                for(int x = 0; x < tilesX; x++){
                                    for(int y = 0; y < tilesY; y++){
                                        Rectangle src{
                                            usable.currentFrame * (float)usable.texture.width / (float)usable.numFrames,
                                            spriteRow * (float)usable.texture.height / 2,
                                            (float)usable.texture.width / (float)usable.numFrames,
                                            (float)usable.texture.height / 2
                                        };
                                        Rectangle dest = {
                                            usable.hitbox.x + x * tileSize,
                                            usable.hitbox.y + y * tileSize,
                                            tileSize,
                                            tileSize
                                        };

                                        //DrawRectangleV({usable.hitbox.x,usable.hitbox.y}, {usable.hitbox.width, usable.hitbox.height}, WHITE);
                                        DrawTexturePro(
                                            usable.texture,
                                            src,
                                            dest,
                                            Vector2{0, 0},
                                            0.0f,
                                            WHITE
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
                
                if(debug.vision){
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
            }

            //Draw player
            DrawPlayer(player);

            //Draw enemies
            DrawEnemies(enemies);

            //Draw projectiles
            for (int i = 0; i < (int)projectiles.size(); i++){
                //DrawRectanglePro(projectiles[i].hitbox, Vector2{projectiles[i].size.x / 2.0f, projectiles[i].size.y / 2.0f}, projectiles[i].rotation * RAD2DEG, projectiles[i].color);
                //DrawRectangleRec(projectiles[i].hitbox, RED);
                Rectangle srcRec = {
                    0,
                    0,
                    (float)projectiles[i].sprite.width,
                    (float)projectiles[i].sprite.height
                };

                Rectangle destRec = {
                    projectiles[i].hitbox.x,
                    projectiles[i].hitbox.y,
                    (float)projectiles[i].hitbox.width,
                    (float)projectiles[i].hitbox.height
                };

                DrawTexturePro(projectiles[i].sprite, srcRec, destRec, {0,0}, 0.0f, WHITE);
            } 

            //Draw weapon
            DrawWeapons(player, playerToMouseRotation, enemies);

            //Draw in front of player walls/decor
            for(int roomIt = 0; roomIt < (int)dungeon.rooms.size(); roomIt++){
                Room& room = dungeon.rooms[roomIt];

                if(room.exists == true){
                    float scale = room.roomContent.roomUnscaledSize.x;
                    float tileSize = 16 * scale;

                    //Draw foreground
                    DrawTextureEx(
                        room.roomContent.foregroundTexture,
                        room.position,
                        0.0f,
                        scale,
                        WHITE
                    );

                    //Draw decor that is in front of the player
                    for(int decorIt = 0; decorIt < (int)room.roomContent.decor.size(); decorIt++){
                        RoomDecor& decor = room.roomContent.decor[decorIt];
                        
                        if((decor.hitbox.height + decor.hitbox.y >= player.hitbox.y + player.hitbox.height && !decor.alwaysOnBottom) || decor.alwaysOnTop){

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
                                        assets.textures.at("assets/textures/environment/dungeonTileset.png"),
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

                    //Draw usables that is in front of the player
                    
                    //Draw usables
                    for(int usableIt = 0; usableIt < (int)room.roomContent.usables.size(); usableIt++){
                        RoomUsable& usable = room.roomContent.usables[usableIt];

                        if(usable.visible){
                            if(usable.yLevel >= player.hitbox.y + player.hitbox.height){
                                
                                if(usable.type == "RoomClearRewardSpawn"){
                                    tileSize = 24 * scale;
                                } else if(usable.type == "Staircase"){
                                    tileSize = 32 * scale;
                                }
                                int tilesX = (int)roundf(usable.hitbox.width / tileSize);
                                int tilesY = (int)roundf(usable.hitbox.height / tileSize);

                                int spriteRow;
                                if(CheckCollisionRecs(usable.hitbox, player.hurtbox)){
                                    spriteRow = 1;
                                } else {
                                    spriteRow = 0;
                                }

                                for(int x = 0; x < tilesX; x++){
                                    for(int y = 0; y < tilesY; y++){
                                        Rectangle src{
                                            usable.currentFrame * (float)usable.texture.width / (float)usable.numFrames,
                                            spriteRow * (float)usable.texture.height / 2,
                                            (float)usable.texture.width / (float)usable.numFrames,
                                            (float)usable.texture.height / 2
                                        };
                                        Rectangle dest = {
                                            usable.hitbox.x + x * tileSize,
                                            usable.hitbox.y + y * tileSize,
                                            tileSize,
                                            tileSize
                                        };

                                        //DrawRectangleV({usable.hitbox.x,usable.hitbox.y}, {usable.hitbox.width, usable.hitbox.height}, WHITE);
                                        DrawTexturePro(
                                            usable.texture,
                                            src,
                                            dest,
                                            Vector2{0, 0},
                                            0.0f,
                                            WHITE
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
      
        EndMode2D();
        
        //Draw cursor
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
        DrawTexturePro(weaponHudBoxBackgroundTexture, weaponHudBoxTextureSrc, weaponHudBoxTextureDest, {0,0}, 0, Color{255,255,255,150});
        
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
            weaponHudBoxPos.y + (weaponHudBoxSize.y / 8), 
            fontSize, 
            WHITE
        );
        
        //Draw mag counter
        if(!player.weapons[player.equippedWeaponIndex].reloading){
            textWidth = MeasureText(TextFormat("%i", player.weapons[player.equippedWeaponIndex].mag), fontSize);
            DrawText(
                TextFormat("%i", player.weapons[player.equippedWeaponIndex].mag), 
                weaponHudBoxPos.x + weaponHudBoxSize.x/2 - textWidth - MeasureText(TextFormat("/"), fontSize), 
                weaponHudBoxPos.y + (weaponHudBoxSize.y / 8), 
                fontSize, 
                WHITE
            ); 
        } else {
            textWidth = MeasureText(TextFormat("-"), fontSize);
            DrawText(
                TextFormat("-"), 
                weaponHudBoxPos.x + weaponHudBoxSize.x/2 - textWidth - MeasureText(TextFormat("/"), fontSize), 
                weaponHudBoxPos.y + (weaponHudBoxSize.y / 8), 
                fontSize, 
                WHITE
            ); 
        }
        
        //Draw magSize counter
        if(player.weapons[player.equippedWeaponIndex].maxAmmo != -1){
            textWidth = MeasureText(TextFormat("%i", player.weapons[player.equippedWeaponIndex].currentAmmo), fontSize);
            DrawText(
                TextFormat("%i", player.weapons[player.equippedWeaponIndex].currentAmmo), 
                weaponHudBoxPos.x + weaponHudBoxSize.x/2 + (MeasureText(TextFormat("/"), fontSize) / 2) + 10, 
                weaponHudBoxPos.y + (weaponHudBoxSize.y / 8), 
                fontSize, 
                WHITE
            );
        } else {

            const char* infinitySymbol = "\u221E";

            fontSize = 66;
            textWidth = MeasureTextEx(font, infinitySymbol, fontSize, 0).x;

            DrawTextEx(
                font,
                infinitySymbol,
                Vector2{
                    weaponHudBoxPos.x + weaponHudBoxSize.x / 2 + (MeasureText("/", fontSize) / 2),
                    weaponHudBoxPos.y + (weaponHudBoxSize.y / 8) - 25
                },
                fontSize,
                0,
                WHITE
            );
        }

        //Minimap
        DrawMinimap(minimap, minimapPos, minimapScale, dungeon, screenWidth, screenHeight);
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

void DrawPlayer(const Player& player){
    
    int fontSize;
    int textWidth;
    
    Rectangle playerRec;
    Rectangle playerDest; 
    float frameWidth;
    
    Vector2 playerTextureSize = {80.0f, 80.0f};
    
    if(player.movementState == PlayerMovementState::IDLE || player.movementState == PlayerMovementState::WALKING){
        frameWidth = (float)player.texture.width / player.walkAnimationFrames;
        playerRec = {frameWidth * player.currentFrame,0,frameWidth,(float)player.texture.height};
    } else if(player.movementState == PlayerMovementState::DASHING){
        frameWidth = (float)player.dashTexture.width / player.dashAnimationFrames;
        playerRec = {frameWidth * player.currentFrame,0,frameWidth,(float)player.dashTexture.height};
    }
    
    //Draw hurtbox
    if(debug.hurtbox){
        DrawRectangleRec(player.hurtbox, BLUE);  
    }
    //Draw hitbox
    if(debug.hitbox){
        DrawRectangleRec(player.hitbox, ORANGE);
    }
    
    //Draw sprite
    playerDest = {player.center.x - playerTextureSize.x/2.0f, player.center.y - playerTextureSize.y/2.0f + 1.5f, playerTextureSize.x, playerTextureSize.y};
    if(player.facingDirection == -1){
        playerRec = {frameWidth * player.currentFrame,0,-frameWidth,(float)player.texture.height};
    } else if(player.facingDirection == 1){
        playerRec = {frameWidth * player.currentFrame,0,frameWidth,(float)player.texture.height};
    }
    
    Color opacity;
    
    if(player.iFrames != 0.0f && player.movementState != PlayerMovementState::DASHING && (int)(player.iFrames * 10)% 2 == 0){
        opacity = Color{255, 255, 255, 128};
    } else {
        opacity = Color{WHITE};
    }
    
    if(player.movementState == PlayerMovementState::IDLE || player.movementState == PlayerMovementState::WALKING){
        DrawTexturePro(player.texture, playerRec, playerDest, Vector2{0, 0}, 0, opacity);
    } else if(player.movementState == PlayerMovementState::DASHING){
        DrawTexturePro(player.dashTexture, playerRec, playerDest, Vector2{0, 0}, 0, opacity);
    }
    
    //Draw stats
    fontSize = 15;
    textWidth = MeasureText("Player", fontSize);
    DrawText(
        "Player", player.center.x - textWidth/2, 
        player.hurtbox.y - fontSize*2.2f, 
        fontSize, 
        BLACK
    );   
    textWidth = MeasureText(TextFormat("%0.2f",player.health), fontSize);
    DrawText(
        TextFormat("%0.2f",player.health), player.center.x - textWidth/2, 
        player.hurtbox.y - fontSize*1.1f, 
        fontSize, 
        BLACK
    );  
    
}

void DrawEnemies(vector<Enemy>& enemies){
    for (int i = 0; i < (int)enemies.size(); i++){

                int fontSize = 15;
                int textWidth = MeasureText(enemies[i].name, fontSize);
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

                Rectangle srcRec;
                Rectangle destRec;
                Vector2 enemyTextureSize = {80.0f, 80.0f};
                float frameWidth = (float)enemies[i].texture.width / enemies[i].walkAnimationFrames;
                int textureRow;
                if(enemies[i].status != EnemyStatus::DEAD){
                    if(enemies[i].status == EnemyStatus::ALIVE){
                        textureRow = 0;
                    } else if(enemies[i].status == EnemyStatus::STUNNED){
                        textureRow = 1;
                    }
                } else {
                    textureRow = 2;
                }

                if(enemies[i].movementVector.x >= 0.0f){
                    srcRec = {
                        frameWidth * enemies[i].currentFrame,
                        ((float)enemies[i].texture.height / 3.0f) * (float)textureRow,
                        frameWidth,
                        (float)enemies[i].texture.height / 3.0f
                    };
                } else {
                    srcRec = {
                        frameWidth * enemies[i].currentFrame,
                        ((float)enemies[i].texture.height / 3.0f) * (float)textureRow,
                        -frameWidth,
                        (float)enemies[i].texture.height / 3.0f
                    };
                }

                destRec = {
                    enemies[i].center.x - enemyTextureSize.x/2.0f,
                    enemies[i].center.y - enemyTextureSize.y/2.0f + 1.5f,
                    enemyTextureSize.x,
                    enemyTextureSize.y
                };

                if(debug.hurtbox){
                    DrawRectangleRec(enemies[i].hitbox, RED);
                }
                if(enemies[i].status != EnemyStatus::DEAD){
                    DrawTexturePro(enemies[i].texture, srcRec, destRec, Vector2{0, 0}, 0, WHITE);
                } else {
                    DrawTexturePro(enemies[i].texture, srcRec, destRec, Vector2{0, 0}, 0, Color{130,130,130,130});
                }
            }
}

void DrawWeapons(Player& player, float playerToMouseRotation, vector<Enemy>& enemies){
    Vector2 gunPos;
    Rectangle gunSrc;
    Rectangle gunDest;
    float gunAngle;
    
    //Player gun
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

    if(player.iFrames != 0.0f && player.movementState != PlayerMovementState::DASHING && (int)(player.iFrames * 10)% 2 == 0){
        opacity = Color{255, 255, 255, 128};
    } else {
        opacity = Color{WHITE};
    }
    
    DrawTexturePro(player.weapons[player.equippedWeaponIndex].texture, gunSrc, gunDest, Vector2{0, gunDest.height / 2.0f}, gunAngle, opacity);
    //              texture                                           source   dest               origin/pivot              rotation   color

    //Enemy guns
    for(int i = 0; i < (int)enemies.size(); i++){
        if(enemies[i].status != EnemyStatus::DEAD){
            gunPos = {
                enemies[i].center.x + cos(enemies[i].aimAngle) * enemies[i].weapon.positionOffset.x,
                enemies[i].center.y + sin(enemies[i].aimAngle) * enemies[i].weapon.positionOffset.x
            };
            gunSrc = {0,0,(float)enemies[i].weapon.texture.width / enemies[i].weapon.textureFrames,(float)enemies[i].weapon.texture.height - enemies[i].weapon.positionOffset.y};
            gunDest = {gunPos.x,gunPos.y,enemies[i].weapon.size,enemies[i].weapon.size};
            
            if (enemies[i].aimAngle * RAD2DEG < -90 || enemies[i].aimAngle * RAD2DEG > 90){
                gunSrc.height = -gunSrc.height;
            }
            if(enemies[i].stunDuration <= 0.0f){
                DrawTexturePro(enemies[i].weapon.texture, gunSrc, gunDest, Vector2{0, gunDest.height / 2.0f}, enemies[i].aimAngle * RAD2DEG, WHITE);
            } else {
                DrawTexturePro(enemies[i].weapon.texture, gunSrc, gunDest, Vector2{0, gunDest.height / 2.0f}, enemies[i].aimAngle * RAD2DEG, Color{255,255,255,130});
            }
        }
    }
}

void DrawMinimap(bool minimap, Vector2 minimapPos, float minimapScale, Dungeon& dungeon, int screenWidth, int screenHeight){
    if(minimap){
        DrawRectangle(0,0, screenWidth, screenHeight, Color{0,0,0, 130});
        Color roomColor = {100,100,100,255};

        for(int roomIt = 0; roomIt < (int)dungeon.rooms.size(); roomIt++){
            Room room = dungeon.rooms[roomIt];

            float x = roundf(room.position.x / minimapScale - minimapPos.x + screenWidth / 2.0f);
            float y = roundf(room.position.y / minimapScale - minimapPos.y + screenHeight / 2.0f);
            float w = roundf(room.size.x / minimapScale);
            float h = roundf(room.size.y / minimapScale);

            DrawRectangle(x, y, w + 1, h + 1, roomColor);

            roomColor.b += 5;
            roomColor.g += 5;
            roomColor.r += 5;
        }
    }
}