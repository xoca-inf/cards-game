#include "LocalSettings.h"

LocalSettings::LocalSettings()
{
    W4_LOG_DEBUG("Load settings");

    nlohmann::json json;

    W4_LOG_DEBUG("settings : %s", m_configPath.c_str());
    if (auto file = filesystem::open(m_configPath); file && file->good())
    {
        json = nlohmann::json::parse(file->data(), file->data() + file->size());
    }
    else
    {
        W4_LOG_ERROR("configuration file not found : %s", m_configPath.c_str());
    }

    W4_LOG_DEBUG("Load float settings");
    for (auto& value : json["settingsFloat"])
    {
        auto settings = nlohmann::getFloatFromJson(value);
        m_floatSettings.emplace(settings.name, settings.value);
        W4_LOG_DEBUG("Json float settings: {%s}, {%f}", settings.name.c_str(), settings.value);
    }

    W4_LOG_DEBUG("Load int settings");
    for (auto& value : json["settingsInt"])
    {
        auto settings = nlohmann::getIntFromJson(value);
        m_intSettings.emplace(settings.name, settings.value);
        W4_LOG_DEBUG("Json int settings: {%s}: {%d}", settings.name.c_str(), settings.value);
    }

    W4_LOG_DEBUG("Load string settings");
    for (auto& value : json["settingsString"])
    {
        auto settings = nlohmann::getStringFromJson(value);
        m_StringSettings.emplace(settings.name, settings.value);
        W4_LOG_DEBUG("Json string settings: {%s}: {%s}", settings.name.c_str(), settings.value.c_str());
    }

    W4_LOG_DEBUG("Load vec2 settings");
    for (auto& value : json["settingsVec2"])
    {
        auto settings = nlohmann::getVec2FromJson(value);
        m_vec2Settings.emplace(settings.name, settings.value);
        W4_LOG_DEBUG("Json vec2 settings: {%s}: {%f %f}", settings.name.c_str(), settings.value.x, settings.value.y);
    }

    W4_LOG_DEBUG("Load vec3 settings");
    for (auto& value : json["settingsVec3"])
    {
        auto settings = nlohmann::getVec3FromJson(value);
        m_vec3Settings.emplace(settings.name, settings.value);
        W4_LOG_DEBUG("Json vec3 settings: {%s}: {%f %f %f}", settings.name.c_str(), settings.value.x, settings.value.y, settings.value.z);
    }

    W4_LOG_DEBUG("Load vec4 settings");
    for (auto& value : json["settingsVec4"])
    {
        auto settings = nlohmann::getVec4FromJson(value);
        m_vec4Settings.emplace(settings.name, settings.value);
        W4_LOG_DEBUG("Json vec4 settings: {%s}: {%f %f %f %f}", settings.name.c_str(), settings.value.x, settings.value.y, settings.value.z, settings.value.w);
    }
}

LocalSettings::~LocalSettings()
{
    W4_LOG_DEBUG("~Settings");
}

const float LocalSettings::getFloat(const std::string&& settings) const
{
    float ret = 0.0f;
    auto it = m_floatSettings.find(settings);
    if (it == m_floatSettings.end())
    {
        W4_LOG_DEBUG("getFloat: no settings: {%s}", settings.c_str());
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

const int LocalSettings::getInt(const std::string&& settings) const
{
    int ret = 0;
    auto it = m_intSettings.find(settings);
    if (it == m_intSettings.end())
    {
        W4_LOG_DEBUG("getInt: no settings: {%s}", settings.c_str());
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

const vec2 LocalSettings::getVec2(const std::string&& settings) const
{
    vec2 ret = { 0.f, 0.f };
    auto it = m_vec2Settings.find(settings);
    if (it == m_vec2Settings.end())
    {
        W4_LOG_DEBUG("getVec2: no settings: {%s}", settings.c_str());
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

const vec3 LocalSettings::getVec3(const std::string&& settings) const
{
    vec3 ret = { 0.f, 0.f, 0.f };
    auto it = m_vec3Settings.find(settings);
    if (it == m_vec3Settings.end())
    {
        W4_LOG_DEBUG("getVec3: no settings: {%s}", settings.c_str());
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

const vec4 LocalSettings::getVec4(const std::string&& settings) const
{
    vec4 ret = { 0.f, 0.f, 0.f, 0.f };
    auto it = m_vec4Settings.find(settings);
    if (it == m_vec4Settings.end())
    {
        W4_LOG_DEBUG("getVec4: no settings: {%s}", settings.c_str());
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

const std::string LocalSettings::getString(const std::string&& settings) const
{
    std::string ret = "";
    auto it = m_StringSettings.find(settings);
    if (it == m_StringSettings.end())
    {
        W4_LOG_DEBUG("getString: no settings: {%s}", settings.c_str());
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

