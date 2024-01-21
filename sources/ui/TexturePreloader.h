#pragma once

#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

class TexturePreloader
{
public:
  TexturePreloader() = default;
  void intialize( std::function<void()> onComplete, std::vector<std::string> listToLoad);
  void update(float dt);
private:

  std::function<void()> m_onComplete;
  std::vector<sptr<Texture>> m_textures;
  float m_time;
  float m_timeout;
};
