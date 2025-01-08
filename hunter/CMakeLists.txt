cmake_minimum_required(VERSION 3.27)

project(hunterGame)

set(CMAKE_CXX_STANDARD 23)

include(../cmake/CPM.cmake)

CPMAddPackage(
        NAME raylib
        GITHUB_REPOSITORY raysan5/raylib
        GIT_TAG 5.0
        OPTIONS "BUILD_EXAMPLES OFF" "BUILD_GAMES OFF" "BUILD_SHARED_LIBS OFF" "CMAKE_POSITION_INDEPENDENT_CODE ON" "raylib_USE_STATIC_LIBS ON"
)

add_executable(${PROJECT_NAME}
        main.cpp
        src/Hunter.cpp
        ../graphic/raylib_graphic/RaylibGraphic.cpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE includes
        PRIVATE ${CMAKE_SOURCE_DIR}/ecs/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/graphic/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/graphic/raylib_graphic
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE raylib
        PRIVATE dylib
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX OR APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support window and unix")
endif ()
