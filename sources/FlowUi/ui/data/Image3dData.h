#pragma once

#include "W4JSON.h"
#include "FillData.h"

namespace games::benice::ui::data
{
    struct Image3dData
    {
        Image3dData()
        {}
    
        Image3dData(std::string id, std::string material, std::string resourceId, std::string rectId, float order)
        {
            m_id = id;
            m_material = material;
            m_resourceId = resourceId;
            m_rectId = rectId;
            m_fill = FillData(FillType::IGNORE_ASPECT,1.0);
            m_order = order;
            
        }
        Image3dData(std::string id, std::string material, std::string resourceId, std::string rectId, const FillData& fillData, float order)
        {
            m_id = id;
            m_material = material;
            m_resourceId = resourceId;
            m_rectId = rectId;
            m_fill = fillData;
            m_order = order;
        }
    
        Image3dData(std::string id, sptr<MaterialInst> material_inst, std::string resourceId, std::string rectId, float order)
        {
            m_id = id;
            m_material_inst = material_inst;
            m_resourceId = resourceId;
            m_rectId = rectId;
            m_fill = FillData(FillType::IGNORE_ASPECT,1.0);
            m_order = order;
            
        }
        Image3dData(std::string id, sptr<MaterialInst> material_inst, std::string resourceId, std::string rectId, const FillData& fillData, float order)
        {
            m_id = id;
            m_material_inst = material_inst;
            m_resourceId = resourceId;
            m_rectId = rectId;
            m_fill = fillData;
            m_order = order;
        }

        std::string m_id;
        std::string m_material;
        sptr<MaterialInst> m_material_inst{nullptr};
        std::string m_resourceId;
        std::string m_rectId;
        FillData m_fill;
        float m_order;
    };
}
namespace nlohmann
{
    using games::benice::ui::data::Image3dData;

    template<>
    struct adl_serializer<Image3dData>
    {
        static void to_json(json &j, const Image3dData &obj)
        {
            j["id"] = obj.m_id;
            j["material"] = obj.m_material;
            j["resourceId"] = obj.m_resourceId;
            j["rectId"] = obj.m_rectId;
            j["fill"] = obj.m_fill;
            j["order"] = obj.m_order;
        }

        static void from_json(const json &j, Image3dData &obj)
        {
            obj.m_id = j["id"];
            obj.m_material = j["material"];
            obj.m_resourceId = j["resourceId"];
            obj.m_rectId = j["rectId"];
            obj.m_fill = j["fill"];
            obj.m_order = j["order"];
        }
    };
}
