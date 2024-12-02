# cmake_minimum_required(VERSION 3.27.4)

# project(rtype)

set(CMAKE_CXX_STANDARD 23)

project(R-Type)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(ecs)
add_subdirectory(server)

add_custom_target(clean_server
    COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:r-type_server>
    COMMENT "Removing server executable"
)

add_dependencies(clean_server r-type_server)
