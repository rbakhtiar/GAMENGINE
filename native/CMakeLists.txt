
cmake_minimum_required(VERSION 3.20)

project(GAMENGINE
    VERSION 0.5.0
    DESCRIPTION "GAMENGINE Hybrid Game Engine"
    LANGUAGES C CXX
)

# ============================================================
# LANGUAGE CONFIGURATION
# ============================================================

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(CTest)

find_package(Threads REQUIRED)

# ============================================================
# ENGINE OPTIONS
# ============================================================

option(GAMENGINE_BUILD_TESTS
    "Build GAMENGINE tests"
    ON
)

option(GAMENGINE_BUILD_SHARED
    "Build GAMENGINE as a shared library"
    ON
)

option(GAMENGINE_BUILD_JAVA
    "Build optional Java JNI bridge"
    OFF
)

option(GAMENGINE_BUILD_CLI
    "Build GAMENGINE command-line application"
    ON
)

# ============================================================
# COMPILER CONFIGURATION
# ============================================================

function(gamengine_configure_target target)

    target_compile_features(${target}
        PUBLIC cxx_std_20
    )

    if(MSVC)

        target_compile_options(${target}
            PRIVATE
                /W4
                /permissive-
        )

    else()

        target_compile_options(${target}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )

    endif()

endfunction()

# ============================================================
# ENGINE CORE
# ============================================================

set(GAMENGINE_CORE_SOURCE
    src/bridge.cpp
)

if(GAMENGINE_BUILD_SHARED)

    add_library(gamengine_core SHARED
        ${GAMENGINE_CORE_SOURCE}
    )

else()

    add_library(gamengine_core STATIC
        ${GAMENGINE_CORE_SOURCE}
    )

endif()

add_library(GAMENGINE::Core ALIAS gamengine_core)

target_include_directories(gamengine_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(gamengine_core
    PRIVATE
        GAMENGINE_BUILD
)

target_link_libraries(gamengine_core
    PUBLIC
        Threads::Threads
)

gamengine_configure_target(gamengine_core)

# ============================================================
# ENGINE COMMAND LINE
# ============================================================

if(GAMENGINE_BUILD_CLI)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

        add_executable(gamengine-cli
            src/main.cpp
        )

        target_link_libraries(gamengine-cli
            PRIVATE
                GAMENGINE::Core
        )

        gamengine_configure_target(gamengine-cli)

    else()

        message(WARNING
            "src/main.cpp not found. CLI disabled."
        )

    endif()

endif()

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
    INTERFACE
        cxx_std_20
)

# ============================================================
# WORLD HEADER VALIDATION
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

        message(WARNING
            "World header missing: ${header}"
        )

    endif()

endforeach()

# ============================================================
# ASSET SYSTEM HEADERS
# ============================================================

set(GAMENGINE_ASSET_HEADERS
    asset_types.hpp
    asset_handle.hpp
    asset_database.hpp
    asset_importer.hpp
    asset_registry.hpp
)

foreach(header IN LISTS GAMENGINE_ASSET_HEADERS)

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${header}")

        message(WARNING
            "Asset header missing: ${header}"
        )

    endif()

endforeach()

# ============================================================
# AUDIO SYSTEM HEADERS
# ============================================================

set(GAMENGINE_AUDIO_HEADERS
    audio_types.hpp
    audio_source.hpp
    audio_listener.hpp
    audio_mixer.hpp
    audio_system.hpp
)

foreach(header IN LISTS GAMENGINE_AUDIO_HEADERS)

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${header}")

        message(WARNING
            "Audio header missing: ${header}"
        )

    endif()

endforeach()

# ============================================================
# TEST SYSTEM
# ============================================================

if(GAMENGINE_BUILD_TESTS)

    enable_testing()

    # --------------------------------------------------------
    # Terrain test
    # --------------------------------------------------------

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/terrain_test.cpp")

        add_executable(gamengine_terrain_test
            terrain_test.cpp
        )

        target_link_libraries(gamengine_terrain_test
            PRIVATE
                GAMENGINE::World
        )

        gamengine_configure_target(
            gamengine_terrain_test
        )

        add_test(
            NAME terrain_test
            COMMAND gamengine_terrain_test
        )

    endif()

    # --------------------------------------------------------
    # World systems test
    # --------------------------------------------------------

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/world_systems_test.cpp")

        add_executable(gamengine_world_systems_test
            world_systems_test.cpp
        )

        target_link_libraries(gamengine_world_systems_test
            PRIVATE
                GAMENGINE::World
        )

        gamengine_configure_target(
            gamengine_world_systems_test
        )

        add_test(
            NAME world_systems_test
            COMMAND gamengine_world_systems_test
        )

    endif()

    # --------------------------------------------------------
    # Asset and audio test
    # --------------------------------------------------------

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/asset_audio_test.cpp")

        add_executable(gamengine_asset_audio_test
            asset_audio_test.cpp
        )

        target_link_libraries(gamengine_asset_audio_test
            PRIVATE
                GAMENGINE::World
        )

        gamengine_configure_target(
            gamengine_asset_audio_test
        )

        add_test(
            NAME asset_audio_test
            COMMAND gamengine_asset_audio_test
        )

    endif()

endif()

# ============================================================
# OPTIONAL JAVA JNI BRIDGE
# ============================================================

if(GAMENGINE_BUILD_JAVA)

    find_package(JNI REQUIRED)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/java_bridge.cpp")

        add_library(gamengine_java SHARED
            src/java_bridge.cpp
        )

        target_include_directories(gamengine_java
            PRIVATE
                ${JNI_INCLUDE_DIRS}
        )

        target_link_libraries(gamengine_java
            PRIVATE
                GAMENGINE::Core
                ${JNI_LIBRARIES}
        )

        gamengine_configure_target(
            gamengine_java
        )

    else()

        message(FATAL_ERROR
            "Java bridge requested but src/java_bridge.cpp is missing."
        )

    endif()

endif()

# ============================================================
# INSTALLATION
# ============================================================

install(
    TARGETS gamengine_core
    EXPORT GAMENGINETargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(TARGET gamengine-cli)

    install(
        TARGETS gamengine-cli
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

endif()

if(TARGET gamengine_java)

    install(
        TARGETS gamengine_java
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")

    install(
        DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

endif()

install(
    FILES ${GAMENGINE_WORLD_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gamengine/world
    OPTIONAL
)

install(
    FILES ${GAMENGINE_ASSET_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gamengine/assets
    OPTIONAL
)

install(
    FILES ${GAMENGINE_AUDIO_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gamengine/audio
    OPTIONAL
)

install(
    EXPORT GAMENGINETargets
    FILE GAMENGINETargets.cmake
    NAMESPACE GAMENGINE::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GAMENGINE
)

# ============================================================
# ENGINE BUILD SUMMARY
# ============================================================

message(STATUS "")
message(STATUS "======================================")
message(STATUS "          GAMENGINE 0.5.0")
message(STATUS "======================================")
message(STATUS "C Standard:       ${CMAKE_C_STANDARD}")
message(STATUS "C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "Shared Library:   ${GAMENGINE_BUILD_SHARED}")
message(STATUS "CLI:              ${GAMENGINE_BUILD_CLI}")
message(STATUS "Tests:            ${GAMENGINE_BUILD_TESTS}")
message(STATUS "Java JNI:         ${GAMENGINE_BUILD_JAVA}")
message(STATUS "Threads:          Enabled")
message(STATUS "World Systems:    Registered")
message(STATUS "Asset System:     Registered")
message(STATUS "Audio System:     Registered")
message(STATUS "======================================")
message(STATUS "")
