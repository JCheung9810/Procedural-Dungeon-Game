#include "projectile.h"

#include "player.h"
#include "enemy.h"
#include "dungeon.h"

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

void FireProjectile(Projectile projectiles[], int maxProjectiles, float playerToMouseRotation, const char* team, float projectileLifespan, Player& player, Enemy enemy){    
    
    int projectilesCreated = 0;
    
    for (int i = 0; i < maxProjectiles; i++){
        if (!projectiles[i].active){
            if(TextIsEqual(team,"Player")){
                //Activate projectile and start lifespan
                projectiles[i].active = true;
                projectiles[i].lifeSpan = projectileLifespan; 
                
                //Set projectile team
                projectiles[i].team = "Player";
                
                //Projectile rotation from gun barrel to target
                int spread = (int) (player.weapon[player.equippedWeaponIndex].bulletSpread * 10);
                float randSpread = GetRandomValue(-spread, spread) / 10.0f;
                float projectileRotation = playerToMouseRotation * RAD2DEG + randSpread;
                projectiles[i].rotation = projectileRotation * DEG2RAD;
                
                //Projectile starting position
                projectiles[i].position = (Vector2){
                    player.center.x + cos(playerToMouseRotation) * (player.weapon[player.equippedWeaponIndex].projectileOffset), 
                    player.center.y + sin(playerToMouseRotation) * (player.weapon[player.equippedWeaponIndex].projectileOffset)
                };
                
                //Set projectile damage and type
                projectiles[i].damage = player.weapon[player.equippedWeaponIndex].damage;
                projectiles[i].type = player.weapon[player.equippedWeaponIndex].type;

            } else if(TextIsEqual(team,"Enemy")){
                //Activate projectile and start lifespan
                projectiles[i].active = true;
                projectiles[i].lifeSpan = projectileLifespan; 
                
                //Set projectile team
                projectiles[i].team = "Enemy";
                
                //Projectile rotation from center to player center
                projectiles[i].rotation = atan2(player.center.y - enemy.center.y, player.center.x - enemy.center.x);
                
                //Projectile starting position
                projectiles[i].position = enemy.center;
                
                //Set projectile damage and type
                projectiles[i].damage = 1.0f;
                projectiles[i].type = "Pistol";
            }
            
            //Set trajectory
            projectiles[i].speed.x = cos(projectiles[i].rotation) * 500;
            projectiles[i].speed.y = sin(projectiles[i].rotation) * 500;
            
            //Set projectile hitbox
            projectiles[i].hitBox = {projectiles[i].position.x - projectiles[i].size.x/2.0f, projectiles[i].position.y  - projectiles[i].size.y/2.0f, projectiles[i].size.x, projectiles[i].size.y};
            
            //Increase projectilesCreated
            projectilesCreated++;
            
            if(TextIsEqual(team,"Player") && projectilesCreated >= player.weapon[player.equippedWeaponIndex].numProjectiles){
                
                //Set fire timer
                player.weapon[player.equippedWeaponIndex].fireTimer = player.weapon[player.equippedWeaponIndex].fireTime;
                
                //Decrease number of bullets in mag
                player.weapon[player.equippedWeaponIndex].mag--;
                
                //Auto reload
                if(player.weapon[player.equippedWeaponIndex].mag == 0){
                    ReloadWeapon(player);
                }
                
                PlaySound(player.weapon[player.equippedWeaponIndex].shotSound);
                
                break;
                
            } else if(TextIsEqual(team,"Enemy")){ //&& projectilesCreated >= enemy.weapon[equippedWeaponIndex].numProjectiles){
                break;
            }
        }
    }    
}

void UpdateProjectiles(Projectile projectiles[], int maxProjectiles, Dungeon dungeon, Enemy enemies[], int numEnemies, Player& player){
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
                for(int j = 0; j < dungeon.numRoomLocs; j++){
                    if(dungeon.rooms[j].exists == true){
                        for(int k = 0; k < dungeon.rooms[j].numWalls; k++){
                            if(CheckCollisionRecs(dungeon.rooms[j].walls[k],projectiles[i].hitBox)){
                                projectiles[i].active = false;
                                projectiles[i].lifeSpan = 0.0f;
                            }
                        }
                    }
                }
                
                //Skip entity collision check if collided with wall
                if(!projectiles[i].active){
                    continue;
                }
                
                //Check enemy collision
                for(int j = 0; j < numEnemies; j++){
                    if(enemies[j].active == true && CheckCollisionRecs(enemies[j].hitBox,projectiles[i].hitBox) && TextIsEqual(projectiles[i].team, "Player")){
                        projectiles[i].active = false;
                        projectiles[i].lifeSpan = 0.0f;
                        
                        enemies[j].health -= projectiles[i].damage;
                        if(enemies[j].health <= 0.0f){
                            enemies[j].active = false;
                        }
                        
                        break;
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
