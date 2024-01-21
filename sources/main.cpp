#include "W4Framework.h"

#include "FlowUi/AdaptiveFov.h"
#include "UiWindows/GameWorldUi.h"
#include "UiWindows/GameDomUi.h"
#include "UiWindows/StartWorldUi.h"
#include "FlowUi/ui/Image3d.h"
#include "FlowUi/ui/FlexGuiWidget.h"
#include "components/UiLayoutComponent.h"
#include "components/GameComponent.h"
#include "settings/Config.h"
#include "settings/ResourceLibary.h"
#include "settings/Settings.h"
#include "ui/TexturePreloader.h"
#include "ui/MainFSM.h"
#include "ui/UiSwitcher.h"
#include "ads/AdsNetworkWrapper.h"

W4_USE_UNSTRICT_INTERFACE

using namespace games::benice::ui;
using namespace games::benice::ui::data;

static constexpr float preloadingTime = 0.03f; //seconds. after this time css loader will disappear

struct gameApp : public IGame
{
    void onConfig() override
    {
        Game::Config.StopUpdateWhenFocusLoss = false;
        Game::Config.AutoUsePreloader = false;
        Game::Config.EnableFrustumCulling = false;
    }

    void onStart() override
    {
        W4_LOG_DEBUG("onStart");
        
        platform::Platform::hideLoader();
        Render::enableDebugView(true);

        m_gameRoot = make::sptr<Node>("GameRoot");
        Render::getRoot()->addChild(m_gameRoot);
        m_camera = Render::getScreenCamera();

        m_cameraNode = make::sptr<Node>("CameraNode");
        m_gameRoot->addChild(m_cameraNode);
        m_cameraNode->addChild(m_camera);
        
        m_camera->setLocalTranslation(vec3(0.0f,0.0f,-10.0f));
        m_cameraNode->setWorldTranslation(m_camera->getWorldTranslation());

        // add lighting
        m_camera->setClearColor(Settings::instance()->getVec4("clearColor"));
        //m_camera->setClearMask(ClearMask::Color | ClearMask::Depth | ClearMask::Skybox);
        m_camera->setClearMask(ClearMask::Color | ClearMask::Depth);
        m_camera->setFov(
                games::benice::utils::AdaptiveFov::getAdaptiveFov(Render::getSize()));

        Settings::instance();       // settings initialization here
        Config::reload();
        ResourceLibary::instance(); // resource libary initialization here
        initializeFsm();            // FSM initialization here*

        Config::reload();

        Render::getScreenCamera()->setFov(
            games::benice::utils::AdaptiveFov::getAdaptiveFov(Render::getSize()));

        MainUiSwitcher::instance()->resize(Render::getSize());
        m_oldConfigData = Config::getData();
        m_isLandscape = Render::getAspect() >= 1;
        Audio::setGlobalGain(1.0);
    }
    
    void onResize(const math::size& newSize) override
    {
        /*if (m_camera && m_isLandscape != Render::getAspect() >= 1)
        {
            W4_LOG_DEBUG("qqq Change aspect, restart");
            m_isLandscape = Render::getAspect() >= 1.f;
            MainFSM::instance()->processEvent(MainFSM::FSM::Events::Restart);
        }*/
    }

    void initializeUi() {
        W4_LOG_DEBUG("qqq initializeUi()");
        if (m_cameraUi)
        {
            if (m_cameraNode)
            {
                m_cameraNode->removeChild(m_cameraUi);
                m_cameraNode = nullptr;
            }
            m_cameraUi = nullptr;
        }
        m_cameraUi = make::sptr<Node>("CameraUI");
        m_cameraNode->addChild(m_cameraUi, false);
        MainUiSwitcher::instance().createScenes(m_cameraUi, m_cameraNode, m_gameComponent);
    }


