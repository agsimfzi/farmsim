#pragma once

#include "detail_data.hpp"
#include "season.hpp"

/// DETAIL ///
///
/// \brief information for world objects such as rocks and trees
///
class Detail : private Detail_Data {
public:
    Detail() = default;

/// FULL CONSTRUCTOR ///
///
    Detail(sf::Vector2i coordinates, Detail_Data data);

    sf::Vector2i getCoordinates;

    bool dead();
    void hit(int dmg);
    void resetHealth();

    std::string getName();
    std::string getProduct();
    unsigned int quantity();

    enum Type {
        TREE,
        ROCK,
        NULL_TYPE
    };

    Detail::Type getType();

    sf::IntRect getTextureRect();

    void setSeasonalTextureRect(Season s);

    static Detail::Type stringToType(std::string s);
    static std::string typeToString(Detail::Type t);

    std::string getSubtype();

    sf::Vector2i coordinates;

private:

    Type type;
};
