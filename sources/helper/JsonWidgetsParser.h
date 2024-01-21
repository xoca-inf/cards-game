#pragma once

#include "W4Framework.h"
#include "W4JSON.h"

W4_USE_UNSTRICT_INTERFACE

namespace nlohmann::commonStruct
{
	struct FloatSettings
	{
		std::string name;
		float value;
	};

	struct IntSettings
	{
		std::string name;
		int value;
	};

	struct Vec2Settings
	{
		std::string name;
		vec2 value;
	};

	struct Vec3Settings
	{
		std::string name;
		vec3 value;
	};

	struct Vec4Settings
	{
		std::string name;
		vec4 value;
	};

	struct StringSettings
	{
		std::string name;
		std::string value;
	};
}

namespace nlohmann {
	commonStruct::StringSettings getStringFromJson(const json& value);
	commonStruct::Vec2Settings getVec2FromJson(const json& value);
	commonStruct::Vec3Settings getVec3FromJson(const json& value);
	commonStruct::Vec4Settings getVec4FromJson(const json& value);
	commonStruct::FloatSettings getFloatFromJson(const json& value);
	commonStruct::IntSettings getIntFromJson(const json& value);
}