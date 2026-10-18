#include "Projectile.h"

#include "Player.h"
#include "Enemy.h"
#include "Room.h"

#include <raymath.h>

void InitializeProjectiles(Projectile projectiles[], int maxProjectiles){
    for(int i = 0; i < maxProjectiles; i++){
        projectiles[i].position = (Vector2){0,0};
        projectiles[i].speed = (Vector2){0,0};
        projectiles[i].size = {8,8};
        projectiles[i].active = false;
        projectiles[i].lifeSpan = 0.0f;
        projectiles[i].damage = 0.0f;
        projectiles[i].color = RED;
        projectiles[i].hitBox = {projectiles[i].position.x, projectiles[i].position.y, projectiles[i].size.x, projectiles[i].size.y};
        
    }    
}

void FireProjectile(Projectile projectiles[], int maxProjectiles, float playerToMouseRotation, char* team, char* gunType, float gunSize, float projectileLifespan, Player player, Enemy enemy, float gunOffset){       
    for (int i = 0; i < maxProjectiles; i++){
        if (!projectiles[i].active){
            projectiles[i].active = true;
            projectiles[i].lifeSpan = projectileLifespan; 
            
            if(TextIsEqual(team,"Player")){
                projectiles[i].team = (char*)"Player";
                projectiles[i].rotation = playerToMouseRotation;
                
                if(TextIsEqual(gunType, "Pistol")){
                    projectiles[i].position = (Vector2){player.center.x + cos(projectiles[i].rotation) * (gunSize - 45), player.center.y + sin(projectiles[i].rotation) * (gunSize - 45)};
                    projectiles[i].damage = 1.0f;
                    projectiles[i].type = (char*)"Pistol";
                }
                
            } else if(TextIsEqual(team,"Enemy")){
                projectiles[i].team = (char*)"Enemy";
                projectiles[i].rotation = atan2(player.center.y - enemy.center.y, player.center.x - enemy.center.x);
                
                projectiles[i].position = enemy.center;
                projectiles[i].damage = 1.0f;
                projectiles[i].type = (char*)"Pistol";
            }
            projectiles[i].speed.x = cos(projectiles[i].rotation) * 500;
            projectiles[i].speed.y = sin(projectiles[i].rotation) * 500;
            
            projectiles[i].hitBox = {projectiles[i].position.x - projectiles[i].size.x/2.0f, projectiles[i].position.y  - projectiles[i].size.y/2.0f, projectiles[i].size.x, projectiles[i].size.y};
            
            break;
        }
    }    
}

void UpdateProjectiles(Projectile projectiles[], int maxProjectiles, Room rooms[], int numRoomLocs, Enemy enemies[], int numEnemies, Player& player){
    //Translate projectile
        for(int i = 0; i < maxProjectiles; i++){
            if(projectiles[i].active){
                
                projectiles[i].position = {
                    projectiles[i].position.x + projectiles[i].speed.x * GetFrameTime(), 
                    projectiles[i].position.y + projectiles[i].speed.y * GetFrameTime()
                };    

                projectiles[i].hitBox = {projectiles[i].position.x - projectiles[i].size.x/2.0f, projectiles[i].position.y  - projectiles[i].size.y/2.0f, projectiles[i].size.x, projectiles[i].size.y};
                
            }
        }
        
        //Projectile collision/despawn
        for(int i = 0; i < maxProjectiles; i++){
            if(projectiles[i].active){
                //Check walls
                for(int j = 0; j < numRoomLocs; j++){
                    if(rooms[j].exists == true){
                        for(int k = 0; k < rooms[j].numWalls; k++){
                            if(CheckCollisionRecs(rooms[j].walls[k],projectiles[i].hitBox)){
                                projectiles[i].active = false;
                                projectiles[i].lifeSpan = 0.0f;
                            }
                        }
                    }
                }
                
                //Check enemy collision
                for(int j = 0; j < numEnemies; j++){
                    if(enemies[j].active == true && CheckCollisionRecs(enemies[j].hitBox,projectiles[i].hitBox) && TextIsEqual(projectiles[i].team, "Player")){
                        projectiles[i].active = false;
                        projectiles[i].lifeSpan = 0.0f;
                        
                        enemies[j].health -= 10.0f;
                        if(enemies[j].health <= 0.0f){
                            enemies[j].active = false;
                        }
                    }
                }
                
                //Check player collision
                if(CheckCollisionRecs(player.hitBox,projectiles[i].hitBox) && TextIsEqual(projectiles[i].team, "Enemy") && player.iFrames <= 0.0f){
                    projectiles[i].active = false;
                    projectiles[i].lifeSpan = 0.0f;
                    
                    player.health -= projectiles[i].damage;
                    player.iFrames = 1.0f;
                }
                
                projectiles[i].lifeSpan -= GetFrameTime(); 
               
                if(projectiles[i].lifeSpan <= 0.0f){
                   projectiles[i].active = false;
                }
            }                    
        }
}