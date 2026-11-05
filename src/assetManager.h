#pragma once

#include "raylib.h"

#include <string>
#include <unordered_map>

using namespace std;

struct AssetManager {
    unordered_map<std::string, Texture2D> textures;
    unordered_map<std::string, Sound> sounds;
    
    Texture2D LoadGameTexture(const char* path);
    Sound LoadGameSound(const char* path);

    void UnloadAll();
};