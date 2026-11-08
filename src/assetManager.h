#pragma once

#include "raylib.h"

#include <string>
#include <unordered_map>

using namespace std;

enum SpriteID{
    SmallWallTop,
    SmallWallBottom,
    SmallWallLeft,
    SmallWallRight,
    MediumWallTop,
    MediumWallBottom,
    MediumWallLeft,
    MediumWallRight,
    LargeWallTop,
    LargeWallBottom,
    LargeWallLeft,
    LargeWallRight,
    InnerCornerWallTopLeft,
    InnerCornerWallTopRight,
    InnerCornerWallBottomLeft,
    InnerCornerWallBottomRight,
    OuterCornerWallTopLeft,
    OuterCornerWallTopRight,
    OuterCornerWallBottomLeft,
    OuterCornerWallBottomRight
  
};

struct SpriteInfo {
    Rectangle textureSrc;
};

struct AssetManager {
    unordered_map<std::string, Texture2D> textures;
    unordered_map<std::string, Sound> sounds;
    
    SpriteInfo spriteInfo[30] = {};
    
    Vector2 CalcTileCoord(int row, int col, int borderSpacing, int spacing, int size);
    void SetSpriteInfo(SpriteID id, int col, int row, int borderSpacing, int spacing, int size);
    
    Texture2D LoadGameTexture(const char* path);
    Sound LoadGameSound(const char* path);
    
    void LoadSpriteInfo(); 

    void UnloadAll();
};

SpriteID GetSpriteID(const string& name);


extern AssetManager assets;