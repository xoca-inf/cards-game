#pragma once

#include "W4Framework.h"
#include "ui/UiParser.h"
#include "components/UiLayoutComponent.h"
#include "UiWindows/GameWorldUi.h"
#include "UiWindows/GameDomUi.h"
#include <chrono>
#include <thread>
#include "../helper/LocalSettings.h"

using namespace std::literals::chrono_literals;
using TimerTaskPool = TaskPool<void (float)>;
using TimerHandle = TimerTaskPool::Handle;

W4_USE_UNSTRICT_INTERFACE

using CardClickCallback 
  = std::function<void(
      sptr<VisibleNode> card_front,
      const std::string card_name,
      const CollisionInfo &collisionInfo)>;

struct CardTemplate {
  nlohmann::Pivot pivot{nlohmann::Pivot::CenterCenter};
  vec3 rotation{0.0f};
  vec3 size{};
  vec3 offset{};
  std::string name{};
  std::string frontTexture{};
  std::string backTexture{};
  float renderOrder{};
};

struct CardNode {
  CardNode(sptr<VisibleNode> node, 
  const vec3& offsetDiff, 
  const nlohmann::json& document)
  : node(node)
  , offsetDiff(offsetDiff)
  , document(document)
  {}

  sptr<VisibleNode> node{nullptr};
  vec3 offsetDiff{};
  nlohmann::json document;
};

class CardScaleComponent : public IComponent{
    W4_COMPONENT(CardScaleComponent, IComponent)
    W4_COMPONENT_DISABLE_CLONING
public:
    CardScaleComponent(CloneTag, const CardScaleComponent& from, Node& owner)
    : Super(CloneTag{}, from, owner)
    {}
    virtual ~CardScaleComponent();
    void initializeFsm();
    void reset();
    void setup(float delaySeconds, float tweenDuration, 
      float tweenSpeed, vec3 startScale, vec3 endScale);

    void setTweenDoneCallback(std::function<void()>&& cb) {
        m_onTweenDone = std::move(cb);
    }

    void setTweenStartCallback(std::function<void()>&& cb) {
        m_onTweenStart = std::move(cb);
    }
    
    void onEnabled(bool value) override;

    virtual void initialize(const variant::Variant& data) override;
    virtual void update(float dt) override;
    virtual void finalize() override;

    void scaleCard(float dt);

    size_t tweenDoneCount() const {
        return m_tweenDoneCount;
    }

    vec3 startPos() const {
      return m_startScale;
    }

    vec3 endPos() const {
      return m_endScale;
    }

private:
    float m_timeElapsed{0.0f};
    size_t m_tweenDoneCount{0};

    float m_delaySeconds{0.0f};
    float m_tweenSpeed = 0.07f;
    sptr<Tween<vec3>> m_tween;
    float m_tweenTime{0.0f};
    float m_tweenTimeout{0.0f};
    vec3 m_startScale = { 0.f, 0.f, 0.f };
    vec3 m_endScale = { 0.f, 0.f, 0.f };
    std::function<void()> m_onTweenDone;
    std::function<void()> m_onTweenStart;
};

class CardMoveComponent : public IComponent{
    W4_COMPONENT(CardMoveComponent, IComponent)
    W4_COMPONENT_DISABLE_CLONING
public:
    CardMoveComponent(CloneTag, const CardMoveComponent& from, Node& owner)
    : Super(CloneTag{}, from, owner)
    {}
    virtual ~CardMoveComponent();
    void initializeFsm();
    void reset();
    void setup(float delaySeconds, float tweenDuration, 
      float tweenSpeed, vec3 startPos, vec3 endPos);

    void setTweenDoneCallback(std::function<void()>&& cb) {
        m_onTweenDone = std::move(cb);
    }

    void setTweenStartCallback(std::function<void()>&& cb) {
        m_onTweenStart = std::move(cb);
    }
    
    void onEnabled(bool value) override;

    virtual void initialize(const variant::Variant& data) override;
    virtual void update(float dt) override;
    virtual void finalize() override;

    void moveCard(float dt);

