project(Network)

file(GLOB NETWORK_SOURCES
    "src/NetworkEcs.cpp"
)

add_library(NetworkLib STATIC ${NETWORK_SOURCES})

list(APPEND NETWORK_LIB_INCLUDES "includes")
target_include_directories(NetworkLib PUBLIC ${NETWORK_LIB_INCLUDES})
