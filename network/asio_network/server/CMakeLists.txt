cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)

project(asio_server)

add_library(${PROJECT_NAME} SHARED
        AsioServer.cpp
        AsioServer.hpp
)

if (asio_ADDED)
    target_compile_definitions(${PROJECT_NAME}
            PRIVATE ASIO_STANDALONE ASIO_NO_DEPRECATED
    )

    if (WIN32)
        macro(get_win32_winnt version)
            if (CMAKE_SYSTEM_VERSION)
                set(ver ${CMAKE_SYSTEM_VERSION})
                string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
                string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
                if ("${verMajor}" MATCHES "10")
                    set(verMajor "A")
                    string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
                endif ("${verMajor}" MATCHES "10")
                string(REPLACE "." "" ver ${ver})
                string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
                set(${version} "0x${ver}")
            endif ()
        endmacro()

        if (NOT DEFINED _WIN32_WINNT)
            get_win32_winnt(ver)
            set(_WIN32_WINNT ${ver})
        endif ()

        message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")
        target_compile_definitions(${PROJECT_NAME}
                PRIVATE _WIN32_WINNT=${_WIN32_WINNT} WIN32_LEAN_AND_MEAN
        )
    endif ()
endif ()

target_include_directories(${PROJECT_NAME}
        PRIVATE ${asio_SOURCE_DIR}/asio/include
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE Threads::Threads
)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else ()
    message(FATAL_ERROR "Only support Windows and UNIX")
endif ()
