#include "GameDomUi.h"
#include "settings/ResourceLibary.h"

void GameDomUi::setEnabled(bool value) {
  DomUi::setEnabled(value);
  W4_LOG_DEBUG("GameDomUi ENABLED CHANGED TO %d",  value);
}

void GameDomUi::updateSize(const math::size& newSize)
{
  DomUi::updateSize(newSize);

  if (Config::getData().orientation != m_oldConfigData.orientation) 
  {
    // ...
  }
  m_oldConfigData = Config::getData();
}

void GameDomUi::initContent()
{
    sptr<UiRect> mainRect = getRect();

    createItem(
            mainRect,
            "dom_main_bg",
            Offset(0.0, OffsetType::PERCENT, PivotAt::LEFT ),
            Offset(0.0, OffsetType::PERCENT, PivotAt::RIGHT ),
            Offset(0.0, OffsetType::PERCENT, PivotAt::TOP),
            Offset(0.0, OffsetType::PERCENT, PivotAt::BOTTOM),
            //"textures/bg1.png",
            "textures/1px.png",
            /*order*/ 999.0);
    float aspect = Render::getAspect();
    sptr<UiRect> bgRect = mainRect->findChild("dom_main_bg");
    FillData fillData(FillType::IGNORE_ASPECT, /*aspect*/ 1.0, HorizontalAlign::Center, VerticalAlign::Center);
    bgRect->setFillData(fillData);
    sptr<FlexGuiWidget> bgFlexWidget = getItem("dom_main_bg");
    bgFlexWidget->setClickable(false);
    sptr<Widget> bgWidget = bgFlexWidget->getWidget();
    bgWidget->setOpacity(0.3f);
    bgWidget->setIgnoreScreenshot(true);
}