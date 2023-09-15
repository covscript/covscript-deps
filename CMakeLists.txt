cmake_minimum_required(VERSION 3.4)

project(covscript-deps)

enable_language(C ASM)

if (APPLE)
    # x86 version of ucontext
    add_library(ucontext STATIC
        libucontext/arch/x86_64/getcontext.S
        libucontext/arch/x86_64/makecontext.c
        libucontext/arch/x86_64/setcontext.S
        libucontext/arch/x86_64/swapcontext.S
        libucontext/arch/x86_64/trampoline.c)
    target_compile_options(ucontext PUBLIC -target x86_64-apple-macos)
    target_link_options(ucontext PUBLIC -target x86_64-apple-macos)
    target_include_directories(ucontext PRIVATE libucontext/arch/common)
    target_include_directories(ucontext PUBLIC libucontext/include libucontext/arch/x86_64/include)

    # ARM version of ucontext
    add_library(ucontext_arm STATIC
        libucontext/arch/aarch64/getcontext.S
        libucontext/arch/aarch64/makecontext.c
        libucontext/arch/aarch64/setcontext.S
        libucontext/arch/aarch64/swapcontext.S
        libucontext/arch/aarch64/trampoline.c)
    target_compile_options(ucontext_arm PUBLIC -target arm64-apple-macos)
    target_link_options(ucontext_arm PUBLIC -target arm64-apple-macos)
    target_include_directories(ucontext_arm PRIVATE libucontext/arch/common)
    target_include_directories(ucontext_arm PUBLIC libucontext/include libucontext/arch/aarch64/include)
elseif (UNIX)
    # Build system specified libucontext
    add_library(ucontext STATIC
        libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/getcontext.S
        libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/makecontext.c
        libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/setcontext.S
        libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/swapcontext.S
        libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/trampoline.c)
    target_include_directories(ucontext PRIVATE libucontext/arch/common)
    target_include_directories(ucontext PUBLIC libucontext/include libucontext/arch/${CMAKE_SYSTEM_PROCESSOR}/include)
endif ()
