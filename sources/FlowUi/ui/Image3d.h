#pragma once

#include "W4Framework.h"
#include "FlowUi/ui/data/FillData.h"
#include "FlowUi/ui/UiRect.h"
#include "FlowUi/ui/data/Image3dData.h"

W4_USE_UNSTRICT_INTERFACE

namespace games::benice::ui {

    using namespace games::benice::ui::data;

    class Image3d {

    public:
        Image3d() = default;
        ~Image3d();
        void initialize(sptr<Node> root, Image3dData data);
        void setUiRect(sptr<UiRect> rect);
        void setFillData(const FillData& data);
        const FillData& getFillData() const;
        void setEnabled(bool value);

        void setClickable(bool flag);
        void setClickCallback( std::function<void()> callback);
        const std::string& getId() const;
    //private:
        float getDistance() const;
        void setZOffset(float zOffset) {
            m_zOffset = zOffset;
        }
        float getZOffset() const {
            return m_zOffset;
        }
        void onRectChange(math::rect newRect);

        sptr<Node> m_root;
        sptr<VisibleNode> m_visual;
        sptr<UiRect> m_uiRect;
        Image3dData m_data;
        sptr<Collider> m_collider;
        std::function<void()> m_clickCallback;
        float m_zOffset{0.0f};
    };
}