    size_t tweenDoneCount() const {
        return m_tweenDoneCount;
    }

    vec3 startPos() const {
      return m_startMovePos;
    }

    vec3 endPos() const {
      return m_endMovePos;
    }

private:
    float m_timeElapsed{0.0f};
    size_t m_tweenDoneCount{0};

    float m_moveDelaySeconds{0.0f};
    float m_moveTweenSpeed = 0.07f;
    sptr<Tween<vec3>> m_cardMoveTween;
    float m_moveTweenTime{0.0f};
    float m_moveTweenTimeout{0.0f};
    vec3 m_startMovePos = { 0.f, 0.f, 0.f };
    vec3 m_endMovePos = { 0.f, 0.f, 0.f };
    std::function<void()> m_onTweenDone;
    std::function<void()> m_onTweenStart;
};

class CardRotationComponent : public IComponent{
    W4_COMPONENT(CardRotationComponent, IComponent)
    W4_COMPONENT_DISABLE_CLONING
public:
    CardRotationComponent(CloneTag, const CardRotationComponent& from, Node& owner)
    : Super(CloneTag{}, from, owner)
    {}
    virtual ~CardRotationComponent();
    void initializeFsm();
    void reset();
    void setup(float delaySeconds, float tweenDuration, 
      float tweenSpeed, float startFlipDegree, float endFlipDegree);

    void setTweenDoneCallback(std::function<void()>&& cb) {
        m_onTweenDone = std::move(cb);
    }

    void setTweenStartCallback(std::function<void()>&& cb) {
        m_onTweenStart = std::move(cb);
    }
    
    void onEnabled(bool value) override;

    virtual void initialize(const variant::Variant& data) override;
    virtual void update(float dt) override;
    virtual void finalize() override;

    void rotateCard(float dt);

    size_t tweenDoneCount() const {
        return m_tweenDoneCount;
    }

    float startFlipDegree() const {
      return m_startFlipDegree;
    }

    float endFlipDegree() const {
      return m_endFlipDegree;
    }

private:
    float m_timeElapsed{0.0f};
    size_t m_tweenDoneCount{0};
    float m_flipDelaySeconds{0.0f};
    float m_flipTweenTime{0.0f};
    float m_flipTweenTimeout{0.0f};
    sptr<Tween<float>> m_cardFlipTween;
    float m_flipTweenSpeed = 1.0f;
    float m_startFlipDegree = PI;
    float m_endFlipDegree = 2 * PI;
    std::function<void()> m_onTweenDone;
    std::function<void()> m_onTweenStart;
};

//class GameComponent : public IComponent{
//    W4_COMPONENT(GameComponent, IComponent)
//    W4_COMPONENT_DISABLE_CLONING
class GameComponent {
public:
    struct FSM
    {
        enum class Events
        {
            Preloading,
            Next
        };
        struct States
        {
            struct Initialize : w4::fsm::FuncState<Events>
            {
            };

            struct Turn1Initialize : w4::fsm::FuncState<Events>
            {
            };
            struct Turn1 : w4::fsm::FuncState<Events>
            {
            };

            struct Turn2Initialize : w4::fsm::FuncState<Events>
            {
            };
            struct Turn2 : w4::fsm::FuncState<Events>
            {
            };

            struct Turn3Initialize : w4::fsm::FuncState<Events>
            {
            };
            struct Turn3 : w4::fsm::FuncState<Events>
            {
            };

            struct Turn4Initialize : w4::fsm::FuncState<Events>
            {
            };
            struct Turn4 : w4::fsm::FuncState<Events>
            {
            };

            struct Turn5Initialize : w4::fsm::FuncState<Events>
            {
            };
            struct Turn5 : w4::fsm::FuncState<Events>
            {
            };

            struct Finish : w4::fsm::FuncState<Events>
            {
            };
        };
        using Type = w4::fsm::FSM<
            w4::fsm::Transition<Events::Next, States::Initialize, States::Turn1Initialize>,
            
            w4::fsm::Transition<Events::Next, States::Turn1Initialize, States::Turn1>,
            w4::fsm::Transition<Events::Next, States::Turn1, States::Turn2Initialize>,

