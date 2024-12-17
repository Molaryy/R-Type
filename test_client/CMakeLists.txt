project(test_client)

file(GLOB SOURCES
    "main.cpp"
)

add_executable(r-type_client ${SOURCES})

target_include_directories(r-type_client PUBLIC ${asio_SOURCE_DIR}/asio/include)

target_link_libraries(r-type_client PRIVATE NetworkLib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)

add_custom_command(TARGET r-type_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_client> ${CMAKE_SOURCE_DIR}
)
