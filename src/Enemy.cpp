#include "Enemy.h"

void AddEnemy(Enemy enemies[], char* name, int& numEnemies){
    if(TextIsEqual(name, "Dummy")){
        enemies[numEnemies].name = (char*)"Dummy";
        enemies[numEnemies].position = (Vector2){300,-300};
        enemies[numEnemies].health = 100.0f;
        enemies[numEnemies].size = (Vector2){40.0f,40.0f};
        
        enemies[numEnemies].hitBox = {enemies[numEnemies].position.x, enemies[numEnemies].position.y, enemies[numEnemies].size.x, enemies[numEnemies].size.y};
        Vector2 dummyCenter = (Vector2){enemies[numEnemies].position.x + enemies[numEnemies].size.x/2 ,enemies[numEnemies].position.y + enemies[numEnemies].size.y/2};
        
        enemies[numEnemies].active = true;
    }
    
    numEnemies++;
}