    void initializeFsm() {
        //*************************PRELOADING****************************************
        MainFSM::instance()->state<MainFSM::FSM::States::Preloading>().onEnterFunc = [&](auto &, auto &,
                                                                 auto &) {
            W4_LOG_INFO("qqq Preloading onEnter"); 

            
            if (m_gameComponent) //restart
            {
                W4_LOG_DEBUG("qqq restart");
                m_gameComponent->show(false);

                /*MainUiSwitcher::instance()->
                MainUiSwitcher::instance()->destroyScenes();*/
                MainUiSwitcher::instance().destroyScenes();
                m_startScreenTexturePreloader = nullptr;
                m_gameScreenTexturePreloader = nullptr;
                m_gameComponent = nullptr;
                m_SpadesMania_Place_card = nullptr;
                m_gameComponent = make::sptr<GameComponent>();
                MainUiSwitcher::instance()->resize(Render::getSize());
            }
            else
            {
                m_gameComponent = make::sptr<GameComponent>();
            }

            Config::reload();

            Render::getScreenCamera()->setFov(
                games::benice::utils::AdaptiveFov::getAdaptiveFov(Render::getSize()));
            platform::Platform::hideLoader();
            //platform::Platform::showLoader();

            // Preload UiSwitcher::TC_START textures
            std::vector<std::string> textureList =
            {
                "textures/bg_landscape.jpg",
                "textures/bg_portrait.jpg",
                "textures/text/loading.png"
            };
            m_startScreenTexturePreloader = make::sptr<TexturePreloader>();
            m_startScreenTexturePreloader->intialize([](){
                platform::Platform::hideLoader();
                MainFSM::instance()->processEvent(MainFSM::FSM::Events::Initialize);
            }, textureList);
        };
        MainFSM::instance()->state<MainFSM::FSM::States::Preloading>().onUpdateFunc = [this](auto&, float dt)
        {
            if (m_startScreenTexturePreloader != nullptr)
                m_startScreenTexturePreloader->update(dt);
        };
        MainFSM::instance()->state<MainFSM::FSM::States::Preloading>().onLeaveFunc =
                [&](auto &, auto &, auto &) { 
            W4_LOG_INFO("Preloading onLeave"); 

            initializeUi();             // UI initialization here
            m_touchEndHandler = Touch::End::subscribe(std::bind(
                    &gameApp::onTouchEnd, this, std::placeholders::_1));
            m_touchMoveHandler = event::Touch::Move::subscribe(std::bind(
                    &gameApp::onTouchMove, this, std::placeholders::_1));
            m_touchBeginHandler = event::Touch::Begin::subscribe(std::bind(
                    &gameApp::onTouchBegin, this, std::placeholders::_1));

            MainUiSwitcher::instance()->resize(Render::getSize());

            m_oldConfigData = Config::getData(); 

            m_SpadesMania_Place_card 
                = make::sptr<Audio>("sounds/SpadesMania_Place_card.mp3");
        };

        //*************************MAIN MENU***************************************
        MainFSM::instance()->state<MainFSM::FSM::States::StartMenu>().onEnterFunc = [&](auto &, auto &,
                                                               auto &) {
            W4_LOG_INFO("StartMenu onEnter");
            //auto points = Inventory::instance().getPoints();
            //m_label->m_label->setText(utils::format("Points: %d", points));
               
            MainUiSwitcher::instance()->switchUi(MainUiSwitcher::TC_START);

            // Preload TC_GAME textures
            std::vector<std::string> textureList =
            {
                "textures/blackout.png",
                "textures/1px.png",
                "textures/1px_black.png",
                "textures/Ace_Of_Diamonds.png",
                "textures/Ace_Of_Hearts.png",
                "textures/Five_Of_Hearts.png",
                "textures/Four_Of_Clubs.png",
                "textures/Four_Of_Hearts.png",
                "textures/Jack_Of_Hearts.png",
                "textures/King_Of_Diamonds.png",
                "textures/King_Of_Hearts.png",
                "textures/Nine_Of_Clubs.png",
                "textures/Nine_Of_Clubs_2x.png",
                "textures/Nine_Of_Hearts.png",
                "textures/Queen_Of_Clubs.png",
                "textures/Queen_Of_Hearts.png",
                "textures/Seven_Of_Hearts.png",
                "textures/Seven_Of_Spades.png",
                "textures/Seven_Of_Spades_2x.png",
                "textures/Six_Of_Clubs.png",
                "textures/Ten_Of_Clubs.png",
                "textures/Ten_Of_Clubs_2x.png",
                "textures/Three_Of_Clubs.png",
                "textures/Two_Of_Clubs.png",
                "textures/Two_Of_Clubs_2x.png",
                "textures/Two_Of_Hearts.png",
                "textures/awatar_01.png",
                "textures/awatar_02.png",
                "textures/awatar_03(2).png",
                "textures/awatar_03.png",
                "textures/awatar_04(2).png",
                "textures/awatar_04(3).png",
                "textures/awatar_04.png",
                "textures/bg.png",
                "textures/bg_fix.jpg",
                "textures/bg_game.png",
                "textures/bg_landscape.jpg",
                "textures/bg_portrait.jpg",
                "textures/card_back_1.png",
                "textures/download_btn.png",
                "textures/download_btn_fix.png",
                "textures/game_bg_landscape.jpg",
                "textures/game_bg_portrait.jpg",
                "textures/icn_hand_512.png",
                "textures/jack_of_clubs.png",
                "textures/text/loading.png",
                "textures/text/got_it.png",
                "textures/text/i_win.png",
                "textures/text/no_in_app_purchases.png",
                "textures/text/partner_wins.png",
                "textures/text/partner_wins_2.png",
                "textures/text/put_the_highest_card.png",
                "textures/text/spades_beat_everything.png",
                "textures/text/spades_broken.png",
                "textures/text/start_with_smaller_card.png",
                "textures/rays_blend.png",
                "textures/spades.png",
                "textures/spades_mania_icon.png",
                "textures/spades_mania_logo.png",
                "textures/spades_mania_logo_fix.png"
            };
            if (m_gameScreenTexturePreloader)
                m_gameScreenTexturePreloader = nullptr;
            m_gameScreenTexturePreloader = make::sptr<TexturePreloader>();
            m_gameScreenTexturePreloader->intialize([](){
                MainFSM::instance()->processEvent(MainFSM::FSM::Events::Next);
            }, textureList);
        };
        MainFSM::instance()->state<MainFSM::FSM::States::StartMenu>().onUpdateFunc = [this](auto&, float dt)
        {
            if (m_gameScreenTexturePreloader != nullptr)
                m_gameScreenTexturePreloader->update(dt);
        };
        MainFSM::instance()->state<MainFSM::FSM::States::StartMenu>().onLeaveFunc =
                [&](auto &, auto &, auto &) { 
            W4_LOG_INFO("StartMenu onLeave"); 
            //platform::Platform::hideLoader();
        };

        //***************************GAME_PROCESS********************************************
        MainFSM::instance()->state<MainFSM::FSM::States::Game>().onEnterFunc = [&](auto &, auto &, auto &) {
            W4_LOG_INFO("Game onEnter");

            MainUiSwitcher::instance()->switchUi(MainUiSwitcher::TC_GAME);
            //GameComponent& gameComponent = m_cameraNode->getFirstComponent<GameComponent>();
            m_gameComponent->show(true);
            
            UiLayoutComponent& gameLayout = MainUiSwitcher::instance()->getUiComponent();
            
            m_gameComponent->setup(MainUiSwitcher::instance()->getUiNode(),
                m_cameraUi, 
                m_cameraNode,
                m_camera,
                gameLayout.getRoot());
                //Render::getRoot());
                //m_cameraUi);

            sptr<LayoutButton> downLoadBtn = gameLayout.getButton("GameDownloadBtn");
            downLoadBtn->setClickCallback([this](){
                W4_LOG_DEBUG("Call CTA");
                w4::network::ads::CTA();
                if (m_SpadesMania_Place_card != nullptr) {
                    m_SpadesMania_Place_card->play();
                }
            });

            m_isGameStarted = true;
        };
        MainFSM::instance()->state<MainFSM::FSM::States::Game>().onLeaveFunc = [&](auto &, auto &, auto &) {
            W4_LOG_INFO("Game onLeave");
            //GameComponent& gameComponent = m_cameraNode->getFirstComponent<GameComponent>();
            m_gameComponent->show(false);
            //m_cameraNode->template removeAllComponents<GameComponent>();
        };
        MainFSM::instance()->state<MainFSM::FSM::States::Game>().onUpdateFunc = [this](auto&, float dt)
        {
            m_gameComponent->update(dt);
            /*if (m_cameraNode->getAllComponents<GameComponent>().size() == 1)
            {
                //GameComponent& gameComponent = m_cameraNode->getFirstComponent<GameComponent>();
                m_gameComponent.update(dt);
            } else {
              W4_LOG_ERROR("wrong GameComponent count in gameLayout.onUpdateFunc");
            }*/
        };

        //***************************RESULT_PROCESS********************************************
        MainFSM::instance()->state<MainFSM::FSM::States::Result>().onEnterFunc = [&](auto &, auto &, auto &) {
            W4_LOG_INFO("Result onEnter");
 
            MainUiSwitcher::instance()->switchUi(MainUiSwitcher::TC_RESULT);
            
            UiLayoutComponent& resultLayout = MainUiSwitcher::instance()->getUiComponent();

            sptr<LayoutButton> downLoadBtn = resultLayout.getButton("EndDownloadBtn");
            downLoadBtn->setClickCallback([this](){
                W4_LOG_DEBUG("Call CTA"); 
                w4::network::ads::CTA();
                if (m_SpadesMania_Place_card != nullptr) {
                    m_SpadesMania_Place_card->play();
                }
            });
        };
        MainFSM::instance()->state<MainFSM::FSM::States::Result>().onLeaveFunc = [&](auto &, auto &, auto &) {
            W4_LOG_INFO("Result onLeave");
        };

        //***********************LAUNCH FIRST STATE*********************************
        MainFSM::instance()->init<MainFSM::FSM::States::Preloading>(MainFSM::FSM::Events::Initialize);
        W4_LOG_DEBUG("qqq FSM initialized");
    }

