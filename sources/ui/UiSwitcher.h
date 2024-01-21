#pragma once

#include "W4Framework.h"
#include "settings/Config.h"
#include "components/UiLayoutComponent.h"
#include "components/GameComponent.h"

using namespace games::benice::ui;

class UiSwitcher 
{
public:
  UiSwitcher(){}
  
  sptr<Node> getUiNode(const std::string& uiName = "") {
    auto res = m_uiCollection[(uiName.empty() ? m_currentUi : uiName)];
    if (res == nullptr) W4_LOG_ERROR("UANBLE TO GET UI %s", uiName.c_str());
    return res;
  }

  UiLayoutComponent& getUiComponent(const std::string& uiName = "") {
    auto res = m_uiCollection[(uiName.empty() ? m_currentUi : uiName)];
    if (res == nullptr) 
      W4_LOG_ERROR("UANBLE TO GET UI %s", uiName.c_str());
    if (res->getAllComponents<UiLayoutComponent>().empty()) 
      W4_LOG_ERROR("UANBLE TO GET UI");
    return res->getFirstComponent<UiLayoutComponent>();
  }

  void resize(const math::size& newSize) {
    for (const auto& [key, ui] : m_uiCollection) {
      if (ui->getAllComponents<UiLayoutComponent>().empty()) 
        W4_LOG_ERROR("UANBLE TO GET UI");
      ui->getFirstComponent<UiLayoutComponent>().resize(newSize);
    }
  }

  void enable(const bool value) {
    for (const auto &[key, ui]: m_uiCollection) {
      if (ui->getAllComponents<UiLayoutComponent>().empty()) 
        W4_LOG_ERROR("UANBLE TO GET UI");
      ui->getFirstComponent<UiLayoutComponent>().show(value);
    }
  }

  void enableImage(
    bool value, const std::string& name, const std::string& ui_name = "")
  {
    sptr<LayoutImage> layoutImage = getUiComponent(ui_name).getImage(name);
    layoutImage->enable(value);
  }

  void enableButton(
    bool value, const std::string& name, const std::string& ui_name = "")
  {
    sptr<LayoutButton> layoutButton = getUiComponent(ui_name).getButton(name);
    layoutButton->enable(value);
  }

  sptr<LayoutImage> getImage(const std::string& name, const std::string& ui_name = "")
  {
    sptr<LayoutImage> layoutImage = getUiComponent(ui_name).getImage(name);
    return layoutImage;
  }

  sptr<LayoutButton> getButton(const std::string& name, const std::string& ui_name = "")
  {
    sptr<LayoutButton> layoutButton = getUiComponent(ui_name).getButton(name);
    return layoutButton;
  }

  void switchUi(const std::string& uiName);

  UiLayoutComponent& addUi(const std::string &uiName, sptr<Node> root); 

  std::unordered_map<std::string, sptr<Node>>* operator->()     
  {
      return &m_uiCollection;
  }

  const std::string& currentUiName() const {
    return m_currentUi;
  }

private:
  std::unordered_map<std::string, sptr<Node>> m_uiCollection;
  std::string m_currentUi;
};

class MainUiSwitcher 
{
private:
    MainUiSwitcher() {};

public:
  static const std::string TC_START;
  static const std::string TC_GAME;
  static const std::string TC_RESULT;

  static MainUiSwitcher& instance() {
      static MainUiSwitcher instance;
      return instance;
  }

  UiSwitcher* operator->()     
  {
      return &m_uiSwitcher;
  }

  void destroyScenes();
  void createScenes(sptr<Node> cameraUi, sptr<Node> cameraNode, 
      sptr<GameComponent> gameComponent);
private:
  UiSwitcher m_uiSwitcher{};
  sptr<Node> m_cameraUi;
  sptr<Node> m_cameraNode;
};

class InGameUiSwitcher 
{
private:
    InGameUiSwitcher() {};

public:
  static const std::string TC_TURN1;
  static const std::string TC_TURN2;
  static const std::string TC_TURN3;
  static const std::string TC_TURN4;
  static const std::string TC_TURN5;

  static InGameUiSwitcher& instance() {
      static InGameUiSwitcher instance;
      return instance;
  }

  UiSwitcher* operator->()     
  {
      return &m_uiSwitcher;
  }

  void createScenes(sptr<Node> cameraUi, sptr<Node> cameraNode);
private:
  UiSwitcher m_uiSwitcher{};
  sptr<Node> m_cameraUi;
  sptr<Node> m_cameraNode;
};
