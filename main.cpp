#include <SFML/Graphics.hpp>
#include <sstream>
#include "ZombieArena.h"
#include "Player.h"
#include "Bullet.h"
#include "Pickup.h"
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
  Pickup healthPickup(1);
  Pickup ammoPickup(2);
  int score = 0;
  int hiScore = 0;

  // For the home/game screen
  sf::Sprite spriteGameOver;
  sf::Texture textureGameOver = TextureHolder::GetTexture("../graphics/background.png");
  spriteGameOver.setTexture(textureGameOver);
  spriteGameOver.setPosition(0,0);
  sf::View hudView(sf::FloatRect(0,0, 1920, 1080));
  sf::Sprite spriteAmmoIcon;
  sf::Texture textureAmmoIcon = TextureHolder::GetTexture("../graphics/ammo_icon.png");
  spriteAmmoIcon.setTexture(textureAmmoIcon);
  spriteAmmoIcon.setPosition(20, 980);

  sf::Font font;
  font.loadFromFile("../fonts/zombiecontrol.ttf");

  // Paused
  sf::Text pausedText;
  pausedText.setFont(font);
  pausedText.setCharacterSize(155);
  pausedText.setFillColor(sf::Color::White);
  pausedText.setPosition(400, 400);
  pausedText.setString("Press Enter \nto continue");

  // Game over
  sf::Text gameOverText;
  gameOverText.setFont(font);
  gameOverText.setCharacterSize(125);
  gameOverText.setFillColor(sf::Color::White);
  gameOverText.setPosition(250, 850);
  gameOverText.setString("Press Enter to play");

  // Leveling Up
  sf::Text levelUpText;
  levelUpText.setFont(font);
  levelUpText.setCharacterSize(80);
  levelUpText.setFillColor(sf::Color::White);
  levelUpText.setPosition(150, 250);
  std::stringstream levelUpStream;
  levelUpStream <<
    "1 - Increased rate of fire" << "\n" <<
      "2 - Increased clip size(next reload)" << "\n" <<
        "3 - Increased max health" << "\n" <<
          "4 - Increased run speed" << "\n" <<
            "5 - More and better health grabs" << "\n" <<
              "6 - More and better ammo grabs";
  levelUpText.setString(levelUpStream.str());

  // Ammo
  sf::Text ammoText;
  ammoText.setFont(font);
  ammoText.setCharacterSize(55);
  ammoText.setFillColor(sf::Color::White);
  ammoText.setPosition(200, 980);
  // Score
  sf::Text scoreText;
  scoreText.setFont(font);
  scoreText.setCharacterSize(55);
  scoreText.setFillColor(sf::Color::White);
  scoreText.setPosition(20, 0);
  // Hi score
  sf::Text hiScoreText;
  hiScoreText.setFont(font);
  hiScoreText.setCharacterSize(55);
  hiScoreText.setFillColor(sf::Color::White);
  hiScoreText.setPosition(1400, 0);
  std::stringstream s;
  s << "Hi Score:" << hiScore;
  hiScoreText.setString(s.str());
  // Zombies Remaining
  sf::Text zombiesRemainingText;
  zombiesRemainingText.setFont(font);
  zombiesRemainingText.setCharacterSize(55);
  zombiesRemainingText.setFillColor(sf::Color::White);
  zombiesRemainingText.setPosition(1500, 980);
  zombiesRemainingText.setString("Zombies: 100");
  // Wave number
  int wave = 0;
  sf::Text waveNumberText;
  waveNumberText.setFont(font);
  waveNumberText.setCharacterSize(55);
  waveNumberText.setFillColor(sf::Color::White);
  waveNumberText.setPosition(1250, 980);
  waveNumberText.setString("Wave: 0"); //?
  //Health bar
  sf::RectangleShape healthBar;
  healthBar.setFillColor(sf::Color::Red);
  healthBar.setPosition(450, 980);

  // when did we last update hud
  int framesSinceLastHUDUpdate = 0;
  int fpsMeasurementFrameInterval = 1000;

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
      healthPickup.setArena(arena);
      ammoPickup.setArena(arena);

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
      healthPickup.update(dt.asSeconds());
      ammoPickup.update(dt.asSeconds());
      for (int i = 0; i < 100; i++)
      {
        for (int j = 0; j < numZombies; j++)
        {
          if (bullets[i].isInFlight() && zombies[j].isAlive())
          {
            if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
            {
              bullets[i].stop();
              if (zombies[j].hit())
              {
                score += 10;
                if (score >= hiScore)
                  hiScore = score;
                numZombiesAlive--;
                if (numZombiesAlive == 0)
                {
                  state = State::LEVELING_UP;
                }
              }

            }
          }
        }
      } // End zombie check if it has been shot

      for (int i = 0; i < numZombies; i++)
      {
        if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
        {
          if (player.hit(gameTimeTotal))
          {
            // More to add here, later
          }
          if (player.getHealth() <= 0)
          {
            state = State::GAME_OVER;
          }
        }
      } // End player contact with zombie

      //Check if player has picked up a pick up
      if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
      {
        player.increaseHealthLevel(healthPickup.gotIt());
      }
      if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
      {
        bulletsSpare += ammoPickup.gotIt();
      }

      healthBar.setSize(sf::Vector2f(player.getHealth()*3, 50));
      framesSinceLastHUDUpdate++;

      if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
      {
        std::stringstream ssAmmo;
        std::stringstream ssScore;
        std::stringstream ssHiScore;
        std::stringstream ssWave;
        std::stringstream ssZombiesAlive;

        ssAmmo << bulletsInClip << "/" << bulletsSpare;
        ammoText.setString(ssAmmo.str());

        ssScore << "Score: " << score;
        scoreText.setString(ssScore.str());

        ssHiScore << "HiScore: " << hiScore;
        hiScoreText.setString(ssHiScore.str());

        ssWave << "Wave: " << wave;
        waveNumberText.setString(ssWave.str());

        ssZombiesAlive << "Zombies: " << numZombiesAlive;
        zombiesRemainingText.setString(ssZombiesAlive.str());
        framesSinceLastHUDUpdate = 0;
      } // End updating the HUD


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
      if (ammoPickup.isSpawned())
      {
        window.draw(ammoPickup.getSprite());
      }
      if (healthPickup.isSpawned())
      {
        window.draw(healthPickup.getSprite());
      }
      window.draw(spriteCrosshair);
      window.draw(player.getSprite());

      window.setView(hudView);
      window.draw(spriteAmmoIcon);
      window.draw(ammoText);
      window.draw(scoreText);
      window.draw(hiScoreText);
      window.draw(healthBar);
      window.draw(waveNumberText);
      window.draw(zombiesRemainingText);
    }
    if (state == State::LEVELING_UP)
    {
      window.draw(spriteGameOver);
      window.draw(levelUpText);
    }
    if (state == State::PAUSED)
    {
      window.draw(pausedText);
    }
    if (state == State::GAME_OVER)
    {
      window.draw(spriteGameOver);
      window.draw(gameOverText);
      window.draw(scoreText);
      window.draw(hiScoreText);
    }
    window.display();
  }
  delete[] zombies;
  return 0;
}
