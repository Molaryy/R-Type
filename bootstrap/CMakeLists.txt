project(ecs)

file(GLOB ECS_SOURCES
    "src/Main.cc"
)

add_library(EcsLib STATIC ${ECS_SOURCES})

list(APPEND ECS_LIB_INCLUDES "include")
target_include_directories(EcsLib PUBLIC ${ECS_LIB_INCLUDES})
