#include "TexturePreloader.h"

#include <cmath>

void TexturePreloader::intialize(std::function<void()> onComplete, std::vector<std::string> listToLoad)
{
  m_onComplete = onComplete;

  for (auto texturePath : listToLoad)
  {
      m_textures.push_back(Texture::get(texturePath));
  }
  m_time = 0.0f;
  m_timeout = std::max(0.5f, std::min(listToLoad.size()*0.05f, 2.0f));
}

void TexturePreloader::update(float dt)
{
  m_time += dt;

  if (m_time > m_timeout)
  {
      if (m_onComplete)
      {
          m_onComplete();
          m_onComplete = nullptr;
      }
  } else {
    // ...
  }
}