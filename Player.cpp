#include "Player.h"

Player::Player(): m_Speed(START_SPEED), m_Health(START_HEALTH), m_MaxHealth(START_HEALTH), m_Texture(), m_Sprite() {
  m_Texture.loadFromFile("../graphics/player.png");
  m_Sprite.setTexture(m_Texture);
  m_Sprite.setOrigin(25, 25);
  }

void Player::spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize)
{
  m_Position.x = arena.width / 2;
  m_Position.y = arena.height / 2;
  m_Arena.left = arena.left;
  m_Arena.width = arena.width;
  m_Arena.top = arena.top;
  m_Arena.height = arena.height;
  m_TileSize = tileSize;
  m_Resolution.x = resolution.x;
  m_Resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
  m_Speed = START_SPEED;
  m_Health = START_HEALTH;
  m_MaxHealth = START_HEALTH;
}

sf::Time Player::getLastHitTime() const
{
  return m_LastHit;
}

bool Player::hit(sf::Time timeHit)
{
  if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 2000)
  {
    m_LastHit = timeHit;
    m_Health -= 10;
    return true;
  } else
    return false;
}

sf::FloatRect Player::getPosition() const
{
  return m_Sprite.getGlobalBounds();
}

sf::Vector2f Player::getCenter() const
{
  return m_Position:
}

float Player::getRotation() const
{
  return m_Sprite.getRotation();
}

sf::Sprite Player::getSprite() const
{
  return m_Sprite;
}

int Player::getHealth() const
{
  return m_Health;
}

void Player::moveLeft()
{
  m_LeftPressed = true;
}

void Player::moveRight()
{
  m_RightPressed = true;
}

void Player::moveUp()
{
  m_UpPressed = true;
}

void Player::moveDown()
{
  m_DownPressed = true;
}

void Player::stopLeft()
{
  m_LeftPressed = false;
}

void Player::stopRight()
{
  m_RightPressed = false;
}

void Player::stopUp()
{
  m_UpPressed = false;
}

void Player::stopDown()
{
  m_DownPressed = false;
}

