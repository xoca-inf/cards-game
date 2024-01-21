#pragma once

#include "W4Framework.h"
#include "SettingsContainer.h"
#include "json.hpp"
//C:\W4SDK\sdk\include\external\json.hpp

#include <memory>

W4_USE_UNSTRICT_INTERFACE

class SettingsNode {
public:
    SettingsNode(const nlohmann::json& root);
    
    virtual ~SettingsNode();
    
    nlohmann::json& json()     
    {
        return m_root;
    }

    int getInt(const std::string& key)     
    {
        return m_intData[key];
    }

    bool hasInt(const std::string& key) const 
    {
        return m_intData.has(key);
    }

    float getFloat(const std::string& key)   
    {
        return m_floatData[key];
    }

    bool hasFloat(const std::string& key) const 
    {
        return m_floatData.has(key);
    }

    std::vector<std::string> getStringVec(const std::string& key)  
    {
        return m_stringVecData[key];
    }

    bool hasStringVec(const std::string& key) const 
    {
        return m_stringVecData.has(key);
    }

    std::string getString(const std::string& key)  
    {
        return m_stringData[key];
    }

    bool hasString(const std::string& key) const 
    {
        return m_stringData.has(key);
    }

    vec2 getVec2(const std::string& key)    
    {
        return m_vec2Data[key];
    }

    bool hasVec2(const std::string& key) const 
    {
        return m_vec2Data.has(key);
    }

    vec3 getVec3(const std::string& key)    
    {
        return m_vec3Data[key];
    }

    bool hasVec3(const std::string& key) const 
    {
        return m_vec3Data.has(key);
    }

    vec4 getVec4(const std::string& key)    
    {
        return m_vec4Data[key];
    }

    bool hasVec4(const std::string& key) const 
    {
        return m_vec4Data.has(key);
    }

private:
    nlohmann::json m_root;
    SettingsContainer<int>          m_intData;
    SettingsContainer<float>        m_floatData;
    SettingsContainer<std::string>  m_stringData;
    SettingsContainer<std::vector<std::string>>  m_stringVecData;
    SettingsContainer<vec2>         m_vec2Data;
    SettingsContainer<vec3>         m_vec3Data;
    SettingsContainer<vec4>         m_vec4Data;
};

class Settings {
private:
    Settings();

public:
    static Settings& instance() {
        static Settings instance;
        return instance;
    }
    
    virtual ~Settings();

    nlohmann::json& json()     
    {
        return m_json;
    }

    SettingsNode* get()     
    {
        return m_storage.get();
    }

    SettingsNode* operator->()     
    {
        return m_storage.get();
    }

private:
    nlohmann::json m_json;
    std::unique_ptr<SettingsNode> m_storage;
};

class OrientationSettings {
private:
    OrientationSettings();
public:
    static OrientationSettings& instance() {
        static OrientationSettings instance;
        return instance;
    }
    
    virtual ~OrientationSettings();

    nlohmann::json& portrait_json()     
    {
        return m_portrait_json;
    }

    nlohmann::json& landscape_json()     
    {
        return m_landscape_json;
    }

    SettingsNode* get();

    SettingsNode* operator->()     
    {
        return get();
    }

private:
    nlohmann::json m_portrait_json;
    nlohmann::json m_landscape_json;
    std::unique_ptr<SettingsNode> m_portrait_storage;
    std::unique_ptr<SettingsNode> m_landscape_storage;
};