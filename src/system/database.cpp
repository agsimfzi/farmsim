#include <system/database.hpp>

#include <SFML/Audio/Sound.hpp>

#include <audio/sound_context.hpp>

#include <animation/animation.hpp>

#include <input/convert_action_trigger.hpp>

#include <world/season.hpp>

//////////////////////////////////////////////////////////////

sqlite3* Database::db = nullptr;
std::vector<char*> Database::font_buffers = std::vector<char*>();

int Database::rc = 0;

Database::~Database()
{
    for (unsigned int i = 0; i < font_buffers.size(); ++i) {
        delete[] font_buffers[i];
    }
}

void Database::saveSettings(Settings_Package p)
{
    open();

    for (const auto& v : p.volume) {
        std::string sql = "UPDATE VOLUME SET VALUE = '" + std::to_string(v.second) + "' WHERE ID = '" + volumeTypeToString(v.first) + "';";
        execute(sql);
    }

    close();
}

Settings_Package Database::getSettings()
{
    Settings_Package p;

    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'VOLUME'";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        std::string id(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        p.volume[stringToVolumeType(id)] = sqlite3_column_double(statement, 1);
    }

    sqlite3_finalize(statement);

    close();

    return p;
}

void Database::open()
{
    rc = sqlite3_open("data.db", &db);
}

void Database::execute(std::string& sql)
{
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, NULL);
}

void Database::close()
{
    rc = sqlite3_close(db);
}

int Database::callback(void* notUsed, int argc, char** argv, char** azColName)
{
    return 0;
}

void Database::getTextures(std::map<std::string, sf::Texture>& t)
{
    open();

    std::string sql = "SELECT * FROM 'TEXTURES';";

    sqlite3_stmt* statement;

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    int row = 0;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));

        sf::Texture texture;
        sqlite3_blob* blob;

        rc = sqlite3_blob_open(db, "main", "TEXTURES", "DATA", ++row, 0, &blob);

        int bsize = sqlite3_blob_bytes(blob);
        char* buffer = new char[bsize];

        rc = sqlite3_blob_read(blob, buffer, bsize, 0);

        texture.loadFromMemory(buffer, bsize);
        delete[] buffer;

        sqlite3_blob_close(blob);

        t[id] = texture;
    }
    rc = sqlite3_finalize(statement);

    close();
}

std::vector<Vehicle_Data> Database::getVehicles()
{
    std::vector<Vehicle_Data> data;

    open();

    std::string sql = "SELECT * FROM 'VEHICLES';";

    //execute(sql);

    sqlite3_stmt* statement;

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        Vehicle_Data d;
        int column = 0;

        //base data
        //name text
        //speed double
        d.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(d.name.begin(), d.name.end(), d.name.begin(), ::tolower);
        d.speed_factor = sqlite3_column_double(statement, column++);

        //size data
        //size_x int
        //size_y int
        int x = sqlite3_column_int(statement, column++);
        int y = sqlite3_column_int(statement, column++);
        d.size = sf::Vector2i(x, y);

        //animation counts
        //idle_count int
        //moving_count int
        d.counts[Vehicle_State::IDLE] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
        d.counts[Vehicle_State::MOVING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));

        //animation thresholds
        //idle_threshold int
        //moving_threshold int
        d.thresholds[Vehicle_State::IDLE] = sqlite3_column_int(statement, column++);
        d.thresholds[Vehicle_State::MOVING] = sqlite3_column_int(statement, column++);

        data.push_back(d);
    }

    return data;
}

Entity_Data Database::getPlayerData()
{
    Entity_Data p;

    open();

    std::string sql = "SELECT * FROM 'ENTITIES' WHERE NAME = 'PLAYER';";

    execute(sql);

    sqlite3_stmt* statement;

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        p = readEntity(statement);
    }

    sqlite3_finalize(statement);

    close();

    return p;
}

