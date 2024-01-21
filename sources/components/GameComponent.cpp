#include "GameComponent.h"
#include "settings/ResourceLibary.h"
#include "settings/Settings.h"
#include "components/UiLayoutComponent.h"
#include "ui/UiSwitcher.h"

template<typename T>
void setParamInAllJsonNodes(nlohmann::json& document, 
  const std::string& name, const T& value) 
{
  document[name] = value;
  if (document.contains("landscape"))
    document["landscape"][name] = value;
  if (document.contains("portrait"))
    document["portrait"][name] = value;
}

void CardScaleComponent::initialize(const variant::Variant& data) 
{
  IComponent::initialize(data);
  W4_LOG_DEBUG("[CardScaleComponent] INITIALIZE");
}

void CardScaleComponent::setup( 
  float delaySeconds, float tweenDuration, 
  float tweenSpeed, vec3 startScale, vec3 endScale)
{
  //m_cardNode = cardNode;
  m_tweenSpeed = tweenSpeed;
  m_delaySeconds = delaySeconds;
  m_startScale = startScale;
  m_endScale = endScale;
  m_tweenTimeout = tweenDuration;
  reset();
}

void CardScaleComponent::reset() {
  m_tweenTime = 0.0f;
  m_timeElapsed = 0.0f;
  m_tween = make::sptr<Tween<vec3>>(m_startScale);
  m_tween->add(m_endScale, m_tweenTimeout, 
    easing::Linear<vec3>);
}

void CardScaleComponent::finalize() {
  W4_LOG_INFO("CardScaleComponent will be removed");
  IComponent::finalize();
}

void CardScaleComponent::onEnabled(bool value)
{
  W4_LOG_DEBUG("[CardScaleComponent] onEnabled");
  IComponent::onEnabled(value);
}

void CardScaleComponent::update(float deltha) {
  if (!isEnabled() || !getOwner().isEnabled() || m_tweenDoneCount > 0) {
    IComponent::update(deltha);
    return;
  }

  float dt = deltha * 2.0f;

  m_timeElapsed += dt;

  scaleCard(dt);

  IComponent::update(deltha);
}

void CardScaleComponent::scaleCard(float dt)
{
  if (m_tween && m_timeElapsed > m_delaySeconds)
  {
      if (m_onTweenStart && m_tweenTime <= 0.0) {
        m_onTweenStart();
      }
      m_tweenTime += dt * m_tweenSpeed;
      getOwner().setWorldScale(m_tween->getValue(m_tweenTime));
      //if (m_cardNode != nullptr) {
        //setParamInAllJsonNodes(m_cardNode->document, 
        //  "rotation", rotVec);
      //}
      if (m_tweenTime > m_tweenTimeout) {
        //reset();
        //enable(false);
        if (m_onTweenDone) {
          m_onTweenDone();
        }
        m_tweenDoneCount++;
      }
  }
}

CardScaleComponent::~CardScaleComponent() {
  W4_LOG_DEBUG("[CardScaleComponent] Destructor called!");
  /*enable(false);
  reset();
  m_tween = nullptr;*/
}

// -----------------------------------------

void CardMoveComponent::initialize(const variant::Variant& data) 
{
  IComponent::initialize(data);
  W4_LOG_DEBUG("[CardMoveComponent] INITIALIZE");
}

void CardMoveComponent::setup(
  float delaySeconds, float tweenDuration, 
  float tweenSpeed, vec3 startPos, vec3 endPos)
{
  //m_cardNode = cardNode;
  m_moveTweenSpeed = tweenSpeed;
  m_moveDelaySeconds = delaySeconds;
  m_startMovePos = startPos;
  m_endMovePos = endPos;
  m_moveTweenTimeout = tweenDuration;
  reset();
}

void CardMoveComponent::reset() {
  m_moveTweenTime = 0.0f;
  m_timeElapsed = 0.0f;
  m_cardMoveTween = make::sptr<Tween<vec3>>(m_startMovePos);
  m_cardMoveTween->add(m_endMovePos, m_moveTweenTimeout, 
    easing::Linear<vec3>);
}

void CardMoveComponent::finalize() {
  W4_LOG_INFO("CardMoveComponent will be removed");
  IComponent::finalize();
}

void CardMoveComponent::onEnabled(bool value)
{
  W4_LOG_DEBUG("[CardMoveComponent] onEnabled");
  IComponent::onEnabled(value);
}

void CardMoveComponent::update(float deltha) {
  if (!isEnabled() || !getOwner().isEnabled() || m_tweenDoneCount > 0) {
    IComponent::update(deltha);
    return;
  }

  float dt = deltha * 2.0f;

  m_timeElapsed += dt;

  moveCard(dt);

  IComponent::update(deltha);
}

void CardMoveComponent::moveCard(float dt)
{
  if (m_cardMoveTween && m_timeElapsed > m_moveDelaySeconds)
  {
      if (m_onTweenStart && m_moveTweenTime <= 0.0) {
        m_onTweenStart();
      }
      m_moveTweenTime += dt * m_moveTweenSpeed;
      getOwner().setWorldTranslation(m_cardMoveTween->getValue(m_moveTweenTime));
      /*if (m_cardNode != nullptr) {
        //setParamInAllJsonNodes(m_cardNode->document, 
        //  "rotation", rotVec);
      }*/
      if (m_moveTweenTime > m_moveTweenTimeout) {
        //reset();
        //enable(false);
        if (m_onTweenDone) {
          m_onTweenDone();
        }
        m_tweenDoneCount++;
      }
  }
}


CardMoveComponent::~CardMoveComponent() {
  W4_LOG_DEBUG("[CardMoveComponent] Destructor called!");
  /*enable(false);
  reset();
  m_cardMoveTween = nullptr;*/
}

// -----------------------------------------

void CardRotationComponent::initialize(const variant::Variant& data) 
{
  IComponent::initialize(data);
  W4_LOG_DEBUG("[CardRotationComponent] INITIALIZE");
}

void CardRotationComponent::setup(float delaySeconds, float tweenDuration, 
  float tweenSpeed, float startFlipDegree, float endFlipDegree)
{
  //m_cardNode = cardNode;
  m_flipTweenSpeed = tweenSpeed;
  m_flipDelaySeconds = delaySeconds;
  m_startFlipDegree = startFlipDegree;
  m_endFlipDegree = endFlipDegree;
  m_flipTweenTimeout = tweenDuration;
  reset();
}

void CardRotationComponent::reset() {
  m_flipTweenTime = 0.0f;
  m_timeElapsed = 0.0f;
  m_cardFlipTween = make::sptr<Tween<float>>(m_startFlipDegree);
  m_cardFlipTween->add(m_endFlipDegree, m_flipTweenTimeout, 
    easing::Linear<float>);
}

void CardRotationComponent::finalize() {
  W4_LOG_INFO("CardRotationComponent will be removed");
  IComponent::finalize();
}

void CardRotationComponent::onEnabled(bool value)
{
  W4_LOG_DEBUG("[CardRotationComponent] onEnabled");
  IComponent::onEnabled(value);
}

void CardRotationComponent::update(float deltha) {
  if (!isEnabled() || !getOwner().isEnabled() || m_tweenDoneCount > 0) {
    IComponent::update(deltha);
    return;
  }

  float dt = deltha * 2.0f;

  m_timeElapsed += dt;

  rotateCard(dt);

  IComponent::update(deltha);
}

void CardRotationComponent::rotateCard(float dt)
{
  if (m_cardFlipTween && m_timeElapsed > m_flipDelaySeconds)
  {
      if (m_onTweenStart && m_flipTweenTime <= 0.0) {
        m_onTweenStart();
      }
      m_flipTweenTime += dt * m_flipTweenSpeed;
      vec3 rotVec(0, m_cardFlipTween->getValue(m_flipTweenTime), 0);
      getOwner().setLocalRotation(/*getOwner().getLocalRotation()) 
        +*/ Rotator(rotVec));
      /*if (m_cardNode != nullptr) {
        setParamInAllJsonNodes(m_cardNode->document, 
          "rotation", rotVec);
      }*/
      if (m_flipTweenTime > m_flipTweenTimeout) {
        //reset();
        //enable(false);
        if (m_onTweenDone) {
          m_onTweenDone();
        }
        m_tweenDoneCount++;
      }
  }
}

CardRotationComponent::~CardRotationComponent() {
  W4_LOG_DEBUG("[CardRotationComponent] Destructor called!");
  /*enable(false);
  reset();
  m_cardFlipTween = nullptr;*/
}

// -----------------------------------------

template<typename T>
T getValueOr(const nlohmann::json &document, const std::string& keyId, const T& fallback) {
  nlohmann::json value;
  // per-orientation config
  if (document.contains(Config::getData().orientation)) {
    value = document[Config::getData().orientation];
    //if(value.contains(keyId))
    //  W4_LOG_DEBUG("value.contains(%s) %s", keyId.c_str(), document.dump().c_str());
  } 
  return value.contains(keyId) ? value[keyId].get<T>() 
          : (document.contains(keyId) ? document[keyId].get<T>() : fallback);
}

static CardTemplate cardTemplateFromJson(
  const nlohmann::json& document, 
  std::optional<CardTemplate> defaults)
{
  CardTemplate result;
  
  result.renderOrder = getValueOr<float>(document,
    "renderOrder", defaults.has_value() 
    ? defaults->renderOrder
    : 0.0f);

  result.frontTexture = getValueOr<std::string>(document, 
    "frontTexture", defaults.has_value() 
    ? defaults->frontTexture
    : "Ace_Of_Diamonds");

  result.backTexture = getValueOr<std::string>(document, 
    "backTexture", defaults.has_value() 
    ? defaults->backTexture 
    : "card_back_1");
  
  result.pivot = getValueOr<nlohmann::Pivot>(document, 
    "pivot", defaults.has_value() 
    ? defaults->pivot
    : nlohmann::Pivot::CenterCenter);

  result.offset = getValueOr<vec3>(document, 
    "offset", defaults.has_value() 
    ? defaults->offset
    : vec3(0.0, 0.01, 0.0f));

  result.size = getValueOr<vec3>(document, 
    "size", defaults.has_value() 
    ? defaults->size
    : vec3(0.03, 0.035, 0.0001f));

  result.rotation = getValueOr<vec3>(document, 
    "rotation", defaults.has_value() 
    ? defaults->rotation
    : vec3(0.0, PI, 0.0));

  //Rotator rot = Rotator(result.rotation * vec3(math::DEG2RAD));
  //result.rotation = rot.toVector();

  return result;
}

