#pragma once

#include "raylib.h"

struct AssetManager {
    // Load textures
    Texture2D LoadGameTexture(const char* path);

    // Load sounds
    Sound LoadGameSound(const char* path);

    // Unload all assets
    void UnloadAll();
};