Entity_Data Database::readEntity(sqlite3_stmt* statement)
{
    Entity_Data d;
    int column = 0;

    //base data
    //name text
    //description text
    //speed double
    d.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
    std::transform(d.name.begin(), d.name.end(), d.name.begin(), ::tolower);
    d.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
    d.speed = sqlite3_column_double(statement, column++);

    //size data
    //size_x int
    //size_y int
    int x = sqlite3_column_int(statement, column++);
    int y = sqlite3_column_int(statement, column++);
    d.size = sf::Vector2i(x, y);

    //animation counts
    //idle_count int
    //moving_count int
    // boat count int
    // broom count int
    d.animation_count[Entity_State::IDLE] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.animation_count[Entity_State::MOVING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.animation_count[Entity_State::BOATING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.animation_count[Entity_State::BROOMING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));


    //animation thresholds
    //idle_threshold int
    //moving_threshold int
    // boat threshold int
    // broom threshold int
    d.animation_threshold[Entity_State::IDLE] = sqlite3_column_int(statement, column++);
    d.animation_threshold[Entity_State::MOVING] = sqlite3_column_int(statement, column++);
    d.animation_threshold[Entity_State::BOATING] = sqlite3_column_int(statement, column++);
    d.animation_threshold[Entity_State::BROOMING] = sqlite3_column_int(statement, column++);

    //bounds y offset
    //bounds x size
    //bounds y size
    d.bounds_y_offset = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.bounds_size.x = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.bounds_size.y = static_cast<unsigned int>(sqlite3_column_int(statement, column++));

    return d;
}

void Database::getFonts(std::map<Font, sf::Font>& f)
{
    open();

    std::string sql = "SELECT * FROM 'FONTS';";

    sqlite3_stmt* statement;

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    int row = 0;

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        int column = 0;
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column));

        sqlite3_blob* blob;

        rc = sqlite3_blob_open(db, "main", "FONTS", "DATA", ++row, 0, &blob);
        errorCheck(std::string("opening font blob " + name));

        int bsize = sqlite3_blob_bytes(blob);
        font_buffers.push_back(new char[bsize]);

        rc = sqlite3_blob_read(blob, font_buffers.back(), bsize, 0);
        errorCheck(std::string("\treading font blob " + name));

        f[stringToFont(name)].loadFromMemory(font_buffers.back(), bsize);

        rc = sqlite3_blob_close(blob);
    }

    sqlite3_finalize(statement);

    close();
}

std::vector<Item_Data> Database::getItemPrototypes()
{
    std::vector<Item_Data> items;

    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'ITEMS'";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        int column = 0;

        // uid
        // name
        // type
        // subtype
        // description
        // value
        // max_stack
        // use_factor

        Item_Data d;

        d.uid = sqlite3_column_int(statement, column++);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        d.name = name;
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        d.type = stringToItemType(type);
        d.subtype = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        d.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        d.value = sqlite3_column_int(statement, column++);
        d.stack_size = sqlite3_column_int(statement, column++);
        d.use_factor = sqlite3_column_int(statement, column++);

        items.push_back(d);
    }

    close();

    return items;
}

std::map<size_t, Building_Animation_Data> Database::getBuildingAnimationData()
{
    std::map<size_t, Building_Animation_Data> data;

    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'BUILDINGS';";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        Building_Animation_Data d;
        int column = 0;

        d.uid = sqlite3_column_int(statement, column++);

    //animation counts
        // idle_count int
        // starting_count int
        // running_count int
        // ending_count int
        d.counts[Building_State::IDLE] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
        d.counts[Building_State::STARTING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
        d.counts[Building_State::RUNNING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
        d.counts[Building_State::ENDING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));


    //animation thresholds
        // idle_threshold int
        // starting_threshold int
        // running_thresholdint
        // ending_threshold int
        d.thresholds[Building_State::IDLE] = sqlite3_column_int(statement, column++);
        d.thresholds[Building_State::STARTING] = sqlite3_column_int(statement, column++);
        d.thresholds[Building_State::RUNNING] = sqlite3_column_int(statement, column++);
        d.thresholds[Building_State::ENDING] = sqlite3_column_int(statement, column++);

    // start data
        // start_x int
        // start_y int
        int x = sqlite3_column_int(statement, column++);
        int y = sqlite3_column_int(statement, column++);
        d.start = sf::Vector2i(x, y);

    // texture key
        d.tkey = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));

        data[d.uid] = d;
    }

    close();

    return data;
}

