cmake_minimum_required(VERSION 3.27)

project(PacketNetwork)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} STATIC
        Packet.cpp
        Packet.hpp
        PacketHandler.cpp
        PacketHandler.hpp
        PacketHandler.cpp
        PacketHandler.hpp
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else()
    message(FATAL_ERROR "Only support window and unix")
endif()
