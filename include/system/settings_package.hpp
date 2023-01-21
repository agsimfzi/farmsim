#pragma once

#include <map>

#include <audio/volume_type.hpp>

/////////////////////////////////////////////////////////////
/// \brief
///
struct Settings_Package {
    std::map<Volume_Type, float> volume;
};
