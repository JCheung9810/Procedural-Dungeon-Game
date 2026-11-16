#include "player.h"

#include "dungeon.h"

#include "json.hpp"

#include <fstream>
#include <cstring>
#include <string>

using json = nlohmann::json;
using namespace std;

#include <raymath.h>

void InitializePlayer(Player& player){

    ifstream file("config/player.json");

    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Could not open player config: config/player.json");
    }

    try {
        json data;
        file >> data;

        player.maxHealth = data.at("maxHealth").get<float>();
        player.baseSpeed = data.at("speed").get<float>();
        
        player.dashDistance = data.at("dashDistance").get<float>();
        player.dashTime = data.at("dashTime").get<float>();
        
        player.walkAnimationFrames = data.at("walkAnimationFrames").get<int>();
        player.dashAnimationFrames = data.at("dashAnimationFrames").get<int>();
        
        string texturePath = data.at("texturePath").get<string>();
        string dashTexturePath = data.at("dashTexturePath").get<string>();
        
        player.texture = assets.LoadGameTexture(strdup(texturePath.c_str())); 
        player.dashTexture = assets.LoadGameTexture(strdup(dashTexturePath.c_str())); 

        vector size = data.at("size").get<vector<float>>();
        player.size = {size[0],size[1]};

        player.dashCD = data.at("dashCD").get<float>();

        player.health = player.maxHealth;
        player.iFrames = 0.0f;
        
        player.center = {0,0};
        player.position = {
            player.center.x - (player.size.x/2.0f), 
            player.center.y - (player.size.y/2.0f)
        };
        player.hurtbox = {
            player.position.x, 
            player.position.y, 
            player.size.x, 
            player.size.y
        };
        player.hitbox = {
            player.position.x,
            player.position.y + player.hurtbox.height - 3.0f,
            player.size.x,
            3.0f
        };
        
        player.currentSpeed = player.baseSpeed;
        player.direction = {0, 0};
        
        player.tempDirection = {0, 0};
        player.facingDirection = 1;
        
        player.currentFrame = 0;
        player.frameTimer = 0.0f;
        player.frameSpeed = 0.1f;
    
        player.movementState = PlayerMovementState::IDLE;
        player.attackState = PlayerAttackState::IDLE;

        player.equippedWeaponIndex = 0;
        
        player.dashTimer = 0.0f;
        player.dashCDTimer = 0.0f;

        player.maxWeapons = 3;
        player.weapons.push_back(LoadWeapon("Pistol"));
        player.weapons.push_back(LoadWeapon("Double Barrel"));
        player.weapons.push_back(LoadWeapon("Dev Gun"));

    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Invalid player config: %s", e.what());
        throw;
    } 
}