void GameComponent::onTouchEnd(const Touch::End &evt) {
    /*W4_LOG_DEBUG("onTouchEnd {%d, %d}", evt.point.x, evt.point.y);
    auto ray = m_camera->createRayFromScreen(evt.point);
    ray.length = 100;
    auto hit = Render::raycast(ray);
    if (hit.target != nullptr && hit.target->getParent() != nullptr) {
        W4_LOG_DEBUG("[RAYCAST] hitted %s",
                      hit.target->getParent()->getName().c_str());
    }*/
}

void GameComponent::onTouchBegin(const Touch::Begin& evt)
{
    /*W4_LOG_DEBUG("onTouchBegin {%d, %d}", evt.point.x, evt.point.y);
    auto ray = m_camera->createRayFromScreen(evt.point);
    ray.length = 100;
    auto hit = Render::raycast(ray);
    if (hit.target != nullptr && hit.target->getParent() != nullptr) {
        W4_LOG_DEBUG("[RAYCAST] hitted %s",
                      hit.target->getParent()->getName().c_str());
    }*/
}

void GameComponent::onTouchMove(const Touch::Move& evt)
{
    /*W4_LOG_DEBUG("onTouchMove {%d, %d}", evt.point.x, evt.point.y);
    auto ray = m_camera->createRayFromScreen(evt.point);
    ray.length = 100;
    auto hit = Render::raycast(ray);
    if (hit.target != nullptr && hit.target->getParent() != nullptr) {
        W4_LOG_DEBUG("[RAYCAST] hitted %s",
                      hit.target->getParent()->getName().c_str());
    }*/
}

void GameComponent::setup(sptr<Node> layoutNode,
  sptr<Node> cameraUi, 
  sptr<Node> cameraNode,
  sptr<Camera> camera,
  sptr<Node> cards_root,
  const std::string& json_filepath) 
{
  m_layoutNode = layoutNode;  
  m_camera = camera;
  m_touchEndHandler = Touch::End::subscribe(std::bind(
    &GameComponent::onTouchEnd, this, std::placeholders::_1));
  m_touchMoveHandler = event::Touch::Move::subscribe(std::bind(
    &GameComponent::onTouchMove, this, std::placeholders::_1));
  m_touchBeginHandler = event::Touch::Begin::subscribe(std::bind(
    &GameComponent::onTouchBegin, this, std::placeholders::_1));
  
  setAudio();

  InGameUiSwitcher::instance().createScenes(cameraUi, cameraNode);

  UiLayoutComponent& gameLayout 
    = m_layoutNode->getFirstComponent<UiLayoutComponent>();

  {
      m_helper = make::sptr<LocalSettings>();
    m_noUIblackout = Mesh::create::plane({ 100.f, 100.f });

    sptr<LayoutButton> downLoadBtn = MainUiSwitcher::instance()->getButton("GameDownloadBtn");
    
    W4_LOG_DEBUG("downLoadBtn->m_image->m_node->getWorldTranslation().z %f", 
      downLoadBtn->m_image->m_mesh->getWorldTranslation().z);
    m_noUIblackout->setWorldTranslation({0.f, 0.f, 
      downLoadBtn->m_image->m_mesh->getWorldTranslation().z});

    auto mat = Material::get("materials/simple.mat")->createInstance();
    mat->setTexture(w4::resources::TextureId::TEXTURE_0, Texture::get("textures/blackout.png"));
    m_noUIblackout->setMaterialInst(mat);
    m_noUIblackout->setRenderOrder(80);
    Render::getRoot()->addChild(m_noUIblackout);
    m_noUIblackout->setEnabled(false);

    m_glow = Mesh::create::plane({ 1.0f, 1.0f });
    vec3 pos = { 0.f, 0.005f, downLoadBtn->m_image->m_mesh->getWorldTranslation().z };
    m_glow->setWorldTranslation(pos);
    auto glowMat = Material::get("materials/simple.mat")->createInstance();
    glowMat->setParam("texture0", Texture::get("textures/card_glow.png"));
    m_glow->setMaterialInst(glowMat);
    m_glow->setRenderOrder(50);
    Render::getRoot()->addChild(m_glow);
    m_glow->setEnabled(false);

    pos = { 0.f, 0.005f, -19.8 };
    float particleScale = 0.001f;
    m_emitter = ParticlesEmitter::get("particles/particle.part");
    m_emitter->setWorldScale({ particleScale, particleScale, particleScale });
    m_emitter->setWorldTranslation(pos);
    m_emitter->setTransformMode(ParticlesEmitterParameters::TransformMode::EMITTER);
    m_emitter->pause();
    m_emitter->setRenderOrder(Settings::instance()->getInt("emitterRenderOrder"));
  }

  m_worldUi = gameLayout.getWorldUi<GameWorldUi>();
  m_domUi = gameLayout.getDomUi<GameDomUi>();
  
  m_filenameFull = json_filepath;
  auto file = w4::filesystem::open(m_filenameFull);
  const nlohmann::json& json 
    = nlohmann::json::parse(file->data(), file->data() + file->size());
  m_parsedCardsFile = json;

  if (json.contains("StartingDeck")) {
    m_cardDeck.clear();
    const nlohmann::json& json_starting_deck 
      = json["StartingDeck"];
    const std::vector<std::string>& startingDeck 
      = json_starting_deck.get<std::vector<std::string>>();
    for (const std::string& value : startingDeck)
    {
      m_cardDeck.push_back(value);
    }
  }

  {
    m_defaultCardTemplate = cardTemplateFromJson(
          json["DefaultCardTemplate"], 
          std::nullopt);
    vec3 distBetweenCards(-m_defaultCardTemplate.size.x / 2.0, 0, 0);
    vec3 maxDist = (vec3(m_defaultCardTemplate.size.x, 0.0f, 0.0f) 
      + distBetweenCards) * (float)(m_cardDeck.size());
    float maxRenderOrder = m_defaultCardTemplate.renderOrder 
      + m_cardDeck.size() + 1;
    
    for(int i = 0; i < m_cardDeck.size(); i++) {
      std::string card_name = m_cardDeck.at(i);
      nlohmann::json document = json["DefaultCardTemplate"];
      CardTemplate cardTemplate = m_defaultCardTemplate;
      if (json.contains(card_name)) {
        document = json[card_name];
      }
      setParamInAllJsonNodes(document, 
        "frontTexture", card_name);
      setParamInAllJsonNodes(document, 
        "renderOrder", /*maxRenderOrder -*/ (float)(i));
      cardTemplate = cardTemplateFromJson(
        document, 
        cardTemplate);
      vec3 cardOffsetDiff = 
        - maxDist / 2.0
        + distBetweenCards / 2.0
        + cardTemplate.size.x / 2.0
        + (float)(i) * distBetweenCards
        + vec3((float)(i) * cardTemplate.size.x, 0.0f, 0.0f);
      float zOffset = -(float)(i + 1) * (0.0001f + cardTemplate.size.z);
      //cardOffsetNew = cardOffset;
      cardOffsetDiff.z += zOffset;
      sptr<VisibleNode> card = createCard(cardTemplate.pivot, 
        cardTemplate.rotation, 
        cardTemplate.size, 
        cardTemplate.offset + cardOffsetDiff, 
        card_name, 
        cardTemplate.renderOrder, 
        gameLayout.getRoot(), 
        cardTemplate.frontTexture, 
        cardTemplate.backTexture, 
        isShown());
      sptr<CardNode> newCard = make::sptr<CardNode>(
        card, cardOffsetDiff, document);
      moveCardZUnderBlackout(newCard);
      m_cardsFromDeck[card_name] = newCard;
      float moveTweenDelay = 0.0f;
      float moveTweenTimeout = 2.0f;
      float moveTweenSpeed = m_helper->getFloat("moveTweenSpeed"); //1.0f
      float flipDelaySeconds = moveTweenTimeout + 1.0f * (float)(i + 1);
      float flipTweenTimeout = moveTweenTimeout + 0.5f;
      float flipTweenSpeed = m_helper->getFloat("flipTweenSpeed"); //1.0f
      float startFlipDegree = PI;
      float endFlipDegree = 2.0 * PI;
      
      CardRotationComponent& cardRotationComponent
          = flipCard(newCard->node, flipDelaySeconds, flipTweenTimeout, 
        flipTweenSpeed, startFlipDegree, endFlipDegree);
      float sound0 = m_helper->getFloat("sound0");
      cardRotationComponent.setTweenStartCallback([this, sound0]() {
        addTimerTask(sound0 / 2.0f,
          [this]() 
          {
            playSound(m_SpadesMania_Take_trick);
            return false;
          });
      });

      vec3 startMovePos = { 0.2, -0.02, Settings::instance()->getFloat("DefaultZOffset") };
      vec3 endMovePos = newCard->node->getWorldTranslation();
      CardMoveComponent& cardMoveComponent
        = moveCard(newCard->node, moveTweenDelay, moveTweenTimeout, 
            moveTweenSpeed, startMovePos, endMovePos);
      cardMoveComponent.setTweenDoneCallback([this](){
        // animation done...
      });
    }
  }

  initializeFsm();
}

sptr<LayoutImage> GameComponent::findAwatar(const std::string& awatar_name)
{
  sptr<LayoutImage> layoutImage = MainUiSwitcher::instance()->getImage(awatar_name);
  /// \note assume we have <= 5 options of same awatar, just get any option
  for (int i = 0; i < 5; i++) 
  {
    if (!layoutImage) {
      layoutImage = MainUiSwitcher::instance()->getImage(
        awatar_name + "("+std::to_string(i)+")");
    } else {
      break;
    }
  }
  return layoutImage;
}

