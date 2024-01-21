#pragma once

#include "W4Framework.h"
#include "W4JSON.h"
#include "JsonWidgetsParser.h"

W4_USE_UNSTRICT_INTERFACE

class LocalSettings
{
public:

    LocalSettings();
    ~LocalSettings();
    const float getFloat(const std::string&& options) const;
    const int getInt(const std::string&& options) const;
    const vec2 getVec2(const std::string&& options) const;
    const vec3 getVec3(const std::string&& options) const;
    const vec4 getVec4(const std::string&& options) const;
    const std::string getString(const std::string&& options) const;

private:
    std::map<std::string, float> m_floatSettings;
    std::map<std::string, int> m_intSettings;
    std::map<std::string, vec2> m_vec2Settings;
    std::map<std::string, vec3> m_vec3Settings;
    std::map<std::string, vec4> m_vec4Settings;
    std::map<std::string, std::string> m_StringSettings;

    const std::string m_configPath = "config/config.json";
};
