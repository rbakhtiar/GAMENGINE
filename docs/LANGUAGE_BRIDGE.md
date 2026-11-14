
# GAMENGINE Language Bridge

Version: 0.2.0

## Overview

The Language Bridge exposes the GAMENGINE native
core to external programming languages.

The public interface uses a C-compatible ABI.

## Supported adapters

| Language | Integration |
|---|---|
| C | Native API |
| C++ | Native API |
| Python | ctypes |
| C# | P/Invoke |
| Java | JNI |
| Ruby | Fiddle |
| Lisp | CFFI |
| Shell | Native CLI |

## Public API

- ge_engine_create()
- ge_engine_destroy()
- ge_object_create()
- ge_object_count()
- ge_object_id_at()
- ge_object_get_name()
- ge_object_set_position()
- ge_object_get_position()
- ge_object_remove()
- ge_scene_clear()

## Build

From the repository root:

    cmake -S native -B native/build
    cmake --build native/build

## Python

    python languages/python/bridge.py

## Ruby

    ruby languages/ruby/bridge.rb

## Shell

    bash languages/shell/gamengine.sh run

## Java

Build the JNI library with a compatible JDK and
CMake JNI development environment.

Compile GAMENGINE.java and run it with the native
library directory included in java.library.path.

## C#

Reference Bridge.cs in a .NET project and ensure
the native library is available to the process.

## Lisp

Install Common Lisp, Quicklisp and CFFI.
Load bridge.lisp after building the native library.

## Important architecture note

Every adapter currently creates its own engine
instance inside its own process.

The API is shared, but independent processes do
not automatically share a live scene.

A future IPC service will support shared engine
sessions across different language processes.
