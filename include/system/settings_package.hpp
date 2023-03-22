#pragma once

#include <map>

#include <audio/volume_type.hpp>

/// SETTINGS PACKAGE ///
///
/// \brief POD for saving and loading settings
///
struct Settings_Package {
    std::map<Volume_Type, float> volume;
};
