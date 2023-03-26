#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <animation/animation.hpp>

#include <audio/sound.hpp>
#include <audio/sound_context.hpp>

#include <entity/entity_data.hpp>
#include <entity/entity_state.hpp>

#include <input/action.hpp>

#include <item/item_data.hpp>
#include <item/reaction.hpp>

#include <resources/font.hpp>

#include <sqlite3.h>

#include <world/building.hpp>
#include <world/building_data.hpp>
#include <world/crafting.hpp>
#include <world/machine.hpp>
#include <world/crop_data.hpp>
#include <world/detail.hpp>
#include <world/vehicle_data.hpp>

#include "settings_package.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Database {
public:
    ~Database();

    static void saveSettings(Settings_Package p);
    static Settings_Package getSettings();

    static std::map<size_t, Building_Animation_Data> getBuildingAnimationData();

    static void getTextures(std::map<std::string, sf::Texture>& t);

    static std::vector<Vehicle_Data> getVehicles();

    static Entity_Data getPlayerData();

    static void getFonts(std::map<Font, sf::Font>& f);

    static std::vector<Item_Data> getItemPrototypes();

    static std::vector<Crop_Data> getCropPrototypes();

    static std::vector<Detail_Data> getDetails();

    static std::map<Machine_Type, std::vector<Reaction>> getReactions();

    static std::map<Crafting_Type, std::vector<Reaction>> getRecipes();

    static std::map<std::string, Action_Trigger> getActions();

    static void saveActions(std::vector<Action> actions);

    static void loadSounds(std::map<Sound_Game, Sound_Context>& game_sounds,
        std::map<Sound_UI, Sound_Context>& ui_sounds);

private:
    static int callback(void* notUsed, int argc, char** argv, char** azColName);

    static sqlite3* db;

    static void open();
    static void execute(std::string& sql);
    static void close();

    static int rc;

    static void errorCheck(std::string id);

    static std::vector<char*> font_buffers;

    static Entity_Data readEntity(sqlite3_stmt* statement);
};
