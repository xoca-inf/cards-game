#pragma once
#include "W4Framework.h"
#include "W4JSON.h"

namespace games::benice::ui::data
{
    enum struct OffsetType {
        OFFSET_ERROR = 0,
        POINTS = 1,
        PERCENT = 2
    };
}

namespace nlohmann {
    using games::benice::ui::data::OffsetType;
    W4_NLOHMANN_JSON_SERIALIZE_ENUM(OffsetType, {
        { OffsetType::OFFSET_ERROR, "OFFSET_ERROR" },
        { OffsetType::POINTS, "POINTS" },
        { OffsetType::PERCENT, "PERCENT" }
    });
}
