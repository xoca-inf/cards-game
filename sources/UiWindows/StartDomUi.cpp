#include "StartDomUi.h"
#include "settings/ResourceLibary.h"

void StartDomUi::updateSize(const math::size& newSize)
{
  DomUi::updateSize(newSize);

  if (Config::getData().orientation != m_oldConfigData.orientation) 
  {
    // ...
  }
  m_oldConfigData = Config::getData();
}

void StartDomUi::initContent()
{
    sptr<UiRect> mainRect = getRect();

    createItem(
            mainRect,
            "dom_main_bg",
            Offset(0.0, OffsetType::PERCENT, PivotAt::LEFT ),
            Offset(0.0, OffsetType::PERCENT, PivotAt::RIGHT ),
            Offset(0.0, OffsetType::PERCENT, PivotAt::TOP),
            Offset(0.0, OffsetType::PERCENT, PivotAt::BOTTOM),
            "textures/bg1.png",
            /*order*/ 0.0);
    float aspect = Render::getAspect();
    sptr<UiRect> bgRect = mainRect->findChild("dom_main_bg");
    FillData fillData(FillType::IGNORE_ASPECT, /*aspect*/ 1.0, HorizontalAlign::Center, VerticalAlign::Center);
    bgRect->setFillData(fillData);
}