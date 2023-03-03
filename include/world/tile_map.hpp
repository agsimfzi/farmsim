#pragma once

#include <SFML/System/Vector2.hpp>

namespace std {
  template<>
  struct hash<sf::Vector2i> {
    size_t operator()(const sf::Vector2i& v) const {
      return (v.x << 16) ^ v.y;
    }
  };
}


template <class T>
using Tile_Map = std::unordered_map<sf::Vector2i, T>;
