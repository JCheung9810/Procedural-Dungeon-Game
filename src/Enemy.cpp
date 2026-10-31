#include "enemy.h"

#include "player.h"
#include "room.h"

#include <raymath.h>

void AddEnemy(Enemy enemies[], char* name, int& numEnemies){
    if(TextIsEqual(name, "Dummy")){
        enemies[numEnemies].name = (char*)"Dummy";
        enemies[numEnemies].speed = 150.0f;
        enemies[numEnemies].position = (Vector2){300,-300};
        enemies[numEnemies].health = 100.0f;
        enemies[numEnemies].size = (Vector2){40.0f,40.0f};
        
        enemies[numEnemies].hitBox = {enemies[numEnemies].position.x, enemies[numEnemies].position.y, enemies[numEnemies].size.x, enemies[numEnemies].size.y};
        enemies[numEnemies].center = (Vector2){enemies[numEnemies].position.x + enemies[numEnemies].size.x/2 ,enemies[numEnemies].position.y + enemies[numEnemies].size.y/2};
        
        enemies[numEnemies].attackRate = 1.0f;
        enemies[numEnemies].attackCD = enemies[numEnemies].attackRate;
        
        enemies[numEnemies].state = (char*)"idle";
        
        enemies[numEnemies].active = true;
    }
    
    numEnemies++;
}

void UpdateEnemies(Enemy enemies[], int& numEnemies, Player player, Room rooms[], int numRoomLocs){
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
                enemies[i].state = (char*)"walking";
            }

            //Update enemy positions
            enemies[i].hitBox.x = enemies[i].position.x;
            enemies[i].hitBox.y = enemies[i].position.y;
            enemies[i].center = (Vector2){enemies[i].position.x + enemies[i].size.x/2, enemies[i].position.y + enemies[i].size.y/2};

            //Check x
            for(int j = 0; j < numRoomLocs; j++){
                if(rooms[j].exists == true){
                    for(int k = 0; k < rooms[j].numWalls; k++){
                        if(CheckCollisionRecs(enemies[i].hitBox, rooms[j].walls[k])){
                            if(previousPosition.x + enemies[i].hitBox.width <= rooms[j].walls[k].x){
                                enemies[i].position.x = rooms[j].walls[k].x - enemies[i].hitBox.width;
                            } else if(previousPosition.x >= rooms[j].walls[k].x + rooms[j].walls[k].width){
                                enemies[i].position.x = rooms[j].walls[k].x + rooms[j].walls[k].width;
                            }

                            enemies[i].hitBox.x = enemies[i].position.x;
                            enemies[i].center.x = enemies[i].position.x + enemies[i].size.x/2;
                        }
                    }
                }
            }

            //Check y
            for(int j = 0; j < numRoomLocs; j++){
                if(rooms[j].exists == true){
                    for(int k = 0; k < rooms[j].numWalls; k++){
                        if(CheckCollisionRecs(enemies[i].hitBox, rooms[j].walls[k])){
                            if(previousPosition.y + enemies[i].hitBox.height <= rooms[j].walls[k].y){
                                enemies[i].position.y = rooms[j].walls[k].y - enemies[i].hitBox.height;
                            } else if(previousPosition.y >= rooms[j].walls[k].y + rooms[j].walls[k].height){
                                enemies[i].position.y = rooms[j].walls[k].y + rooms[j].walls[k].height;
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