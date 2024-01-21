#pragma once

#include "W4Framework.h"
#include "FlexGuiWidget.h"

namespace games::benice::ui
{
    class DomUi
    {
    public:
        DomUi(float baseOrder);

        void init(bool fullSize);

        ~DomUi();

        virtual void updateSize(const math::size& newSize);
        
        virtual void setEnabled(bool value);

        void addItem(sptr<FlexGuiWidget> item);

        void removeItem(sptr<FlexGuiWidget> item);

        sptr<FlexGuiWidget> getItem(const std::string &id) const;
    
        void createItem(sptr<UiRect> root, std::string id, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order);

        void createImageButton(sptr<UiRect> root, std::string id, Offset left, Offset right, Offset top, Offset bottom, std::string resIdNormal,std::string resIdPressed, float order);
        
        sptr<UiRect> getRect();
        void setCloseCallback( std::function<void()> closeCallback);

    protected:
        virtual void initContent() = 0;
        std::function<void()> m_closeCallback;
    private:
        std::vector<sptr<FlexGuiWidget>> m_items;
        sptr<UiRect> m_rect;
        float m_baseOrder;
        
    };

}