void GameComponent::movePlayerCardToTrash(sptr<CardNode> cardNode)
{
    int index = 0;
    ++m_moveTrashIndex;
    float moveTweenTimeout = m_helper->getFloat("toTrashMoveTweenTimeout");//2.0f;

    float flipTweenTimeout = m_helper->getFloat("toTrashFlipTweenTimeout"); // 2.0f;
    float flipTweenSpeed = m_helper->getFloat("toTrashFlipTweenSpeed") + 0.25f * index;//5.0f + 0.25f * index;

    float moveTweenSpeed = m_helper->getFloat("toTrashmoveTweenSpeed") + 0.5f * index;//1.5f + 0.5f * index;

    float scaleTweenTimeout = 2.0f;
    float scaleTweenSpeed = 1.0f;

    float flipTweenDelaySeconds = 0.5f * index;
    float moveTweenDelay = flipTweenTimeout + 1.0f * index;
    float scaleTweenDelaySeconds = flipTweenTimeout + 0.5f * index;
    std::string trash_node_name = cardNode->document["cardsTrash"];
    if (m_moveTrashIndex == 2 || m_moveTrashIndex == 4)
        trash_node_name = "cards_trash_bin_2";
    {
        sptr<LayoutImage> layoutImage = findAwatar(trash_node_name);
        cardNode->node->setRenderOrder(layoutImage->m_mesh->getRenderOrder() - 1);
        cardNode->node->setWorldTranslation(vec3(
            cardNode->node->getWorldTranslation().x,
            cardNode->node->getWorldTranslation().y,
            layoutImage->m_mesh->getWorldTranslation().z));
    }

    vec3 startScale = cardNode->node->getWorldScale();

    float startFlipDegree = 2.0 * PI; //cardNode->node->getWorldTranslation().y;
    float endFlipDegree = /*startFlipDegree +*/ 3.0 * PI;

    CardRotationComponent& cardRotationComponent
        = flipCard(cardNode->node, flipTweenDelaySeconds, flipTweenTimeout,
            flipTweenSpeed, startFlipDegree, endFlipDegree);
    float sound1 = m_helper->getFloat("sound1");
    cardRotationComponent.setTweenStartCallback([this, sound1]() {
        addTimerTask(sound1 / 2.0f,
            [this]()
            {
                //playSound(m_SpadesMania_Take_trick);
                return false;
            });
        });

    sptr<LayoutImage> layoutImage = findAwatar(trash_node_name);

    vec3 endScale{ startScale * (0.7f - 0.05f * index) };
    scaleCard(cardNode->node, scaleTweenDelaySeconds, scaleTweenTimeout,
        scaleTweenSpeed, startScale, endScale);
    vec3 startMovePos = vec3(cardNode->node->getWorldTranslation().x,
        cardNode->node->getWorldTranslation().y,
        layoutImage->m_node->getWorldTranslation().z);
    //cardNode->node->getWorldTranslation().z);

  //startMovePos.z = zOffset;
    auto getMovePoint = [layoutImage, cardNode, trash_node_name]()
    {
        //sptr<core::OBB> oBB = make::sptr<core::OBB>(layoutImage->m_mesh.get());
        if (layoutImage) {
            return vec3(
                layoutImage->m_node->getWorldTranslation().x //+ oBB->getSize().x * 0.02f
                , layoutImage->m_node->getWorldTranslation().y //- oBB->getSize().y * 0.02f
                , layoutImage->m_node->getWorldTranslation().z);
            //, zOffset);
            //, cardNode->node->getWorldTranslation().z);
        }
        return vec3(0.0); // fallback
    };
    vec3 endMovePos = getMovePoint(); //fly player's card from table

    CardMoveComponent& cardMoveComponent
        = moveCard(cardNode->node, moveTweenDelay, moveTweenTimeout,
            moveTweenSpeed, startMovePos, endMovePos);
    //cardMoveComponent.setTweenDoneCallback(
    //    [this, json_turnPicks, cardNode]()
    //    {
    //        //playSound(m_After_loading_SpadesMania_Deal_cards);
    //        cardNode->node->setWorldScale(vec3(0.0f));
    //    });

}

void GameComponent::moveTurnPicksBackAndReset(const nlohmann::json& json_turnPicks)
{
  if (m_cardPicksDeck.size() != 3) {
    W4_LOG_ERROR("m_cardPicksDeck.size() != 3. Size is %d", m_cardPicksDeck.size());
  }
  
  float moveTweenTimeout = 2.0f;

  float index = 0;
  for (auto it = m_cardPicksDeck.rbegin(); it != m_cardPicksDeck.rend(); it++) 
  {
    float flipTweenTimeout = m_helper->getFloat("flipTweenTimeout_0");//2.0f; 
    float flipTweenSpeed = 5.0f + 0.25f * index;

    float moveTweenSpeed = m_helper->getFloat("moveTweenSpeed_0") + 0.5f * index;;//1.5f + 0.5f * index;

    float scaleTweenTimeout = m_helper->getFloat("scaleTweenTimeout_0");//2.0f;
    float scaleTweenSpeed = m_helper->getFloat("scaleTweenSpeed_0");//1.0f; 

    float flipTweenDelaySeconds = 0.5f * index;
    float moveTweenDelay = flipTweenTimeout + 0.5f * index;
    float scaleTweenDelaySeconds = flipTweenTimeout + 0.5f * index;
    
    auto it2 = m_cardPicks.find(*it);
    if (it2 == m_cardPicks.end()) {
      W4_LOG_ERROR("resetTurnPicks failed for %s", it->c_str());
    }
    auto cardNode = it2->second;

    const std::string trash_node_name = cardNode->document["cardsTrash"];
    
    // card must fall under avatar
    {
      sptr<LayoutImage> layoutImage = findAwatar(trash_node_name);
      cardNode->node->setRenderOrder(layoutImage->m_mesh->getRenderOrder() - 1);
      cardNode->node->setWorldTranslation(vec3(
        cardNode->node->getWorldTranslation().x,
        cardNode->node->getWorldTranslation().y,
        layoutImage->m_mesh->getWorldTranslation().z));
    }
    
    vec3 startScale = cardNode->node->getWorldScale();

    float startFlipDegree = 2.0 * PI; //cardNode->node->getWorldTranslation().y;
    float endFlipDegree = /*startFlipDegree +*/ 3.0 * PI;

    CardRotationComponent& cardRotationComponent
        = flipCard(cardNode->node, flipTweenDelaySeconds, flipTweenTimeout, 
            flipTweenSpeed, startFlipDegree, endFlipDegree);
    cardRotationComponent.setTweenStartCallback([this, flipTweenTimeout]() {
      addTimerTask(0.1f / 2.0f, 
        [this]() 
        {
          //playSound(m_SpadesMania_Take_trick);
          return false;
        });
    });
    
    sptr<LayoutImage> layoutImage = findAwatar(trash_node_name);

    vec3 endScale{startScale * (0.7f - 0.05f * index)};
    scaleCard(cardNode->node, scaleTweenDelaySeconds, scaleTweenTimeout, 
      scaleTweenSpeed, startScale, endScale);
    vec3 startMovePos = vec3(cardNode->node->getWorldTranslation().x,
      cardNode->node->getWorldTranslation().y,
      layoutImage->m_node->getWorldTranslation().z);
      //cardNode->node->getWorldTranslation().z);

    //startMovePos.z = zOffset;
    auto getMovePoint = [layoutImage, cardNode, trash_node_name]()
    {
      //sptr<core::OBB> oBB = make::sptr<core::OBB>(layoutImage->m_mesh.get());
      if (layoutImage) {
        return vec3(
          layoutImage->m_node->getWorldTranslation().x //+ oBB->getSize().x * 0.02f
          , layoutImage->m_node->getWorldTranslation().y //- oBB->getSize().y * 0.02f
          , layoutImage->m_node->getWorldTranslation().z);
          //, zOffset);
          //, cardNode->node->getWorldTranslation().z);
      }
      return vec3(0.0); // fallback
    };
    vec3 endMovePos = getMovePoint(); //fly opponent's card from table

    /*float renderOrder = layoutImage->m_mesh->getRenderOrder();
    cardNode->node->setRenderOrder(renderOrder - 1);
    addTimerTask(moveTweenDelay + moveTweenTimeout / 1.2f, 
      [this, cardNode, renderOrder = layoutImage->m_mesh->getRenderOrder()]() 
      {
        cardNode->node->setRenderOrder(renderOrder - 1);
        cardNode->node->setWorldScale(vec3(0.0f));
        return false;
      });*/

    CardMoveComponent& cardMoveComponent
      = moveCard(cardNode->node, moveTweenDelay, moveTweenTimeout, 
          moveTweenSpeed, startMovePos, endMovePos);
    cardMoveComponent.setTweenDoneCallback(
      [this, json_turnPicks, cardNode]()
      {
        //playSound(m_After_loading_SpadesMania_Deal_cards);
        cardNode->node->setWorldScale(vec3(0.0f));
      });
      
    addTimerTask(0.01f, 
      [this]() 
      {
        playSound(m_After_loading_SpadesMania_Deal_cards);
        return false;
      });
    index += 1.0f;
  }

  addTimerTask(moveTweenTimeout + 0.5f, [this, json_turnPicks]() 
  {
    resetTurnPicks(json_turnPicks);
    return false;
  });
}

void GameComponent::resetTurnPicks(const nlohmann::json& json_turnPicks)
{
  //return;
  //
  for (auto it = m_cardPicksDeck.rbegin(); it != m_cardPicksDeck.rend(); it++) 
  {
    auto it2 = m_cardPicks.find(*it);
    if (it2 == m_cardPicks.end()) {
      W4_LOG_ERROR("resetTurnPicks failed for %s", it->c_str());
    }
    auto cardNode = it2->second;
    removeCardFromPicks(cardNode);
  }
  //
  UiLayoutComponent& gameLayout 
    = m_layoutNode->getFirstComponent<UiLayoutComponent>();

  int cardIndex = 0;
  for (nlohmann::json document : json_turnPicks) //TODO card
  {
    std::string card_name = document["frontTexture"].get<std::string>();
    m_cardPicksDeck.push_back(card_name);
    //setParamInAllJsonNodes(document, 
    //  "renderOrder", 999.0f); /// \todo
    CardTemplate cardTemplate = cardTemplateFromJson(
      document, 
      m_defaultCardTemplate);
    //cardTemplate.renderOrder = 999.0f; /// \todo
    sptr<VisibleNode> card = createCard(cardTemplate.pivot, 
      cardTemplate.rotation, 
      cardTemplate.size, 
      cardTemplate.offset, 
      card_name, 
      cardTemplate.renderOrder, 
      gameLayout.getRoot(), 
      cardTemplate.frontTexture, 
      cardTemplate.backTexture, 
      isShown());
    sptr<CardNode> newCard = make::sptr<CardNode>(
      card, vec3(), document);
    m_cardPicks[card_name] = newCard;

    card->setLocalTranslation(card->getLocalTranslation() + vec3(0.0, 0.0001f, 0.0));
    moveCardZAboveBlackout(newCard);
    float delaySeconds = m_helper->getFloat("cardPlayerStartDelaySeconds"); // 1.0f;
    float flipTweenTimeout = 1.5f; //2.5
    float flipTweenSpeed = m_helper->getFloat("cardPlayerStartflipTweenSpeed"); //5.0f;
    float startFlipDegree = PI;
    float endFlipDegree = 2.0 * PI;
    float moveTweenTimeout = flipTweenTimeout + 2.0f;
    float moveTweenDelay = m_helper->getFloat("cardPlayerDelay") * cardIndex; //delay 1.0
    float moveTweenSpeed = m_helper->getFloat("cardPlayermoveTweenSpeed"); //4.0f;
    CardRotationComponent& cardRotationComponent
        = flipCard(newCard->node, delaySeconds + moveTweenDelay, flipTweenTimeout,
            flipTweenSpeed, startFlipDegree, endFlipDegree);
    if (m_muteCountSound > 0)
    {
        --m_muteCountSound;
    }
    else
    {
        cardRotationComponent.setTweenStartCallback([this, flipTweenTimeout]() {
            addTimerTask(0.5f / 2.f,
                [this]()
                {
                    playSound(m_SpadesMania_Take_trick);
                    return false;
                });
            });
    }

    

    vec3 endMovePos = newCard->node->getWorldTranslation(); //move card from opponent's and player to center table
    newCard->node->setWorldTranslation({-10.f, 0.f, -19.9});
    //const std::string awatar_name = *(document["playerAwatar"].end()-1);
    const std::string awatar_name = document["playerAwatar"];
    sptr<LayoutImage> layoutImage = findAwatar(awatar_name);
    auto getMovePoint = [layoutImage, awatar_name]()
    {
      //sptr<core::OBB> oBB = make::sptr<core::OBB>(layoutImage->m_mesh.get());
      if (layoutImage) {
        return vec3(
          layoutImage->m_node->getWorldTranslation().x //+ oBB->getSize().x * 0.02f
          , layoutImage->m_node->getWorldTranslation().y //- oBB->getSize().y * 0.02f
          , layoutImage->m_node->getWorldTranslation().z);
          //, getOwner().getWorldTranslation().z);
      }
      return vec3(0.0); // fallback
    };
    vec3 startMovePos = getMovePoint();
    //vec3 startMovePos = { 0.2, -0.02, Settings::instance()->getFloat("DefaultZOffset") };
    CardMoveComponent& cardMoveComponent
        = moveCard(newCard->node, moveTweenDelay, moveTweenTimeout, 
      moveTweenSpeed, startMovePos, endMovePos);
    cardMoveComponent.setTweenDoneCallback([this]() {
      //playSound(m_After_loading_SpadesMania_Deal_cards);
    });
    ++cardIndex;
  }
}

