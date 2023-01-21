#include <resources/texture_manager.hpp>

#include <system/database.hpp>

//////////////////////////////////////////////////////////////

std::map<std::string, sf::Texture> Texture_Manager::textures = std::map<std::string, sf::Texture>();

Texture_Manager* Texture_Manager::instance = nullptr;

Texture_Manager::Texture_Manager()
{
    Database::getTextures(textures);
}

Texture_Manager::~Texture_Manager()
{
    delete instance;
}

sf::Texture& Texture_Manager::get(std::string key)
{
    if (instance == nullptr) {
        instance = new Texture_Manager();
    }

    if (textures.count(key)) {
        return textures[key];
    }

    static sf::Texture blank;
    return blank;
}
