#pragma once

#include "OffsetType.h"
#include "PivotAt.h"
#include "W4Framework.h"

namespace games::benice::ui::data
{
    struct Offset
    {
        float value;
        OffsetType offset;
        PivotAt pivot;

        Offset()
        {};

        Offset(float val, OffsetType ot, PivotAt p) : value(val), offset(ot), pivot(p)
        {}

        Offset(const Offset &other) : value(other.value), offset(other.offset), pivot(other.pivot)
        {}

        float calcDimension(float a, float b) const
        {
            float dimension = b - a;
            float val = offset == OffsetType::POINTS ? value : dimension * value;
            float result = 0.0;
            switch (pivot)
            {
                case PivotAt::LEFT:
                    result = a + val;
                    break;
                case PivotAt::RIGHT:
                    result = b - val;
                    break;
                case PivotAt::CENTER:
                    result = a + dimension / 2.0 + val;
                    break;
                case PivotAt::TOP:
                    result = b - val;
                    break;
                case PivotAt::BOTTOM:
                    result = a + val;
                    break;
                default:
                    W4_LOG_ERROR("[Offset] cant use pivot %d", (int) pivot);
                    break;
            }
            return result;
        }

        void log()
        {
            W4_LOG_DEBUG("value %f, offset %d, pivot %d ", value, (int) offset, (int) pivot);
        }
    };
}


namespace nlohmann
{

    using games::benice::ui::data::Offset;

    template<>
    struct adl_serializer<Offset>
    {
        static void to_json(json &j, const Offset &obj)
        {
            j["value"] = obj.value;
            j["offset"] = obj.offset;
            j["pivot"] = obj.pivot;
        }

        static void from_json(const json &j, Offset &obj)
        {
            obj.value = j["value"];
            obj.offset = j["offset"];
            obj.pivot = j["pivot"];
        }
    };
}
