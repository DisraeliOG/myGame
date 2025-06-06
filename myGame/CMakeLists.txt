cmake_minimum_required(VERSION 3.30)
project(myGame)

set(CMAKE_CXX_STANDARD 20)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 2.6.2 COMPONENTS graphics window system audio REQUIRED)

add_executable(myGame
        Game.cpp
        Player.cpp
        main.cpp
        Game.h
        Player.h
        Enemy.h
        Enemy.cpp
        Bullet.h
        Bullet.cpp
        EnvironmentObjects.h
        EnvironmentObjects.cpp
        EnvironmentManager.h
        EnvironmentManager.cpp
        HUD.cpp
        HUD.h
        ExperienceOrb.cpp
        ExperienceOrb.h
        Upgrade.cpp
        Upgrade.h
        UpgradeManager.cpp
        UpgradeManager.h
        Boss.cpp
        Boss.h
)

target_link_libraries(myGame
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/sounds DESTINATION ${CMAKE_BINARY_DIR})
