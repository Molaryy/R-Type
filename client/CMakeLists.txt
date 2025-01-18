cmake_minimum_required(VERSION 3.27)
project(r-type_client)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
        src/Main.cpp
        src/Client.cpp
        src/ArgHandler.cpp
        src/scenes/menu/Menu.cpp
        src/scenes/menu/LeaderBoard.cpp
        src/scenes/settings/Settings.cpp
        src/scenes/game/Lobby.cpp
    includes/LeaderBoard.hpp
    includes/LeaderBoard.hpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE includes
        PRIVATE ${CMAKE_SOURCE_DIR}/ecs/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/graphic/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/network/packet_network
        PRIVATE ${CMAKE_SOURCE_DIR}/network/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/includes
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE packet_network
    PRIVATE dylib
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support window and unix")
endif ()

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}
)
