#include "Player.h"
#include "Room.h"

#include <raymath.h>

void InitializePlayer(Player& player){

    Vector2 playerSize = {35.0f, 45.0f};
    player.speed = 600.0f;
    player.health = 100.0f;
    player.iFrames = 0.0f;
    
    player.center = {0,0};
    player.position = {
        -player.center.x/2.0f, 
        -player.center.y/2.0f
    };
    player.hitBox = {
        player.position.x, 
        player.position.y, 
        playerSize.x, 
        playerSize.y
    };
    
    player.direction = {0, 0};

    player.canMove = true;
    player.dashing = false;
    player.tempDirection = {0, 0};
    player.facingDirection = 1;

    player.dashTime = 0.0f;
    player.dashCD = 0.0f;
    
    player.texture = LoadTexture("../assets/Player.png"); 
    
}

void UpdatePlayer(Player& player, Room rooms[], int& numRoomLocs, bool debugSpeed, bool debugWall){
    
    //------------------------------MOVEMENT------------------------------
        //Player movement
        float xDir = 0;
        float yDir = 0;
        
        if(debugSpeed){
            player.speed = 1800.0f;
        } else {
            player.speed = 600.0f;
        }
        
        if(player.canMove){
            if(IsKeyDown(KEY_D))
                xDir += 1.0f;
            
            if(IsKeyDown(KEY_A))
                xDir -= 1.0f;
            
            if(IsKeyDown(KEY_S))
                yDir += 1.0f;
            
            if(IsKeyDown(KEY_W))
                yDir -= 1.0f;
        }
        
        //Normalize
        player.direction = Vector2Normalize({xDir,yDir});
        
        //Scale
        player.direction = Vector2Scale(player.direction, player.speed);
        
        //Update facing player.direction
        if(player.direction.x > 0){
            player.facingDirection = 1;
        }
        else if(player.direction.x < 0){
            player.facingDirection = -1;
        }
        
        //Translate player, detect collision (x)
        player.position.x += player.direction.x * GetFrameTime();
        player.hitBox.x = player.position.x;
        if(!debugWall){
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == true){
                    for(int j = 0; j < rooms[i].numWalls; j++){
                        if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                            if (player.direction.x > 0){
                                player.position.x = rooms[i].walls[j].x - player.hitBox.width;
                            } else if (player.direction.x < 0){
                                player.position.x = rooms[i].walls[j].x + rooms[i].walls[j].width;
                            }
                            player.hitBox.x = player.position.x;
                        }
                    }
                }
            }
        }
        
        //Translate player, detect collision (y)
        player.position.y += player.direction.y * GetFrameTime();
        player.hitBox.y = player.position.y;
        if(!debugWall){
            for(int i = 0; i < numRoomLocs; i++){
                if(rooms[i].exists == true){
                    for(int j = 0; j < rooms[i].numWalls; j++){
                        if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                            if (player.direction.y > 0){
                                player.position.y = rooms[i].walls[j].y - player.hitBox.height;
                            } else if (player.direction.y < 0){
                                player.position.y = rooms[i].walls[j].y + rooms[i].walls[j].height;
                            }
                            player.hitBox.y = player.position.y;
                        }
                    }
                }
            }
        }
                
        //Dashing/roll
        if(IsKeyPressed(KEY_SPACE) && player.dashCD <= 0.0f && player.dashing == false && (player.direction.x != 0 || player.direction.y != 0)){
            player.dashing = true;
            player.canMove = false;
            player.tempDirection = Vector2Scale(player.direction, 2.0f);
            player.dashTime = 0.2f;
            player.iFrames = player.dashTime;
        }
        
        if(player.dashing){                        
            //Translate player, detect collision (x)
            player.position.x += player.tempDirection.x * GetFrameTime();
            player.hitBox.x = player.position.x;
            if(!debugWall){
                for(int i = 0; i < numRoomLocs; i++){
                    if(rooms[i].exists == true){
                        for(int j = 0; j < rooms[i].numWalls; j++){
                            if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                                if (player.tempDirection.x > 0){
                                    player.position.x = rooms[i].walls[j].x - player.hitBox.width;
                                } else if (player.tempDirection.x < 0){
                                    player.position.x = rooms[i].walls[j].x + rooms[i].walls[j].width;
                                }
                                player.hitBox.x = player.position.x;
                            }
                        }
                    }
                }
            }
            
            //Translate player, detect collision (y)
            player.position.y += player.tempDirection.y * GetFrameTime();
            player.hitBox.y = player.position.y;
            if(!debugWall){
                for(int i = 0; i < numRoomLocs; i++){
                    if(rooms[i].exists == true){
                        for(int j = 0; j < rooms[i].numWalls; j++){
                            if(CheckCollisionRecs(player.hitBox,rooms[i].walls[j])){
                                if (player.tempDirection.y > 0){
                                    player.position.y = rooms[i].walls[j].y - player.hitBox.height;
                                } else if (player.tempDirection.y < 0){
                                    player.position.y = rooms[i].walls[j].y + rooms[i].walls[j].height;
                                }
                                player.hitBox.y = player.position.y;
                            }
                        }
                    }
                }
            }
        
            player.dashTime -= GetFrameTime();
            if(player.dashTime <= 0){
                player.dashing = false;
                player.canMove = true;
                player.dashCD = 1.0f;
            }
        }
        
        if(player.dashCD > 0.0f){
            player.dashCD -= GetFrameTime();
        }
        
        
    //Update player center    
    player.center = {
        player.position.x + player.hitBox.width/2.0f, 
        player.position.y + player.hitBox.height/2.0f
    };
    
}

void DrawPlayer(const Player& player){
    
    int fontSize;
    int textWidth;
    
    Rectangle playerDest; 
    Rectangle playerRec = {0,0,(float)player.texture.width / 4.0f,(float)player.texture.height};
    Vector2 playerTextureSize = {80.0f, 80.0f};
    //Draw player
    
    DrawRectangleRec(player.hitBox, BLUE);  
    
    playerDest = {player.center.x - playerTextureSize.x/2.0f, player.center.y - playerTextureSize.y/2.0f, playerTextureSize.x, playerTextureSize.y};
    if(player.facingDirection == -1){
        playerRec = {0,0,(float)-player.texture.width / 4.0f,(float)player.texture.height};
    } else if(player.facingDirection == 1){
        playerRec = {0,0,(float)player.texture.width / 4.0f,(float)player.texture.height};
    }
    DrawTexturePro(player.texture, playerRec, playerDest, (Vector2){0, 0}, 0, WHITE);
    
    fontSize = 15;
    textWidth = MeasureText("Player", fontSize);
    DrawText(
        "Player", player.center.x - textWidth/2, 
        player.hitBox.y - fontSize*2.2f, 
        fontSize, 
        BLACK
    );   
    textWidth = MeasureText(TextFormat("%0.2f",player.health), fontSize);
    DrawText(
        TextFormat("%0.2f",player.health), player.center.x - textWidth/2, 
        player.hitBox.y - fontSize*1.1f, 
        fontSize, 
        BLACK
    );  
    
    
    
}