std::vector<Crop_Data> Database::getCropPrototypes()
{
    std::vector<Crop_Data> crops;

    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'CROPS'";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        int column = 0;

        // uid
        // name
        // growth_coef
        // water_factor
        // stage_count
        // y_size
        // seasons
        // regrows
        // quantity_min
        // quantity_max

        Crop_Data c;

        c.uid = sqlite3_column_int(statement, column++);
        c.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(c.name.begin(), c.name.end(), c.name.begin(), ::tolower);
        c.growth_coef = sqlite3_column_double(statement, column++);
        c.water_factor = sqlite3_column_double(statement, column++);
        c.stage_count = sqlite3_column_int(statement, column++);
        c.y_size = sqlite3_column_int(statement, column++);
        std::string seasons = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        c.m_regrows = sqlite3_column_int(statement, column++);
        c.quantity_min = sqlite3_column_int(statement, column++);
        c.quantity_max = sqlite3_column_int(statement, column++);
        c.m_passable = sqlite3_column_int(statement, column++);

        for (int i = 0; i < 4; i++) {
            Season s = static_cast<Season>(i);
            c.seasons[s] = (seasons.find(seasonToString(s)) != std::string::npos);

        }

        crops.push_back(c);
    }

    close();

    return crops;
}

std::vector<Detail_Data> Database::getDetails()
{
    std::vector<Detail_Data> data;

    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'DETAILS'";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        int column = 0;

        Detail_Data d;

        // name
        // type
        // product
        // quantity_min
        // quantity_max
        // texture_size_x
        // texture_size_y
        // texture_pos_x
        // texture_pos_y
        // health

        d.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(d.name.begin(), d.name.end(), d.name.begin(), ::tolower);
        d.type_string = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        d.product = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(d.product.begin(), d.product.end(), d.product.begin(), ::tolower);
        d.quantity_min = sqlite3_column_int(statement, column++);
        d.quantity_max = sqlite3_column_int(statement, column++);
        d.texture_rect.width = sqlite3_column_int(statement, column++);
        d.texture_rect.height = sqlite3_column_int(statement, column++);
        d.texture_rect.left = sqlite3_column_int(statement, column++);
        d.texture_rect.top = sqlite3_column_int(statement, column++);
        d.max_health = sqlite3_column_int(statement, column++);
        d.health = d.max_health;

        data.push_back(d);
    }

    return data;
}

std::map<Machine_Type, std::vector<Reaction>> Database::getReactions()
{
    std::map<Machine_Type, std::vector<Reaction>> reactions;

    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'REACTIONS'";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        int column = 0;

        std::string name;
        std::string reagant_string;
        std::string product;
        int length;
        std::string location;
        std::string tag;

        name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        reagant_string = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(reagant_string.begin(), reagant_string.end(), reagant_string.begin(), ::tolower);
        product = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(product.begin(), product.end(), product.begin(), ::tolower);
        length = sqlite3_column_int(statement, column++);
        location = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(location.begin(), location.end(), location.begin(), ::tolower);
        tag = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));

        Machine_Type loc_type = stringToMachineType(location);

        std::vector<Reagant> reagants;

        while (reagant_string.find(',') != std::string::npos) {
            std::string r = reagant_string.substr(0, reagant_string.find(','));
            reagants.push_back(Reagant());
            reagants.back().name = r;
            reagant_string = reagant_string.substr(reagant_string.find(',') + 1);
        }
        reagants.push_back(Reagant());
        reagants.back().name = reagant_string;

        Reaction r;
            r.name = name;
            r.reagants = reagants;
            r.product = product;
            r.length = length;
            r.tag = tag;

        reactions[loc_type].push_back(r);
    }

    close();

    return reactions;
}

