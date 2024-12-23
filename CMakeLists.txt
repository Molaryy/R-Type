cmake_minimum_required(VERSION 3.27.4)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

project(R-Type)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(cmake/CPM.cmake)
set(ASIO_TAG "1.32.0")
CPMAddPackage("gh:chriskohlhoff/asio#asio-1-32-0@${ASIO_TAG}")

add_subdirectory(network)
add_subdirectory(server)
add_subdirectory(test_client)
# add_subdirectory(client)
add_subdirectory(bootstrap)

add_custom_target(clean_server
    COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:r-type_server>
    COMMENT "Removing server executable"
)

add_dependencies(clean_server r-type_server)
