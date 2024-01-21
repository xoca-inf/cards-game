#include "UiParser.h"
#include "settings/ResourceLibary.h"
#include "settings/Config.h"
#include "settings/Settings.h"

namespace nlohmann {

static double roundUp(double value, int decimal_places = 10) {
    const double multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}

static vec3 roundUp(vec3 value, int decimal_places = 10) {
    return vec3(roundUp(value.x, decimal_places), 
      roundUp(value.y, decimal_places), 
      roundUp(value.z, decimal_places));
}

template<typename T>
T getValueOr(const nlohmann::json &document, const std::string& keyId, const T& fallback) {
  nlohmann::json value;
  // per-orientation config
  if (document.contains(Config::getData().orientation)) {
    value = document[Config::getData().orientation];
    if(value.contains(keyId))
      W4_LOG_DEBUG("value.contains(%s) %s", keyId.c_str(), document.dump().c_str());
  } 
  return value.contains(keyId) ? value[keyId].get<T>() 
          : (document.contains(keyId) ? document[keyId].get<T>() : fallback);
}

template<typename T>
T getValue(const json &document, const std::string& keyId) {
  nlohmann::json value;
  // per-orientation config
  if (document.contains(Config::getData().orientation)) {
    value = document[Config::getData().orientation];
    if(value.contains(keyId))
      W4_LOG_DEBUG("value.contains(%s) %s", keyId.c_str(), document.dump().c_str());
  } 
  return value.contains(keyId) ? value[keyId].get<T>() 
          : (document.contains(keyId) ? document[keyId].get<T>() : T{});
}

sptr<LayoutLabel> createLabel(const json &document, const math::Frustum &cameraFrustrum){
    auto label = make::sptr<LayoutLabel>();
    label->m_document = document;
    return resetLabel(label, document, cameraFrustrum);
}

static Font toFont(const std::string& str) {
    if (str == "CourierNew") return Font::CourierNew;
    else if (str == "Arial") return Font::Arial;
    else if (str == "Times") return Font::Times;
    else if (str == "Copperplate") return Font::Copperplate;
    else if (str == "Helvetica") return Font::Helvetica;
    else if (str == "FreeMono") return Font::FreeMono;
    else if (str == "ComicSans") return Font::ComicSans;
    else if (str == "Tahoma") return Font::Tahoma;
    else if (str == "Lato") return Font::Lato;
    else {
      W4_LOG_ERROR("UNKNOWN FONT: %s", str.c_str());
      return Font{};
    }
}

static VerticalAlign toVerticalAlign(const std::string& str) {
    if (str == "Top") return VerticalAlign::Top;
    else if (str == "Center") return VerticalAlign::Center;
    else if (str == "Bottom") return VerticalAlign::Bottom;
    else {
      W4_LOG_ERROR("UNKNOWN VerticalAlign: %s", str.c_str());
      return VerticalAlign{};
    }
}

static HorizontalAlign toHorizontalAlign(const std::string& str) {
    if (str == "Left") return HorizontalAlign::Left;
    else if (str == "Center") return HorizontalAlign::Center;
    else if (str == "Right") return HorizontalAlign::Right;
    else {
      W4_LOG_ERROR("UNKNOWN HorizontalAlign: %s", str.c_str());
      return HorizontalAlign{};
    }
}

sptr<LayoutLabel> resetLabel(sptr<LayoutLabel> label, const json &document, const math::Frustum &cameraFrustrum){
  //const w4::math::size screenSize(Render::getSize());
    //gui::getVirtualResolution().w / Render::getSize().w,
    //gui::getVirtualResolution().h / Render::getSize().h);

  std::string name = getValue<std::string>(document, "name");
  std::string material = getValueOr<std::string>(document, "material", "DefaultMaterial");
  std::string texture = getValueOr<std::string>(document, "texture", "DefaultTexture");
  std::string text = getValueOr<std::string>(document, "text", "DefaultText");
  Pivot pivot = getValueOr<Pivot>(document, "pivot",  Pivot::CenterCenter);
  vec2 offset = getValueOr<vec2>(document, "offset", vec2(0,0));
  ivec2 size = getValueOr<ivec2>(document, "size", ivec2(1,1));

  //ivec2 point = getScreenPoint(screenSize, pivot, offset, cameraFrustrum);
  
  auto point =
    nlohmann::getScreenPoint(
      gui::getVirtualResolution(), 
      pivot, 
      offset, 
      Render::getScreenCamera()->getFrustum());
      
  if (label->m_label == nullptr) {
    auto widget = createWidget<Label>(nullptr, text, point);
    widget->setName(name);
    widget->setSizePolicy(SizePolicy::Auto,SizePolicy::Auto);
    
    //label->m_widgetName = widget;
    label->m_label = widget;
  }
  //auto diff = label->m_label->getSize();
  //w4::math::ivec2 newpos(point.x - diff.x / 2.0f, point.y - diff.y / 2.0f);
  label->m_label->setPosition(point); 
  label->m_label->setSize(size);

  label->m_label->setFontSize(
    getValueOr<float>(document, "fontSize", 30.0F));

  label->m_label->setBgColor(
    getValueOr<vec4>(document, "bgColor", vec4(1.0,1.0,1.0,0.0)));
  
  label->m_label->setTextColor(
    getValueOr<vec4>(document, "textColor", vec4(1.0,1.0,1.0,1.0)));
  
  label->m_label->setText(
    getValueOr<std::string>(document, "text", "DOWNLOAD"));

  label->m_label->setBold(
    getValueOr<int>(document, "isBold", 1));

  /// \todo
  //label->m_label->setWidth
  // label->m_label->setTextAlign
  //label->m_label->setShadowOffset
  //label->m_label->setShadowColor
  //label->m_label->setPadding
  //label->m_label->setOpacity
  //label->m_label->setVerticalPolicy(SizePolicy::Auto);
  //label->m_label->setHorizontalPolicy(SizePolicy::Auto);
  
  label->m_label->setHorizontalAlign(toHorizontalAlign(
    getValueOr<std::string>(document, "horizontalAlign", "Center")));

  label->m_label->setVerticalAlign(toVerticalAlign(
    getValueOr<std::string>(document, "verticalAlign", "Center")));

  label->m_label->setFont(toFont(
    getValueOr<std::string>(document, "font", "Helvetica")));
  
  label->m_pivot = pivot;
  label->m_offset = offset;
  label->m_size = size;

  return label;
}

sptr<LayoutImage> resetImage(sptr<LayoutImage> image, const bool updateLocalTranslation, 
                       const json &document, 
                       const math::Frustum &cameraFrustrum, vec3 cameraPos,
                       sptr<Node> root) {
  //root = Render::getRoot(); /// \\todo

  std::string name = getValue<std::string>(document, "name");
  std::string material = getValueOr<std::string>(document, "material", "DefaultMaterial");
  std::string texture = getValueOr<std::string>(document, "texture", "DefaultTexture");
  Pivot pivot = getValueOr<Pivot>(document, "pivot", Pivot::CenterCenter);
  vec2 offset = getValueOr<vec2>(document, "offset", vec2(0,0));
  vec2 size = getValueOr<vec2>(document, "size", vec2(0.001f,0.001f));
  int renderOrder = getValueOr<int>(document, "renderOrder", 0);

  float zOffset = getValueOr<float>(document, "zOffset", 
    Settings::instance()->getFloat("DefaultZOffset"));
  W4_LOG_DEBUG("parsed zOffset=%f for %s", zOffset, name.c_str());

  vec4 color = getValueOr<vec4>(document, "color", vec4(1.0f));

  w4::sptr<Mesh> plane = image->m_mesh;
  if (image->m_mesh == nullptr) {
    plane = Mesh::create::plane(vec2(1, 1));
    plane->setName(name);
  }

  //if (updateLocalTranslation) 
  {
    //plane->setWorldScale(vec3(size.x, size.y, 1));
    plane->setLocalScale(vec3(size.x, size.y, 0.001f));
    plane->setLocalTranslation(roundUp(cameraPos + nlohmann::getPivot(pivot,cameraFrustrum))
      + vec3(offset.x, offset.y, 0.0));
    plane->setWorldTranslation(vec3(plane->getWorldTranslation().x, plane->getWorldTranslation().y, zOffset));
    //plane->setLocalTranslation(
    //  cameraPos + getPivot(pivot,cameraFrustrum) 
    //  + vec3(offset.x, offset.y, zOffset));
  }

  root = Render::getRoot(); /// \todo

  size_t rootChildNum = root 
    ? root->findChildren(name).size() 
    : 0U;
  if(!rootChildNum) {
    root->addChild(name, plane);
  } 

  //setup material
  sptr<MaterialInst> materialInst;
  if(material != "DefaultMaterial") {
    materialInst = ResourceLibary::instance().getMaterial(material)->createInstance();
  }
  else materialInst = Material::getDefaultLambert()->createInstance();

  if (material == "SpriteMaterial") {
    materialInst->setParam("color", color);
  }
  if (material != "UnlitMaterial") {
    materialInst->enableBlending(true);
  }

  sptr<Texture> tex = ResourceLibary::instance().getTexture(texture);
  auto strToWrapType = [](const std::string& str){
    w4::resources::Wrapping wrapping = w4::resources::Wrapping::ClampToEdge;
    if (str == "ClampToEdge") {
      wrapping = w4::resources::Wrapping::ClampToEdge;
    } else if (str == "MirroredRepeat") {
      wrapping = w4::resources::Wrapping::MirroredRepeat;
    } else if (str == "Repeat") {
      wrapping = w4::resources::Wrapping::Repeat;
    } else {
      W4_LOG_ERROR("unknown wrapping type in json: %s", str.c_str());
    }
    return wrapping;
  };

  tex->setSWrapping(strToWrapType(
    getValueOr<std::string>(document, "SWrapping", "ClampToEdge")));

  tex->setTWrapping(strToWrapType(
    getValueOr<std::string>(document, "TWrapping", "ClampToEdge")));

  materialInst->setTexture(TextureId::TEXTURE_0, tex);
  plane->setMaterialInst(materialInst);
  plane->setRenderOrder(renderOrder);

  image->m_mesh = plane;
  image->m_node = plane;
  image->m_pivot = pivot;
  image->m_offset = offset;
  image->m_size = size;
  return image;
}

sptr<LayoutImage> createImage(const json &document, const math::Frustum &cameraFrustrum, vec3 cameraPos,
                       sptr<Node> root) {
  auto image = make::sptr<LayoutImage>();
  image->m_document = document;
  resetImage(image, true, document, cameraFrustrum, cameraPos, root);

  return image;
}

sptr<LayoutButton> createImageButton(const json &document,
                                     const math::Frustum &cameraFrustrum,
                                     vec3 cameraPos, sptr<Node> root) {
  auto button = make::sptr<LayoutButton>(nullptr);
  button->m_document = document;
  if (button->m_label != nullptr)
    button->m_label->m_document = document;
  return resetImageButton(button, document, cameraFrustrum, cameraPos, root);
}

sptr<LayoutButton> resetImageButton(sptr<LayoutButton> button, const json &document,
                                     const math::Frustum &cameraFrustrum,
                                     vec3 cameraPos, sptr<Node> root) {
  button->m_document = document;
  if (button->m_image != nullptr) {
    auto image = resetImage(button->m_image, true, document,cameraFrustrum,cameraPos,root);
    image->m_document = document;
    button->resetImage(image);
  } else {
    auto image = make::sptr<LayoutImage>();
    image->m_document = document;
    resetImage(image, true, document, cameraFrustrum, cameraPos, root);
    //button->m_image = image;
    //image = createImage(document,cameraFrustrum,cameraPos,root);
    image->m_document = document;
    button->resetImage(image);
  } 

  if (document.contains("label")) {
    if (button->m_label != nullptr) {
      button->m_label = resetLabel(button->m_label, document["label"], cameraFrustrum);
      button->m_label->m_document = document["label"];
    } else {
      auto label = make::sptr<LayoutLabel>();
      label->m_document = document["label"];
      resetLabel(label, document["label"], cameraFrustrum);
      button->m_label = label;
    } 
  }

  auto buttonNode = button->m_node;
  auto buttonMesh = button->m_mesh;
  
  if (buttonNode != nullptr) {
    cref<Collider> old_collider = buttonNode->getColliders().size() 
      ? buttonNode->getCollider("ui_collider")
      : nullptr;
    if (old_collider == nullptr && buttonMesh != nullptr) {
      cref<Collider> collider = buttonNode->addCollider("ui_collider", make::sptr<core::AABB>(buttonMesh));
      collider->setReceiveRaycasts();
      collider->setBlockRaycasts(true);
      collider->setScreencastCallback(
        [button](const CollisionInfo& collisionInfo){
          W4_LOG_DEBUG("setScreencastCallback");
          if (button->m_node != nullptr) {
            W4_LOG_DEBUG("[BUTTON] %s hitted!", 
              button->m_node->getName().c_str());
          }
          button->invokeCallback();
        });
      button->setCollider(collider);
    }
  }

  return button;
}

vec3 getPivot(Pivot pivot, const math::Frustum &cameraFrustrum) {
  switch (pivot) {
  case Pivot::TopLeft:
    return cameraFrustrum.points[1];
  case Pivot::TopCenter:
    return (cameraFrustrum.points[2] + cameraFrustrum.points[1]) * 0.5f;
  case Pivot::TopRight:
    return cameraFrustrum.points[2];
  case Pivot::CenterLeft:
    return (cameraFrustrum.points[1] + cameraFrustrum.points[0]) * 0.5f;
  case Pivot::CenterCenter:
    return (getPivot(Pivot::CenterRight,cameraFrustrum) + getPivot(Pivot::CenterLeft,cameraFrustrum)) * 0.5f;
  case Pivot::CenterRight:
    return (cameraFrustrum.points[2] + cameraFrustrum.points[3]) * 0.5f;
  case Pivot::BottomLeft:
    return cameraFrustrum.points[0];
  case Pivot::BottomCenter:
    return (cameraFrustrum.points[3] + cameraFrustrum.points[0]) * 0.5f;
  case Pivot::BottomRight:
    return cameraFrustrum.points[3];
  }
}
ivec2 getScreenPoint(w4::math::size screenSize, Pivot pivot, vec2 offset, const math::Frustum &cameraFrustrum) {
  auto point = getPivot(pivot, cameraFrustrum) + vec3(offset.x,offset.y,0);
  auto leftPointOffset = point.x - cameraFrustrum.points[1].x;
  auto topPointOffset = cameraFrustrum.points[1].y - point.y;
  auto frustrumWidth = (cameraFrustrum.points[2].x - cameraFrustrum.points[1].x);
  auto frustrumHeight = (cameraFrustrum.points[1].y - cameraFrustrum.points[0].y);
  W4_LOG_DEBUG("FRUSTRUM 1 Y = %f", cameraFrustrum.points[1].y);
  W4_LOG_DEBUG("FRUSTRUM 0 Y = %f", cameraFrustrum.points[0].y);
  W4_LOG_DEBUG("LEFTPOINTOFFSET = %f", leftPointOffset);
  W4_LOG_DEBUG("TOPPOINTOFFSET = %f", topPointOffset);
  W4_LOG_DEBUG("FRUSTRUM WIDTH = %f", frustrumWidth);
  W4_LOG_DEBUG("FRUSTRUM HEIGHT = %f", frustrumHeight);
  auto leftOffsetMultiplier = leftPointOffset / frustrumWidth;
  auto topOffsetMultiplier = topPointOffset / frustrumHeight;
  W4_LOG_DEBUG("leftOffset = %d", leftOffsetMultiplier);
  W4_LOG_DEBUG("topOffset = %d", topOffsetMultiplier);

  
    //gui::getVirtualResolution().w / Render::getSize().w,
    //gui::getVirtualResolution().h / Render::getSize().h);
  return ivec2(leftOffsetMultiplier * screenSize.w, 
    topOffsetMultiplier * screenSize.h);
}
} // namespace nlohmann
void LayoutButton::setClickable(bool clickable) {
  m_clickable = clickable;
  m_collider->setReceiveRaycasts(clickable);
  m_collider->setBlockRaycasts(clickable);
  m_collider->setBlockScreencasts(clickable);
}
void LayoutButton::setClickCallback(std::function<void()> callback) {
  m_clickCallback = callback;
}
LayoutButton::LayoutButton(sptr<LayoutImage> image) {
  if (image != nullptr) {
    resetImage(image);
  }
}
void LayoutButton::resetImage(sptr<LayoutImage> image) {
  this->m_image = image;
  this->m_node = image->m_node;
  this->m_mesh = image->m_mesh;
  this->m_offset = image->m_offset;
  this->m_pivot = image->m_pivot;
  this->m_size = image->m_size;
}
void LayoutButton::invokeCallback() {
  W4_LOG_DEBUG("invokeCallback");
  if (m_clickCallback!= nullptr && m_clickable)
  {
    m_clickCallback();
  }
  else{
    if(!m_clickable)      W4_LOG_DEBUG("[%s] BUTTON NOT CLICKABLE!", m_node->getName().c_str());
    if(!m_clickCallback)  W4_LOG_DEBUG("[%s] CALLBACK IS EMPTY!", m_node->getName().c_str());
  }

}
void LayoutButton::setCollider(sptr<Collider> collider) {
  m_collider = collider;
}