CardRotationComponent& GameComponent::flipCard(sptr<VisibleNode> card,
  float delaySeconds,
  float tweenTimeout,
  float tweenSpeed,
  float startFlipDegree, 
  float endFlipDegree)
{
  /*const std::unordered_set<CardRotationComponent*>& components 
    = card->getAllComponents<CardRotationComponent>();
  if (!components.empty()) {
    for (CardRotationComponent* component : components) {
      if (component != nullptr) {
        component->reset();
      }
    }
  }*/

  {
    CardRotationComponent& cardRotationComponent 
      = setupAnimation<CardRotationComponent>(card, delaySeconds, 
          tweenTimeout, tweenSpeed, startFlipDegree, endFlipDegree);
    return cardRotationComponent;
  }
}

CardMoveComponent& GameComponent::moveCard(sptr<VisibleNode> card,
  float delaySeconds,
  float tweenTimeout,
  float tweenSpeed,
  vec3 startPos, 
  vec3 endPos)
{
  /*const std::unordered_set<CardMoveComponent*>& components 
    = card->getAllComponents<CardMoveComponent>();
  if (!components.empty()) {
    for (CardMoveComponent* component : components) {
      if (component != nullptr) {
        component->reset();
      }
    }
  }*/
  
  {
    CardMoveComponent& cardMoveComponent 
      = setupAnimation<CardMoveComponent>(card, delaySeconds, 
          tweenTimeout, tweenSpeed, startPos, endPos);
    return cardMoveComponent;
  }
}

CardScaleComponent& GameComponent::scaleCard(sptr<VisibleNode> card,
  float delaySeconds,
  float tweenTimeout,
  float tweenSpeed,
  vec3 startScale, 
  vec3 endScale)
{
  /*const std::unordered_set<CardScaleComponent*>& components 
    = card->getAllComponents<CardScaleComponent>();
  if (!components.empty()) {
    for (CardScaleComponent* component : components) {
      if (component != nullptr) {
        component->reset();
      }
    }
  }*/
  
  {
    CardScaleComponent& cardScaleComponent 
      = setupAnimation<CardScaleComponent>(card, delaySeconds, 
          tweenTimeout, tweenSpeed, startScale, endScale);
    return cardScaleComponent;
  }
}

void GameComponent::show(bool value)
{
  W4_LOG_DEBUG("[GameComponent] show");

  //m_root->setEnabled(value);

  for (auto& [name, card] : m_animatedNodes) {
    card->setEnabled(value);
    /*card->traversalTyped<VisibleNode>([value](cref<VisibleNode> entry)
      {
          entry->setEnabled(value);
      });*/
  }

  // reset card scale, rotation, position etc.
  //resize(Render::getSize());
  m_isShown = value;
}

static void setUnlitMaterial(sptr<Texture> tex, sptr<VisibleNode> node)
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
  node->traversalTyped<VisibleNode>([&materialInst](cref<VisibleNode> entry)
    {
        entry->setMaterialInst(materialInst);
    });
}

static void setSpriteMaterial(sptr<Texture> tex, sptr<VisibleNode> node)
{
  tex->setSWrapping(Wrapping::Repeat);
  tex->setTWrapping(Wrapping::Repeat);
  auto material = Material::get("materials/sprite.mat");
  sptr<MaterialInst> materialInst 
      = material->createInstance();
  materialInst->setTexture(
      w4::resources::TextureId::TEXTURE_0, tex);
  materialInst->enableBlending(true);
  node->traversalTyped<VisibleNode>([&materialInst](cref<VisibleNode> entry)
    {
        entry->setMaterialInst(materialInst);
    });
}

void GameComponent::resize(const math::size& newSize) {
  W4_LOG_DEBUG("GameComponent::resize");
    
  auto cameraPos = Render::getScreenCamera()->getWorldTranslation();
  auto cameraFrustrum = Render::getScreenCamera()->getFrustum();
  
  MainUiSwitcher::instance()->resize(Render::getSize());

  // re-parse UI from json due to orientation changes
  if (Config::getData().orientation != m_oldConfigData.orientation) 
  {
    // ...
  }
  m_oldConfigData = Config::getData();
}

void GameComponent::updateCardFromTemplate(sptr<CardNode> cardNode, 
  const CardTemplate& config)
{
  auto cameraPos = Render::getScreenCamera()->getWorldTranslation();
  auto cameraFrustrum = Render::getScreenCamera()->getFrustum();
  sptr<VisibleNode> card = cardNode->node;
  card->setLocalTranslation(
    cameraPos + getPivot(config.pivot, cameraFrustrum) 
    + config.offset + cardNode->offsetDiff);
  card->setLocalScale(config.size);
  card->setRenderOrder(config.renderOrder);
  //W4_LOG_DEBUG("UPDATED CARD ROTATION TO %d", config.rotation);
  card->setLocalRotation(config.rotation); //  * vec3(math::DEG2RAD)
}

void GameComponent::enableBlackout(bool value)
{
  if (m_noUIblackout != nullptr)
    m_noUIblackout->setEnabled(value);
}

sptr<VisibleNode> GameComponent::createCard(nlohmann::Pivot pivot, 
  vec3 buttonRotation, vec3 size, vec3 offset, 
  const std::string& name, float renderOrder, sptr<Node> root, 
  const std::string& frontTexture, const std::string& backTexture, 
  bool enabled) 
{
  root = Render::getRoot(); /// \todo

  auto cameraPos = Render::getScreenCamera()->getWorldTranslation();
  auto cameraFrustrum = Render::getScreenCamera()->getFrustum();

  sptr<VisibleNode> card_front = Mesh::create::plane(vec2(1, 1));
  sptr<Texture> tex = ResourceLibary::instance().getTexture(frontTexture);
  setSpriteMaterial(tex, card_front);
  //card_front->setWorldTranslation(pos);
  card_front->setEnabled(enabled);
  card_front->setName(name);
  card_front->setLocalScale(size * m_scaleModificator);
  card_front->setLocalTranslation(
    cameraPos + getPivot(pivot, cameraFrustrum) 
    + offset);
  card_front->setRenderOrder(renderOrder);
  card_front->setLocalRotation(buttonRotation);
  root->addChild(card_front, true);

  {
    sptr<VisibleNode> card_reversed = Mesh::create::plane(vec2(1, 1));
    sptr<Texture> tex = ResourceLibary::instance().getTexture(backTexture);
    setSpriteMaterial(tex, card_reversed);
    card_reversed->setEnabled(enabled);
    card_reversed->setName(name);
    card_front->addChild(card_reversed, true);
    //card_reversed->setWorldScale(size);
    //card_reversed->setWorldRotation(buttonRotation + vec3(0, PI, 0));
    card_reversed->setLocalRotation(vec3(0, PI, 0));
    //card_reversed->setLocalRotation(buttonRotation + vec3(0, PI, 0));
    //card_reversed->setWorldTranslation(card_front->getWorldTranslation() + vec3(0, 0, -0.01));
    //card_reversed->setLocalScale(size);
    card_reversed->setLocalScale(vec3(1.0f));
    card_reversed->setLocalTranslation(vec3(0, 0, -0.01));
    /*card_reversed->setLocalTranslation(
      cameraPos + getPivot(pivot, cameraFrustrum) 
      + offset + vec3(0, 0, -0.01));*/
    card_reversed->setRenderOrder(renderOrder - 1);
  }

  return card_front;
}

