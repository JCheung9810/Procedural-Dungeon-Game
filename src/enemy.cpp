#include "enemy.h"

#include "player.h"
#include "dungeon.h"

#include <raymath.h>

#include <vector>

using namespace std;

void AddEnemy(vector<Enemy>& enemies, const char* name, Vector2 position){
    Enemy newEnemy;
    newEnemy.name = name;

    if(TextIsEqual(name, "Dummy Pistol")){
        newEnemy.speed = 150.0f;
        newEnemy.position = position;
        newEnemy.health = 100.0f;
        newEnemy.size = Vector2{40.0f,40.0f};
        
        newEnemy.hitbox = {newEnemy.position.x, newEnemy.position.y, newEnemy.size.x, newEnemy.size.y};
        newEnemy.center = Vector2{newEnemy.position.x + newEnemy.size.x/2 ,newEnemy.position.y + newEnemy.size.y/2};
        
        newEnemy.weapon = LoadWeapon("config/weapon.json", "Enemy Pistol");
        newEnemy.attackRate = 1.0f;
        newEnemy.attackCD = newEnemy.attackRate;

    } else if(TextIsEqual(name, "Dummy Double Barrel")){
        newEnemy.speed = 150.0f;
        newEnemy.position = position;
        newEnemy.health = 100.0f;
        newEnemy.size = Vector2{40.0f,40.0f};
        
        newEnemy.hitbox = {newEnemy.position.x, newEnemy.position.y, newEnemy.size.x, newEnemy.size.y};
        newEnemy.center = Vector2{newEnemy.position.x + newEnemy.size.x/2 ,newEnemy.position.y + newEnemy.size.y/2};
        
        newEnemy.weapon = LoadWeapon("config/weapon.json", "Enemy Double Barrel");
        newEnemy.attackRate = 1.0f;
        newEnemy.attackCD = newEnemy.attackRate;
    } else {
        TraceLog(LOG_ERROR, "Enemy type for spawning not found");
        return;
    }

    newEnemy.state = "idle";
    newEnemy.active = true;

    enemies.push_back(newEnemy);
}

