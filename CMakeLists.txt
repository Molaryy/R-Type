# cmake_minimum_required(VERSION 3.27.4)

# project(rtype)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# include(FetchContent)
# FetchContent_Declare(SFML
#         GIT_REPOSITORY https://github.com/SFML/SFML.git
#         GIT_TAG 2.6.x
#         GIT_SHALLOW ON
#         EXCLUDE_FROM_ALL
#         SYSTEM)
# FetchContent_MakeAvailable(SFML)

# file(GLOB_RECURSE serverSourceFiles "server/*.cpp")
# file(GLOB_RECURSE clientSourceFiles "client/*.cpp")


# add_executable(r-type_server ${serverSourceFiles})
# add_executable(r-type_client ${clientSourceFiles})

# target_link_libraries(r-type_client PRIVATE sfml-graphics)

# if(WIN32)
#     add_custom_command(
#             TARGET r-type_client
#             COMMENT "Copy OpenAL DLL"
#             PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
#             VERBATIM)
# endif()


cmake_minimum_required(VERSION 3.10)

project(R-Type)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(ecs)
add_subdirectory(server)

add_custom_target(clean_server
    COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:r-type_server>
    COMMENT "Removing server executable"
)

add_dependencies(clean_server r-type_server)
