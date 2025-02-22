cmake_minimum_required(VERSION 3.10)

project(covscript-deps)

enable_language(C ASM)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    # x86 version of ucontext
    file(GLOB UCONTEXT_FILES "libucontext/arch/x86_64/*")
    add_library(ucontext STATIC ${UCONTEXT_FILES})
    target_compile_options(ucontext PUBLIC -target x86_64-apple-macos -Wno-unknown-attributes)
    target_link_options(ucontext PUBLIC -target x86_64-apple-macos)
    target_include_directories(ucontext PRIVATE libucontext/arch/common)
    target_include_directories(ucontext PUBLIC libucontext/include libucontext/arch/x86_64/include)

    set(CMAKE_OSX_ARCHITECTURES arm64)
    # ARM version of ucontext
    file(GLOB UCONTEXT_FILES "libucontext/arch/aarch64/*")
    add_library(ucontext_arm STATIC ${UCONTEXT_FILES})
    target_compile_options(ucontext_arm PUBLIC -target arm64-apple-macos -Wno-unknown-attributes)
    target_link_options(ucontext_arm PUBLIC -target arm64-apple-macos)
    target_include_directories(ucontext_arm PRIVATE libucontext/arch/common)
    target_include_directories(ucontext_arm PUBLIC libucontext/include libucontext/arch/aarch64/include)
elseif (UNIX)
    # Build system specified libucontext
    file(GLOB UCONTEXT_FILES "libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/*")
    add_library(ucontext STATIC ${UCONTEXT_FILES})
    target_compile_options(ucontext PUBLIC -Wno-unknown-attributes)
    target_include_directories(ucontext PRIVATE libucontext/arch/common)
    target_include_directories(ucontext PUBLIC libucontext/include libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/include)
endif ()