void GameComponent::update(float deltha) {
  if (!isShown())
  {
    return;
  }

  float dt = deltha * 2.0f;
  
  m_fsm.update(dt); 
  
  if (m_isNeedToAnimateRays && m_rays_blend != nullptr && m_spades != nullptr 
      && m_rays_blend->m_mesh->isEnabled() && m_spades->m_mesh->isEnabled())
  {
    m_raysRotator += m_raysRotateSpeed * dt;
    m_rays_blend->m_mesh->setWorldRotation(Rotator{0.f, 0.f, m_raysRotator });
    m_rays_blend->m_mesh->setWorldTranslation(m_spades->m_mesh->getWorldTranslation());
    m_rays_blend->m_mesh->setRenderOrder(m_spades->m_mesh->getRenderOrder() - 1);
  }

#if 0
  for (auto& [name, card] : m_animatedNodes) {
    {
      const std::unordered_set<CardRotationComponent*>& components = card->getAllComponents<CardRotationComponent>();
      std::vector<CardRotationComponent*> componentsToRemove;
      for (CardRotationComponent* component : components) {
        if (component != nullptr) {
          if (component->tweenDoneCount() > 0) {
            component->enable(false);
            componentsToRemove.push_back(component);
          } else {
            //component->update(dt);
          }
        }
      }
      /// \note can not call removeComponent while iterating over getAllComponents, so had to use componentsToRemove
      for (CardRotationComponent* component : componentsToRemove) {
        //
        if (component != nullptr)
          component->getOwner().removeComponent<CardRotationComponent>(component->id());
      }
    }
    {
      const std::unordered_set<CardMoveComponent*>& components = card->getAllComponents<CardMoveComponent>();
      std::vector<CardMoveComponent*> componentsToRemove;
      for (CardMoveComponent* component : components) {
        if (component != nullptr) {
          if (component->tweenDoneCount() > 0) {
            component->enable(false);
            componentsToRemove.push_back(component);
          } else {
            //component->update(dt);
          }
        }
      }
      /// \note can not call removeComponent while iterating over getAllComponents, so had to use componentsToRemove
      for (CardMoveComponent* component : componentsToRemove) {
        //
        if (component != nullptr)
          component->getOwner().removeComponent<CardMoveComponent>(component->id());
      }
    }
    {
      const std::unordered_set<CardScaleComponent*>& components = card->getAllComponents<CardScaleComponent>();
      std::vector<CardScaleComponent*> componentsToRemove;
      for (CardScaleComponent* component : components) {
        if (component != nullptr) {
          if (component->tweenDoneCount() > 0) {
            component->enable(false);
            componentsToRemove.push_back(component);
          } else {
            //component->update(dt);
          }
        }
      }
      /// \note can not call removeComponent while iterating over getAllComponents, so had to use componentsToRemove
      for (CardScaleComponent* component : componentsToRemove) {
        //
        if (component != nullptr)
          component->getOwner().removeComponent<CardScaleComponent>(component->id());
      }
    }
  }
#endif

  m_timeElapsed += dt;

  if (m_isNeedToAnimateHand)
    animateHand(dt);
}

void GameComponent::animateHand(float dt)
{
    if (!m_handNode)
    {
        UiLayoutComponent& gameLayout
            = m_layoutNode->getFirstComponent<UiLayoutComponent>();
        m_handNode = gameLayout.getImage("pointy_hand")->m_node;
    }
    if (m_handNode->isEnabled())
    {
        m_handRotator += m_handRotateSpeed * dt;
        m_handNode->setWorldRotation(Rotator {0.f, 0.f, m_handRotator });
        auto moveStep = m_handMoveVec * dt;
        m_handNode->setWorldTranslation(m_handNode->getWorldTranslation() + moveStep);

        if (m_handRotator >= 0)
        {
            m_handRotator = 0;
            m_handRotateSpeed *= -1;
            m_handMoveVec *= -1;
        }
        if (m_handRotator <= -1)
        {
            m_handRotator = -1;
            m_handRotateSpeed *= -1;
            m_handMoveVec *= -1;
        }
    }
}

void GameComponent::resetAnimateHand()
{
    m_isNeedToAnimateHand = false;
    m_handRotator = 0.f;
    m_handRotateSpeed = 0.8f;
    m_handMoveVec = { -0.01f, 0.01f, 0.f };
}

void GameComponent::setAudio()
{
  if (m_After_loading_SpadesMania_Deal_cards == nullptr)
    m_After_loading_SpadesMania_Deal_cards 
      = make::sptr<Audio>("sounds/After_loading_SpadesMania_Deal_cards.mp3");
  else
    m_After_loading_SpadesMania_Deal_cards->stop();
    
  if (m_SpadesMania_End_round_anim_label_fan == nullptr)
    m_SpadesMania_End_round_anim_label_fan 
      = make::sptr<Audio>("sounds/SpadesMania_End_round_anim_label_fan.mp3");
  else
    m_SpadesMania_End_round_anim_label_fan->stop();

  if (m_SpadesMania_Place_card == nullptr)
    m_SpadesMania_Place_card 
      = make::sptr<Audio>("sounds/SpadesMania_Place_card.mp3");
  else
    m_SpadesMania_Place_card->stop();

  if (m_SpadesMania_Take_trick == nullptr)
    m_SpadesMania_Take_trick 
      = make::sptr<Audio>("sounds/SpadesMania_Take_trick.mp3");
  else
    m_SpadesMania_Take_trick->stop();
}

GameComponent::~GameComponent() {
  W4_LOG_DEBUG("qqq [GAME COMPONENT] Destructor called!");
  
  for (auto& [name, card] : m_animatedNodes) {
    //
    card->template removeAllComponents<CardRotationComponent>();
    card->template removeAllComponents<CardMoveComponent>();
    card->template removeAllComponents<CardScaleComponent>();
  }
  m_animatedNodes.clear();

  m_cardsFromDeck.clear();
  m_cardDeck.clear();

  m_cardPicksDeck.clear();
  m_cardPicks.clear();

  if (m_noUIblackout != nullptr && m_noUIblackout->getParent() != nullptr)
    m_noUIblackout->getParent()->removeChild(m_noUIblackout);
  m_noUIblackout = nullptr;


  if (m_glow != nullptr && m_glow->getParent() != nullptr)
    m_glow->getParent()->removeChild(m_glow);
  m_glow = nullptr;

  if (m_emitter != nullptr)
  {
    m_emitter->stop();
    if (m_emitter->getParent() != nullptr)
      m_emitter->getParent()->removeChild(m_emitter);
    m_emitter = nullptr;
  }

  /*if (m_root != nullptr)
    m_root->getParent()->removeChild(m_root);
  m_root = nullptr;*/
}

void GameComponent::setFinishCallback(std::function<void()>&& cb)
{
  m_finishCallback = std::move(cb);
}

sptr<CardNode> GameComponent::getCardAtRight()
{
  const std::string& card_name = m_cardDeck.back();
  auto iter = m_cardsFromDeck.find(
    card_name);
  if (iter == m_cardsFromDeck.end()) {
    W4_LOG_ERROR("Unable to find card %s", card_name.c_str());
  }
  sptr<CardNode> cardAtRight = iter->second;
  return cardAtRight;
}

void GameComponent::removeAnimationComponents(sptr<VisibleNode> card)
{
  {
    const std::unordered_set<CardRotationComponent*>& components 
      = card->getAllComponents<CardRotationComponent>();
    if (!components.empty()) {
      for (CardRotationComponent* component : components) {
        if (component != nullptr && component->tweenDoneCount() <= 0) {
          component->enable(false);
        }
      }
    } 
    //
    card->template removeAllComponents<CardRotationComponent>();
  }

  {
    const std::unordered_set<CardMoveComponent*>& components 
      = card->getAllComponents<CardMoveComponent>();
    if (!components.empty()) {
      for (CardMoveComponent* component : components) {
        if (component != nullptr && component->tweenDoneCount() <= 0) {
          component->enable(false);
        }
      }
    } 
    //
    card->template removeAllComponents<CardMoveComponent>();
  }

  {
    const std::unordered_set<CardScaleComponent*>& components 
      = card->getAllComponents<CardScaleComponent>();
    if (!components.empty()) {
      for (CardScaleComponent* component : components) {
        if (component != nullptr && component->tweenDoneCount() <= 0) {
          component->enable(false);
        }
      }
    } 
    //
    card->template removeAllComponents<CardMoveComponent>();
  }
}

void GameComponent::removeCardFromDeck(sptr<CardNode> cardNode)
{
  sptr<VisibleNode> card = cardNode->node;
  const std::string card_name = card->getName();
  card->setEnabled(false);

  if (card->getParent() != nullptr)
    card->getParent()->removeChild(card); /// \todo
  
  //return;
  //

  const std::unordered_map<std::string, sptr<Collider>>& collidersMap
    = card->getColliders();
  std::vector<sptr<Collider>> colliders;
  for (auto& [name, collider] : collidersMap) {
    colliders.push_back(collider);
  }
  for (auto it = colliders.rbegin(); it != colliders.rend(); it++)
  {
    sptr<Collider> collider = *it;
    if (collider == nullptr) continue;
    collider->resetRaycastCallback();
    collider->resetScreencastCallback();
    collider->setReceiveRaycasts(false);
    collider->setBlockRaycasts(false);
    collider->setBlockScreencasts(false);
    /// \note do not remove collider while iterating over getColliders()!
    //if (collider->getParent() != nullptr)
    //  collider->getParent()->removeCollider(collider);
    //collider = nullptr;
  }
  {
    auto it = m_cardsFromDeck.find(card_name);
    if (it != m_cardsFromDeck.end())
      m_cardsFromDeck.erase(it);
  }
  {
    auto it = std::find(m_cardDeck.begin(), m_cardDeck.end(), card_name);
    if (it != m_cardDeck.end())
      m_cardDeck.erase(it);
  }
  {
    auto it = m_animatedNodes.find(card_name);
    if (it != m_animatedNodes.end())
      m_animatedNodes.erase(it);
  }

  /// \todo Uncaught RuntimeError: null function or function signature mismatch
  // at w4::core::ComponentsSystem::update(float)
  //removeAnimationComponents(card);
}

void GameComponent::removeCardFromPicks(sptr<CardNode> cardNode)
{
  sptr<VisibleNode> card = cardNode->node;
  const std::string card_name = card->getName();
  card->setEnabled(false);

  if (card->getParent() != nullptr)
    card->getParent()->removeChild(card); /// \todo

  const std::unordered_map<std::string, sptr<Collider>>& collidersMap
    = card->getColliders();
  std::vector<sptr<Collider>> colliders;
  for (auto& [name, collider] : collidersMap) {
    colliders.push_back(collider);
  }
  for (auto it = colliders.rbegin(); it != colliders.rend(); it++)
  {
    sptr<Collider> collider = *it;
    if (collider == nullptr) continue;
    collider->resetRaycastCallback();
    collider->resetScreencastCallback();
    collider->setReceiveRaycasts(false);
    collider->setBlockRaycasts(false);
    collider->setBlockScreencasts(false);
    /// \note do not remove collider while iterating over getColliders()!
    //if (collider->getParent() != nullptr)
    //  collider->getParent()->removeCollider(collider);
    //collider = nullptr;
  }
  {
    auto it = m_cardPicks.find(card_name);
    if (it != m_cardPicks.end())
      m_cardPicks.erase(it);
  }
  {
    auto it = std::find(m_cardPicksDeck.begin(), m_cardPicksDeck.end(), card_name);
    if (it != m_cardPicksDeck.end())
      m_cardPicksDeck.erase(it);
  }
  {
    auto it = m_animatedNodes.find(card_name);
    if (it != m_animatedNodes.end())
      m_animatedNodes.erase(it);
  }

  /// \todo Uncaught RuntimeError: null function or function signature mismatch
  // at w4::core::ComponentsSystem::update(float)
  //removeAnimationComponents(card);
}

