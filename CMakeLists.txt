cmake_minimum_required(VERSION 3.10)

project(covscript-deps)

enable_language(C ASM)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    # x86 version of ucontext
    file(GLOB UCONTEXT_FILES "libucontext/arch/x86_64/*")
    add_library(ucontext OBJECT ${UCONTEXT_FILES})
    target_compile_options(ucontext PUBLIC -target x86_64-apple-macos -Wno-unknown-attributes)
    target_link_options(ucontext PUBLIC -target x86_64-apple-macos)
    target_include_directories(ucontext PRIVATE libucontext/arch/common)
    target_include_directories(ucontext PUBLIC libucontext/include libucontext/arch/x86_64/include)

    set(CMAKE_OSX_ARCHITECTURES arm64)
    # ARM version of ucontext
    file(GLOB UCONTEXT_FILES "libucontext/arch/aarch64/*")
    add_library(ucontext_arm OBJECT ${UCONTEXT_FILES})
    target_compile_options(ucontext_arm PUBLIC -target arm64-apple-macos -Wno-unknown-attributes)
    target_link_options(ucontext_arm PUBLIC -target arm64-apple-macos)
    target_include_directories(ucontext_arm PRIVATE libucontext/arch/common)
    target_include_directories(ucontext_arm PUBLIC libucontext/include libucontext/arch/aarch64/include)
elseif (UNIX)
    if(NOT DEFINED CMAKE_SYSTEM_PROCESSOR OR CMAKE_SYSTEM_PROCESSOR STREQUAL "")
        execute_process(
            COMMAND uname -p
            OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
    endif()
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64")
        set(CMAKE_SYSTEM_PROCESSOR "x86_64")
    endif()
    message(STATUS "Using processor: ${CMAKE_SYSTEM_PROCESSOR}")
    set(UCONTEXT_DIR "libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}")
    file(GLOB UCONTEXT_FILES "${UCONTEXT_DIR}/*")
    if(UCONTEXT_FILES STREQUAL "")
        message(FATAL_ERROR "Could not found corresponding libucontext implementation for arch ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    add_library(ucontext OBJECT ${UCONTEXT_FILES})
    target_compile_options(ucontext PUBLIC -Wno-unknown-attributes -fPIC)
    target_include_directories(ucontext PRIVATE libucontext/arch/common)
    target_include_directories(ucontext PUBLIC
        libucontext/include
        "${UCONTEXT_DIR}/include"
    )
endif ()
