
cmake_minimum_required(VERSION 3.20)

project(GAMENGINE
    VERSION 0.2.0
    DESCRIPTION "GAMENGINE Multi-Language Game Engine"
    LANGUAGES C CXX
)

# ==================================================
# LANGUAGE STANDARDS
# ==================================================

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==================================================
# BUILD CONFIGURATION
# ==================================================

include(GNUInstallDirs)

find_package(Threads REQUIRED)

# ==================================================
# GAMENGINE NATIVE CORE
# ==================================================

add_library(gamengine_core SHARED
    src/bridge.cpp
)

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
    PRIVATE
        Threads::Threads
)

set_target_properties(gamengine_core PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

# ==================================================
# GAMENGINE CLI
# ==================================================

add_executable(gamengine-cli
    src/main.cpp
)

target_link_libraries(gamengine-cli
    PRIVATE
        gamengine_core
)

# ==================================================
# JAVA JNI BRIDGE
# ==================================================

option(GAMENGINE_ENABLE_JAVA
    "Enable Java JNI integration"
    ON
)

if(GAMENGINE_ENABLE_JAVA)

    find_package(JNI QUIET)

    if(JNI_FOUND)

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
                gamengine_core
                ${JNI_LIBRARIES}
        )

        set_target_properties(gamengine_java PROPERTIES
            OUTPUT_NAME gamengine_java
        )

        message(STATUS "GAMENGINE: Java JNI enabled")

    else()

        message(STATUS
            "GAMENGINE: JNI not found; Java bridge disabled"
        )

    endif()

endif()

# ==================================================
# COMPILER WARNINGS
# ==================================================

if(MSVC)

    target_compile_options(gamengine_core PRIVATE /W4)
    target_compile_options(gamengine-cli PRIVATE /W4)

    if(TARGET gamengine_java)
        target_compile_options(gamengine_java PRIVATE /W4)
    endif()

else()

    target_compile_options(gamengine_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

    target_compile_options(gamengine-cli PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

    if(TARGET gamengine_java)
        target_compile_options(gamengine_java PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()

endif()

# ==================================================
# INSTALLATION
# ==================================================

install(
    TARGETS gamengine_core gamengine-cli
    EXPORT GAMENGINETargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(TARGET gamengine_java)

    install(
        TARGETS gamengine_java
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

endif()

# ==================================================
# BUILD SUMMARY
# ==================================================

message(STATUS "====================================")
message(STATUS " GAMENGINE ${PROJECT_VERSION}")
message(STATUS "====================================")
message(STATUS "C Standard:       ${CMAKE_C_STANDARD}")
message(STATUS "C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "Native Core:      Enabled")
message(STATUS "C ABI:            Enabled")
message(STATUS "Native CLI:       Enabled")
message(STATUS "Java JNI:         ${JNI_FOUND}")
message(STATUS "====================================")
