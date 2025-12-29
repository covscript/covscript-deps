# CovScript Dependencies

This repository contains essential build dependencies for the [CovScript Interpreter](https://github.com/covscript/covscript), bundling third-party libraries with CMake-based build configuration for multi-platform support.

## Included Libraries

### 1. **Parallel Hashmap** (`include/parallel_hashmap/`)
A fast, header-only hash table library offering parallel hashing and improved performance for concurrent access patterns.
- **License**: Apache 2.0
- **Source**: https://github.com/greg7mdp/parallel-hashmap
- **Distribution**: Header files included with CovScript SDK

### 2. **libucontext** (`libucontext/`)
A lightweight user-level context management library supporting multiple CPU architectures (x86, x86_64, ARM, aarch64, MIPS, RISC-V, PowerPC, s390x, and more). Implements POSIX-like context switching without kernel involvement.
- **License**: ISC License
- **Source**: https://github.com/kaniini/libucontext
- **Distribution**: Compiled as static library with CovScript SDK

### 3. **UTF8-CPP** (`utfcpp/`)
A simple, header-only C++ library for handling UTF-8 encoded strings with support for validation, iteration, and encoding conversions.
- **License**: Boost Software License 1.0
- **Source**: https://github.com/nemtrif/utfcpp
- **Distribution**: Compiled as static library with CovScript SDK

## Build Instructions

This project uses CMake with automatic platform detection. To build:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Platform Support

- **macOS**: Universal binary compilation (Intel x86_64 + ARM aarch64)
- **Linux**: Automatic CPU architecture detection
- **Windows**: Cross-compilation support available (note: libucontext is not supported on Windows as the platform provides native fiber APIs)

## Licensing and Attribution

### Third-Party Libraries

This repository contains third-party libraries distributed under their respective open-source licenses:

| Library | License | Repository |
|---------|---------|------------|
| Parallel Hashmap | Apache License 2.0 | https://github.com/greg7mdp/parallel-hashmap |
| libucontext | ISC License | https://github.com/kaniini/libucontext |
| UTF8-CPP | Boost Software License 1.0 | https://github.com/nemtrif/utfcpp |

Each library maintains its original license terms. For complete license text, refer to the LICENSE file within each library's directory.

### Open Source Acknowledgments

CovScript acknowledges and expresses gratitude to the creators and maintainers of:
- **Parallel Hashmap** – Developed and maintained by @greg7mdp
- **libucontext** – Developed and maintained by @kaniini and the open-source community
- **UTF8-CPP** – Developed and maintained by @nemtrif and contributors

Their contributions enable CovScript to deliver robust functionality for hash-based data structures, user-level context management, and Unicode text processing.