void GameComponent::moveCardZUnderBlackout(sptr<CardNode> cardNode) {
  //
  sptr<VisibleNode> card = cardNode->node;
  if (card->getRenderOrder() == m_noUIblackout->getRenderOrder() + 1) {
    card->setRenderOrder(m_noUIblackout->getRenderOrder() - 1);
  }
  card->setWorldTranslation(vec3(card->getWorldTranslation().x, 
    card->getWorldTranslation().y, 
    Settings::instance()->getFloat("DefaultCardZOffset")));
    //+ cardNode->offsetDiff.z));
      //m_noUIblackout->getWorldTranslation().z - 0.0001f));
}

void GameComponent::moveCardZAboveBlackout(sptr<CardNode> cardNode) {
  //
  sptr<VisibleNode> card = cardNode->node;
  card->setRenderOrder(m_noUIblackout->getRenderOrder() + 1);
  card->setWorldTranslation(vec3(card->getWorldTranslation().x, 
    card->getWorldTranslation().y, 
    m_noUIblackout->getWorldTranslation().z));
}

void GameComponent::pointAtCard(sptr<CardNode> cardNode)
{
  MainUiSwitcher::instance()->enableImage(true, "pointy_hand");

  //return;//

  UiLayoutComponent& gameLayout 
    = m_layoutNode->getFirstComponent<UiLayoutComponent>();
  sptr<LayoutImage> layoutImage = gameLayout.getImage("pointy_hand");
  sptr<core::OBB> cardBB = make::sptr<core::OBB>(cardNode->node.get());
  if (layoutImage != nullptr) {
    if (layoutImage->m_node != nullptr) {
      //layoutImage->m_node->setRenderOrder(cardNode->node->getRenderOrder() + 1);
      layoutImage->m_node->setWorldTranslation(vec3(
        cardNode->node->getWorldTranslation().x + cardBB->getSize().x * 0.02f
        , cardNode->node->getWorldTranslation().y - cardBB->getSize().y * 0.02f
        , cardNode->node->getWorldTranslation().z));
    }
    if (layoutImage->m_mesh != nullptr) {
      layoutImage->m_mesh->setRenderOrder(cardNode->node->getRenderOrder() + 1);
      layoutImage->m_mesh->setWorldTranslation(vec3(
        /// \todo fix offset from (+0.007; -0.01f) to (???;???)
        cardNode->node->getWorldTranslation().x + cardBB->getSize().x * 0.02f + 0.007f
        , cardNode->node->getWorldTranslation().y - cardBB->getSize().y * 0.02f - 0.01f
        , cardNode->node->getWorldTranslation().z));
    }
  }
  m_isNeedToAnimateHand = true;

  //
  //m_isNeedToAnimateHand = false;
}

CardScaleComponent& GameComponent::scaleImage(const std::string& imageName, float effectDuration, 
  float scaleTweenTimeout,
  float scaleTweenSpeed,
  float scaleTweenDelaySeconds,
  vec3 startScale,
  vec3 endScale)
{
  sptr<LayoutImage> layoutImage = MainUiSwitcher::instance()->getImage(imageName);

  std::string currentUiName = MainUiSwitcher::instance()->currentUiName();
  w4::sptr<Mesh> mesh = layoutImage->m_mesh;

  return scaleCard(mesh, scaleTweenDelaySeconds, scaleTweenTimeout, 
    scaleTweenSpeed, startScale, endScale);
}

CardScaleComponent& GameComponent::playZoomInTween(const std::string& effectName, float effectDuration)
{
  sptr<LayoutImage> layoutImage = MainUiSwitcher::instance()->getImage(effectName);
  vec3 endScale = layoutImage->m_mesh->getWorldScale();
  vec3 startScale = layoutImage->m_mesh->getWorldScale() * vec3(0.5f);
  float tweenTimeout = effectDuration;
  float tweenSpeed = 1.0f;
  float delaySeconds = 0.0f;
  CardScaleComponent& component = scaleImage(
    effectName, effectDuration, tweenTimeout,tweenSpeed, delaySeconds, startScale, endScale);
  return component;
}

CardScaleComponent& GameComponent::playZoomOutTween(sptr<VisibleNode> node,
  float effectDuration)
{
  vec3 startScale = node->getWorldScale();
  vec3 endScale = node->getWorldScale() * vec3(0.0f);
  float tweenTimeout = effectDuration;
  float tweenSpeed = 1.0f;
  float delaySeconds = 0.0f;
  CardScaleComponent& component = scaleCard(
    node, delaySeconds, tweenTimeout,tweenSpeed, startScale, endScale);
  return component;
}

void GameComponent::showImageForTime(const std::string& effectName, float effectDuration)
{
  MainUiSwitcher::instance()->enableImage(true, effectName);

  std::string currentUiName = MainUiSwitcher::instance()->currentUiName();

  addTimerTask(effectDuration + 0.001f, [effectName, currentUiName]() 
  {
    /// \note remember ui name because it may change
    MainUiSwitcher::instance()->enableImage(false, effectName, currentUiName);
    return false;
  });
}

void GameComponent::highlightCard(sptr<CardNode> cardNode)
{
  auto cameraPos = Render::getScreenCamera()->getWorldTranslation();
  auto cameraFrustrum = Render::getScreenCamera()->getFrustum();

  W4_LOG_DEBUG("GameComponent::highlightCard %s", 
    cardNode->node->getName().c_str());
    
  setParamInAllJsonNodes(cardNode->document, 
    "renderOrder", 999.0);
  cardNode->offsetDiff += vec3(0.0f, 0.01f, 0.0f);
  UiLayoutComponent& gameLayout 
    = m_layoutNode->getFirstComponent<UiLayoutComponent>();
  
  sptr<VisibleNode> card = cardNode->node;
  CardTemplate cardTemplate = cardTemplateFromJson(
    cardNode->document, m_defaultCardTemplate);

  float moveTweenTimeout = 1.5f;
  float moveTweenDelay = 0.0f;
  float moveTweenSpeed = 2.0f;
  vec3 startMovePos = card->getLocalTranslation();
  vec3 endMovePos = card->getLocalTranslation()
    + vec3(0.0f, 0.01f, 0.0f);
  CardMoveComponent& cardMoveComponent
    = moveCard(cardNode->node, moveTweenDelay, moveTweenTimeout, 
        moveTweenSpeed, startMovePos, endMovePos);
  cardMoveComponent.setTweenDoneCallback([this, cardNode](){
    moveCardZAboveBlackout(cardNode);
    pointAtCard(cardNode);
    m_canClickCard = true;
  });

  float scaleTweenTimeout = moveTweenTimeout;
  float scaleTweenSpeed = moveTweenSpeed;
  float scaleTweenDelaySeconds = moveTweenDelay;
  vec3 endScale = card->getWorldScale() * vec3(1.1f);
  vec3 startScale = card->getWorldScale();
  scaleCard(card, scaleTweenDelaySeconds, scaleTweenTimeout, 
    scaleTweenSpeed, startScale, endScale);

  /*float maxTime = std::max(scaleTweenDelaySeconds + scaleTweenTimeout, 
    moveTweenDelay + moveTweenTimeout);
  addTimerTask(maxTime + 0.01f, [this, cardNode]() 
  {
    m_canClickCard = true;
    return false;
  });*/
}

sptr<Collider> GameComponent::addCardClickCallback(sptr<CardNode> cardNode, 
  const std::string& colliderName, const std::string& cardName,
  CardClickCallback&& cardClickCallback)
{
  sptr<Collider> collider = cardNode->node->addCollider(colliderName,
    make::sptr<core::AABB>(cardNode->node));
  collider->setReceiveRaycasts(true);
  collider->setBlockRaycasts(false);
  /// \note we use multiple coliders, so do not em make block each other
  collider->setBlockScreencasts(false); 
  collider->setScreencastCallback(
    [this, callback = cardClickCallback, cardNode, name = cardName, collider]
    (const CollisionInfo &collisionInfo)
  {
    if (!m_canClickCard) {
      return;
    }

    auto it = std::find(m_clickedCards.begin(), m_clickedCards.end(), name);
    if (it != m_clickedCards.end())
      return;
    m_clickedCards.push_back(name);

    if (callback != nullptr)
    {
      callback(cardNode->node, name, collisionInfo);
    }

    if (collider != nullptr)
    {
      collider->resetRaycastCallback();
      collider->resetScreencastCallback();
      collider->setReceiveRaycasts(false);
      collider->setBlockRaycasts(false);
      collider->setBlockScreencasts(false);
    }
  });
  return collider;
}

void GameComponent::hideAllPlayerIcons()
{
  MainUiSwitcher::instance()->enableImage(false, "awatar_01");
  MainUiSwitcher::instance()->enableImage(false, "awatar_02");

  MainUiSwitcher::instance()->enableImage(false, "awatar_03");
  MainUiSwitcher::instance()->enableImage(false, "awatar_03(2)");

  MainUiSwitcher::instance()->enableImage(false, "awatar_04");
  MainUiSwitcher::instance()->enableImage(false, "awatar_04(2)");
  MainUiSwitcher::instance()->enableImage(false, "awatar_04(3)");
}

void GameComponent::moveCardBelowTopCenterCard(sptr<CardNode> cardNode, 
  std::function<void()>&& onCardMovedCb)
{
  float moveTweenTimeout = 3.0f;
  float moveTweenDelay = 0.8f;
  float moveTweenSpeed = 4.0f;
  vec3 startMovePos = cardNode->node->getWorldTranslation();
  /// \note m_cardPicksDeck.at(1) must be card at top middle
  sptr<CardNode> cardAtTopMiddle = m_cardPicks[m_cardPicksDeck.at(1)];
  vec3 endMovePos = cardAtTopMiddle->node->getWorldTranslation();
  sptr<core::OBB> cardBB = make::sptr<core::OBB>(cardNode->node.get());
  float cardSizeY = cardBB->getSize().y * 0.05f;
  endMovePos.y -= (cardSizeY + cardSizeY / 2);
  
  addTimerTask(moveTweenDelay, 
    [this]() 
    {
      playSound(m_SpadesMania_Take_trick);
      return false;
    });

  CardMoveComponent& cardMoveComponent
    = moveCard(cardNode->node, moveTweenDelay, moveTweenTimeout, 
        moveTweenSpeed, startMovePos, endMovePos);
  cardMoveComponent.setTweenDoneCallback(std::move(onCardMovedCb));

  float scaleTweenTimeout = 3.0f;
  float scaleTweenSpeed = 4.0f;
  float scaleTweenDelaySeconds = 0.0f;
  vec3 endScale = cardAtTopMiddle->node->getWorldScale();
  vec3 startScale = cardNode->node->getWorldScale();
  scaleCard(cardNode->node, scaleTweenDelaySeconds, scaleTweenTimeout, 
    scaleTweenSpeed, startScale, endScale);
}

