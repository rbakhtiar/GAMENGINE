
cmake_minimum_required(VERSION 3.20)

project(GAMENGINE
    VERSION 0.4.0
    DESCRIPTION "GAMENGINE Native Game Engine"
    LANGUAGES C CXX
)

# ============================================================
# LANGUAGE STANDARDS
# ============================================================

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# BUILD CONFIGURATION
# ============================================================

include(GNUInstallDirs)
include(CTest)

find_package(Threads REQUIRED)

option(GAMENGINE_ENABLE_JAVA
    "Enable Java JNI integration"
    ON
)

option(GAMENGINE_BUILD_TESTS
    "Build GAMENGINE tests"
    ON
)

option(GAMENGINE_BUILD_SHARED
    "Build GAMENGINE core as shared library"
    ON
)

# ============================================================
# COMPILER CONFIGURATION
# ============================================================

function(gamengine_configure_target target)

    if(MSVC)

        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
        )

    else()

        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )

    endif()

endfunction()

# ============================================================
# NATIVE CORE
# ============================================================

if(GAMENGINE_BUILD_SHARED)

    add_library(gamengine_core SHARED
        src/bridge.cpp
    )

else()

    add_library(gamengine_core STATIC
        src/bridge.cpp
    )

endif()

add_library(GAMENGINE::Core ALIAS gamengine_core)

target_include_directories(gamengine_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(gamengine_core
    PRIVATE GAMENGINE_BUILD
)

target_link_libraries(gamengine_core
    PRIVATE Threads::Threads
)

set_target_properties(gamengine_core PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)

gamengine_configure_target(gamengine_core)

# ============================================================
# NATIVE CLI
# ============================================================

add_executable(gamengine-cli
    src/main.cpp
)

target_link_libraries(gamengine-cli
    PRIVATE GAMENGINE::Core
)

gamengine_configure_target(gamengine-cli)

# ============================================================
# WORLD SYSTEMS
# ============================================================

add_library(gamengine_world INTERFACE)

add_library(GAMENGINE::World ALIAS gamengine_world)

target_include_directories(gamengine_world
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(gamengine_world
    INTERFACE cxx_std_17
)

# ============================================================
# WORLD SYSTEM HEADER COLLECTION
# ============================================================

set(GAMENGINE_WORLD_HEADERS
    terrain.hpp
    world_streamer.hpp
    terrain_material.hpp
    biome.hpp
    vegetation.hpp
    world_bounds.hpp
    scene_graph.hpp
    resource_registry.hpp
)

foreach(header IN LISTS GAMENGINE_WORLD_HEADERS)

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${header}")
        message(FATAL_ERROR
            "Missing GAMENGINE world header: ${header}"
        )
    endif()

endforeach()

# ============================================================
# TEST CONFIGURATION
# ============================================================

if(GAMENGINE_BUILD_TESTS AND BUILD_TESTING)

    enable_testing()

    # --------------------------------------------------------
    # TERRAIN TEST
    # --------------------------------------------------------

    add_executable(gamengine_terrain_test
        terrain_test.cpp
    )

    target_link_libraries(gamengine_terrain_test
        PRIVATE GAMENGINE::World
    )

    gamengine_configure_target(
        gamengine_terrain_test
    )

    add_test(
        NAME terrain_generation
        COMMAND gamengine_terrain_test
    )

    # --------------------------------------------------------
    # WORLD SYSTEMS TEST
    # --------------------------------------------------------

    add_executable(gamengine_world_systems_test
        world_systems_test.cpp
    )

    target_link_libraries(gamengine_world_systems_test
        PRIVATE GAMENGINE::World
    )

    gamengine_configure_target(
        gamengine_world_systems_test
    )

    add_test(
        NAME world_systems
        COMMAND gamengine_world_systems_test
    )

endif()

# ============================================================
# JAVA JNI
# ============================================================

set(GAMENGINE_JAVA_ENABLED OFF)

if(GAMENGINE_ENABLE_JAVA)

    find_package(JNI QUIET)

    if(JNI_FOUND)

        if(NOT EXISTS
            "${CMAKE_CURRENT_SOURCE_DIR}/src/java_bridge.cpp"
        )

            message(FATAL_ERROR
                "JNI was found but src/java_bridge.cpp is missing."
            )

        endif()

        add_library(gamengine_java SHARED
            src/java_bridge.cpp
        )

        target_include_directories(gamengine_java
            PRIVATE
                ${JNI_INCLUDE_DIRS}
                ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

        target_link_libraries(gamengine_java
            PRIVATE
                GAMENGINE::Core
                ${JNI_LIBRARIES}
        )

        set_target_properties(gamengine_java PROPERTIES
            OUTPUT_NAME gamengine_java
        )

        gamengine_configure_target(gamengine_java)

        set(GAMENGINE_JAVA_ENABLED ON)

    else()

        message(STATUS
            "JNI unavailable: Java bridge disabled"
        )

    endif()

endif()

# ============================================================
# INSTALLATION
# ============================================================

install(
    TARGETS gamengine_core gamengine-cli
    EXPORT GAMENGINETargets

    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(TARGET gamengine_java)

    install(
        TARGETS gamengine_java

        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

endif()

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES ${GAMENGINE_WORLD_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gamengine/world
)

install(
    EXPORT GAMENGINETargets
    FILE GAMENGINETargets.cmake
    NAMESPACE GAMENGINE::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GAMENGINE
)

# ============================================================
# BUILD SUMMARY
# ============================================================

message(STATUS "")
message(STATUS "============================================")
message(STATUS "             GAMENGINE ${PROJECT_VERSION}")
message(STATUS "============================================")
message(STATUS "C Standard:          ${CMAKE_C_STANDARD}")
message(STATUS "C++ Standard:        ${CMAKE_CXX_STANDARD}")
message(STATUS "Shared Core:         ${GAMENGINE_BUILD_SHARED}")
message(STATUS "Native CLI:          ENABLED")
message(STATUS "World Systems:       ENABLED")
message(STATUS "Terrain Tests:       ${GAMENGINE_BUILD_TESTS}")
message(STATUS "Java JNI:            ${GAMENGINE_JAVA_ENABLED}")
message(STATUS "Threads:             ENABLED")
message(STATUS "============================================")
message(STATUS "")
