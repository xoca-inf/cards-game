#include "DomUi.h"

namespace games::benice::ui
{
    DomUi::DomUi(float baseOrder):m_baseOrder(baseOrder)
    {
    }

    DomUi::~DomUi()
    {
        if (m_rect != nullptr)
        {
            for(sptr<FlexGuiWidget> item : m_items) 
            {
                if (item != nullptr && item->getWidget() != nullptr) {
                    item->getWidget()->setVisible(false);
                }
                if (m_rect != nullptr && item->getRect() != nullptr) {
                    m_rect->removeChild(item->getRect());
                }
            }
            m_rect = nullptr;
        }
        m_items.clear();
    }
    void DomUi::setEnabled(bool value) {
        for(sptr<FlexGuiWidget> item : m_items) {
            if (item != nullptr) {
                item->setClickable(value);
                /*if (item->m_root != nullptr)
                    item->m_root->setEnabled(value);*/
                if (item->getWidget() != nullptr)
                    item->getWidget()->setVisible(value);
            }
            //item->m_visual->setVisible(value);
        }
    }

    void DomUi::init(bool fullSize)
    {
        if (fullSize)
        {
            m_rect = make::sptr<UiRect>(
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER),
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER), 
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER), 
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER));
        }
        else
        {
            m_rect = make::sptr<UiRect>(
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER), 
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER), 
                Offset(0.5, OffsetType::PERCENT, PivotAt::CENTER), 
                Offset(-0.5, OffsetType::PERCENT, PivotAt::CENTER));
            m_rect->setFill(FillType::INSIDE, 1.0);
        }

        initContent();
        updateSize(math::size{});
    }

    void DomUi::addItem(sptr<FlexGuiWidget> item)
    {
        m_items.push_back(item);
    }

    void DomUi::removeItem(sptr<FlexGuiWidget> item)
    {
        if (item != nullptr && item->getWidget() != nullptr) {
            item->getWidget()->setVisible(false);
        }
        if (m_rect != nullptr && item->getRect() != nullptr) {
            m_rect->removeChild(item->getRect());
        }
        std::remove(m_items.begin(), m_items.end(), item);
    }

    sptr<FlexGuiWidget> DomUi::getItem(const std::string &id) const
    {
        for (int i = 0; i < m_items.size(); i++)
        {
            if (m_items[i]->getId() == id)
            {
                return m_items[i];
            }
        }
        return nullptr;
    }

    sptr<UiRect> DomUi::getRect()
    {
        return m_rect;
    }

    void DomUi::updateSize(const math::size&)
    {
        if (m_rect != nullptr)
        {
            size sz = Render::getSize();
            //w4::math::size guiResolution = getVirtualResolution();
    
            m_rect->parentSize(math::rect(sz.w / 2.0, sz.h / 2.0, sz.w, sz.h));
        }
    }
    
    void DomUi::createItem(sptr<UiRect> root, std::string id, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order)
    {
        auto uiRect =  make::sptr<UiRect>(id,left,right,top,bottom);
        root->addChild(uiRect);
    
        auto imageWidget = gui::createWidget<gui::Image>(nullptr,resId,ivec2(1,1), ivec2(1,1), id);
        imageWidget->setOrder(order+m_baseOrder);
        imageWidget->setSizePolicy(SizePolicy::Auto,SizePolicy::Auto);
    
        auto flexWidget = make::sptr<FlexGuiWidget>(id,uiRect, imageWidget);
        
        addItem(flexWidget);
    }

    //DomUi::createLabel /// \todo
    
    void DomUi::createImageButton(sptr<UiRect> root, std::string id, Offset left, Offset right, Offset top, Offset bottom, std::string resIdNormal, std::string resIdPressed, float order)
    {
        auto uiRect =  make::sptr<UiRect>(id,left,right,top,bottom);
        root->addChild(uiRect);
        
        auto buttonWidget = gui::createWidget<gui::ImageButton>(nullptr,resIdNormal, resIdPressed,ivec2(1,1),ivec2(1,1));
        buttonWidget->setSizePolicy(SizePolicy::Auto,SizePolicy::Auto);
        buttonWidget->setOrder(order+m_baseOrder);
        auto flexWidget = make::sptr<FlexGuiWidget>(id,uiRect, buttonWidget);
        
        addItem(flexWidget);
    }
    
    void DomUi::setCloseCallback (std::function<void ()> closeCallback)
    {
        m_closeCallback = closeCallback;
    }
}