#include "JsonWidgetsParser.h"


namespace nlohmann {

    commonStruct::StringSettings getStringFromJson(const json& value)
    {
        commonStruct::StringSettings settings;
        settings.name = value["name"];
        settings.value = value["value"];
        return settings;
    }

    commonStruct::Vec2Settings getVec2FromJson(const json& value)
    {
        commonStruct::Vec2Settings settings;
        settings.name = value["name"];
        settings.value = value["value"];
        return settings;
    }

    commonStruct::Vec3Settings getVec3FromJson(const json& value)
    {
        commonStruct::Vec3Settings settings;
        settings.name = value["name"];
        settings.value = value["value"];
        return settings;
    }

    commonStruct::Vec4Settings getVec4FromJson(const json& value)
    {
        commonStruct::Vec4Settings settings;
        settings.name = value["name"];
        settings.value = value["value"];
        return settings;
    }

    commonStruct::FloatSettings getFloatFromJson(const json& value)
    {
        commonStruct::FloatSettings settings;
        if (auto it = value.find("name"); it != value.end())
        {
            settings.name = (it.value().get<std::string>());
        }
        else {
            W4_LOG_DEBUG("Name no found");
        }
        if (auto it = value.find("value"); it != value.end())
        {
            settings.value = (it.value().get<float>());
        }
        else {
            W4_LOG_DEBUG("Float no found");
        }
        return settings;
    }

    commonStruct::IntSettings getIntFromJson(const json& value)
    {
        commonStruct::IntSettings settings;
        int num = 0;
        if (auto it = value.find("name"); it != value.end())
        {
            settings.name = (it.value().get<std::string>());
        }
        else {
            W4_LOG_DEBUG("Name no found");
        }
        if (auto it = value.find("value"); it != value.end())
        {
            num = (it.value().get<int>());
            settings.value = num;
        }
        else {
            W4_LOG_DEBUG("Int no found");
        }
        return settings;
    }
}
