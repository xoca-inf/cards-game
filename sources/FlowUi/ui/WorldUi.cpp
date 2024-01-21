#include "WorldUi.h"

namespace games::benice::ui {
    WorldUi::WorldUi(sptr<Node> root, sptr<Camera> camera, float distance, float baseOrder) : m_distance(distance), m_camera(camera), m_baseOrder(baseOrder)
    {
        m_root = make::sptr<Node>();
        root->addChild(m_root, false);
        
        m_root->setWorldTranslation( m_camera->getWorldTranslation() + m_camera->getWorldForward()*m_distance);
    }

    WorldUi::~WorldUi()
    {
        for(sptr<Image3d> item : m_items) {
            if (item != nullptr) {
                item->setClickable(false);
                if (item->m_root != nullptr)
                    item->m_root->setEnabled(false);
                if (item->m_visual != nullptr)
                    item->m_visual->setEnabled(false);
            }
            //item->m_visual->setVisible(value);
        }
        if (m_root != nullptr)
        {
            if (m_root->getParent() != nullptr)
            {
                m_root->getParent()->removeChild(m_root);
            }
            m_root = nullptr;
        }
        m_items.clear();
    }

    void WorldUi::setEnabled(bool value) {
        for(sptr<Image3d> item : m_items) {
            if (item != nullptr) {
                item->setEnabled(value);
            }
            //item->m_visual->setVisible(value);
        }
        updateSize(Render::getSize());
    }

    void WorldUi::init(bool fullSize)
    {
        if (fullSize)
        {
            m_rect = make::sptr<UiRect>(
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER ), 
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER ), 
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER), 
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER));
        }
        else
        {
            m_rect = make::sptr<UiRect>(
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER ), 
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER ), 
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER), 
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER));
            m_rect->setFill(FillType::INSIDE, 1.0);
        }

        initContent();
        updateSize(Render::getSize());
    }

    void WorldUi::addItem(sptr<Image3d> item)
    {
        m_items.push_back(item);
    }

    void WorldUi::removeItem(sptr<Image3d> item)
    {
        if (item != nullptr) {
            item->setClickable(false);
            if (item->m_root != nullptr)
                item->m_root->setEnabled(false);
            if (item->m_visual != nullptr)
                item->m_visual->setEnabled(false);
        }
        std::remove(m_items.begin(), m_items.end(), item);
    }

    sptr<Image3d> WorldUi::getItem(const std::string &id) const
    {
        for (int i = 0; i < m_items.size();i++)
        {
            if (m_items[i]->getId() == id)
            {
                return m_items[i];
            }
        }
        return nullptr;
    }

    sptr<UiRect> WorldUi::getRect()
    {
        return m_rect;
    }

    sptr<Node> WorldUi::getRoot() const
    {
        return m_root;
    }

    void WorldUi::updateSize(const math::size&)
    {
        math::rect screenSize = games::benice::ui::utils::getScreenWorldRect(m_camera, m_distance);
        if (m_rect != nullptr)
        {
            m_rect->parentSize(screenSize);
        }
    }

    void WorldUi::createItem(sptr<UiRect> rootUiRect, std::string id, sptr<MaterialInst> material_inst, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order)
    {
        auto uiRect = make::sptr<UiRect>(id,left,right,top,bottom );
        rootUiRect->addChild(uiRect);
        auto imgData = Image3dData(id, material_inst, resId, uiRect->getId(), order+m_baseOrder);
        auto img = make::sptr<Image3d>();
        img->initialize(getRoot(), imgData);
        img->setUiRect(uiRect);
        addItem(img);
    }
    
    void WorldUi::createItem(sptr<UiRect> rootUiRect, std::string id, std::string material, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order)
    {
        auto uiRect = make::sptr<UiRect>(id,left,right,top,bottom );
        rootUiRect->addChild(uiRect);
        auto imgData = Image3dData(id, material, resId, uiRect->getId(), order+m_baseOrder);
        auto img = make::sptr<Image3d>();
        img->initialize(getRoot(), imgData);
        img->setUiRect(uiRect);
        addItem(img);
    }
    
    void WorldUi::createItem(sptr<UiRect> rootUiRect, std::string id, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order)
    {
        auto uiRect = make::sptr<UiRect>(id,left,right,top,bottom );
        rootUiRect->addChild(uiRect);
        auto imgData = Image3dData(id, "materials/sprite.mat",resId, uiRect->getId(), order+m_baseOrder);
        auto img = make::sptr<Image3d>();
        img->initialize(getRoot(), imgData);
        img->setUiRect(uiRect);
        addItem(img);
    }
    
    void WorldUi::setCloseCallback (std::function<void ()> closeCallback)
    {
        m_closeCallback = closeCallback;
    }
}