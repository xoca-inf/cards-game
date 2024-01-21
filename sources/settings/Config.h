#pragma once

#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

struct ConfigData{
    std::string orientation = "";
    w4::math::size guiResolution = w4::math::size(1, 1);
    w4::math::size aspect = w4::math::size(1, 1);
};

class Config
{
public:
    static void reload();
    static const ConfigData& getData();

private:
    static ConfigData m_data;
};
