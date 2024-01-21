#pragma once

#include "W4Framework.h"
#include "FlowUi/ui/DomUi.h"
#include "settings/Config.h"
using namespace games::benice::ui;

class StartDomUi : public DomUi
{
public:
    StartDomUi(float baseOrder) : DomUi(baseOrder) {}
    void updateSize(const math::size& newSize) override;
protected:
    void initContent() override;
private: 
  ConfigData m_oldConfigData{};
};



