#include "weapon.h"

#include "player.h"

#include "json.hpp"

#include <fstream>
#include <cstring>
#include <string>

using json = nlohmann::json;

Weapon LoadWeapon(const char* filePath, const char* weaponName){
    Weapon weapon{};

    TraceLog(LOG_INFO, "Loading weapon: %s", weaponName);

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        TraceLog(LOG_ERROR, "Failed to open weapon config: %s", filePath);
        return weapon;
    }

    try
    {
        json data;
        file >> data;

        for (const auto& item : data.at("weapons"))
        {
            if (item.at("name").get<std::string>() == weaponName)
            {
                std::string name = item.at("name").get<std::string>();
                std::string type = item.at("type").get<std::string>();
                std::string texturePath = item.at("texturePath").get<std::string>();

                weapon.name = strdup(name.c_str());
                weapon.type = strdup(type.c_str());
                weapon.texturePath = strdup(texturePath.c_str());

                weapon.textureFrames = item.at("textureFrames").get<int>();
                weapon.size = item.at("size").get<float>();
                
                std::vector<float> posOffset = item.at("positionOffset").get<std::vector<float>>();
                weapon.positionOffset = {posOffset[0], posOffset[1]};
                
                weapon.projectileOffset = item.at("projectileOffset").get<float>();
                weapon.projectileSize = item.at("projectileSize").get<float>();
                weapon.reloadTime = item.at("reloadTime").get<float>();
                weapon.magSize = item.at("magSize").get<int>();
                weapon.fireTime = item.at("fireTime").get<float>();
                weapon.bulletSpread = item.at("bulletSpread").get<float>();
                weapon.damage = item.at("damage").get<float>();

                weapon.texture = LoadTexture(weapon.texturePath);
                
                weapon.fireTimer = 0.0f;
                weapon.firerate = 1.0f / weapon.fireTime;
                weapon.mag = weapon.magSize;

                if (weapon.texture.id == 0)
                {
                    TraceLog(LOG_ERROR, "Failed to load weapon texture: %s", weapon.texturePath);
                }
                else
                {
                    TraceLog(LOG_INFO, "Loaded weapon: %s (Type: %s)", weapon.name, weapon.type);
                }

                return weapon;
            }
        }

        TraceLog(LOG_WARNING, "Weapon not found: %s", weaponName);
    }
    catch (const std::exception& e)
    {
        TraceLog(LOG_ERROR, "Error reading weapon JSON: %s", e.what());
    }

    return weapon;
}

void ReloadWeapon(Player& player){
    player.weapon[player.equippedWeaponIndex].reloading = true;
    player.weapon[player.equippedWeaponIndex].reloadTimer = player.weapon[player.equippedWeaponIndex].reloadTime;
}

void UpdateWeapon(Player& player){
    if(player.weapon[player.equippedWeaponIndex].reloading == true){
        player.weapon[player.equippedWeaponIndex].reloadTimer -= GetFrameTime();
        if(player.weapon[player.equippedWeaponIndex].reloadTimer <= 0.0f){
            player.weapon[player.equippedWeaponIndex].reloadTimer = 0.0f;
            player.weapon[player.equippedWeaponIndex].reloading = false;
            player.weapon[player.equippedWeaponIndex].mag = player.weapon[player.equippedWeaponIndex].magSize;
        }
    }
    if(player.weapon[player.equippedWeaponIndex].fireTimer > 0.0f){
        player.weapon[player.equippedWeaponIndex].fireTimer -= GetFrameTime();
        if(player.weapon[player.equippedWeaponIndex].fireTimer < 0.0f){
            player.weapon[player.equippedWeaponIndex].fireTimer = 0.0f;
        }
        
    }
}