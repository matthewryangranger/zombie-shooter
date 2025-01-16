#include <SFML/Graphics.hpp>
#include "Player.h"

int main()
{
  enum class State {PAUSED, LEVELING_UP, GAME_OVER, PLAYING};

  auto state = State::GAME_OVER;
  sf::Vector2f resolution;
  resolution.x = sf::VideoMode::getDesktopMode().width;
  resolution.y = sf::VideoMode::getDesktopMode().height;

  sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Shooter", sf::Style::Fullscreen);
  sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
  sf::Clock clock;
  sf::Time gameTimeTotal;
  sf::Vector2f mouseWorldPosition;
  sf::Vector2i mouseScreenPosition;
  Player player;
  sf::IntRect arena;

  while (window.isOpen())
  {

  }
  return 0;
}
