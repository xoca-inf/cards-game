#pragma once

#include "W4Framework.h"
#include "settings/Config.h"
#include "components/UiLayoutComponent.h"

using namespace games::benice::ui;

class MainFSM 
{
private:
  MainFSM(){};
public:
  static MainFSM& instance() {
      static MainFSM instance;
      return instance;
  }

  struct FSM
  {
    enum class Events
    {
      Initialize,
      //Settings,
      //HighScores,
      //Shop,
      Back,
      Start,
      Next,
      Restart,
      //TryAgain,
      //Bonus
    };
    struct States
    {
      struct Preloading : w4::fsm::FuncState<Events>
      {
      };
      struct StartMenu : w4::fsm::FuncState<Events>
      {
      };
      struct Game : w4::fsm::FuncState<Events>
      {
      };
      struct Result : w4::fsm::FuncState<Events>
      {
      };
    };
    using Type = w4::fsm::FSM<
      // initialization
      w4::fsm::Transition<Events::Initialize, States::Preloading, States::StartMenu>
      //, w4::fsm::Transition<Events::Next, States::StartMenu, States::Result>
      , w4::fsm::Transition<Events::Next, States::StartMenu, States::Game>
      // main game cycle
      //, w4::fsm::Transition<Events::Start, States::StartMenu, States::VehicleSelect>
      //, w4::fsm::Transition<Events::Start, States::VehicleSelect, States::Game>
      , w4::fsm::Transition<Events::Next, States::Game, States::Result>
      //, w4::fsm::Transition<Events::TryAgain, States::Result, States::Game>
      //, w4::fsm::Transition<Events::Restart, States::Result, States::VehicleSelect>
      , w4::fsm::Transition<Events::Next, States::Result, States::StartMenu>
      //, w4::fsm::Transition<Events::Bonus, States::Result, States::Ads>
      //, w4::fsm::Transition<Events::Next, States::Ads, States::StartMenu>
      // settings menu
      /*, w4::fsm::Transition<Events::Settings, States::StartMenu, States::SettingsMenu>
      , w4::fsm::Transition<Events::Back, States::SettingsMenu, States::StartMenu>
      // highScores menu
      , w4::fsm::Transition<Events::HighScores, States::StartMenu, States::HighScores>
      , w4::fsm::Transition<Events::Back, States::HighScores, States::StartMenu>
      // shop menu
      , w4::fsm::Transition<Events::Shop, States::StartMenu, States::Shop>
      , w4::fsm::Transition<Events::Back, States::Shop, States::StartMenu>*/
      , w4::fsm::Transition<Events::Restart, States::StartMenu, States::Preloading>
      , w4::fsm::Transition<Events::Restart, States::Game, States::Preloading>
      , w4::fsm::Transition<Events::Restart, States::Result, States::Preloading>
      >;
  };

  MainFSM::FSM::Type* operator->()     
  {
    return &m_fsm;
  }

 private:
    MainFSM::FSM::Type m_fsm;
};