void UpdatePlayer(Player& player, Dungeon& dungeon, bool debugSpeed, bool debugWall){

    float deltaTime = GetFrameTime();

    if(deltaTime > 0.05f){
        deltaTime = 0.05f;
    }
    
    //------------------------------MOVEMENT------------------------------
    float xDir = 0;
    float yDir = 0;
    
    if(debugSpeed){
        player.currentSpeed = player.baseSpeed * 2.0f;
    } else {
        player.currentSpeed = player.baseSpeed;
    }
    
    //Execute movement
    if(player.movementState != PlayerMovementState::DASHING){
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
        player.direction = Vector2Scale(player.direction, player.currentSpeed);
        
        //Update facing player.direction
        if(player.direction.x > 0){
            player.facingDirection = 1;
        }
        else if(player.direction.x < 0){
            player.facingDirection = -1;
        }
    }

    //Detect state
    if((xDir != 0.0f || yDir != 0.0f) && player.movementState != PlayerMovementState::DASHING){
        player.movementState = PlayerMovementState::WALKING;
    } else if(player.movementState != PlayerMovementState::DASHING){
        player.movementState = PlayerMovementState::IDLE;
    }
            
    //Detect dash
    if(IsKeyPressed(KEY_SPACE) && player.dashCDTimer <= 0.0f && player.movementState == PlayerMovementState::WALKING){
        player.movementState = PlayerMovementState::DASHING;
        float dashSpeed = (player.dashDistance / player.dashTime) / player.currentSpeed;
        player.tempDirection = Vector2Scale(player.direction, dashSpeed);            //Dash speed = Distance/time
        player.dashTimer = player.dashTime;
        //Check ongoing iFrames
        if(player.iFrames > 0.0f){
            //If the dash extends old iFrames update iFrames
            if(player.dashTime >= player.iFrames){
                player.iFrames = player.dashTime;
            }
        } else {
            player.iFrames = player.dashTime / 2.0f;
        }
        
        //Reset frames for animation
        player.currentFrame = 0;
        player.frameTimer = 0.0f;
    }

    //Get movement direction for collision detection
    Vector2 movementDirection = player.direction;
    if(player.movementState == PlayerMovementState::DASHING){
        movementDirection = player.tempDirection;
    }
    
    //Translate player, detect collision (x)
    player.position.x += movementDirection.x * deltaTime;
    player.hurtbox.x = player.position.x;
    player.hitbox.x = player.position.x;
    if(!debugWall){
        for(int i = 0; i < (int)dungeon.rooms.size(); i++){
            if(dungeon.rooms[i].exists == true){
                //Wall collision
                for(int j = 0; j < (int)dungeon.rooms[i].roomContent.walls.size(); j++){
                    if(CheckCollisionRecs(player.hitbox,dungeon.rooms[i].roomContent.walls[j].hitbox)){
                        if (movementDirection.x > 0){
                            player.position.x = dungeon.rooms[i].roomContent.walls[j].hitbox.x - player.hitbox.width;
                        } else if (movementDirection.x < 0){
                            player.position.x = dungeon.rooms[i].roomContent.walls[j].hitbox.x + dungeon.rooms[i].roomContent.walls[j].hitbox.width;
                        }
                        player.hurtbox.x = player.position.x;
                        player.hitbox.x = player.position.x;
                    }
                }
                //Decor collision
                for(int j = 0; j < (int)dungeon.rooms[i].roomContent.decor.size(); j++){
                    if(dungeon.rooms[i].roomContent.decor[j].playerCollision && CheckCollisionRecs(player.hitbox,dungeon.rooms[i].roomContent.decor[j].hitbox)){
                        if (movementDirection.x > 0){
                            player.position.x = dungeon.rooms[i].roomContent.decor[j].hitbox.x - player.hitbox.width;
                        } else if (movementDirection.x < 0){
                            player.position.x = dungeon.rooms[i].roomContent.decor[j].hitbox.x + dungeon.rooms[i].roomContent.decor[j].hitbox.width;
                        }
                        player.hurtbox.x = player.position.x;
                        player.hitbox.x = player.position.x;
                    }
                }
                //Door collision
                for(int j = 0; j < (int)dungeon.rooms[i].roomContent.doors.size(); j++){
                    if(dungeon.rooms[i].roomContent.doors[j].active && CheckCollisionRecs(player.hitbox,dungeon.rooms[i].roomContent.doors[j].hitbox)){
                        if (movementDirection.x > 0){
                            player.position.x = dungeon.rooms[i].roomContent.doors[j].hitbox.x - player.hitbox.width;
                        } else if (movementDirection.x < 0){
                            player.position.x = dungeon.rooms[i].roomContent.doors[j].hitbox.x + dungeon.rooms[i].roomContent.doors[j].hitbox.width;
                        }
                        player.hurtbox.x = player.position.x;
                        player.hitbox.x = player.position.x;
                    }
                }
            }
        }
    }
    
    //Translate player, detect collision (y)
    player.position.y += movementDirection.y * deltaTime;
    player.hurtbox.y = player.position.y;
    player.hitbox.y = player.position.y + player.hurtbox.height - 3.0f;
    if(!debugWall){
        for(int i = 0; i < (int)dungeon.rooms.size(); i++){
            if(dungeon.rooms[i].exists == true){
                //Wall collision
                for(int j = 0; j < (int)dungeon.rooms[i].roomContent.walls.size(); j++){
                    if(CheckCollisionRecs(player.hitbox, dungeon.rooms[i].roomContent.walls[j].hitbox)){
                        if(movementDirection.y > 0){
                            player.position.y = dungeon.rooms[i].roomContent.walls[j].hitbox.y - (player.hitbox.y - player.position.y) - player.hitbox.height;
                        } else if(movementDirection.y < 0){
                            player.position.y = dungeon.rooms[i].roomContent.walls[j].hitbox.y + dungeon.rooms[i].roomContent.walls[j].hitbox.height - (player.hitbox.y - player.position.y);
                        }

                        player.hurtbox.y = player.position.y;
                        player.hitbox.y = player.position.y + player.hurtbox.height - 3.0f;
                    }
                }
                //Decor collision
                for(int j = 0; j < (int)dungeon.rooms[i].roomContent.decor.size(); j++){
                    if(dungeon.rooms[i].roomContent.decor[j].playerCollision && CheckCollisionRecs(player.hitbox, dungeon.rooms[i].roomContent.decor[j].hitbox)){
                        if(movementDirection.y > 0){
                            player.position.y = dungeon.rooms[i].roomContent.decor[j].hitbox.y - (player.hitbox.y - player.position.y) - player.hitbox.height;
                        } else if(movementDirection.y < 0){
                            player.position.y = dungeon.rooms[i].roomContent.decor[j].hitbox.y + dungeon.rooms[i].roomContent.decor[j].hitbox.height - (player.hitbox.y - player.position.y);
                        }

                        player.hurtbox.y = player.position.y;
                        player.hitbox.y = player.position.y + player.hurtbox.height - 3.0f;
                    }
                }
                //Door collision
                for(int j = 0; j < (int)dungeon.rooms[i].roomContent.doors.size(); j++){
                    if(dungeon.rooms[i].roomContent.doors[j].active && CheckCollisionRecs(player.hitbox, dungeon.rooms[i].roomContent.doors[j].hitbox)){
                        if(movementDirection.y > 0){
                            player.position.y = dungeon.rooms[i].roomContent.doors[j].hitbox.y - (player.hitbox.y - player.position.y) - player.hitbox.height;
                        } else if(movementDirection.y < 0){
                            player.position.y = dungeon.rooms[i].roomContent.doors[j].hitbox.y + dungeon.rooms[i].roomContent.doors[j].hitbox.height - (player.hitbox.y - player.position.y);
                        }
                        player.hurtbox.y = player.position.y;
                        player.hitbox.y = player.position.y + player.hurtbox.height - 3.0f;
                    }
                }
            }
        }
    }
    
    //dashTimer
    if(player.dashTimer > 0.0f){
        player.dashTimer -= deltaTime;

        if(player.dashTimer < 0.0f){
            player.dashTimer = 0.0f;
            player.movementState = PlayerMovementState::IDLE;
            player.dashCDTimer = player.dashCD;
        }
    }
    
    //dashCDTimer
    if(player.dashCDTimer > 0.0f){
        player.dashCDTimer -= deltaTime;

        if(player.dashCDTimer < 0.0f){
            player.dashCDTimer = 0.0f;
        }
    }
        
    //Player iFrames
    if(player.iFrames > 0.0f){
        player.iFrames -= deltaTime;

        if(player.iFrames < 0.0f){
            player.iFrames = 0.0f;
        }
    }
        
    //Update player center    
    player.center = {
        player.position.x + player.hurtbox.width/2.0f, 
        player.position.y + player.hurtbox.height/2.0f
    };

    //Update attack state
    if(player.weapons[player.equippedWeaponIndex].reloading == true){
        player.attackState = PlayerAttackState::RELOADING;
    } else if(player.attackState != PlayerAttackState::ATTACKING || player.attackState != PlayerAttackState::RELOADING){
        player.attackState = PlayerAttackState::IDLE;
    }


    //Animation  
    if(player.movementState == PlayerMovementState::WALKING){
        player.frameTimer += deltaTime;

        if(player.frameTimer >= player.frameSpeed){
            player.frameTimer = 0.0f;
            player.currentFrame++;

            if (player.currentFrame >= 4)
                player.currentFrame = 0;
        }
    } else if(player.movementState == PlayerMovementState::IDLE){
        //Reset to idle animation
        player.currentFrame = 0;
        player.frameTimer = 0.0f;
    } else if(player.movementState == PlayerMovementState::DASHING){    
        player.frameTimer += deltaTime;
        
        player.currentFrame = (int)((player.frameTimer * player.dashAnimationFrames) / player.dashTime); 

        if(player.currentFrame >= player.dashAnimationFrames)
        {
            player.currentFrame = player.dashAnimationFrames - 1;
        }
    }
    
    
    
}

