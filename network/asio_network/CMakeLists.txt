cmake_minimum_required(VERSION 3.27)

include(../../cmake/CPM.cmake)

find_package(Threads REQUIRED)

CPMAddPackage(
        NAME asio
        GITHUB_REPOSITORY chriskohlhoff/asio
        GIT_TAG asio-1-24-0
        OPTIONS "ASIO_BUILD_TESTS=OFF;ASIO_BUILD_EXAMPLES=OFF"
)

add_subdirectory(client)
add_subdirectory(server)
