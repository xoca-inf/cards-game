#pragma once

#include "W4Framework.h"
#include "ui/UiParser.h"
#include "FlowUi/ui/WorldUi.h"
#include "FlowUi/ui/DomUi.h"
#include "settings/Config.h"

W4_USE_UNSTRICT_INTERFACE

class UiLayoutComponent : public IComponent
{
  W4_COMPONENT(UiLayoutComponent, IComponent)
  W4_COMPONENT_DISABLE_CLONING
public:
  UiLayoutComponent(CloneTag, const UiLayoutComponent& from, Node& owner)
  : Super(CloneTag{}, from, owner)
  {}
  virtual ~UiLayoutComponent();

  virtual void initialize(const variant::Variant& data) override;
  virtual void finalize() override;
  virtual void onEnabled(bool value) override;
  
  void show(bool value);

  sptr<Node> getRoot();
  sptr<LayoutImage> getImage(const std::string& name);
  sptr<LayoutButton> getButton(const std::string& name);
  sptr<LayoutLabel> getLabel(const std::string& name);

  void resize(const math::size& newSize, bool forceReset = true);
  void rebuild();
  void clear();

  template<typename T>
  sptr<T> getDomUi() {
    return std::dynamic_pointer_cast<T>(m_domUi);
  }

  sptr<games::benice::ui::DomUi> domUi() {
    return m_domUi;
  }

  template<typename T>
  sptr<games::benice::ui::DomUi> setDomUi(
    float baseOrder = 0.0f, 
    bool fullSize = true)
  {
    m_domUi = make::sptr<T>(
      baseOrder);
    m_domUi->init(fullSize);
    m_domUi->setCloseCallback([this](){
      m_domUi = nullptr; /// \todo ???
    });
    m_domUi->updateSize(Render::getSize());
    return m_domUi;
  }

  template<typename T>
  sptr<T> getWorldUi() {
    return std::dynamic_pointer_cast<T>(m_worldUi);
  }

  sptr<games::benice::ui::WorldUi> worldUi() {
    return m_worldUi;
  }

  template<typename T>
  sptr<games::benice::ui::WorldUi> setWorldUi(
    sptr<Node> root = nullptr, 
    sptr<Camera> camera = nullptr, 
    float distance = 1.0f, 
    float baseOrder = 10.0f, 
    bool fullSize = true)
  {
    m_worldUi = make::sptr<T>(
      //m_cameraUi, 
      //m_uiRoot,
      root ? root : m_uiRoot,
      camera ? camera : Render::getScreenCamera(), 
      distance, 
      baseOrder);
    m_worldUi->init(fullSize);
    m_worldUi->setCloseCallback([this](){
      m_worldUi = nullptr; /// \todo ???
    });
    m_worldUi->updateSize(Render::getSize());
    return m_worldUi;
  }

private:
  std::string m_filenameFull;
  ConfigData m_oldConfigData{};
  sptr<Node> m_uiRoot;
  std::vector<sptr<LayoutImage>> m_uiImages;
  std::vector<sptr<LayoutButton>> m_uiButtons;
  std::vector<sptr<LayoutLabel>> m_uiLabels;
  sptr<games::benice::ui::WorldUi> m_worldUi;
  sptr<games::benice::ui::DomUi> m_domUi;
};