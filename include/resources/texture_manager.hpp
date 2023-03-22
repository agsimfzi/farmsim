#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <map>

/// TEXTURE MANAGER ///
///
/// \brief singleton object that stores texture objects and provides references to them
///
class Texture_Manager {
public:

/// DEFAULT DESTRUCTOR ///
///
/// \brief deletes the instance
///
    ~Texture_Manager();

/// get ///
///
/// \brief returns a reference to the texture keyed to the passed string
///
    static sf::Texture& get(std::string key);

private:
    static std::map<std::string, sf::Texture> textures; /**< texture storage */

/// DEFAULT CONSTRUCTOR ///
///
/// \brief loads textures from the database. private to enforce singleton behavior
///
    Texture_Manager();

    static Texture_Manager* instance; /**< singleton instance */
};
