#pragma once

#include "W4Framework.h"
#include "W4JSON.h"

W4_USE_UNSTRICT_INTERFACE

struct LayoutImage;
struct LayoutButton;
struct LayoutLabel;

namespace nlohmann{

/*W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::gui::VerticalAlign,
                                {{w4::gui::VerticalAlign::Top, "Top"},
                                 {w4::gui::VerticalAlign::Center, "Center"},
                                 {w4::gui::VerticalAlign::Bottom, "Bottom"}});
                                 
W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::gui::HorizontalAlign,
                                {{w4::gui::HorizontalAlign::Left, "Left"},
                                 {w4::gui::HorizontalAlign::Center, "Center"},
                                 {w4::gui::HorizontalAlign::Right, "Right"}});
                                 
W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::gui::Font,
                                {{w4::gui::Font::CourierNew, "CourierNew"},
                                 {w4::gui::Font::Arial, "Arial"},
                                 {w4::gui::Font::Times, "Times"},
                                 {w4::gui::Font::Copperplate, "Copperplate"},
                                 {w4::gui::Font::Helvetica, "Helvetica"},
                                 {w4::gui::Font::FreeMono, "FreeMono"},
                                 {w4::gui::Font::ComicSans, "ComicSans"},
                                 {w4::gui::Font::Tahoma, "Tahoma"},
                                 {w4::gui::Font::Lato, "Lato"}});*/

enum class Pivot : uint {
  TopLeft = 0,
  TopCenter = 1,
  TopRight = 2,
  CenterLeft = 3,
  CenterCenter = 4,
  CenterRight = 5,
  BottomLeft = 6,
  BottomCenter = 7,
  BottomRight = 8
};

W4_NLOHMANN_JSON_SERIALIZE_ENUM(Pivot,
                                {{Pivot::TopLeft, "TopLeft"},
                                 {Pivot::TopCenter, "TopCenter"},
                                 {Pivot::TopRight, "TopRight"},
                                 {Pivot::CenterLeft, "CenterLeft"},
                                 {Pivot::CenterCenter, "CenterCenter"},
                                 {Pivot::CenterRight, "CenterRight"},
                                 {Pivot::BottomLeft, "BottomLeft"},
                                 {Pivot::BottomCenter, "BottomCenter"},
                                 {Pivot::BottomRight, "BottomRight"}});

w4::math::vec3 getPivot(Pivot pivot, const math::Frustum &cameraFrustrum);
ivec2 getScreenPoint(w4::math::size screenSize, Pivot pivot, vec2 offset, const math::Frustum &cameraFrustrum);

w4::sptr<LayoutLabel> resetLabel(w4::sptr<LayoutLabel>, const json &value, const math::Frustum &cameraFrustrum);
w4::sptr<LayoutImage> resetImage(w4::sptr<LayoutImage>, const bool updateLocalTranslation, const json &value, const math::Frustum &cameraFrustrum, vec3 cameraPos,
                       sptr<Node> root = nullptr);
w4::sptr<LayoutButton> resetImageButton(w4::sptr<LayoutButton>, const json &value, const math::Frustum &cameraFrustrum, vec3 cameraPos,
                               sptr<Node> root = nullptr);

/*sptr<LayoutButton> resetImageButton(sptr<LayoutButton> button, const json &document,
                                     const math::Frustum &cameraFrustrum,
                                     vec3 cameraPos, sptr<Node> root) {
sptr<LayoutImage> resetImage(sptr<LayoutImage> image, const json &document, 
                       const math::Frustum &cameraFrustrum, vec3 cameraPos,
                       sptr<Node> root) {
sptr<LayoutLabel> resetLabel(sptr<LayoutLabel> label, const json &document, const math::Frustum &cameraFrustrum){
  */

w4::sptr<LayoutLabel> createLabel(const json &value, const math::Frustum &cameraFrustrum);
w4::sptr<LayoutImage> createImage(const json &value, const math::Frustum &cameraFrustrum, vec3 cameraPos,
                       sptr<Node> root = nullptr);

w4::sptr<LayoutButton> createImageButton(const json &value, const math::Frustum &cameraFrustrum, vec3 cameraPos,
                               sptr<Node> root = nullptr);
}

struct LayoutLabel{
  //std::string widgetName; // mainUI.getLabel("DownloadBtn")->m_label->getName()
  w4::sptr<Label>     m_label;
  nlohmann::Pivot m_pivot;
  vec2            m_offset;
  ivec2           m_size;
  nlohmann::json m_document;
  
  void enable(const bool needEnable) 
  {
    if (m_label != nullptr) 
    {
      m_label->setVisible(needEnable);
    }
  }
};

struct LayoutImage{
  //std::string widgetName; // mainUI.getImage("DownloadBtn")->m_mesh->getName()
  w4::sptr<Mesh>      m_mesh;
  w4::sptr<Node>      m_node;
  nlohmann::Pivot m_pivot;
  vec2            m_offset;
  vec2            m_size;
  nlohmann::json m_document;

  void enable(const bool needEnable) {
    if (m_node != nullptr)
      m_node->setEnabled(needEnable);
    if (m_mesh != nullptr)
      m_mesh->setEnabled(needEnable);
  }
};

struct LayoutButton : public LayoutImage{
public:
  nlohmann::json m_document;
  w4::sptr<LayoutImage> m_image{nullptr};
  w4::sptr<LayoutLabel> m_label{nullptr};
  //std::string widgetName; // mainUI.getButton("DownloadBtn")->m_label->getName()
  LayoutButton(w4::sptr<LayoutImage> image);
  void resetImage(w4::sptr<LayoutImage> image);
  void setClickable(bool clickable);
  void setClickCallback( std::function<void()> callback);
  void setCollider(w4::sptr<Collider> collider);
  void invokeCallback();

  void enable(const bool needEnable) 
  {
    if (m_node != nullptr) 
    {
      m_node->setEnabled(needEnable);
    }
    if (m_label != nullptr) 
    {
      m_label->m_label->setVisible(needEnable);
    }
    setClickable(needEnable);
  }
  
public:
  w4::sptr<Collider>          m_collider;
  bool                    m_clickable       = true;
  std::function<void()>   m_clickCallback;
};

