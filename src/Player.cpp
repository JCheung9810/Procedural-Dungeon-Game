#include "Player.h"

void InitializePlayer(Player& player){

    Vector2 playerSize = {35.0f, 45.0f};
    player.speed = 600.0f;
    player.health = 100.0f;
    player.iFrames = 0.0f;
    
    player.center = {0,0};
    player.position = {-player.center.x/2.0f, -player.center.y/2.0f};
    player.hitBox = {player.position.x, player.position.y, playerSize.x, playerSize.y};
    
}