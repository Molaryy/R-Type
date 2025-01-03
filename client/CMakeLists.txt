project(client)

file(GLOB SOURCES
    "Gameplay/Game.cpp"
    "Menu/Menu.cpp"
    "raylib/raylib.cc"
    "main.cpp"
)

add_executable(r-type_client ${SOURCES})

list(APPEND CLIENT_EXTRA_INCLUDES "includes")
target_include_directories(r-type_client PUBLIC ${CLIENT_EXTRA_INCLUDES} ${asio_SOURCE_DIR}/asio/include)

target_compile_options(r-type_client PRIVATE -Wall -Wextra -std=c++23 -g3)

target_link_libraries(r-type_client PRIVATE NetworkLib)
target_link_libraries(r-type_client PRIVATE EcsLib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bootstrap)

add_custom_command(TARGET r-type_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_client> ${CMAKE_SOURCE_DIR}
)
