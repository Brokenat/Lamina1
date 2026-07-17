cmake_minimum_required(VERSION 3.15)
project(Lamina LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
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

function(disable_exceptions_rtti target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/EHsc- /GR->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-exceptions -fno-rtti>
    )
endfunction()

foreach(tgt IN ITEMS laminac lamina_runtime laminaCore lamina)
    disable_exceptions_rtti(${tgt})
endforeach()

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /arch:AVX2 /Oi /fp:fast")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -funroll-loops -ffast-math -flto -fomit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -s")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -flto -s")
endif()
