#include "Projectile.h"

#include "Player.h"
#include "Enemy.h"
#include "Room.h"

#include <raymath.h>

void FirePlayerProjectile(Projectile projectiles[], int maxProjectiles, float playerToMouseRotation, char* gunType, float gunSize, float projectileLifespan, Player player, float gunOffset){       
    for (int i = 0; i < maxProjectiles; i++){
        if (!projectiles[i].active){
            projectiles[i].active = true;
            projectiles[i].lifeSpan = projectileLifespan; 
            
            projectiles[i].rotation = playerToMouseRotation;
            projectiles[i].speed.x = cos(projectiles[i].rotation) * 1000;
            projectiles[i].speed.y = sin(projectiles[i].rotation) * 1000;
            
            if(TextIsEqual(gunType, "Pistol")){
                projectiles[i].position = (Vector2){player.center.x + cos(projectiles[i].rotation) * gunSize - (cos(projectiles[i].rotation) * gunOffset*2.3f), player.center.y + sin(projectiles[i].rotation) * gunSize - (sin(projectiles[i].rotation) * gunOffset*2.3f)};
            }
            
            projectiles[i].hitBox = {projectiles[i].position.x - projectiles[i].size.x/2.0f, projectiles[i].position.y  - projectiles[i].size.y/2.0f, projectiles[i].size.x, projectiles[i].size.y};
            
            projectiles[i].team = (char*)"Player";
            projectiles[i].type = (char*)"Pistol";
            break;
        }
    }    
}

void UpdateProjectile(Projectile projectiles[], int maxProjectiles, Room rooms[], int numRoomLocs, Enemy enemies[], int numEnemies, Player& player){
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
                    
                    player.health -= 10.0f;
                    player.iFrames = 1.0f;
                }
                
                projectiles[i].lifeSpan -= GetFrameTime(); 
               
                if(projectiles[i].lifeSpan <= 0.0f){
                   projectiles[i].active = false;
                }
            }                    
        }
}