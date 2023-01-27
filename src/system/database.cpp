#include <system/database.hpp>

#include <SFML/Audio/Sound.hpp>

#include <animation/animation.hpp>

#include <input/convert_action_trigger.hpp>

#include <audio/sound_context.hpp>

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

std::map<Entity_State, Animation> Database::getAnimations()
{
    open();

    std::string sql = "SELECT * FROM 'ANIMATIONS';";

    close();

    return std::map<Entity_State, Animation>();
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
    d.aCount[Entity_State::IDLE] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.aCount[Entity_State::MOVING] = static_cast<unsigned int>(sqlite3_column_int(statement, column++));

    //animation thresholds
    //idle_threshold int
    //moving_threshold int
    d.aThreshold[Entity_State::IDLE] = sqlite3_column_int(statement, column++);
    d.aThreshold[Entity_State::MOVING] = sqlite3_column_int(statement, column++);

    //bounds y offset
    //bounds x size
    //bounds y size
    d.bounds_y_offset = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.bounds_x_size = static_cast<unsigned int>(sqlite3_column_int(statement, column++));
    d.bounds_y_size = static_cast<unsigned int>(sqlite3_column_int(statement, column++));

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

        Item_Data d;

        d.uid = sqlite3_column_int(statement, column++);
        d.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        d.type = stringToItemType(type);
        d.subtype = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        d.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        d.value = sqlite3_column_int(statement, column++);
        d.stack_size = sqlite3_column_int(statement, column++);

        items.push_back(d);
    }

    close();

    return items;
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

        Crop_Data c;

        c.uid = sqlite3_column_int(statement, column++);
        c.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, column++));
        c.growth_coef = sqlite3_column_double(statement, column++);
        c.water_factor = sqlite3_column_double(statement, column++);
        c.stage_count = sqlite3_column_int(statement, column++);
        c.y_size = sqlite3_column_int(statement, column++);

        crops.push_back(c);
    }

    close();

    return crops;
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
