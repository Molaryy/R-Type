cmake_minimum_required(VERSION 3.27)

project(platformGame)

set(CMAKE_CXX_STANDARD 23)

include(../cmake/CPM.cmake)

add_executable(${PROJECT_NAME}
        main.cpp
        src/Platformer.cpp
        src/systems/CameraSystem.cpp
        src/systems/CollisionSystem.cpp
        src/systems/InputSystem.cpp
        src/systems/PlatformGenerationSystem.cpp
        src/systems/PlayerMovementSystem.cpp
        src/systems/RenderingSystem.cpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE includes
        PRIVATE ${CMAKE_SOURCE_DIR}/ecs/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/graphic/includes
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE dylib
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX OR APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support window and unix")
endif ()
