project(server)

file(GLOB SOURCES
    "src/Arguments.cpp"
    "src/Game.cpp"
    "src/Network.cpp"
    "src/Server.cpp"
    "main.cpp"
)

add_executable(r-type_server ${SOURCES})

list(APPEND SERVER_EXTRA_INCLUDES "includes")
target_include_directories(r-type_server PUBLIC ${SERVER_EXTRA_INCLUDES} ${asio_SOURCE_DIR}/asio/include)

target_link_libraries(r-type_server PRIVATE NetworkLib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)

add_custom_command(TARGET r-type_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_server> ${CMAKE_SOURCE_DIR}
)
