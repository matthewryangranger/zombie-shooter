#pragma once
#include <SFML/Graphics.hpp>

class Pickup
{
private:
  const int HEALTH_START_VALUE = 50;
  const int AMMO_START_VALUE = 12;
  const int START_WAIT_TIME = 10;
  const int START_SECONDS_TO_LIVE = 5;
  sf::Sprite m_Sprite;
  sf::IntRect m_Arena;
  int m_Value;
  int m_Type;
  bool m_Spawned;
  float m_SecondsSinceSpawn;
  float m_SecondsSinceDeSpawn;
  float m_SecondsToLive;
  float m_SecondsToWait;
public:
  Pickup(int type);
  void setArena(sf::IntRect arena);
  void spawn();
  sf::FloatRect getPosition() const;
  sf::Sprite getSprite() const;
  void update(float elapsedTime);
  bool isSpawned() const;
  int gotIt();
  void upgrade();
};