    void onTouchEnd(const Touch::End &evt) {
        /*W4_LOG_DEBUG("onTouchEnd");
        auto ray = m_camera->createRayFromScreen(evt.point);
        ray.length = 100;
        auto hit = Render::raycast(ray);
        if (hit.target != nullptr && hit.target->getParent() != nullptr) {
            W4_LOG_DEBUG("[RAYCAST] hitted %s",
                         hit.target->getParent()->getName().c_str());
        }*/
    }

    void onTouchBegin(const Touch::Begin& evt)
    {
        /*W4_LOG_DEBUG("onTouchBegin");
        auto ray = m_screenCamera->createRayFromScreen(evt.point);
        ray.length = 10;
        auto hit = Render::raycast(ray);
        if (hit.target != nullptr && hit.target->getParent() != nullptr)
        {
            W4_LOG_DEBUG("hit something");
            std::string name = hit.target->getParent()->getName();
        }*/
    }

    void onTouchMove(const Touch::Move& evt)
    {
        /*W4_LOG_DEBUG("onTouchMove");
        auto ray = m_screenCamera->createRayFromScreen(evt.point);
        ray.length = 10;
        auto hit = Render::raycast(ray);
        if (hit.target != nullptr)
        {
            std::string name = hit.target->getParent()->getName();
        }*/
    }

