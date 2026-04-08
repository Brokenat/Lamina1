cmake_minimum_required(VERSION 3.15)
project(Lamina LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
include_directories(include)
add_subdirectory(runtime)
add_subdirectory(compiler)

add_library(laminaCore SHARED 
    lamina_core.cpp
    $<TARGET_OBJECTS:laminac>
    $<TARGET_OBJECTS:lamina_runtime>
)

# 保持 laminac 作为独立静态库(供其他目标使用)
set_target_properties(laminac PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(lamina main.cpp)

target_link_libraries(lamina PRIVATE laminaCore)