std::map<Crafting_Type, std::vector<Reaction>> Database::getRecipes()
{
    std::map<Crafting_Type, std::vector<Reaction>> recipes;

    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'RECIPES'";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        int column = 0;

        std::string name;
        std::string reagant_string;
        std::string product;
        std::string location;
        std::string tag;

        name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        reagant_string = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(reagant_string.begin(), reagant_string.end(), reagant_string.begin(), ::tolower);
        product = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(product.begin(), product.end(), product.begin(), ::tolower);
        location = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::transform(location.begin(), location.end(), location.begin(), ::tolower);
        tag = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));

        Crafting_Type loc_type = stringToCraftingType(location);

        std::vector<Reagant> reagants;

        while (reagant_string.find(';') != std::string::npos) {
            std::string r = reagant_string.substr(0, reagant_string.find(';'));
            reagant_string = reagant_string.substr(reagant_string.find(';') + 1);
            std::string reagant = r.substr(0, r.find(','));
            std::string count = r.substr(r.find(',') + 1, r.find(';'));
            reagants.push_back(reagant);
            reagants.back().count = std::stoi(count);
        }

        Reaction r;
            r.name = name;
            r.reagants = reagants;
            r.product = product;
            r.tag = tag;

        recipes[loc_type].push_back(r);
    }

    close();

    return recipes;
}

void Database::errorCheck(std::string id)
{
    //std::cout << id + ": " + sqlite3_errmsg(db) << '\n';
    //log error message here
}

std::map<std::string, Action_Trigger> Database::getActions()
{
    open();

    sqlite3_stmt* statement;

    std::string sql = "SELECT * FROM 'COMMANDS'";

    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, NULL);

    std::map<std::string, Action_Trigger> actions;

    Convert_Action_Trigger converter;

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        unsigned int column = 0;
        std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, column++)));
        std::string key = std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, column++)));

        Action_Trigger t = converter.toKey(key);

        actions[name] = t;
    }

    sqlite3_finalize(statement);

    close();

    return actions;
}

void Database::saveActions(std::vector<Action> actions)
{
    open();

    std::string sql = "DELETE FROM COMMANDS";

    execute(sql);

    sql = "VACUUM COMMANDS";

    execute(sql);

    Convert_Action_Trigger converter;

    for (const auto& action : actions) {
        sql = "INSERT INTO COMMANDS(NAME, KEY) VALUES(\"" + action.name + "\", \"" + converter.toString(action.trigger) + "\");";
        execute(sql);
    }

    close();
}

void Database::loadSounds(std::map<Sound_Game, Sound_Context>& game_sounds,
    std::map<Sound_UI, Sound_Context>& ui_sounds)
{
    open();

    std::string sql = "SELECT * FROM 'SOUNDS';";

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, NULL);
    if (rc == SQLITE_OK) {
        int row = 0;
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int column = 0;
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, column++));
            std::string tag = reinterpret_cast<const char*>(sqlite3_column_text(stmt, column++));
            float threshold = sqlite3_column_double(stmt, column++);

            sqlite3_blob* blob;
            rc = sqlite3_blob_open(db, "main", "SOUNDS", "DATA", ++row, 0, &blob);
            int bsize = sqlite3_blob_bytes(blob);
            char* buffer = new char[bsize];
            rc = sqlite3_blob_read(blob, buffer, bsize, 0);
            sf::SoundBuffer sound;
            sound.loadFromMemory(buffer, bsize);
            delete[] buffer;
            sqlite3_blob_close(blob);

            Sound_Context context(sound, threshold);

            if (tag == "GAME") {
                game_sounds[stringToGameSound(name)] = context;
            }
            else if (tag == "UI") {
                ui_sounds[stringToUISound(name)] = context;
            }
        }
    }

    sqlite3_finalize(stmt);

    close();
}
