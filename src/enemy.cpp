#include "enemy.h"

#include "player.h"
#include "dungeon.h"
#include "projectile.h"

#include "json.hpp"

#include <raymath.h>

#include <fstream>
#include <cstring>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

void AddEnemy(vector<Enemy>& enemies, const char* name, Vector2 position){
    Enemy newEnemy;

    ifstream file("config/enemy.json");

    if(!file.is_open()) {
        TraceLog(LOG_ERROR, "Failed to open enemy json: config/enemy.json");
    }

    try {
        json data;
        file >> data;

        for (const auto& item : data.at("enemies")) {
            if (item.at("name").get<string>() == name) {

                string type = item.at("type").get<string>();
                newEnemy.type = strdup(type.c_str());

                newEnemy.intelligence = item.at("intelligence").get<float>();

                newEnemy.health = item.at("health").get<float>();
                newEnemy.speed = item.at("speed").get<float>();

                vector<float> size = item.at("size").get<vector<float>>();
                newEnemy.size = {size[0], size[1]};

                newEnemy.attackRate = item.at("attackRate").get<float>();
                newEnemy.minRange = item.at("minRange").get<float>();
                newEnemy.maxRange = item.at("maxRange").get<float>();

                string weapon = item.at("weapon").get<string>();
                newEnemy.weapon = LoadWeapon(weapon.c_str());

                newEnemy.name = name;
                newEnemy.position = position;
                newEnemy.hitbox = {newEnemy.position.x, newEnemy.position.y, newEnemy.size.x, newEnemy.size.y};
                newEnemy.center = Vector2{newEnemy.position.x + newEnemy.size.x/2 ,newEnemy.position.y + newEnemy.size.y/2};
                newEnemy.attackCD = newEnemy.attackRate;

                newEnemy.aimAngle = 0.0f;

                newEnemy.movementState = MovementState::IDLE;
                newEnemy.attackState = AttackState::IDLE;
                newEnemy.active = true;

                enemies.push_back(newEnemy);
                break;
            }
        }



    }
    catch (const exception& e)
    {
        TraceLog(LOG_ERROR, "Error reading enemy JSON: %s", e.what());
    }
}

void UpdateEnemies(vector<Enemy>& enemies, Player& player, Dungeon& dungeon, vector<Projectile>& projectiles){
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
            rot = atan2(player.center.y - enemies[i].center.y, player.center.x - enemies[i].center.x);
            Vector2 previousPosition = enemies[i].position;
            Vector2 vectorToPlayer = player.center - enemies[i].center;
            float distanceToPlayer = Vector2Length(vectorToPlayer);
            float minDistanceToPlayer;
            float maxDistanceToPlayer;

            //Aiming
            float leadAmount = enemies[i].intelligence;
            float timeToReachPlayer = distanceToPlayer/enemies[i].weapon.projectileSpeed;

            Vector2 predictedPlayerPosition = Vector2Add(player.center, Vector2Scale(player.direction, (timeToReachPlayer * leadAmount)));
            enemies[i].aimAngle = atan2f(predictedPlayerPosition.y - enemies[i].center.y, predictedPlayerPosition.x - enemies[i].center.x);

            //Movement pattern
            if(TextIsEqual(enemies[i].name,"Dummy Pistol") || TextIsEqual(enemies[i].name,"Dummy Double Barrel")){
                if(TextIsEqual(enemies[i].name,"Dummy Pistol")){
                    maxDistanceToPlayer = 250.0f;
                    minDistanceToPlayer = 225.0f;
                } else if(TextIsEqual(enemies[i].name,"Dummy Double Barrel")){
                    maxDistanceToPlayer = 180.0f;
                    minDistanceToPlayer = 150.0f;
                } else {
                    maxDistanceToPlayer = 60.0f;
                    minDistanceToPlayer = 50.0f;
                    TraceLog(LOG_ERROR, "Enemy max and min distance not set");
                }

                if(distanceToPlayer >= maxDistanceToPlayer){
                    enemies[i].position.x += (cos(rot) * enemies[i].speed) * deltaTime;
                    enemies[i].position.y += (sin(rot) * enemies[i].speed) * deltaTime;
                    enemies[i].movementState = MovementState::WALKING;
                } else if(distanceToPlayer < minDistanceToPlayer){
                    enemies[i].position.x -= (cos(rot) * enemies[i].speed) * deltaTime;
                    enemies[i].position.y -= (sin(rot) * enemies[i].speed) * deltaTime;
                    enemies[i].movementState = MovementState::WALKING;
                } else {
                    enemies[i].movementState = MovementState::IDLE;
                }
            } else if(TextIsEqual(enemies[i].name,"Dummy Suicide Bomber")){
                enemies[i].position.x += (cos(rot) * enemies[i].speed) * deltaTime;
                enemies[i].position.y += (sin(rot) * enemies[i].speed) * deltaTime;
                enemies[i].movementState = MovementState::WALKING;
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


            //Enemy attacking
            enemies[i].attackCD -= deltaTime;

            if(enemies[i].attackCD <= 0.0f){
                enemies[i].attackState = AttackState::ATTACKING;
            }

            if(enemies[i].attackState == AttackState::ATTACKING){
                if(TextIsEqual(enemies[i].name,"Dummy Suicide Bomber")){
                    if(distanceToPlayer < 50.0f){
                        FireProjectile(projectiles, nullptr, "Enemy", player, &enemies[i]);
                    }
                } else {
                    FireProjectile(projectiles, nullptr, "Enemy", player, &enemies[i]);
                }
            }

        } else {
            //Remove inactive enemy
            enemies.erase(enemies.begin() + i);  
            i--;        
        }
        
    }
}
