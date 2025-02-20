cmake_minimum_required(VERSION 3.29)
project(zombie_shooter)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp Player.h Player.cpp Pickup.h Pickup.cpp Bullet.h ZombieArena.h Zombie.h Zombie.cpp CreateHorde.cpp CreateBackground.cpp TextureHolder.h TextureHolder.cpp
        Bullet.cpp)
add_executable(zombie_shooter ${SOURCE_FILES})

set(CMAKE_PREFIX_PATH /opt/homebrew/Cellar/sfml/2.6.2)
find_package(SFML 2.6.2 COMPONENTS graphics window system audio REQUIRED)
target_link_libraries(zombie_shooter sfml-graphics sfml-window sfml-system sfml-audio)