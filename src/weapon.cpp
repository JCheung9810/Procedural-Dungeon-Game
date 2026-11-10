#include "weapon.h"

#include "player.h"
#include "assetManager.h"

#include "json.hpp"

#include <fstream>
#include <cstring>
#include <string>

using json = nlohmann::json;
using namespace std;

Weapon LoadWeapon(const char* filePath, const char* weaponName){
    Weapon weapon{};

    TraceLog(LOG_INFO, "Loading weapon: %s", weaponName);

    ifstream file(filePath);

    if(!file.is_open()) {
        TraceLog(LOG_ERROR, "Failed to open weapon config: %s", filePath);
        return weapon;
    }

    try {
        json data;
        file >> data;

        for (const auto& item : data.at("weapons")) {
            if (item.at("name").get<string>() == weaponName) {
                string name = item.at("name").get<string>();
                string type = item.at("type").get<string>();
                string texturePath = item.at("texturePath").get<string>();
                string hudTexturePath = item.at("hudTexturePath").get<string>();
                string fireMode = item.at("fireMode").get<string>();
                string shotSoundPath = item.at("shotSoundPath").get<string>();
                string reloadSoundPath = item.at("reloadSoundPath").get<string>();

                weapon.name = strdup(name.c_str());
                weapon.type = strdup(type.c_str());
                weapon.texturePath = strdup(texturePath.c_str());
                weapon.hudTexturePath = strdup(hudTexturePath.c_str());
                weapon.shotSoundPath = strdup(shotSoundPath.c_str());
                weapon.reloadSoundPath = strdup(reloadSoundPath.c_str());

                weapon.textureFrames = item.at("textureFrames").get<int>();
                weapon.size = item.at("size").get<float>();
                
                vector<float> posOffset = item.at("positionOffset").get<vector<float>>();
                weapon.positionOffset = {posOffset[0], posOffset[1]};
                
                weapon.projectileOffset = item.at("projectileOffset").get<float>();
                weapon.projectileSize = item.at("projectileSize").get<float>();
                weapon.reloadTime = item.at("reloadTime").get<float>();
                weapon.magSize = item.at("magSize").get<int>();
                weapon.fireTime = item.at("fireTime").get<float>();
                weapon.bulletSpread = item.at("bulletSpread").get<float>();
                weapon.numProjectiles = item.at("numProjectiles").get<int>();
                weapon.damage = item.at("damage").get<float>();

                weapon.texture = assets.LoadGameTexture(weapon.texturePath);
                weapon.hudTexture = assets.LoadGameTexture(weapon.hudTexturePath);

                weapon.shotSound = assets.LoadGameSound(weapon.shotSoundPath);
                weapon.reloadSound = assets.LoadGameSound(weapon.reloadSoundPath);
                
                weapon.fireTimer = 0.0f;
                weapon.firerate = 1.0f / weapon.fireTime;
                weapon.mag = weapon.magSize;
                
                if(fireMode == "SINGLE"){
                    weapon.fireMode = FireMode::SINGLE;
                } else if(fireMode == "AUTOMATIC"){
                    weapon.fireMode = FireMode::AUTOMATIC;
                } else {
                    TraceLog(LOG_ERROR, "Unknown fire mode: %s", fireMode.c_str());
                }

                if (weapon.texture.id == 0) {
                    TraceLog(LOG_ERROR, "Failed to load weapon texture: %s", weapon.texturePath);
                }
                else {
                    TraceLog(LOG_INFO, "Loaded weapon: %s (Type: %s)", weapon.name, weapon.type);
                }

                return weapon;
            }
        }

        TraceLog(LOG_WARNING, "Weapon not found: %s", weaponName);
    }
    catch (const exception& e)
    {
        TraceLog(LOG_ERROR, "Error reading weapon JSON: %s", e.what());
    }

    return weapon;
}

void ReloadWeapon(Player& player){
    if(player.weapons[player.equippedWeaponIndex].reloading == false && player.weapons[player.equippedWeaponIndex].mag < player.weapons[player.equippedWeaponIndex].magSize){
        player.weapons[player.equippedWeaponIndex].reloading = true;
        player.weapons[player.equippedWeaponIndex].reloadTimer = player.weapons[player.equippedWeaponIndex].reloadTime;
        PlaySound(player.weapons[player.equippedWeaponIndex].reloadSound);
    }
}

void UpdateWeapon(Player& player){
    //Update reload and fire times
    for(int i = 0; i < (int)player.weapons.size(); i++){
        if(player.weapons[i].reloading == true){
            player.weapons[i].reloadTimer -= GetFrameTime();
            if(player.weapons[i].reloadTimer <= 0.0f){
                player.weapons[i].reloadTimer = 0.0f;
                player.weapons[i].reloading = false;
                player.weapons[i].mag = player.weapons[i].magSize;
            }
        }
        
        if(player.weapons[i].fireTimer > 0.0f){
            player.weapons[i].fireTimer -= GetFrameTime();
            if(player.weapons[i].fireTimer < 0.0f){
                player.weapons[i].fireTimer = 0.0f;
            }
        
        }
    }
}