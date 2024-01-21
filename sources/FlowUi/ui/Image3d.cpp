#include "Image3d.h"

namespace games::benice::ui
{
    Image3d::~Image3d()
    {
        if (m_root != nullptr && m_root->getParent() != nullptr)
        {
            m_root->getParent()->removeChild(m_root);
        }
        m_root = nullptr;
    
        setUiRect(nullptr);
    }

    void Image3d::initialize(sptr<Node> root, Image3dData data)
    {
        //root = Render::getRoot(); /// \todo

        m_data = data;
        m_root = make::sptr<Node>();
        root->addChild(m_root, false);

        m_visual = Mesh::create::plane(vec2(1, 1));
        m_visual->setName(m_data.m_resourceId);
        auto materialInst = 
            m_data.m_material_inst != nullptr
            ? m_data.m_material_inst
            : Material::get(m_data.m_material)->createInstance();

        if (m_data.m_material_inst == nullptr) {
            materialInst->setTexture(w4::resources::TextureId::TEXTURE_0, Texture::get(m_data.m_resourceId));
        }
        
        m_visual->setMaterialInst(materialInst);
        m_visual->setRenderOrder(m_data.m_order);

        m_root->addChild(m_visual, false);
    }

    void Image3d::setFillData(const FillData& data)
    {
        m_data.m_fill = data;
    }
    const FillData& Image3d::getFillData() const
    {
        return m_data.m_fill;
    }

    const std::string& Image3d::getId() const
    {
        return m_data.m_id;
    }

    float Image3d::getDistance() const
    {
        return m_data.m_order*(-0.0001f) + m_zOffset;
    }

    void Image3d::setEnabled(bool value) {
        if (m_collider != nullptr)
            setClickable(value);
        if (m_root != nullptr)
            m_root->setEnabled(value);
        if (m_visual != nullptr)
            m_visual->setEnabled(value);
    }

    void Image3d::setClickable(bool flag)
    {
        if (flag)
        {
            if (m_collider == nullptr && m_visual != nullptr) { 
                m_collider = m_visual->addCollider("Image3dCollider", make::sptr<core::AABB>(m_visual));
                m_collider->setReceiveRaycasts(true);
                m_collider->setBlockRaycasts(true);

                m_collider->setScreencastCallback([this](const CollisionInfo &collisionInfo)
                {
                    if (m_clickCallback != nullptr)
                    {
                        m_clickCallback();
                    }
                });

            }
        }
        else
        {
            if (m_collider!= nullptr) {
                m_collider->setReceiveRaycasts(false);
                m_collider->setBlockRaycasts(false);
                m_collider->resetScreencastCallback();
                m_collider->resetRaycastCallback();
                if (m_visual->getColliders().size()) {
                  //m_visual->removeCollider(m_collider);
                  //m_visual->removeCollider("Image3dCollider");
                  /*if (m_collider->getParent() != nullptr)
                    m_collider->getParent()->removeCollider(m_collider);*/
                }
                m_collider = nullptr;
            }

        }
    }
    void Image3d::setClickCallback(std::function<void()> callback)
    {
        m_clickCallback = callback;
    }

    void Image3d::onRectChange (math::rect newRect)
    {
        vec3 scale;
        switch (m_data.m_fill.m_fillType)
        {
            case FillType::IGNORE_ASPECT:
                scale = vec3(newRect.size.w, newRect.size.h, 1);
                break;
            case FillType::FIT_HORIZONTAL:
                scale = vec3(1, 1 / m_data.m_fill.m_aspect, 1) * newRect.size.w;
                break;
            case FillType::FIT_VERTICAL:
                scale = vec3(m_data.m_fill.m_aspect, 1, 1) * newRect.size.h;
                break;
            case FillType::INSIDE:
                if (newRect.size.w / newRect.size.h > m_data.m_fill.m_aspect)
                {
                    scale = vec3(m_data.m_fill.m_aspect, 1, 1) * newRect.size.h;
                }
                else
                {
                    scale = vec3(1, 1 / m_data.m_fill.m_aspect, 1) * newRect.size.w;
                }
                break;
            case FillType::OUTSIDE:
                if (newRect.size.w / newRect.size.h > m_data.m_fill.m_aspect)
                {
                    scale = vec3(1, 1 / m_data.m_fill.m_aspect, 1) * newRect.size.w;
                }
                else
                {
                    scale = vec3(m_data.m_fill.m_aspect, 1, 1) * newRect.size.h;
                }
                break;
            default:
                W4_LOG_ERROR("[Image3d] error m_fillType %d", (int) m_data.m_fill.m_fillType);
                break;
        }
    
        m_visual->setLocalScale(scale);
    
        vec3 pos = vec3(0, 0, getDistance());
        switch (m_data.m_fill.m_horizontalAlign)
        {
            case HorizontalAlign::Center:
                pos.x = newRect.position.x;
                break;
            case HorizontalAlign::Left:
                pos.x = newRect.position.x - newRect.size.w / 2.0 + scale.x / 2.0;
                break;
            case HorizontalAlign::Right:
                pos.x = newRect.position.x + newRect.size.w / 2.0 - scale.x / 2.0;
                break;
        }
    
        switch (m_data.m_fill.m_verticalAlign)
        {
            case VerticalAlign::Center:
                pos.y = newRect.position.y;
                break;
            case VerticalAlign::Top:
                pos.y = newRect.position.y + newRect.size.h / 2.0 - scale.y / 2.0;
                break;
            case VerticalAlign::Bottom:
                pos.y = newRect.position.y - newRect.size.h / 2.0 + scale.y / 2.0;
                break;
        }

        m_visual->setLocalTranslation(pos);
        vec3 lpos = m_visual->getLocalTranslation();
        vec3 wpos = m_visual->getWorldTranslation();
        W4_LOG_DEBUG("image l {%f,%f,%f} w {%f,%f,%f}",lpos.x,lpos.y,lpos.z,wpos.x,wpos.y,wpos.z);
    }
    
    void Image3d::setUiRect(sptr<UiRect> rect)
    {
        if (m_uiRect!= nullptr)
        {
            m_uiRect->removeOnChangeHandler();
        }
        
        m_uiRect = rect;
        
        if (m_uiRect!= nullptr)
        {
            m_uiRect->setOnChangeHandler(std::bind(&Image3d::onRectChange,this,std::placeholders::_1));
        }
    };
}