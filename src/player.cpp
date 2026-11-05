#include "player.h"

#include "room.h"

#include "json.hpp"

#include <fstream>
#include <cstring>
#include <string>

using json = nlohmann::json;
using namespace std;

#include <raymath.h>

PlayerConfig LoadPlayerConfig(const char* filePath) {
    ifstream file(filePath);

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
        
        config.dashTime = data.at("dashTime").get<float>();
        
        config.walkAnimationFrames = data.at("walkAnimationFrames").get<int>();
        config.dashAnimationFrames = data.at("dashAnimationFrames").get<int>();
        
        string texturePath = data.at("texturePath").get<string>();
        string dashTexturePath = data.at("dashTexturePath").get<string>();
        
        config.texturePath = strdup(texturePath.c_str());
        config.dashTexturePath = strdup(dashTexturePath.c_str());

        return config;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Invalid player config: %s", e.what());
        throw;
    }
}

void InitializePlayer(Player& player, PlayerConfig playerConfig, AssetManager assets){

    Vector2 playerSize = {35.0f, 45.0f};
    player.speed = playerConfig.speed;
    player.maxHealth = playerConfig.maxHealth;
    player.health = player.maxHealth;
    player.iFrames = 0.0f;
    
    player.center = {0,0};
    player.position = {
        player.center.x - (playerSize.x/2.0f), 
        player.center.y - (playerSize.y/2.0f)
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
    
    player.walkAnimationFrames = playerConfig.walkAnimationFrames;
 
    player.state = State::IDLE;
    
    player.numWeapons = 2;
    player.equippedWeaponIndex = 0;
    player.weapon[0] = LoadWeapon("config/weapon.json", "Pistol", assets);
    player.weapon[1] = LoadWeapon("config/weapon.json", "Double Barrel", assets);

    player.dashTime = playerConfig.dashTime;
    player.dashTimer = 0.0f;
    player.dashCD = 0.0f;
    
    player.dashAnimationFrames = playerConfig.dashAnimationFrames;
    
    player.texture = assets.LoadGameTexture(playerConfig.texturePath); 
    player.dashTexture = assets.LoadGameTexture(playerConfig.dashTexturePath); 
    
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
    if(player.state != State::DASHING){
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
    if((xDir != 0.0f || yDir != 0.0f) && player.state != State::DASHING){
        player.state = State::WALKING;
    } else if(player.state != State::DASHING){
        player.state = State::IDLE;
    }
            
    //Detect dash
    if(IsKeyPressed(KEY_SPACE) && player.dashCD <= 0.0f && player.state == State::WALKING){
        player.state = State::DASHING;
        player.tempDirection = Vector2Scale(player.direction, 2.0f);            //Dash speed = double normal speed
        player.dashTimer = player.dashTime;
        //Check ongoing iFrames
        if(player.iFrames > 0.0f){
            //If the dash extends old iFrames update iFrames
            if(player.dashTime >= player.iFrames){
                player.iFrames = player.dashTime;
            }
        } else {
            player.iFrames = player.dashTime;
        }
        
        //Reset frames for animation
        player.currentFrame = 0;
        player.frameTimer = 0.0f;
    }
    
    //Execute Dash
    if(player.state == State::DASHING){                        
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
    
        //dashTimer
        if(player.dashTimer > 0.0f){
            player.dashTimer -= GetFrameTime();

            if(player.dashTimer < 0.0f){
                player.dashTimer = 0.0f;
                player.state = State::IDLE;
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
    if(player.state == State::WALKING){
        player.frameTimer += GetFrameTime();

        if(player.frameTimer >= player.frameSpeed){
            player.frameTimer = 0.0f;
            player.currentFrame++;

            if (player.currentFrame >= 4)
                player.currentFrame = 0;
        }
    } else if(player.state == State::IDLE){
        //Reset to idle animation
        player.currentFrame = 0;
        player.frameTimer = 0.0f;
    } else if(player.state == State::DASHING){    
        player.frameTimer += GetFrameTime();
        
        player.currentFrame = (int)((player.frameTimer * player.dashAnimationFrames) / player.dashTime); 
    }
    
    
    
}

