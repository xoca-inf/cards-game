#include "UiSwitcher.h"
#include "settings/ResourceLibary.h"
#include "FlowUi/ui/Image3d.h"
#include "settings/Settings.h"
#include "settings/Config.h"
#include "ui/MainFSM.h"
#include "UiWindows/GameWorldUi.h"
#include "UiWindows/GameDomUi.h"
#include "UiWindows/StartWorldUi.h"
#include "components/GameComponent.h"

using namespace games::benice::ui::data;
using namespace games::benice::ui;

// static 
const std::string MainUiSwitcher::TC_START = "start_screen";

// static 
const std::string MainUiSwitcher::TC_GAME = "game_screen";

// static 
const std::string MainUiSwitcher::TC_RESULT = "end_screen";

void MainUiSwitcher::destroyScenes()
{
    W4_LOG_DEBUG("qqq Destroy scenes");
    auto children = m_cameraUi->getChildren();
    for (const auto& child : children)
    {
        m_cameraUi->removeChild(child);
    }
    m_cameraUi = nullptr;
    
    children = m_cameraNode->getChildren();
    for (const auto& child : children)
    {
        m_cameraNode->removeChild(child);
    }
    m_cameraNode = nullptr;
}

void MainUiSwitcher::createScenes(
  sptr<Node> cameraUi, sptr<Node> cameraNode, sptr<GameComponent> gameComponent) 
{
  m_cameraUi = cameraUi;
  
  m_cameraNode = cameraNode;

  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(MainUiSwitcher::TC_START, m_cameraUi);
    if (!mainUI.worldUi()) {
      mainUI.setWorldUi<StartWorldUi>(Render::getRoot());
    }
  }
  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(MainUiSwitcher::TC_GAME, m_cameraUi);

    UiLayoutComponent& gameLayout = m_uiSwitcher.getUiComponent(MainUiSwitcher::TC_GAME);
    gameComponent->show(false);
    gameComponent->setFinishCallback([]()
    {
      MainFSM::instance()->processEvent(MainFSM::FSM::Events::Next);
    });  
    
    if (!mainUI.worldUi()) {
      mainUI.setWorldUi<GameWorldUi>(Render::getRoot());
    }
  }
  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(MainUiSwitcher::TC_RESULT, m_cameraUi);
    if (!mainUI.worldUi()) {
      mainUI.setWorldUi<StartWorldUi>(Render::getRoot());
    }
  }

  m_uiSwitcher.enable(false);
}

UiLayoutComponent& UiSwitcher::addUi(const std::string &uiName, sptr<Node> root)
{
  if (m_uiCollection.find(uiName) != m_uiCollection.end()) {
    W4_LOG_ERROR("DO NOT CREATE SAME UI TWICE: %s", uiName.c_str());
  }
  Config::reload();
  auto node = make::sptr<Node>(uiName);
  node->setName(uiName);
  root->addChild(node, true);
  auto &uiComponent = node->addComponent<UiLayoutComponent>(uiName);
  uiComponent.rebuild();
  m_uiCollection[uiName] = node;
  return uiComponent;
}

void UiSwitcher::switchUi(const std::string& uiName)
{
  m_currentUi = uiName;
  W4_LOG_DEBUG("[UI switched to] %s", uiName.c_str());

  Config::reload();

  for (const auto &[key, item]: m_uiCollection) {
    if (item && item->getName() != uiName) {
      item->getFirstComponent<UiLayoutComponent>().show(false);
    } else if (item) {
      item->getFirstComponent<UiLayoutComponent>().show(true);
    }
  }

  for (const auto &[key, ui]: m_uiCollection) {
    ui->getFirstComponent<UiLayoutComponent>().resize(Render::getSize());
  }
}

// static 
const std::string InGameUiSwitcher::TC_TURN1 = "turn_1";
// static 
const std::string InGameUiSwitcher::TC_TURN2 = "turn_2";
// static 
const std::string InGameUiSwitcher::TC_TURN3 = "turn_3";
// static 
const std::string InGameUiSwitcher::TC_TURN4 = "turn_4";
// static 
const std::string InGameUiSwitcher::TC_TURN5 = "turn_5";

void InGameUiSwitcher::createScenes(sptr<Node> cameraUi, sptr<Node> cameraNode) 
{
  m_cameraUi = cameraUi;
  
  m_cameraNode = cameraNode;

  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(InGameUiSwitcher::TC_TURN1, m_cameraUi);
  }

  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(InGameUiSwitcher::TC_TURN2, m_cameraUi);
  }

  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(InGameUiSwitcher::TC_TURN3, m_cameraUi);
  }

  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(InGameUiSwitcher::TC_TURN4, m_cameraUi);
  }

  {
    UiLayoutComponent& mainUI = m_uiSwitcher.addUi(InGameUiSwitcher::TC_TURN5, m_cameraUi);
  }

  m_uiSwitcher.enable(false);
}