            w4::fsm::Transition<Events::Next, States::Turn2Initialize, States::Turn2>,
            w4::fsm::Transition<Events::Next, States::Turn2, States::Turn3Initialize>,

            w4::fsm::Transition<Events::Next, States::Turn3Initialize, States::Turn3>,
            w4::fsm::Transition<Events::Next, States::Turn3, States::Turn4Initialize>,

            w4::fsm::Transition<Events::Next, States::Turn4Initialize, States::Turn4>,
            w4::fsm::Transition<Events::Next, States::Turn4, States::Turn5Initialize>,

            w4::fsm::Transition<Events::Next, States::Turn5Initialize, States::Turn5>,
            w4::fsm::Transition<Events::Next, States::Turn5, States::Finish>
        >;
    };

    GameComponent()
    {
      m_scaleModificator = { 1.0f, 1.0f, 1.0f };
      m_posModificator = { 0.5f, 0.0f, 0.0f };

      resetAnimateHand();
    }

    /*GameComponent(CloneTag, const GameComponent& from, Node& owner)
    : Super(CloneTag{}, from, owner)
    {}*/

    virtual ~GameComponent();
    void initializeFsm();

    void glowMiddleCard(float effectDuration);

    void show(bool value);
    bool isShown() const {
      return m_isShown;
    }
    void resize(const math::size& newSize);
    
    void setFinishCallback(std::function<void()>&&);
    const std::function<void()>& finishCallback() const {
        return m_finishCallback;
    }

    void onTouchEnd(const Touch::End &evt);

    void onTouchBegin(const Touch::Begin& evt);

    void onTouchMove(const Touch::Move& evt);

    void hideAllPlayerIcons();

    void moveCardZUnderBlackout(sptr<CardNode> cardNode);

    void moveCardZAboveBlackout(sptr<CardNode> cardNode);

    void moveTurnPicksBackAndReset(const nlohmann::json& json_turnPicks);

    void movePlayerCardToTrash(sptr<CardNode> cardNode);
    
    sptr<LayoutImage> findAwatar(const std::string& awatar_name);

    void update(float dt);

    void animateHand(float dt);
    void resetAnimateHand();

    CardRotationComponent& flipCard(sptr<VisibleNode> card,
      float delaySeconds = 0.9f,
      float tweenTimeout = 2.0f,
      float tweenSpeed = 1.0f,
      float startFlipDegree = PI, 
      float endFlipDegree = 2 * PI);

    CardMoveComponent& moveCard(sptr<VisibleNode> card,
      float delaySeconds,
      float tweenTimeout,
      float tweenSpeed,
      vec3 startPos, 
      vec3 endPos);

    CardScaleComponent& scaleCard(sptr<VisibleNode> card,
      float delaySeconds,
      float tweenTimeout,
      float tweenSpeed,
      vec3 startPos, 
      vec3 endPos);

    CardScaleComponent& scaleImage(const std::string& imageName, float effectDuration, 
      float scaleTweenTimeout,
      float scaleTweenSpeed,
      float scaleTweenDelaySeconds,
      vec3 endScale,
      vec3 startScale);
    
    CardScaleComponent& playZoomInTween(const std::string& effectName, 
      float effectDuration);

    CardScaleComponent& playZoomOutTween(sptr<VisibleNode> card, 
      float effectDuration);

    void setup(sptr<Node> layoutNode,
      sptr<Node> cameraUi, 
      sptr<Node> cameraNode,
      sptr<Camera> camera,
      sptr<Node> cards_root = nullptr,
      const std::string& json_filepath = "cards.json");

    void updateCardFromTemplate(sptr<CardNode> card, const CardTemplate& cardTemplate);

    sptr<CardNode> getCardAtRight();

    sptr<Collider> addCardClickCallback(sptr<CardNode> cardNode, 
        const std::string& colliderName, const std::string& cardName,
        CardClickCallback&& cardClickCallback);
    
    void highlightCard(sptr<CardNode> cardNode);
    
    void showImageForTime(const std::string& effectName, float effectDuration);

