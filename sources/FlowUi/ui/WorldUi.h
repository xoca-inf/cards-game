#pragma once

#include "W4Framework.h"
#include "UiRect.h"
#include "Image3d.h"
#include "FlowUi/ui/utils/ScreenRectInWorld.h"

namespace games::benice::ui
{

    class WorldUi
    {
    public:
        WorldUi(sptr<Node> root, sptr<Camera> camera, float distance, float baseOrder);

        void init(bool fullSize);

        ~WorldUi();

        virtual void updateSize(const math::size& newSize);

        virtual void setEnabled(bool value);

        void addItem(sptr<Image3d> item);

        void removeItem(sptr<Image3d> item);

        sptr<Image3d> getItem(const std::string &id) const;

        sptr<UiRect> getRect();
        sptr<Node> getRoot() const;
        
        void createItem(sptr<UiRect> rootUiRect, std::string id, sptr<MaterialInst> material_inst, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order);

        void createItem(sptr<UiRect> rootUiRect, std::string id, std::string material, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order);

        void createItem(sptr<UiRect> root, std::string id, Offset left, Offset right, Offset top, Offset bottom, std::string resId, float order);
    
        void setCloseCallback( std::function<void()> closeCallback);
    protected:
        virtual void initContent() = 0;
        std::function<void()> m_closeCallback;
    private:
        sptr<Node> m_root;
        sptr<Camera> m_camera;
        std::vector<sptr<Image3d>> m_items;
        float m_distance;
        float m_baseOrder;
        sptr<UiRect> m_rect;
        
    };
}