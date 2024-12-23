project(test_client)

file(GLOB SOURCES
    "main.cpp"
)

add_executable(r-type_client ${SOURCES})

target_include_directories(r-type_client PUBLIC
    ${asio_SOURCE_DIR}/asio/include
    ../server/includes
)

target_link_libraries(r-type_client PRIVATE NetworkLib EcsLib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bootstrap)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/server)

add_custom_command(TARGET r-type_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_client> ${CMAKE_SOURCE_DIR}
)
