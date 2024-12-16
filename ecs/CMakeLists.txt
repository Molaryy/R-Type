project(Ecs)

file(GLOB ECS_SOURCES
    "src/NetworkEcs.cpp"
)

add_library(EcsLib STATIC ${ECS_SOURCES})

list(APPEND ECS_LIB_INCLUDES "includes")
target_include_directories(EcsLib PUBLIC ${ECS_LIB_INCLUDES})
