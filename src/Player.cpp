#include "player.h"

#include "room.h"

#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

#include <raymath.h>

PlayerConfig LoadPlayerConfig(const char* filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Could not open player config: %s", filePath);
        return {};
    }

    try {
        json data;
        file >> data;

        PlayerConfig config;

        config.maxHealth = data.at("maxHealth").get<float>();
        config.speed = data.at("speed").get<float>();

        return config;
    }
    catch (const std::exception& e) {
        TraceLog(LOG_ERROR, "Invalid player config: %s", e.what());
        throw;
    }
}

void InitializePlayer(Player& player, PlayerConfig playerConfig){

    Vector2 playerSize = {35.0f, 45.0f};
    player.speed = playerConfig.speed;
    player.maxHealth = playerConfig.maxHealth;
    player.health = player.maxHealth;
    player.iFrames = 0.0f;
    
    player.center = {0,0};
    player.position = {
        -player.center.x/2.0f, 
        -player.center.y/2.0f
    };
    player.hitBox = {
        player.position.x, 
        player.position.y, 
        playerSize.x, 
        playerSize.y
    };
    
    player.direction = {0, 0};
    
    player.tempDirection = {0, 0};
    player.facingDirection = 1;
    
    player.currentFrame = 0;
    player.frameTimer = 0.0f;
    player.frameSpeed = 0.1f;
 
    player.state = (char*)"idle";
    
    player.weapon = LoadWeapon("config/weapon.json", "Pistol");

    player.dashTime = 0.0f;
    player.dashCD = 0.0f;
    
    player.texture = LoadTexture("assets/Player.png"); 
    
}

void UpdatePlayer(Player& player, Room rooms[], int& numRoomLocs, bool debugSpeed, bool debugWall){
    
    //------------------------------MOVEMENT------------------------------
    float xDir = 0;
    float yDir = 0;
    
    if(debugSpeed){
        player.speed = 1800.0f;
    } else {
        player.speed = 600.0f;
    }
    
    //Execute movement
    if(!TextIsEqual(player.state, (char*)"dashing")){
        if(IsKeyDown(KEY_D))
            xDir += 1.0f;
        
        if(IsKeyDown(KEY_A))
            xDir -= 1.0f;
        
        if(IsKeyDown(KEY_S))
            yDir += 1.0f;
        
        if(IsKeyDown(KEY_W))
            yDir -= 1.0f;
        
        //Normalize
        player.direction = Vector2Normalize({xDir,yDir});
        
        //Scale
        player.direction = Vector2Scale(player.direction, player.speed);
        
        //Update facing player.direction
        if(player.direction.x > 0){
            player.facingDirection = 1;
        }
        else if(player.direction.x < 0){
            player.facingDirection = -1;
        }
        
        //Translate player, detect collision (x)
        player.position.x += player.direction.x * GetFrameTime();
        player.hitBox.x = player.position.x;
        if(!debugWall){
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == true){
                    for(int j = 0; j < rooms[i].numWalls; j++){
                        if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                            if (player.direction.x > 0){
                                player.position.x = rooms[i].walls[j].x - player.hitBox.width;
                            } else if (player.direction.x < 0){
                                player.position.x = rooms[i].walls[j].x + rooms[i].walls[j].width;
                            }
                            player.hitBox.x = player.position.x;
                        }
                    }
                }
            }
        }
        
        //Translate player, detect collision (y)
        player.position.y += player.direction.y * GetFrameTime();
        player.hitBox.y = player.position.y;
        if(!debugWall){
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == true){
                    for(int j = 0; j < rooms[i].numWalls; j++){
                        if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                            if (player.direction.y > 0){
                                player.position.y = rooms[i].walls[j].y - player.hitBox.height;
                            } else if (player.direction.y < 0){
                                player.position.y = rooms[i].walls[j].y + rooms[i].walls[j].height;
                            }
                            player.hitBox.y = player.position.y;
                        }
                    }
                }
            }
        }
    }
    
    //Detect state
    if((xDir != 0.0f || yDir != 0.0f) && !TextIsEqual(player.state, (char*)"dashing")){
        player.state = (char*)"walking";
    } else if(!TextIsEqual(player.state, (char*)"dashing")){
        player.state = (char*)"idle";
    }
            
    //Detect dash
    if(IsKeyPressed(KEY_SPACE) && player.dashCD <= 0.0f && (TextIsEqual(player.state, (char*)"walking"))){
        player.state = (char*)"dashing";
        player.tempDirection = Vector2Scale(player.direction, 2.0f);
        player.dashTime = 0.2f;
        player.iFrames = player.dashTime;
    }
    
    //Execute Dash
    if(TextIsEqual(player.state, (char*)"dashing")){                        
        //Translate player, detect collision (x)
        player.position.x += player.tempDirection.x * GetFrameTime();
        player.hitBox.x = player.position.x;
        if(!debugWall){
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == true){
                    for(int j = 0; j < rooms[i].numWalls; j++){
                        if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                            if (player.tempDirection.x > 0){
                                player.position.x = rooms[i].walls[j].x - player.hitBox.width;
                            } else if (player.tempDirection.x < 0){
                                player.position.x = rooms[i].walls[j].x + rooms[i].walls[j].width;
                            }
                            player.hitBox.x = player.position.x;
                        }
                    }
                }
            }
        }
        
        //Translate player, detect collision (y)
        player.position.y += player.tempDirection.y * GetFrameTime();
        player.hitBox.y = player.position.y;
        if(!debugWall){
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == true){
                    for(int j = 0; j < rooms[i].numWalls; j++){
                        if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                            if (player.tempDirection.y > 0){
                                player.position.y = rooms[i].walls[j].y - player.hitBox.height;
                            } else if (player.tempDirection.y < 0){
                                player.position.y = rooms[i].walls[j].y + rooms[i].walls[j].height;
                            }
                            player.hitBox.y = player.position.y;
                        }
                    }
                }
            }
        }
    
        //dashTime
        if(player.dashTime > 0.0f){
            player.dashTime -= GetFrameTime();

            if(player.dashTime < 0.0f){
                player.dashTime = 0.0f;
                player.state = (char*)"idle";
                player.dashCD = 1.0f;
            }
        }
    }
    
    //dashCD
    if(player.dashCD > 0.0f){
        player.dashCD -= GetFrameTime();

        if(player.dashCD < 0.0f){
            player.dashCD = 0.0f;
        }
    }
        
    //Player iFrames
    if(player.iFrames > 0.0f){
        player.iFrames -= GetFrameTime();

        if(player.iFrames < 0.0f){
            player.iFrames = 0.0f;
        }
    }
        
    //Update player center    
    player.center = {
        player.position.x + player.hitBox.width/2.0f, 
        player.position.y + player.hitBox.height/2.0f
    };

    //Animation  
    if(TextIsEqual(player.state, (char*)"walking")){
        player.frameTimer += GetFrameTime();

        if(player.frameTimer >= player.frameSpeed){
            player.frameTimer = 0.0f;
            player.currentFrame++;

            if (player.currentFrame >= 4)
                player.currentFrame = 0;
        }
    } else if(TextIsEqual(player.state, (char*)"idle")){
        //Reset to idle animation
        player.currentFrame = 0;
        player.frameTimer = 0.0f;
    } else if(TextIsEqual(player.state, (char*)"dashing")){
        //Update this
        //Reset to idle animation
        player.currentFrame = 0;
        player.frameTimer = 0.0f;
    }
    
    
    
}

