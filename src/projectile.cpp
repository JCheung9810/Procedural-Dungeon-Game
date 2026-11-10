#include "projectile.h"

#include "player.h"
#include "enemy.h"
#include "dungeon.h"

#include <raymath.h>

void InitializeProjectile(Projectile& projectile){
    projectile.position = Vector2{0,0};
    projectile.speed = Vector2{0,0};
    projectile.size = Vector2{8,8};
    projectile.lifeSpan = 0.0f;
    projectile.damage = 0.0f;
    projectile.color = RED;
    projectile.hitbox = {projectile.position.x, projectile.position.y, projectile.size.x, projectile.size.y};
           
}

void FireProjectile(vector<Projectile>& projectiles, float playerToMouseRotation, const char* team, float projectileLifespan, Player& player, Enemy* enemy){    
    
    int projectilesToCreate = 0;
    if(TextIsEqual(team,"Player")){
        projectilesToCreate = player.weapons[player.equippedWeaponIndex].numProjectiles;
    } else if(TextIsEqual(team,"Enemy")){
        projectilesToCreate = 1;
    }
    
    for (int i = 0; i < projectilesToCreate; i++){
        Projectile projectile;
        InitializeProjectile(projectile);

        //Start lifespan
        projectile.lifeSpan = projectileLifespan; 

        if(TextIsEqual(team,"Player")){
            //Set projectile team
            projectile.team = "Player";
            
            //Projectile rotation from gun barrel to target
            int spread = (int) (player.weapons[player.equippedWeaponIndex].bulletSpread * 10);
            float randSpread = GetRandomValue(-spread, spread) / 10.0f;
            float projectileRotation = playerToMouseRotation * RAD2DEG + randSpread;
            projectile.rotation = projectileRotation * DEG2RAD;
            
            //Projectile starting position
            projectile.position = Vector2{
                player.center.x + cos(playerToMouseRotation) * (player.weapons[player.equippedWeaponIndex].projectileOffset), 
                player.center.y + sin(playerToMouseRotation) * (player.weapons[player.equippedWeaponIndex].projectileOffset)
            };
            
            //Set projectile damage and type
            projectile.damage = player.weapons[player.equippedWeaponIndex].damage;
            projectile.type = player.weapons[player.equippedWeaponIndex].type;

        } else if(TextIsEqual(team,"Enemy")){

            if(enemy == nullptr)
                return;
            
            //Set projectile team
            projectile.team = "Enemy";
            
            //Projectile rotation from center to player center
            projectile.rotation = atan2(player.center.y - enemy->center.y, player.center.x - enemy->center.x);
            
            //Projectile starting position
            projectile.position = enemy->center;
            
            //Set projectile damage and type
            projectile.damage = 1.0f;
            projectile.type = "Pistol";
        }
        
        //Set trajectory
        projectile.speed.x = cos(projectile.rotation) * 500;
        projectile.speed.y = sin(projectile.rotation) * 500;
        
        //Set projectile hitbox
        projectile.hitbox = {projectile.position.x - projectile.size.x/2.0f, projectile.position.y  - projectile.size.y/2.0f, projectile.size.x, projectile.size.y};

        projectiles.push_back(projectile);
        
    }  
    
    if(TextIsEqual(team,"Player")){
            
        //Set fire timer
        player.weapons[player.equippedWeaponIndex].fireTimer = player.weapons[player.equippedWeaponIndex].fireTime;
        
        //Decrease number of bullets in mag
        player.weapons[player.equippedWeaponIndex].mag--;
        
        //Auto reload
        if(player.weapons[player.equippedWeaponIndex].mag == 0){
            ReloadWeapon(player);
        }
        
        PlaySound(player.weapons[player.equippedWeaponIndex].shotSound);

        
    } else if(TextIsEqual(team,"Enemy")){ //&& projectilesCreated >= enemy.weapons[equippedWeaponIndex].numProjectiles){
        
    }
}


void UpdateProjectiles(vector<Projectile>& projectiles, Dungeon& dungeon, vector<Enemy>& enemies, Player& player){

    for(int i = 0; i < (int)projectiles.size(); i++){

        //Translate projectile
        projectiles[i].position.x += projectiles[i].speed.x * GetFrameTime();
        projectiles[i].position.y += projectiles[i].speed.y * GetFrameTime();

        //Update hitbox
        projectiles[i].hitbox = {
            projectiles[i].position.x - projectiles[i].size.x / 2.0f,
            projectiles[i].position.y - projectiles[i].size.y / 2.0f,
            projectiles[i].size.x,
            projectiles[i].size.y
        };

        bool collisionDetected = false;

        //Check walls/decor
        for(int j = 0; j < (int)dungeon.rooms.size() && !collisionDetected; j++){

            if(!dungeon.rooms[j].exists)
                continue;

            for(int k = 0; k < (int)dungeon.rooms[j].roomContent.walls.size(); k++){

                if(CheckCollisionRecs(dungeon.rooms[j].roomContent.walls[k].hitbox, projectiles[i].hitbox)){
                    collisionDetected = true;
                    break;
                }
            }

            if(collisionDetected)
                break;

            for(int k = 0; k < (int)dungeon.rooms[j].roomContent.decor.size(); k++){

                if(dungeon.rooms[j].roomContent.decor[k].projCollision &&
                   CheckCollisionRecs(dungeon.rooms[j].roomContent.decor[k].hitbox, projectiles[i].hitbox)){
                    collisionDetected = true;
                    break;
                }
            }
        }

        //Check enemy collision
        if(!collisionDetected && TextIsEqual(projectiles[i].team, "Player")){

            for(int j = 0; j < (int)enemies.size(); j++){

                if(enemies[j].active &&
                   CheckCollisionRecs(enemies[j].hitBox, projectiles[i].hitbox)){

                    enemies[j].health -= projectiles[i].damage;
                    collisionDetected = true;
                    break;
                }
            }
        }

        //Check player collision
        if(!collisionDetected && TextIsEqual(projectiles[i].team, "Enemy") &&
           player.iFrames <= 0.0f &&
           CheckCollisionRecs(player.hitBox, projectiles[i].hitbox)){

            player.health -= projectiles[i].damage;
            player.iFrames = 1.0f;
            collisionDetected = true;
        }

        //Update lifespan
        projectiles[i].lifeSpan -= GetFrameTime();

        //Remove projectile
        if(collisionDetected || projectiles[i].lifeSpan <= 0.0f){
            projectiles.erase(projectiles.begin() + i);
            i--;
        }
    }
}

