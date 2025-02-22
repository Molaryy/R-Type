cmake_minimum_required(VERSION 3.27.4)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
else ()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif ()

if(DEBUG_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fsanitize=address -g3")
    add_link_options(-fsanitize=undefined -fsanitize=address -g3)
endif()

project(R-Type)

include(cmake/CPM.cmake)

add_subdirectory(cmake)
add_subdirectory(network)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(graphic)
add_subdirectory(hunter)
add_subdirectory(platformer)

add_custom_target(clean_server
    COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:r-type_server>
    COMMENT "Removing server executable"
)


add_dependencies(clean_server r-type_server)
