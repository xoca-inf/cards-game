#pragma once

#include "W4Framework.h"
#include "W4JSON.h"
#include "FillType.h"

W4_USE_UNSTRICT_INTERFACE

namespace games::benice::ui::data
{
    struct FillData
    {
        FillData()
        {
            m_fillType = FillType::IGNORE_ASPECT;
            m_aspect = 1.0f;
            m_verticalAlign = VerticalAlign::Center;
            m_horizontalAlign = HorizontalAlign::Center;
        }

        FillData(FillType fillType, float aspect, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
        {
            m_fillType = fillType;
            m_aspect = aspect;
            m_horizontalAlign = horizontalAlign;
            m_verticalAlign = verticalAlign;
        }

        FillData(FillType fillType, float aspect)
        {
            m_fillType = fillType;
            m_aspect = aspect;
            m_horizontalAlign = HorizontalAlign::Center;
            m_verticalAlign = VerticalAlign::Center;
        }

        FillData(HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
        {
            m_fillType = FillType::IGNORE_ASPECT;
            m_aspect = 1.0f;
            m_horizontalAlign = horizontalAlign;
            m_verticalAlign = verticalAlign;
        }

        FillType m_fillType;
        float m_aspect;
        HorizontalAlign m_horizontalAlign;
        VerticalAlign m_verticalAlign;
    };
}
namespace nlohmann
{
    using games::benice::ui::data::FillData;
    template<>
    struct adl_serializer<FillData>
    {
        static void to_json(json &j, const FillData &obj)
        {
            j["fillType"] = obj.m_fillType;
            j["aspect"] = obj.m_aspect;
            j["horizontalAlign"] = obj.m_horizontalAlign;
            j["verticalAlign"] = obj.m_verticalAlign;
        }

        static void from_json(const json &j, FillData &obj)
        {
            obj.m_fillType = j["fillType"];
            obj.m_aspect = j["aspect"];
            obj.m_horizontalAlign = j["horizontalAlign"];
            obj.m_verticalAlign = j["verticalAlign"];
        }
    };
}

