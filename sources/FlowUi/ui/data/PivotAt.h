#pragma once

#include "W4Framework.h"
#include "W4JSON.h"
namespace games::benice::ui::data
{
    enum struct PivotAt
            {
        PIVOT_ERROR = 0,
        LEFT = 1,
        RIGHT = 2,
        TOP = 3,
        BOTTOM = 4,
        CENTER = 5
            };
}

namespace nlohmann {
    using games::benice::ui::data::PivotAt;
    W4_NLOHMANN_JSON_SERIALIZE_ENUM(PivotAt, {
        { PivotAt::PIVOT_ERROR, "PIVOT_ERROR" },
        { PivotAt::LEFT, "LEFT" },
        { PivotAt::RIGHT, "RIGHT" },
        { PivotAt::TOP, "TOP" },
        { PivotAt::BOTTOM, "BOTTOM" },
        { PivotAt::CENTER, "CENTER" }
    });
}