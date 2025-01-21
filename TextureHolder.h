#pragma once
#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H
#include <SFML/Graphics.hpp>
#include <map>



class TextureHolder
{
private:
  std::map<std::string, sf::Texture> m_Textures;
  static TextureHolder* m_s_Instance;
public:
  TextureHolder();
  static sf::Texture& GetTexture(std::string const& filename);
};
#endif