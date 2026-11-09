#include "enemy.h"

#include "player.h"
#include "dungeon.h"

#include <raymath.h>

void AddEnemy(Enemy enemies[], const char* name, int& numEnemies){
    if(TextIsEqual(name, "Dummy")){
        enemies[numEnemies].name = "Dummy";
        enemies[numEnemies].speed = 150.0f;
        enemies[numEnemies].position = (Vector2){300,-300};
        enemies[numEnemies].health = 100.0f;
        enemies[numEnemies].size = (Vector2){40.0f,40.0f};
        
        enemies[numEnemies].hitBox = {enemies[numEnemies].position.x, enemies[numEnemies].position.y, enemies[numEnemies].size.x, enemies[numEnemies].size.y};
        enemies[numEnemies].center = (Vector2){enemies[numEnemies].position.x + enemies[numEnemies].size.x/2 ,enemies[numEnemies].position.y + enemies[numEnemies].size.y/2};
        
        enemies[numEnemies].attackRate = 1.0f;
        enemies[numEnemies].attackCD = enemies[numEnemies].attackRate;
        
        enemies[numEnemies].state = "idle";
        
        enemies[numEnemies].active = true;
    }
    
    numEnemies++;
}

void UpdateEnemies(Enemy enemies[], int& numEnemies, Player player, Dungeon& dungeon){
    //Check every enemy
    for(int i = 0; i < numEnemies; i++){
            
        if(enemies[i].active == true){
            float rot = 0.0f;
            Vector2 previousPosition = enemies[i].position;

            //Straight line towards player
            if(TextIsEqual(enemies[i].name,"Dummy")){
                rot = atan2(player.center.y - enemies[i].center.y, player.center.x - enemies[i].center.x);
                enemies[i].position.x += (cos(rot) * enemies[i].speed) * GetFrameTime();
                enemies[i].position.y += (sin(rot) * enemies[i].speed) * GetFrameTime();
                enemies[i].state = "walking";
            }

            //Update enemy positions
            enemies[i].hitBox.x = enemies[i].position.x;
            enemies[i].hitBox.y = enemies[i].position.y;
            enemies[i].center = (Vector2){enemies[i].position.x + enemies[i].size.x/2, enemies[i].position.y + enemies[i].size.y/2};

            //Check x
            for(int j = 0; j < dungeon.numRoomLocs; j++){
                if(dungeon.rooms[j].exists == true){
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.walls.size(); k++){
                        Rectangle wall = dungeon.rooms[j].roomContent.walls[k].hitbox;

                        if(CheckCollisionRecs(enemies[i].hitBox, wall)){
                            if(previousPosition.x + enemies[i].hitBox.width <= wall.x){
                                enemies[i].position.x = wall.x - enemies[i].hitBox.width;
                            } else if(previousPosition.x >= wall.x + wall.width){
                                enemies[i].position.x = wall.x + wall.width;
                            }

                            enemies[i].hitBox.x = enemies[i].position.x;
                            enemies[i].center.x = enemies[i].position.x + enemies[i].size.x/2;
                        }
                    }
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.decor.size(); k++){
                        Rectangle decor = dungeon.rooms[j].roomContent.decor[k].hitbox;

                        if(dungeon.rooms[j].roomContent.decor[k].playerCollision && CheckCollisionRecs(enemies[i].hitBox, decor)){
                            if(previousPosition.x + enemies[i].hitBox.width <= decor.x){
                                enemies[i].position.x = decor.x - enemies[i].hitBox.width;
                            } else if(previousPosition.x >= decor.x + decor.width){
                                enemies[i].position.x = decor.x + decor.width;
                            }

                            enemies[i].hitBox.x = enemies[i].position.x;
                            enemies[i].center.x = enemies[i].position.x + enemies[i].size.x/2;
                        }
                    }
                }
            }

            //Check y
            for(int j = 0; j < dungeon.numRoomLocs; j++){
                if(dungeon.rooms[j].exists == true){
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.walls.size(); k++){
                        Rectangle wall = dungeon.rooms[j].roomContent.walls[k].hitbox;

                        if(CheckCollisionRecs(enemies[i].hitBox, wall)){
                            if(previousPosition.y + enemies[i].hitBox.height <= wall.y){
                                enemies[i].position.y = wall.y - enemies[i].hitBox.height;
                            } else if(previousPosition.y >= wall.y + wall.height){
                                enemies[i].position.y = wall.y + wall.height;
                            }

                            enemies[i].hitBox.y = enemies[i].position.y;
                            enemies[i].center.y = enemies[i].position.y + enemies[i].size.y/2;
                        }
                    }
                    for(int k = 0; k < (int)dungeon.rooms[j].roomContent.decor.size(); k++){
                        Rectangle decor = dungeon.rooms[j].roomContent.decor[k].hitbox;

                        if(dungeon.rooms[j].roomContent.decor[k].playerCollision && CheckCollisionRecs(enemies[i].hitBox, decor)){
                            if(previousPosition.y + enemies[i].hitBox.height <= decor.y){
                                enemies[i].position.y = decor.y - enemies[i].hitBox.height;
                            } else if(previousPosition.y >= decor.y + decor.height){
                                enemies[i].position.y = decor.y + decor.height;
                            }

                            enemies[i].hitBox.y = enemies[i].position.y;
                            enemies[i].center.y = enemies[i].position.y + enemies[i].size.y/2;
                        }
                    }
                }
            }
        } else {
            //Remove inactive enemy
            for(int j = i; j < numEnemies - 1; j++){
                    enemies[j] = enemies[j+1];
            }
            numEnemies--;
            i--;
            
        }
        
    }
}
