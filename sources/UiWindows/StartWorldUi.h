#pragma once

#include "W4Framework.h"
#include "FlowUi/ui/WorldUi.h"
#include "settings/Config.h"

using namespace games::benice::ui;

class StartWorldUi : public WorldUi
{
public:
    StartWorldUi(sptr<Node> root, sptr<Camera> camera, float distance, float baseOrder): WorldUi(root,camera,distance, baseOrder){};
    //void setCloseCallback( std::function<void()> closeCallback);
    void updateSize(const math::size& newSize) override;
    
    void setEnabled(bool value) override;
protected:
    void initContent() override;
private:
    void updateBackground();
public:
    //std::function<void()> m_closeCallback;
    //sptr<games::benice::ui::Image3d> m_main_bg;
    //sptr<MaterialInst> m_main_bg_materialInst;
    //sptr<Material> m_main_bg_material;
    //sptr<Texture> m_main_bg_tex;
    sptr<UiRect> m_bgRect;
    ConfigData m_oldConfigData{};
    //sptr<UiRect> m_blackoutRect;
};