void GameComponent::delayedRemoveCardAndGoNext(sptr<CardNode> cardNode, 
  float delaySec)
{
  addTimerTask(delaySec + 0.01f, [this, cardNode]() 
  {
    m_fsm.processEvent(FSM::Events::Next);
    removeCardFromDeck(cardNode);
    return false;
  });
}

void GameComponent::glowMiddleCard(float effectDuration)
{
  /// \note m_cardPicksDeck.at(1) must be card at top middle
  sptr<CardNode> cardAtTopMiddle = m_cardPicks[m_cardPicksDeck.at(1)];
  vec3 endMovePos = cardAtTopMiddle->node->getWorldTranslation();
  sptr<core::OBB> cardBB = make::sptr<core::OBB>(cardAtTopMiddle->node.get());
  float cardSizeY = cardBB->getSize().y * 0.05f;
  endMovePos.y -= (cardSizeY + cardSizeY / 2);
  //sptr<LayoutButton> downLoadBtn = MainUiSwitcher::instance()->getButton("GameDownloadBtn");
  // downLoadBtn->m_image->m_mesh->getWorldTranslation().z
  vec3 posGlow = { endMovePos.x, endMovePos.y, endMovePos.z };
  m_glow->setWorldTranslation(posGlow);
  m_glow->setRenderOrder(cardAtTopMiddle->node->getRenderOrder() - 1);
  m_glow->setWorldScale(cardAtTopMiddle->node->getWorldScale() * vec3(1.8f));
  m_glow->setEnabled(true);
  
  float particleScale = 0.001f;
  
  if (m_emitter->getParent() != nullptr)
    m_emitter->getParent()->removeChild(m_emitter);
  m_glow->addChild(m_emitter);

  //  m_emitter->setRenderOrder(Settings::instance()->getInt("emitterRenderOrder"));
  //  pos = { 0.f, 0.005f, -19.8 };

  //m_emitter->setWorldScale({ particleScale, particleScale, particleScale });
  sptr<LayoutImage> layoutImage = MainUiSwitcher::instance()->getImage("awatar_01");
  vec3 posEmitter = { endMovePos.x, endMovePos.y, 
    layoutImage->m_mesh->getWorldTranslation().z + 0.01f };
  m_emitter->setWorldTranslation(posEmitter);
  //m_emitter->setTransformMode(ParticlesEmitterParameters::TransformMode::EMITTER);

  //m_emitter->setRenderOrder(Settings::instance()->getInt("emitterRenderOrder"));
  m_emitter->setRenderOrder(layoutImage->m_mesh->getRenderOrder() - 100);
  //m_emitter->setWorldTranslation(pos);
  //m_emitter->setWorldScale(cardAtTopMiddle->node->getWorldScale() * vec3(1.8f));
  //m_emitter->setRenderOrder(cardAtTopMiddle->node->getRenderOrder() - 1);
  m_emitter->start();
  addTimerTask(effectDuration, [this](){
    m_emitter->stop();
    m_glow->setEnabled(false);
    if (m_emitter->getParent() != nullptr)
      m_emitter->getParent()->removeChild(m_emitter);
  });
}

