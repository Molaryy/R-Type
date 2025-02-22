cmake_minimum_required(VERSION 3.27)

project(r-type_server)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/Main.cpp
        src/ArgHandler.cpp
        src/Server.cpp
        src/Lobby.cpp
        src/Score.cpp
        src/entities/Player.cpp
        src/entities/EnemyFly.cpp
        src/entities/EnemyTank.cpp
        src/entities/Boss.cpp
        src/entities/BossHeart.cpp
        src/entities/EnemyTurret.cpp
        src/entities/PlayerBullet.cpp
        src/entities/EnemyBullet.cpp
        src/entities/BonusHealth.cpp
        src/entities/BonusForce.cpp
        src/entities/BonusTripleShot.cpp
        src/entities/Wall.cpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE includes/
        PRIVATE ${CMAKE_SOURCE_DIR}/network/packet_network
        PRIVATE ${CMAKE_SOURCE_DIR}/network/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/ecs/includes
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE packet_network
        PRIVATE dylib
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX OR APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support window and unix")
endif ()
