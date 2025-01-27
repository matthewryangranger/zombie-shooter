#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include "Player.h"
#include "Bullet.h"
#include "TextureHolder.h"

int main()
{
  TextureHolder holder;
  enum class State {PAUSED, LEVELING_UP, GAME_OVER, PLAYING};

  auto state = State::GAME_OVER;
  sf::Vector2f resolution;
  resolution.x = sf::VideoMode::getDesktopMode().width;
  resolution.y = sf::VideoMode::getDesktopMode().height;

  sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Shooter", sf::Style::Default);
  sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
  sf::Clock clock;
  sf::Time gameTimeTotal;
  sf::Vector2f mouseWorldPosition;
  sf::Vector2i mouseScreenPosition;
  Player player;
  sf::IntRect arena;
  sf::VertexArray background;
  sf::Texture textureBackground = TextureHolder::GetTexture("../graphics/background_sheet.png");

  int numZombies;
  int numZombiesAlive;
  Zombie* zombies = NULL;
  Bullet bullets[100];
  int currentBullet = 0;
  int bulletsSpare = 24;
  int bulletsInClip = 6;
  int clipSize = 6;
  float fireRate = 1;
  sf::Time lastPressedTrigger;

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::KeyPressed)
      {
        if (event.key.code == sf::Keyboard::Return && state == State::PLAYING)
        {
          state = State::PAUSED;
        } else if (event.key.code == sf::Keyboard::Return && state == State::PAUSED)
        {
          state = State::PLAYING;
          clock.restart();
        }
        else if (event.key.code == sf::Keyboard::Return && state == State::GAME_OVER)
        {
          state = State::LEVELING_UP;
        }
        // if (state == State::PLAYING)
        // {
        //
        // }
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      window.close();
    if (state == State::PLAYING)
    {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        player.moveUp();
      else
        player.stopUp();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        player.moveDown();
      else
        player.stopDown();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        player.moveLeft();
      else
        player.stopLeft();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        player.moveRight();
      else
        player.stopRight();
    }
    if (state == State::LEVELING_UP)
    {
      if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 ||
        event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Num4 ||
        event.key.code == sf::Keyboard::Num5 || event.key.code == sf::Keyboard::Num6)
      {
        state = State::PLAYING;
      }
      if (state == State::PLAYING){
        arena.width = 500;
        arena.height = 500;
        arena.left = 0;
        arena.top = 0;
        int tileSize = createBackground(background, arena);
        player.spawn(arena, resolution, tileSize);
        numZombies = 10;
        delete[] zombies;
        zombies = createHorde(numZombies, arena);
        numZombiesAlive = numZombies;
        clock.restart();
      }
    }
    /*
     * UPDATING THE FRAME
     **/
    if (state == State::PLAYING)
    {
      sf::Time dt = clock.restart();
      gameTimeTotal += dt;
      float dtAsSeconds = dt.asSeconds();
      mouseScreenPosition = sf::Mouse::getPosition();
      mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);
      player.update(dtAsSeconds, sf::Mouse::getPosition());
      sf::Vector2f playerPosition(player.getCenter());
      mainView.setCenter(player.getCenter());
      for (int i = 0; i < numZombies; i++)
      {
        if (zombies[i].isAlive())
        {
          zombies[i].update(dtAsSeconds, playerPosition);
        }
      }
    }
    /*
     * DRAW THE SCENE
     **/
    if (state == State::PLAYING)
    {
      window.clear();
      window.setView(mainView);
      window.draw(background, &textureBackground);
      for (int i = 0; i < numZombies; i++)
      {
        window.draw(zombies[i].getSprite());
      }
      window.draw(player.getSprite());
    }
    if (state == State::LEVELING_UP){}
    if (state == State::PAUSED){}
    if (state == State::GAME_OVER){}
    window.display();
  }
  delete[] zombies;
  return 0;
}
