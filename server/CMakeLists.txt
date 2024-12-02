project(server)

file(GLOB SOURCES
    "src/ErrorHandling.cpp"
    "src/Game.cpp"
    "src/Network.cpp"
    "src/Server.cpp"
    "main.cpp"
)

add_executable(r-type_server ${SOURCES})

list(APPEND SERVER_EXTRA_INCLUDES "includes")
target_include_directories(r-type_server PUBLIC ${SERVER_EXTRA_INCLUDES})

target_link_libraries(r-type_server PRIVATE EcsLib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ecs)

add_custom_command(TARGET r-type_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_server> ${CMAKE_SOURCE_DIR}
)