void GameComponent::initializeFsm()
{
  m_fsm.state<FSM::States::Initialize>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Initialize onEnter");
    m_canClickCard = false;
    //setAudio();
    enableBlackout(false);
    MainUiSwitcher::instance()->enableImage(false, "got_it");
    MainUiSwitcher::instance()->enableImage(false, "spades_broken");
    MainUiSwitcher::instance()->enableImage(false, "spades");
    MainUiSwitcher::instance()->enableImage(false, "rays_blend");
    MainUiSwitcher::instance()->enableImage(false, "pointy_hand");
    resetAnimateHand();
    MainUiSwitcher::instance()->enableImage(false, "i_win");
    MainUiSwitcher::instance()->enableImage(false, "partner_wins");
    hideAllPlayerIcons();
    m_fsm.processEvent(FSM::Events::Next);
  };

  m_fsm.state<FSM::States::Initialize>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Initialize>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Initialize onLeaveFunc");
  };

  //------------
  m_fsm.state<FSM::States::Turn1Initialize>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn1Initialize onEnter");
    m_canClickCard = false;
    //setAudio();
    {
      InGameUiSwitcher::instance()->switchUi(InGameUiSwitcher::TC_TURN1);
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements

      hideAllPlayerIcons();

      MainUiSwitcher::instance()->enableImage(true, "awatar_01");
      MainUiSwitcher::instance()->enableImage(true, "awatar_02");
      MainUiSwitcher::instance()->enableImage(true, "awatar_03");
      MainUiSwitcher::instance()->enableImage(true, "awatar_04");
    }
    {
      const nlohmann::json& json_turnPicks
        = m_parsedCardsFile["TurnPicks1"];
      moveTurnPicksBackAndReset(json_turnPicks);
    }

    addTimerTask(m_helper->getFloat("addTimerTask_0"), [this]()
    {
      m_fsm.processEvent(FSM::Events::Next);
      return false;
    });
  };

  m_fsm.state<FSM::States::Turn1Initialize>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn1Initialize>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn1Initialize onLeaveFunc");
    //playSound(m_After_loading_SpadesMania_Deal_cards);
  };

  //------------
  m_fsm.state<FSM::States::Turn1>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn1 onEnter");
    {
      if (m_cardDeck.empty()) {
        W4_LOG_ERROR("Card deck is empty!");
      }
      enableBlackout(true);
      highlightCard(getCardAtRight());
      InGameUiSwitcher::instance()->enableImage(true, "put_the_highest_card");
    
      sptr<Collider> collider = addCardClickCallback(
        getCardAtRight(), "card_click_collider", m_cardDeck.back(),
        [this, cardNode = getCardAtRight()]
        (sptr<VisibleNode> card_front,
        const std::string card_name,
        const CollisionInfo &collisionInfo) 
      {
        W4_LOG_DEBUG("CLICKED ON RIGHT CARD: %s", card_name.c_str());

        playSound(m_SpadesMania_Place_card);
        float effectDuration = 2.5f; // show texts, "Got it!", etc.
        //
        moveCardBelowTopCenterCard(
          cardNode,
          [this, effectDuration, cardNode = getCardAtRight()]()
          {
            InGameUiSwitcher::instance()->enableImage(false, "put_the_highest_card");
            float effectDelay = 1.0f;
            //
            addTimerTask(effectDelay, [this, cardNode, effectDuration = effectDuration - effectDelay]() 
            {
              playSound(m_SpadesMania_End_round_anim_label_fan);
              showImageForTime("got_it", effectDuration);
              playZoomInTween("got_it", effectDuration - 0.1f);
              glowMiddleCard(effectDuration);
              addTimerTask(effectDuration, [this, cardNode, effectDuration]() 
              {
                const nlohmann::json& json_turnPicks
                  = m_parsedCardsFile["TurnPicks2"];
                //playZoomOutTween(cardNode->node, effectDuration);
                movePlayerCardToTrash(cardNode);
                moveTurnPicksBackAndReset(json_turnPicks);
                return false;
              });
              return false;
            });
            enableBlackout(false);
            MainUiSwitcher::instance()->enableImage(false, "pointy_hand");
            resetAnimateHand();
          });
        //
        delayedRemoveCardAndGoNext(cardNode, 
          effectDuration * 1.9);
      });
      m_cardColliders.push_back(collider);
    }
  };

  m_fsm.state<FSM::States::Turn1>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn1>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn1 onLeaveFunc");
    m_cardColliders.clear();
    m_glow->setEnabled(false);
  };

  //------------
  m_fsm.state<FSM::States::Turn2Initialize>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn2Initialize onEnter");
    m_canClickCard = false;
    //setAudio();
    {
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements
      InGameUiSwitcher::instance()->switchUi(InGameUiSwitcher::TC_TURN2);
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements

      hideAllPlayerIcons();

      MainUiSwitcher::instance()->enableImage(true, "awatar_01");
      MainUiSwitcher::instance()->enableImage(true, "awatar_02");
      MainUiSwitcher::instance()->enableImage(true, "awatar_03");
      MainUiSwitcher::instance()->enableImage(true, "awatar_04(2)");
    }
    addTimerTask(m_helper->getFloat("addTimerTask_1"), [this]() /////
    {
      m_fsm.processEvent(FSM::Events::Next);
      return false;
    });
  };

  m_fsm.state<FSM::States::Turn2Initialize>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn2Initialize>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn2Initialize onLeaveFunc");
  };

  //------------
  m_fsm.state<FSM::States::Turn2>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn2 onEnter");
    {
      if (m_cardDeck.empty()) {
        W4_LOG_ERROR("Card deck is empty!");
      }
      enableBlackout(true);
      highlightCard(getCardAtRight());
      InGameUiSwitcher::instance()->enableImage(true, "start_with_smaller_card");
      sptr<Collider> collider = addCardClickCallback(
        getCardAtRight(), "card_click_collider", m_cardDeck.back(),
        [this, cardNode = getCardAtRight()]
        (sptr<VisibleNode> card_front,
        const std::string card_name,
        const CollisionInfo &collisionInfo) 
      {
        W4_LOG_DEBUG("CLICKED ON RIGHT CARD: %s", card_name.c_str());
        playSound(m_SpadesMania_Place_card);
        float effectDuration = 2.5f; // show texts, "Got it!", etc.
        //
        moveCardBelowTopCenterCard(
          cardNode,
          // onCardMovedCb
          [this, effectDuration, cardNode = getCardAtRight()]()
          {
            InGameUiSwitcher::instance()->enableImage(false, "start_with_smaller_card");
            float effectDelay = 1.0f;
            //
            addTimerTask(effectDelay, [this, effectDuration = effectDuration - effectDelay, cardNode]() 
            {
              playSound(m_SpadesMania_End_round_anim_label_fan);
              showImageForTime("got_it", effectDuration);
              playZoomInTween("got_it", effectDuration - 0.1f);
              glowMiddleCard(effectDuration);
              addTimerTask(effectDuration, [this, cardNode, effectDuration]() 
              {
                const nlohmann::json& json_turnPicks
                  = m_parsedCardsFile["TurnPicks3"];
                //playZoomOutTween(cardNode->node, effectDuration);
                movePlayerCardToTrash(cardNode);
                moveTurnPicksBackAndReset(json_turnPicks);
                return false;
              });
              return false;
            });
            enableBlackout(false);
            MainUiSwitcher::instance()->enableImage(false, "pointy_hand");
            resetAnimateHand();
          });
        //
        delayedRemoveCardAndGoNext(cardNode, 
          effectDuration * 1.9);
      });
      m_cardColliders.push_back(collider);
    }
  };

  m_fsm.state<FSM::States::Turn2>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn2>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn2 onLeaveFunc");
    m_cardColliders.clear();
    m_glow->setEnabled(false);
  };

  //------------
  m_fsm.state<FSM::States::Turn3Initialize>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn3Initialize onEnter");
    m_canClickCard = false;
    //setAudio();
    {
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements
      InGameUiSwitcher::instance()->switchUi(InGameUiSwitcher::TC_TURN3);
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements

      hideAllPlayerIcons();

      MainUiSwitcher::instance()->enableImage(true, "awatar_01");
      MainUiSwitcher::instance()->enableImage(true, "awatar_02");
      MainUiSwitcher::instance()->enableImage(true, "awatar_03(2)");
      MainUiSwitcher::instance()->enableImage(true, "awatar_04(2)");
    }
    addTimerTask(m_helper->getFloat("addTimerTask_2"), [this]()
    {
      m_fsm.processEvent(FSM::Events::Next);
      return false;
    });
  };

  m_fsm.state<FSM::States::Turn3Initialize>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn3Initialize>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn3Initialize onLeaveFunc");
  };

  //------------
  m_fsm.state<FSM::States::Turn3>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn3 onEnter");
    {
      if (m_cardDeck.empty()) {
        W4_LOG_ERROR("Card deck is empty!");
      }
      enableBlackout(true);
      highlightCard(getCardAtRight());
      InGameUiSwitcher::instance()->enableImage(true, "spades_beat_everything");
      sptr<Collider> collider = addCardClickCallback(
        getCardAtRight(), "card_click_collider", m_cardDeck.back(),
        [this, cardNode = getCardAtRight()]
        (sptr<VisibleNode> card_front,
        const std::string card_name,
        const CollisionInfo &collisionInfo) 
      {
        W4_LOG_DEBUG("CLICKED ON RIGHT CARD: %s", card_name.c_str());
        playSound(m_SpadesMania_Place_card);
        float effectDuration = 2.5f; // show texts, "Got it!", etc.
        //
        moveCardBelowTopCenterCard(
          cardNode,
          // onCardMovedCb
          [this, effectDuration, cardNode = getCardAtRight()]()
          {
            InGameUiSwitcher::instance()->enableImage(false, "spades_beat_everything");
            showImageForTime("spades_broken", effectDuration);
            float effectDelay = 1.0f;
            //
            addTimerTask(effectDelay, [this, effectDuration = effectDuration - effectDelay, cardNode]() 
            {
              playSound(m_SpadesMania_End_round_anim_label_fan);
              showImageForTime("spades", effectDuration);
              playZoomInTween("spades", effectDuration - 0.1f);
              showImageForTime("rays_blend", effectDuration);
              playZoomInTween("rays_blend", effectDuration - 0.1f);
              m_rays_blend = MainUiSwitcher::instance()->getImage("rays_blend");
              m_spades = MainUiSwitcher::instance()->getImage("spades");
              glowMiddleCard(effectDuration);
              addTimerTask(effectDuration, [this, cardNode, effectDuration]() 
              {
                const nlohmann::json& json_turnPicks
                  = m_parsedCardsFile["TurnPicks4"];
                //playZoomOutTween(cardNode->node, effectDuration);
                movePlayerCardToTrash(cardNode);
                moveTurnPicksBackAndReset(json_turnPicks);
                return false;
              });
              return false;
            });
            enableBlackout(false);
            MainUiSwitcher::instance()->enableImage(false, "pointy_hand");
            resetAnimateHand();
          });
        //
        delayedRemoveCardAndGoNext(cardNode, 
          effectDuration * 1.9);
      });
      m_cardColliders.push_back(collider);
    }
  };

  m_fsm.state<FSM::States::Turn3>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn3>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn3 onLeaveFunc");
    m_cardColliders.clear();
    m_glow->setEnabled(false);
  };

  //------------
  m_fsm.state<FSM::States::Turn4Initialize>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn4Initialize onEnter");
    m_canClickCard = false;
    //setAudio();
    {
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements
      InGameUiSwitcher::instance()->switchUi(InGameUiSwitcher::TC_TURN4);
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements

      hideAllPlayerIcons();

      MainUiSwitcher::instance()->enableImage(true, "awatar_01");
      MainUiSwitcher::instance()->enableImage(true, "awatar_02");
      MainUiSwitcher::instance()->enableImage(true, "awatar_03");
      MainUiSwitcher::instance()->enableImage(true, "awatar_04(3)");
    }
    addTimerTask(m_helper->getFloat("addTimerTask_3"), [this]()
    {
      m_fsm.processEvent(FSM::Events::Next);
      return false;
    });
  };

  m_fsm.state<FSM::States::Turn4Initialize>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn4Initialize>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn4Initialize onLeaveFunc");
  };

  //------------
  m_fsm.state<FSM::States::Turn4>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn4 onEnter");
    {
      if (m_cardDeck.empty()) {
        W4_LOG_ERROR("Card deck is empty!");
      }
      highlightCard(getCardAtRight());
      sptr<Collider> collider = addCardClickCallback(
        getCardAtRight(), "card_click_collider", m_cardDeck.back(),
        [this, cardNode = getCardAtRight()]
        (sptr<VisibleNode> card_front,
        const std::string card_name,
        const CollisionInfo &collisionInfo) 
      {
        W4_LOG_DEBUG("CLICKED ON RIGHT CARD: %s", card_name.c_str());
        playSound(m_SpadesMania_Place_card);
        float effectDuration = 2.5f; // show texts, "Got it!", etc.
        //
        moveCardBelowTopCenterCard(
          cardNode,
          // onCardMovedCb
          [this, effectDuration, cardNode = getCardAtRight()]()
          {
            float effectDelay = 1.0f;
            //
            addTimerTask(effectDelay, [this, effectDuration = effectDuration - effectDelay, cardNode]() 
            {
              playSound(m_SpadesMania_End_round_anim_label_fan);
              showImageForTime("got_it", effectDuration);
              playZoomInTween("got_it", effectDuration - 0.1f);
              glowMiddleCard(effectDuration);
              addTimerTask(effectDuration, [this, cardNode, effectDuration]() 
              {
                const nlohmann::json& json_turnPicks
                  = m_parsedCardsFile["TurnPicks5"];
                //playZoomOutTween(cardNode->node, effectDuration);
                movePlayerCardToTrash(cardNode);
                moveTurnPicksBackAndReset(json_turnPicks);
                return false;
              });
              return false;
            });
            showImageForTime("partner_wins", effectDuration);
            enableBlackout(false);
            MainUiSwitcher::instance()->enableImage(false, "pointy_hand");
            resetAnimateHand();
          });
        //
        delayedRemoveCardAndGoNext(cardNode, 
          effectDuration * 1.9);
      });
      m_cardColliders.push_back(collider);
    }
  };

  m_fsm.state<FSM::States::Turn4>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn4>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn4 onLeaveFunc");
    m_cardColliders.clear();
    m_glow->setEnabled(false);
  };

  //------------

  m_fsm.state<FSM::States::Turn5Initialize>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn5Initialize onEnter");
    m_canClickCard = false;
    //setAudio();
    {
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements
      InGameUiSwitcher::instance()->switchUi(InGameUiSwitcher::TC_TURN5);
      InGameUiSwitcher::instance()->enable(false); // hide all ui elements

      hideAllPlayerIcons();

      MainUiSwitcher::instance()->enableImage(true, "awatar_01");
      MainUiSwitcher::instance()->enableImage(true, "awatar_02");
      MainUiSwitcher::instance()->enableImage(true, "awatar_03(2)");
      MainUiSwitcher::instance()->enableImage(true, "awatar_04(3)");
    }
    addTimerTask(m_helper->getFloat("addTimerTask_4"), [this]()
    {
      m_fsm.processEvent(FSM::Events::Next);
      return false;
    });
  };

  m_fsm.state<FSM::States::Turn5Initialize>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn5Initialize>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn5Initialize onLeaveFunc");
  };

  //------------

  m_fsm.state<FSM::States::Turn5>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn5 onEnter");
    {
      if (m_cardDeck.empty()) {
        W4_LOG_ERROR("Card deck is empty!");
      }
      highlightCard(getCardAtRight());
    }
    sptr<Collider> collider = addCardClickCallback(
      getCardAtRight(), "card_click_collider", m_cardDeck.back(),
      [this, cardNode = getCardAtRight()]
      (sptr<VisibleNode> card_front,
      const std::string card_name,
      const CollisionInfo &collisionInfo) 
    {
      W4_LOG_DEBUG("CLICKED ON RIGHT CARD: %s", card_name.c_str());
      playSound(m_SpadesMania_Place_card);
      float effectDuration = 2.5f; // show texts, "Got it!", etc.
      //
      moveCardBelowTopCenterCard(
        cardNode,
        // onCardMovedCb
        [this, effectDuration, cardNode = getCardAtRight()]()
        {
          playSound(m_SpadesMania_End_round_anim_label_fan);
          enableBlackout(false);
          showImageForTime("i_win", effectDuration);
          MainUiSwitcher::instance()->enableImage(false, "pointy_hand");
          resetAnimateHand();
          glowMiddleCard(effectDuration);
          addTimerTask(effectDuration, [this, cardNode, effectDuration]() 
          {
            //playZoomOutTween(cardNode->node, effectDuration);
            movePlayerCardToTrash(cardNode);
            moveTurnPicksBackAndReset(nlohmann::json{});
            return false;
          });
        });
      //
      delayedRemoveCardAndGoNext(cardNode, 
        effectDuration * 1.9);
    });
    m_cardColliders.push_back(collider);
  };

  m_fsm.state<FSM::States::Turn5>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Turn5>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Turn5 onLeaveFunc");
    m_cardColliders.clear();
    m_glow->setEnabled(false);
  };

  //------------

  m_fsm.state<FSM::States::Finish>().onEnterFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Finish onEnter");
    InGameUiSwitcher::instance()->enable(false); // hide all ui elements

    float finishDelay = 0.5f;
    //
    addTimerTask(finishDelay, [this]() 
    {
      if (m_finishCallback) {
        playSound(m_SpadesMania_End_round_anim_label_fan);
        m_finishCallback();
      }
      return false;
    });
  };

  m_fsm.state<FSM::States::Finish>().onUpdateFunc = [this](auto&, float dt)
  {
  };

  m_fsm.state<FSM::States::Finish>().onLeaveFunc = [this](auto&, auto&, auto&)
  {
    W4_LOG_INFO("Finish onLeaveFunc");
    //
    m_cardColliders.clear();
  };

  //------------

  m_fsm.init<FSM::States::Initialize>(FSM::Events::Preloading);
}