    void pointAtCard(sptr<CardNode> cardNode);

    void enableBlackout(bool);

    sptr<VisibleNode> createCard(nlohmann::Pivot pivot, 
      vec3 buttonRotation, vec3 size, vec3 offset, 
      const std::string& name, float renderOrder, 
      sptr<Node> root, const std::string& frontTexture, 
      const std::string& backTexture, bool enabled = true);

    void resetTurnPicks(const nlohmann::json& json_turnPicks);
    
    void removeAnimationComponents(sptr<VisibleNode> card); 

    void removeCardFromPicks(sptr<CardNode> cardNode);

    void removeCardFromDeck(sptr<CardNode> cardNode);

    void moveCardBelowTopCenterCard(sptr<CardNode> cardNode, 
      std::function<void()>&& onCardMovedCb);

    void delayedRemoveCardAndGoNext(sptr<CardNode> cardNode, 
      float delaySec);

    void setAudio();

    template<typename T, typename... Args>
    T& setupAnimation(sptr<VisibleNode> node, Args&&... args) {
      node->removeAllComponents<T>();
      //
      T& component 
        = node->addComponent<T>();
      m_animatedNodes[node->getName()] = node;
      component.setup(std::forward<Args>(args)...);
      return component;
    }

  void playSound(sptr<Audio> sound) {
    if (sound != nullptr) {
      sound->stop();
      sound->play();
    }
  }

  TimerHandle addTimerTask(float finishDelay, 
    std::function<void()>&& callback,
    bool isLooped = false)
  {
    return Timer::addTask(finishDelay, [this, cb = std::move(callback)]() 
    {
      if (cb) {
        cb();
      }
      return false;
    }, isLooped);
  }

private:
  std::vector<std::string> m_clickedCards{};

  bool m_isShown{false};

  bool m_canClickCard{false};

  sptr<Audio> m_After_loading_SpadesMania_Deal_cards;
  sptr<Audio> m_SpadesMania_End_round_anim_label_fan;
  sptr<Audio> m_SpadesMania_Place_card;
  sptr<Audio> m_SpadesMania_Take_trick;
    
  sptr<Camera> m_camera;

  sptr<LayoutImage> m_rays_blend;
  sptr<LayoutImage> m_spades;
  float m_raysRotator{};
  float m_raysRotateSpeed{1.0f};
  bool m_isNeedToAnimateRays{true};

  sptr<event::Touch::Begin::Handle> m_touchBeginHandler;
  sptr<event::Touch::End::Handle> m_touchEndHandler;
  sptr<event::Touch::Move::Handle> m_touchMoveHandler;

  std::map<std::string, sptr<VisibleNode>> m_animatedNodes;

  std::vector<std::string> m_cardPicksDeck{};
  std::map<std::string, sptr<CardNode>> m_cardPicks;

  std::vector<sptr<Collider>> m_cardColliders{};
  
  sptr<Mesh> m_noUIblackout;
  
  std::vector<std::string> m_cardDeck{};
  std::map<std::string, sptr<CardNode>> m_cardsFromDeck;

  std::map<std::string, sptr<CardNode>> m_cardsToUpdate;

  sptr<Node> m_layoutNode{nullptr};
  sptr<GameWorldUi> m_worldUi;
  sptr<GameDomUi> m_domUi;

  FSM::Type m_fsm;
  std::function<void()> m_finishCallback;

  ConfigData m_oldConfigData{};
  std::string m_filenameFull;
  CardTemplate m_defaultCardTemplate;

  float m_timeElapsed{0.0f};
  nlohmann::json m_parsedCardsFile; 

  vec3 m_scaleModificator;
  vec3 m_posModificator;
  bool m_isNeedToAnimateHand{false};
  sptr<Node> m_handNode;
  float m_handRotator;
  float m_handRotateSpeed;
  vec3 m_handMoveVec;

  sptr<Mesh> m_glow;
  sptr<ParticlesEmitter> m_emitter;
  sptr<LocalSettings> m_helper;
  int m_muteCountSound = 3;
  int m_moveTrashIndex = 0;
};
