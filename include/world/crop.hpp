#pragma once

#include <SFML/Graphics.hpp>

#include "crop_data.hpp"

/// CROP ///
///
/// \brief class representing a crop grown by the player
///
class Crop : public sf::Drawable, private Crop_Data {
public:
    Crop() = default;

/// FULL CONSTRUCTOR ///
///
/// \brief constructs the crop of the type in the passed Crop_Data
///
    Crop(Crop_Data d);

/// setSprite ///
///
/// \brief copies the passed sprite into the class
///
    void setSprite(sf::Sprite sprite);

/// getSprite ///
///
/// \brief returns a reference to the class sprite
///
    sf::Sprite& getSprite();

/// tick ///
///
/// \brief advances crop growth if appropriate
///
    void tick(bool watered);

/// getCoordinates ///
///
/// \brief returns the crops world tile coordinates
///
    sf::Vector2i getCoordinates();

/// fullyGrown ///
///
/// \brief returns true if the plant is in its final stage
///
    bool fullyGrown();

/// getUID ///
///
/// \brief returns the crop's UID (which matches the seed)
///
    size_t getUID();

/// harvestUID ///
///
/// \brief returns the UID of the plant harvested from this crop (this->UID + 1000)
///
    size_t harvestUID();

/// place ///
///
/// \brief sets the crop's coordinates and position
///
    void place(sf::Vector2i coordinates, sf::Vector2f pos);

/// checkSeason ///
///
/// \brief returns true if the crop can grow during the passed season
///
    bool checkSeason(Season s);

/// kill ///
///
/// \brief puts the plant in a state where it will not grow and can never be harvested again
///
    void kill();

/// dead ///
///
/// \brief returns true if the plant has been killed
///
    bool dead();

    std::function<void(sf::Vector2i)> unwater; /**< callback which converts the crop's tile from WATERED to TILLED */

/// getQuantity ///
///
/// \brief generates a random quantity, based on the min and max
///
    unsigned int getQuantity();

/// harvestRegrowable ///
///
/// \brief decrements stage to the penultimate
///
    void harvestRegrowable();

/// passable ///
///
/// \brief returns true if the crop does not have entity-blocking world collision
///
    bool passable();

/// regrows ///
///
/// \brief returns true if the crop continues to produce after harvesting
///
    bool regrows();

private:
    sf::Sprite sprite; /**< the crop's sprite */

    float growth = 0.f; /**< current stage growth */
    const static float stage_threshold; /**< growth threshold for advancing the stage */

    sf::Vector2i coordinates; /**< world tile coordinates */

    size_t stage = 1; /**< current growth stage */

/// nextStage ///
///
/// \brief increments stage by one and adjusts the textureRect
///
    void nextStage();

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
