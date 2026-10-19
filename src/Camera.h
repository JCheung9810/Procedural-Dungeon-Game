#ifndef CAMERA_H
#define CAMERA_H

#include "Room.h"
#include "Player.h"

#include "raylib.h"

void InitializeCamera(Camera2D& camera, Player player, int screenHeight, int screenWidth);
void UpdateGameCamera(Room rooms[], int numRoomLocs, Player player, Camera2D& camera, bool debugCam);

#endif