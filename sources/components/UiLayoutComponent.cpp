#include "UiLayoutComponent.h"
#include "W4JSON.h"
#include "ui/UiParser.h"

static double roundUp(double value, int decimal_places = 10) {
    const double multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}

static vec3 roundUp(vec3 value, int decimal_places = 10) {
    return vec3(roundUp(value.x, decimal_places), 
      roundUp(value.y, decimal_places), 
      roundUp(value.z, decimal_places));
}

UiLayoutComponent::~UiLayoutComponent() {
  clear();
  if (m_uiRoot != nullptr) {
    this->getOwner().removeChild(m_uiRoot);
  }
}

void UiLayoutComponent::initialize(const variant::Variant &data) {
  IComponent::initialize(data);
  auto filename = data.get<std::string>();
  m_filenameFull = "ui/" + filename + ".json";

  m_uiRoot = make::sptr<Node>(filename);
  this->getOwner().addChild(m_uiRoot);
}

void UiLayoutComponent::clear() {
  W4_LOG_DEBUG("UiLayoutComponent::clear");
  if (m_uiRoot == nullptr) return;

  for (auto it = m_uiLabels.rbegin(); it != m_uiLabels.rend(); it++)
  {
    sptr<LayoutLabel> value = *it;
    if (value != nullptr && value->m_label != nullptr)
    {
        value->m_label->setVisible(false);
        value->m_label = nullptr;
    }
  }
  m_uiLabels.clear();

  for (auto it = m_uiImages.rbegin(); it != m_uiImages.rend(); it++)
  {
    sptr<LayoutImage> value = *it;
    if (value == nullptr || value->m_mesh == nullptr) 
      continue;
    m_uiRoot->removeChild(value->m_mesh);
  }
  m_uiImages.clear();

  for (auto it = m_uiButtons.rbegin(); it != m_uiButtons.rend(); it++)
  {
    sptr<LayoutButton> value = *it;
    if (value->m_label != nullptr && value->m_label->m_label != nullptr) {
      value->m_label->m_label->setOpacity(0.0f);
      value->m_label->m_label->setVisible(false);
    }
  }
  m_uiButtons.clear();
}

void UiLayoutComponent::rebuild() {
  W4_LOG_DEBUG("UiLayoutComponent::rebuild");
  if (m_uiRoot == nullptr) return;

  auto file = w4::filesystem::open(m_filenameFull);
  auto json = nlohmann::json::parse(file->data(), file->data() + file->size());
  auto cameraPos = Render::getScreenCamera()->getWorldTranslation();
  auto frustrum = Render::getScreenCamera()->getFrustum();

  clear();

  for (nlohmann::json& value : json["labels"]){
    sptr<LayoutLabel> label = nlohmann::createLabel(value, frustrum);
    m_uiLabels.push_back(label);
  }

  for (nlohmann::json& value : json["images"]){
    sptr<LayoutImage> image = nlohmann::createImage(
      value, frustrum, cameraPos, 
      Render::getRoot());
      //m_uiRoot);
    m_uiImages.push_back(image);
  }
  
  for (nlohmann::json& value : json["imageButtons"]){
    sptr<LayoutButton> button = nlohmann::createImageButton(
      value, frustrum, cameraPos, 
      Render::getRoot());
      //m_uiRoot);
    m_uiButtons.push_back(button);
  }
}

void UiLayoutComponent::resize(const math::size& newSize, bool forceReset) {
  m_oldConfigData = Config::getData();

  if (m_worldUi != nullptr)
  {
      m_worldUi->updateSize(Render::getSize());
  }

  if (m_domUi != nullptr)
  {
      m_domUi->updateSize(Render::getSize());
  }
}

void UiLayoutComponent::finalize() {
  IComponent::finalize();
  W4_LOG_DEBUG("UiLayoutComponent finalized!");
}

void UiLayoutComponent::show(bool needEnable) {
  W4_LOG_DEBUG("UiLayoutComponent::show %d", needEnable);

  /*if (m_uiRoot != nullptr) {
    W4_LOG_DEBUG("%s UI COMPONENT ENABLED CHANGED TO %d", 
      m_uiRoot->getName().c_str(), needEnable);
    m_uiRoot->setEnabled(needEnable);
  }*/

  for (auto it = m_uiButtons.rbegin(); it != m_uiButtons.rend(); it++)
  {
    sptr<LayoutButton> item = *it;
    if (item != nullptr && item->m_node != nullptr) {
      W4_LOG_DEBUG("BUTTON [%s] COMPONENT ENABLED CHANGED TO %d", 
        item->m_node->getName().c_str(), needEnable);
      
        item->enable(needEnable);
    }
  }
  for (auto it = m_uiLabels.rbegin(); it != m_uiLabels.rend(); it++)
  {
    sptr<LayoutLabel> label = *it;
    if (label != nullptr && label->m_label != nullptr) 
    {
      label->enable(needEnable);
    }
  }
  for (auto it = m_uiImages.rbegin(); it != m_uiImages.rend(); it++)
  {
    sptr<LayoutImage> image = *it;
    if (image != nullptr && image->m_node != nullptr) 
    {
      image->enable(needEnable);
    }
  }

  if (m_worldUi != nullptr) {
    m_worldUi->setEnabled(needEnable);
  }
  
  if (m_domUi != nullptr) {
    m_domUi->setEnabled(needEnable);
  }
}

void UiLayoutComponent::onEnabled(bool needEnable) {
  W4_LOG_DEBUG("UiLayoutComponent::onEnabled %d", needEnable);
  IComponent::onEnabled(needEnable);

  //
  //show(needEnable);
}

sptr<LayoutImage> UiLayoutComponent::getImage(const std::string &name) {
  for (auto it = m_uiImages.rbegin(); it != m_uiImages.rend(); it++)
  {
    sptr<LayoutImage> image = *it;
    if (image != nullptr && image->m_node != nullptr && image->m_node->getName() == name) 
      return image;
  }
  W4_LOG_DEBUG("Image with name [%s] was not found", name.c_str());
  return w4::sptr<LayoutImage>();
}
sptr<LayoutButton> UiLayoutComponent::getButton(const std::string &name) {
  W4_LOG_DEBUG("UiLayoutComponent::getButton");
  for (const auto &button : m_uiButtons)
  {
    if(button->m_node->getName() == name) return button;
  }
  W4_LOG_DEBUG("Button with name [%s] was not found", name.c_str());
  return w4::sptr<LayoutButton>();
}
sptr<LayoutLabel> UiLayoutComponent::getLabel(const std::string &name) {
  W4_LOG_DEBUG("UiLayoutComponent::getLabel");
  for (const auto &label : m_uiLabels)
  {
    if(label->m_label->getName() == name) return label;
  }
  W4_LOG_DEBUG("Label with name [%s] was not found", name.c_str());
  return w4::sptr<LayoutLabel>();
}
sptr<Node> UiLayoutComponent::getRoot() {
  //return m_uiRoot;
  return Render::getRoot();
}
