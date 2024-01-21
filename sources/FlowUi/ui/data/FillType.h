#pragma once

#include "W4Framework.h"
#include "W4JSON.h"

namespace games::benice::ui::data
{
    enum struct FillType {
        FILLTYPE_ERROR,
        IGNORE_ASPECT,
        FIT_HORIZONTAL,
        FIT_VERTICAL,
        INSIDE,
        OUTSIDE
    };
}
namespace nlohmann {
    using games::benice::ui::data::FillType;
    W4_NLOHMANN_JSON_SERIALIZE_ENUM(FillType, {
        { games::benice::ui::data::FillType::FILLTYPE_ERROR, "FILLTYPE_ERROR" },
        { FillType::IGNORE_ASPECT, "IGNORE_ASPECT" },
        { FillType::FIT_HORIZONTAL, "FIT_HORIZONTAL" },
        { FillType::FIT_VERTICAL, "FIT_VERTICAL" },
        { FillType::INSIDE, "INSIDE" },
        { FillType::OUTSIDE, "OUTSIDE" }
    });
}
