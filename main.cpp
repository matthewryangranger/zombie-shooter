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
  sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y),"Zombie Arena", sf::Style::Fullscreen);

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
  sf::Time lastPressed;

  window.setMouseCursorVisible(true);
  sf::Sprite spriteCrosshair;
  sf::Texture crosshairTexture = TextureHolder::GetTexture("../graphics/crosshair.png");
  spriteCrosshair.setTexture(crosshairTexture);
  spriteCrosshair.setOrigin(25, 25);

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
        if (state == State::PLAYING)
        {
          if (event.key.code == sf::Keyboard::R)
          {
            if (bulletsSpare >= clipSize)
            {
              bulletsInClip = clipSize;
              bulletsSpare -= clipSize;
            } else if (bulletsSpare > 0) // there are only a few bullets left, add those
            {
              bulletsInClip = bulletsSpare;
              bulletsSpare = 0;
            } else
            {
              // More to add here later?
            }
          }
        }
      }
    } // End event polling
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
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
        if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
        {
          bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
          currentBullet++;
          if (currentBullet > 99)
            currentBullet = 0;
          lastPressed = gameTimeTotal;
          bulletsInClip--;
        }
      } // End fire a bullet
    } // End WASD while playing

    // Handle the LEVELING up state
    if (state == State::LEVELING_UP)
    {
      // Handle the player LEVELING up
      if (event.key.code == sf::Keyboard::Num1)
      {
        state = State::PLAYING;
      }
      if (event.key.code == sf::Keyboard::Num2)
      {
        state = State::PLAYING;
      }
      if (event.key.code == sf::Keyboard::Num3)
      {
        state = State::PLAYING;
      }
      if (event.key.code == sf::Keyboard::Num4)
    {
      state = State::PLAYING;
    }
    if (event.key.code == sf::Keyboard::Num5)
    {
      state = State::PLAYING;
    }
    if (event.key.code == sf::Keyboard::Num6)
    {
      state = State::PLAYING;
    }

    if (state == State::PLAYING)
    {
      // Prepare the level
      // We will modify the next two lines later
      arena.width = 500;
      arena.height = 500;
      arena.left = 0;
      arena.top = 0;
      // Pass the vertex array by reference
      // to the createBackground function
      int tileSize = createBackground(background, arena);
      // We will modify this line of code later
      // int tileSize = 50;

      // Spawn the player in middle of the arena
      player.spawn(arena, resolution, tileSize);
      // Create a horde of zombies
      // Configure the pick-ups

      numZombies = 10;
      // Delete the previously allocated memory (if it exists)
      delete[] zombies;
      zombies = createHorde(numZombies, arena);
      numZombiesAlive = numZombies;


      // Reset clock so there isn't a frame jump
      clock.restart();
    }
  }// End LEVELING up
    /*
     * UPDATING THE FRAME
     **/
    if (state == State::PLAYING)
    {
      sf::Time dt = clock.restart();
      gameTimeTotal += dt;
      mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);
      spriteCrosshair.setPosition(mouseWorldPosition);
      player.update(dt.asSeconds(), sf::Mouse::getPosition());
      sf::Vector2f playerPosition(player.getCenter());
      mainView.setCenter(player.getCenter());
      for (int i = 0; i < numZombies; i++)
      {
        if (zombies[i].isAlive())
        {
          zombies[i].update(dt.asSeconds(), playerPosition);
        }
      }
      for (int i = 0; i < 100; i++)
      {
        if (bullets[i].isInFlight())
        {
          bullets[i].update(dt.asSeconds());
        }
      }
    } // end updating the scene
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
      for (int i = 0; i < 100; i++)
      {
        window.draw(bullets[i].getShape());
      }
      window.draw(player.getSprite());
      window.draw(spriteCrosshair);
    }
    if (state == State::LEVELING_UP){}
    if (state == State::PAUSED){}
    if (state == State::GAME_OVER){}
    window.display();
  }
  delete[] zombies;
  return 0;
}