void UpdateEnemies(vector<Enemy>& enemies, Player& player, Dungeon& dungeon){
    float deltaTime = GetFrameTime();

    if(deltaTime > 0.05f){
        deltaTime = 0.05f;
    }

    //Check every enemy
    for(int i = 0; i < (int)enemies.size(); i++){

        if(enemies[i].health <= 0.0f){
            enemies[i].active = false;
        }
            
        if(enemies[i].active == true){
            float rot = 0.0f;
            Vector2 previousPosition = enemies[i].position;

            //Straight line towards player
            if(TextIsEqual(enemies[i].name,"Dummy Pistol") || TextIsEqual(enemies[i].name,"Dummy Double Barrel")){
                rot = atan2(player.center.y - enemies[i].center.y, player.center.x - enemies[i].center.x);
                enemies[i].position.x += (cos(rot) * enemies[i].speed) * deltaTime;
                enemies[i].position.y += (sin(rot) * enemies[i].speed) * deltaTime;
                enemies[i].state = "walking";
            }

            //Update enemy positions
            enemies[i].hitbox.x = enemies[i].position.x;
            enemies[i].hitbox.y = enemies[i].position.y;
            enemies[i].center = Vector2{enemies[i].position.x + enemies[i].size.x/2, enemies[i].position.y + enemies[i].size.y/2};

            //Check x
            for(int j = 0; j < (int)dungeon.rooms.size(); j++){
                if(dungeon.rooms[j].exists == true){
                    //Wall collision
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.walls.size(); k++){
                        Rectangle wall = dungeon.rooms[j].roomContent.walls[k].hitbox;

                        if(CheckCollisionRecs(enemies[i].hitbox, wall)){
                            if(previousPosition.x + enemies[i].hitbox.width <= wall.x){
                                enemies[i].position.x = wall.x - enemies[i].hitbox.width;
                            } else if(previousPosition.x >= wall.x + wall.width){
                                enemies[i].position.x = wall.x + wall.width;
                            }

                            enemies[i].hitbox.x = enemies[i].position.x;
                            enemies[i].center.x = enemies[i].position.x + enemies[i].size.x/2;
                        }
                    }
                    //Decor collision
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.decor.size(); k++){
                        Rectangle decor = dungeon.rooms[j].roomContent.decor[k].hitbox;

                        if(dungeon.rooms[j].roomContent.decor[k].playerCollision && CheckCollisionRecs(enemies[i].hitbox, decor)){
                            if(previousPosition.x + enemies[i].hitbox.width <= decor.x){
                                enemies[i].position.x = decor.x - enemies[i].hitbox.width;
                            } else if(previousPosition.x >= decor.x + decor.width){
                                enemies[i].position.x = decor.x + decor.width;
                            }

                            enemies[i].hitbox.x = enemies[i].position.x;
                            enemies[i].center.x = enemies[i].position.x + enemies[i].size.x/2;
                        }
                    }
                    //Doors collision
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.doors.size(); k++){
                        Rectangle door = dungeon.rooms[j].roomContent.doors[k].hitbox;

                        if(dungeon.rooms[j].roomContent.doors[k].active && CheckCollisionRecs(enemies[i].hitbox, door)){
                            if(previousPosition.x + enemies[i].hitbox.width <= door.x){
                                enemies[i].position.x = door.x - enemies[i].hitbox.width;
                            } else if(previousPosition.x >= door.x + door.width){
                                enemies[i].position.x = door.x + door.width;
                            }

                            enemies[i].hitbox.x = enemies[i].position.x;
                            enemies[i].center.x = enemies[i].position.x + enemies[i].size.x/2;
                        }
                    }
                }
            }

            //Check y
            for(int j = 0; j < (int)dungeon.rooms.size(); j++){
                if(dungeon.rooms[j].exists == true){
                    //Wall collision
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.walls.size(); k++){
                        Rectangle wall = dungeon.rooms[j].roomContent.walls[k].hitbox;

                        if(CheckCollisionRecs(enemies[i].hitbox, wall)){
                            if(previousPosition.y + enemies[i].hitbox.height <= wall.y){
                                enemies[i].position.y = wall.y - enemies[i].hitbox.height;
                            } else if(previousPosition.y >= wall.y + wall.height){
                                enemies[i].position.y = wall.y + wall.height;
                            }

                            enemies[i].hitbox.y = enemies[i].position.y;
                            enemies[i].center.y = enemies[i].position.y + enemies[i].size.y/2;
                        }
                    }
                    //Decor collision
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.decor.size(); k++){
                        Rectangle decor = dungeon.rooms[j].roomContent.decor[k].hitbox;

                        if(dungeon.rooms[j].roomContent.decor[k].playerCollision && CheckCollisionRecs(enemies[i].hitbox, decor)){
                            if(previousPosition.y + enemies[i].hitbox.height <= decor.y){
                                enemies[i].position.y = decor.y - enemies[i].hitbox.height;
                            } else if(previousPosition.y >= decor.y + decor.height){
                                enemies[i].position.y = decor.y + decor.height;
                            }

                            enemies[i].hitbox.y = enemies[i].position.y;
                            enemies[i].center.y = enemies[i].position.y + enemies[i].size.y/2;
                        }
                    }
                    //Doors collision
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.doors.size(); k++){
                        Rectangle door = dungeon.rooms[j].roomContent.doors[k].hitbox;

                        if(dungeon.rooms[j].roomContent.doors[k].active && CheckCollisionRecs(enemies[i].hitbox, door)){
                            if(previousPosition.y + enemies[i].hitbox.height <= door.y){
                                enemies[i].position.y = door.y - enemies[i].hitbox.height;
                            } else if(previousPosition.y >= door.y + door.height){
                                enemies[i].position.y = door.y + door.height;
                            }

                            enemies[i].hitbox.y = enemies[i].position.y;
                            enemies[i].center.y = enemies[i].position.y + enemies[i].size.y/2;
                        }
                    }
                }
            }
        } else {
            //Remove inactive enemy
            enemies.erase(enemies.begin() + i);  
            i--;        
        }
        
    }
}
