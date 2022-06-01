[![Actions Status](https://github.com/mx-payn/rogue-engine/workflows/MacOS/badge.svg)](https://github.com/mx-payn/rogue-engine/actions)
[![Actions Status](https://github.com/mx-payn/rogue-engine/workflows/Windows/badge.svg)](https://github.com/mx-payn/rogue-engine/actions)
[![Actions Status](https://github.com/mx-payn/rogue-engine/workflows/Ubuntu/badge.svg)](https://github.com/mx-payn/rogue-engine/actions)
[![Actions Status](https://github.com/mx-payn/rogue-engine/workflows/Style/badge.svg)](https://github.com/mx-payn/rogue-engine/actions)
[![Actions Status](https://github.com/mx-payn/rogue-engine/workflows/Install/badge.svg)](https://github.com/mx-payn/rogue-engine/actions)
[![codecov](https://codecov.io/gh/mx-payn/rogue-engine/branch/master/graph/badge.svg)](https://codecov.io/gh/mx-payn/rogue-engine)

# Rogue-Engine

A 2D rogue-like engine offering special modules to check and compile code of foreign programming languages. The aim is of the engine is
to provide functionalities for educational games on programming languages.

## Roadmap

- [] -- v0.1 -- simple 'snake' clone
- [] -- v0.2 -- original 'rogue' clone
- [] -- v0.3 -- 'rogue' with 'code combat system'
- [] -- v1.0 -- 'rogue-haskell'

## Usage

The engine is built upon the [ModernCppStarter template](https://github.com/TheLartians/ModernCppStarter) and uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)
for package and dependency management. The repository includes standalone targets as example programs and development test environments.

### Add to your own project

#### With CPM.cmake

``` cmake
include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME rogue-engine 
  SOURCE_DIR ${PATH_TO_ROGUE_ENGINE_ROOT}
)

# ...

target_link_libraries(${PROJECT_NAME} rogue-engine)
```

#### As submodule

``` bash
git submodule add https://github.com/mx-payn/rogue-engine submodules/rogue-engine
```

This will only call the engines CMakeLists.txt and not any of the repository's sub-features

``` cmake
add_subdirectory(submodules/rogue-engine)

# ...

target_link_libraries(${PROJECT_NAME} rogue-engine)
```

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/Greeter --help
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# or simply call the executable: 
./build/test/GreeterTests
```

To collect code coverage information, run CMake with the `-DENABLE_TEST_COVERAGE=1` option.

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -S test -B build/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details.

### Build the documentation

The documentation is automatically built and [published](https://mx-payn.github.io/rogue-engine) whenever a [GitHub Release](https://help.github.com/en/github/administering-a-repository/managing-releases-in-a-repository) is created.
To manually build documentation, call the following command.

```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
# view the docs
open build/doc/doxygen/html/index.html
```

To build the documentation locally, you will need Doxygen, jinja2 and Pygments on installed your system.

### Build everything at once

The project also includes an `all` directory that allows building all targets at the same time.
This is useful during development, as it exposes all subprojects to your IDE and avoids redundant builds of the library.

```bash
cmake -S all -B build
cmake --build build

# run tests
./build/test/GreeterTests
# format code
cmake --build build --target fix-format
# run standalone
./build/standalone/Greeter --help
# build docs
cmake --build build --target GenerateDocs
```

### Additional tools

The test and standalone subprojects include the [tools.cmake](cmake/tools.cmake) file which is used to import additional tools on-demand through CMake configuration arguments.
The following are currently supported.

#### Sanitizers

Sanitizers can be enabled by configuring CMake with `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`.

#### Static Analyzers

Static Analyzers can be enabled by setting `-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>`, or a combination of those in quotation marks, separated by semicolons.
By default, analyzers will automatically find configuration files such as `.clang-format`.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS` variables.

#### Ccache

Ccache can be enabled by configuring with `-DUSE_CCACHE=<ON | OFF>`.