    void onUpdate(float dt) override
    {
        if (m_isPaused) {
            return;
        }
        MainFSM::instance()->update(dt); 
        m_time += dt;
    }

    void onPause() override
    {
        m_isPaused = true;
        mute(true);
        //Platform::showLoader();
    }

    void onResume() override
    {
        m_isPaused = false;
        mute(false);
        //Platform::hideLoader();
    }
    void mute(float flag)
    {
        m_isMuted = flag;
        updateMuteState();
    }
    void switchMute()
    {
        m_isMuted = !m_isMuted;
        updateMuteState();
    }
    
    void updateMuteState()
    {
        if (m_isMuted)
        {
            Audio::setGlobalGain(0);
        }
        else
        {
            Audio::setGlobalGain(1.0);
        }
    }
private:
    sptr<Audio> m_SpadesMania_Place_card;

    sptr<Node> m_gameRoot;
    sptr<event::Touch::Begin::Handle> m_touchBeginHandler;
    sptr<event::Touch::End::Handle> m_touchEndHandler;
    sptr<event::Touch::Move::Handle> m_touchMoveHandler;

    float m_time;
    //w4::sptr<Mesh> m_plane;

    sptr<Camera> m_camera;
    sptr<Node> m_cameraUi;
    sptr<Node> m_cameraNode;

    sptr<GameComponent> m_gameComponent;
    
    ConfigData m_oldConfigData{};
    sptr<TexturePreloader> m_startScreenTexturePreloader;
    sptr<TexturePreloader> m_gameScreenTexturePreloader;
    bool m_isMuted{false};
    bool m_isPaused{false};
    bool m_isLandscape{ false };
    bool m_isGameStarted{ false };
};

W4_RUN(gameApp)
