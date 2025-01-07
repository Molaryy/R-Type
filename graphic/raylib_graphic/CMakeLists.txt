cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)

include(../../cmake/CPM.cmake)

project(raylib_graphics)

add_library(${PROJECT_NAME} SHARED
        RaylibGraphic.cpp
        RaylibGraphic.hpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../includes"
)

CPMAddPackage(
        NAME raylib
        GITHUB_REPOSITORY raysan5/raylib
        GIT_TAG 5.0
        OPTIONS "BUILD_EXAMPLES OFF" "BUILD_GAMES OFF" "BUILD_SHARED_LIBS OFF" "CMAKE_POSITION_INDEPENDENT_CODE ON" "raylib_USE_STATIC_LIBS ON"
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE raylib
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support Windows and UNIX")
endif ()
