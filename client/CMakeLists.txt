project(r-type_client)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        "Gameplay/Game.cpp"
        "Menu/Menu.cpp"
        "raylib/raylib.cc"
        "main.cpp"
)

include(../cmake/CPM.cmake)

CPMAddPackage(
        NAME raylib
        GITHUB_REPOSITORY raysan5/raylib
        GIT_TAG 5.0
        OPTIONS "BUILD_EXAMPLES OFF" "BUILD_GAMES OFF" "BUILD_SHARED_LIBS OFF" "CMAKE_POSITION_INDEPENDENT_CODE ON" "raylib_USE_STATIC_LIBS ON"
)

target_include_directories(${PROJECT_NAME}
        PRIVATE includes/
        PRIVATE ${CMAKE_SOURCE_DIR}/network/packet_network
        PRIVATE ${CMAKE_SOURCE_DIR}/network/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/ecs/include
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE PacketNetwork
        PRIVATE dylib
        PRIVATE raylib
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX OR APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support window and unix")
endif ()
