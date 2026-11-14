
cmake_minimum_required(VERSION 3.20)

project(GAMENGINE
    VERSION 0.2.0
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(gamengine_core SHARED
    src/bridge.cpp
)

target_include_directories(gamengine_core
    PUBLIC include
)

target_compile_definitions(gamengine_core
    PRIVATE GAMENGINE_BUILD
)

target_link_libraries(gamengine_core
    PRIVATE Threads::Threads
)

set_target_properties(gamengine_core PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

add_executable(gamengine-cli
    src/main.cpp
)

target_link_libraries(gamengine-cli
    PRIVATE gamengine_core
)

if(MSVC)
    target_compile_options(gamengine_core PRIVATE /W4)
    target_compile_options(gamengine-cli PRIVATE /W4)
else()
    target_compile_options(gamengine_core PRIVATE
        -Wall -Wextra -Wpedantic
    )
    target_compile_options(gamengine-cli PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

install(TARGETS gamengine_core gamengine-cli)
install(
    DIRECTORY include/
    DESTINATION include
)
