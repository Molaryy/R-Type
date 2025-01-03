cmake_minimum_required(VERSION 3.27)

project(r-type_server)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/Arguments.cpp
#        src/Game.cpp
        src/Server.cpp
        main.cpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE includes/
        PRIVATE ${CMAKE_SOURCE_DIR}/network/packet_network
        PRIVATE ${CMAKE_SOURCE_DIR}/network/includes
        PRIVATE ${CMAKE_SOURCE_DIR}/includes
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE PacketNetwork
        PRIVATE dylib
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX OR APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support window and unix")
endif ()
