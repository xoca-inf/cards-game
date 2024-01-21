#include "Settings.h"
#include "Config.h"

SettingsNode::SettingsNode(const nlohmann::json& root)
    : m_root(root)
{
    if (m_root.contains("int"))
        m_intData.init(m_root.at("int"));
    
    if (m_root.contains("float"))
        m_floatData.init(m_root.at("float"));
    
    if (m_root.contains("string"))
        m_stringData.init(m_root.at("string"));
    
    if (m_root.contains("vector2"))
        m_vec2Data.init(m_root.at("vector2"));
    
    if (m_root.contains("vector3"))
        m_vec3Data.init(m_root.at("vector3"));
    
    if (m_root.contains("vector4"))
        m_vec4Data.init(m_root.at("vector4"));
    
    if (m_root.contains("strings"))
        m_stringVecData.init(m_root.at("strings"));

    W4_LOG_DEBUG("[SETTINGS NODE] Loaded");
}

SettingsNode::~SettingsNode() {}

Settings::Settings()
{
    auto file = w4::filesystem::open("settings.json");
    m_json = nlohmann::json::parse(file->data(), file->data() + file->size());
    m_storage = std::make_unique<SettingsNode>(m_json);
    W4_LOG_DEBUG("[SETTINGS] Loaded");
}

Settings::~Settings() {}

OrientationSettings::OrientationSettings()
{
    m_portrait_json = Settings::instance().json()["portrait"];
    m_landscape_json = Settings::instance().json()["landscape"];
    m_portrait_storage = std::make_unique<SettingsNode>(m_portrait_json);
    m_landscape_storage = std::make_unique<SettingsNode>(m_landscape_json);
    W4_LOG_DEBUG("[SETTINGS] Loaded");
}

OrientationSettings::~OrientationSettings() {}

SettingsNode* OrientationSettings::get()     
{
    if (Config::getData().orientation == "portrait")
        return m_portrait_storage.get();
    else if (Config::getData().orientation == "landscape")
        return m_landscape_storage.get();
    else
        W4_LOG_ERROR("INVALID ORIENTATION IN CONFIG: %s", 
            Config::getData().orientation.c_str());
    
    return m_landscape_storage.get();
}