cmake_minimum_required(VERSION 3.27.4)

project(R-Type)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(cmake/CPM.cmake)
set(ASIO_TAG "1.32.0")
CPMAddPackage("gh:chriskohlhoff/asio#asio-1-32-0@${ASIO_TAG}")

add_subdirectory(ecs)
add_subdirectory(server)

add_custom_target(clean_server
    COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:r-type_server>
    COMMENT "Removing server executable"
)

add_dependencies(clean_server r-type_server)
