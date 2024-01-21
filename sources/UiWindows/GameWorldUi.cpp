#include "GameWorldUi.h"
#include "settings/ResourceLibary.h"
#include "FlowUi/ui/Image3d.h"
#include "settings/Settings.h"

using namespace games::benice::ui::data;
using namespace games::benice::ui;

static const bool isBgImageRepeating = false;

static const std::string bg_repeating = "textures/bg_fix.jpg";
static const std::string bg_portrait = "textures/game_bg_portrait.jpg";
static const std::string bg_landscape = "textures/game_bg_landscape.jpg";

static const std::string bg_res_repeating = "bg_fix";
static const std::string bg_res_portrait = "game_bg_portrait";
static const std::string bg_res_landscape = "game_bg_landscape";

static void setUnlitMaterial(sptr<Texture> tex, sptr<games::benice::ui::Image3d> node, const std::string& texture_name)
{
    tex->setSWrapping(Wrapping::Repeat);
    tex->setTWrapping(Wrapping::Repeat);
    auto material = Material::get("materials/unlit.mat");
    sptr<MaterialInst> materialInst 
        = material->createInstance();
    materialInst->setTexture(
        w4::resources::TextureId::TEXTURE_0, tex);
    materialInst->setParam("tileMultipleX", 1.0F );
    materialInst->setParam("tileMultipleY", 1.0F );
    materialInst->enableBlending(false);
    node->m_visual->setMaterialInst(materialInst);
    node->m_data.m_material = texture_name;
    node->m_data.m_material_inst = materialInst;
}

static void setSpriteMaterial(sptr<Texture> tex, sptr<games::benice::ui::Image3d> node, const std::string& texture_name)
{
    tex->setSWrapping(Wrapping::Repeat);
    tex->setTWrapping(Wrapping::Repeat);
    auto material = Material::get("materials/sprite.mat");
    sptr<MaterialInst> materialInst 
        = material->createInstance();
    materialInst->setTexture(
        w4::resources::TextureId::TEXTURE_0, tex);
    materialInst->enableBlending(false);
    node->m_visual->setMaterialInst(materialInst);
    node->m_data.m_material = texture_name;
    node->m_data.m_material_inst = materialInst;
}

void GameWorldUi::updateBackground()
{
    const std::string bg_texture_name = isBgImageRepeating 
        ? bg_repeating 
        : (Config::getData().orientation == "portrait" 
          ? bg_portrait
          : bg_landscape);
    const std::string bg_resource_name = isBgImageRepeating 
        ? bg_res_repeating 
        : (Config::getData().orientation == "portrait" 
          ? bg_res_portrait
          : bg_res_landscape);
    sptr<Texture> main_bg_tex 
        = ResourceLibary::instance().getTexture(bg_resource_name);
    sptr<Image3d> main_bg = getItem("main_bg");
    if (isBgImageRepeating)
      setUnlitMaterial(main_bg_tex, main_bg, bg_texture_name);
    else
      setSpriteMaterial(main_bg_tex, main_bg, bg_texture_name);
}

void GameWorldUi::updateSize(const math::size& newSize)
{
    WorldUi::updateSize(newSize);

    if (Config::getData().orientation != m_oldConfigData.orientation) 
    {
      updateBackground();
    }

    sptr<games::benice::ui::Image3d> main_bg = getItem("main_bg");
    if (isBgImageRepeating && main_bg != nullptr && m_bgRect != nullptr) 
    {
        math::rect rectValue = m_bgRect->getValue();
        W4_LOG_DEBUG("bgRect->getValue x=%f y=%f", rectValue.size.x, rectValue.size.y);
        main_bg->m_data.m_material_inst->setParam(
            "tileMultipleX", rectValue.size.x );
        main_bg->m_data.m_material_inst->setParam(
            "tileMultipleY", rectValue.size.y );
    }

    m_oldConfigData = Config::getData();
}

void GameWorldUi::setEnabled(bool value) {
  WorldUi::setEnabled(value);
  W4_LOG_DEBUG("GameWorldUi ENABLED CHANGED TO %d",  value);
}

void GameWorldUi::initContent()
{
    auto defaultResId = "gui/world/1px.png";

    sptr<UiRect> mainRect = getRect();

    {
      const std::string bg_texture_name = isBgImageRepeating 
          ? bg_repeating 
          : (Config::getData().orientation == "portrait" 
            ? bg_portrait
            : bg_landscape);
      createItem(
              mainRect,
              "main_bg",
              Offset(0.0, OffsetType::PERCENT, PivotAt::LEFT ),
              Offset(0.0, OffsetType::PERCENT, PivotAt::RIGHT ),
              Offset(0.0, OffsetType::PERCENT, PivotAt::TOP),
              Offset(0.0, OffsetType::PERCENT, PivotAt::BOTTOM),
              bg_texture_name,
              0.0);
      m_bgRect = mainRect->findChild("main_bg");
      FillData fillData(FillType::IGNORE_ASPECT, /*aspect*/ 1.0, 
          HorizontalAlign::Center, VerticalAlign::Center);
      m_bgRect->setFillData(fillData);
    }

#if 0
    {
      float order = 999.0f;
      if (Settings::instance()->hasFloat("blackoutRenderOrder")) 
      {
        order = Settings::instance()->getFloat("blackoutRenderOrder");
      }
      const std::string texture_name = "textures/1px.png" ;
      createItem(
              mainRect,
              "main_blackout",
              Offset(0.0, OffsetType::PERCENT, PivotAt::LEFT ),
              Offset(0.0, OffsetType::PERCENT, PivotAt::RIGHT ),
              Offset(0.0, OffsetType::PERCENT, PivotAt::TOP),
              Offset(0.0, OffsetType::PERCENT, PivotAt::BOTTOM),
              texture_name,
              order);
      m_blackoutRect = mainRect->findChild("main_blackout");
      FillData fillData(FillType::IGNORE_ASPECT, /*aspect*/ 1.0, 
          HorizontalAlign::Center, VerticalAlign::Center);
      m_blackoutRect->setFillData(fillData);
      sptr<Texture> main_blackout_tex 
          = ResourceLibary::instance().getTexture("1px");
      sptr<Image3d> main_blackout = getItem("main_blackout");
      setSpriteMaterial(main_blackout_tex, 
        main_blackout, texture_name);
      main_blackout->setEnabled(true);
      main_blackout->setClickable(false);
      
      if (Settings::instance()->hasFloat("blackoutZOffset")) 
      {
        main_blackout->setZOffset(Settings::instance()->getFloat("blackoutZOffset"));
      }
    }
#endif
}
