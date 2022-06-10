cmake_minimum_required(VERSION 3.14...3.22)

project(RogueTests LANGUAGES CXX)

# ---- IDE support ----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

cpmaddpackage("gh:onqtam/doctest@2.4.8")
cpmaddpackage("gh:TheLartians/Format.cmake@1.7.3")

if(TEST_INSTALLED_VERSION)
  find_package(rogue REQUIRED)
else()
  cpmaddpackage(NAME rogue SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create binary ----

add_executable(
  ${PROJECT_NAME} source/main.cpp source/Game.cpp
  source/core/utils/Logger.cpp
  source/core/utils/Assertion.cpp
  source/core/memory/SmartPointer.cpp
  source/core/memory/PoolAllocator.cpp
  source/core/types/Stack.cpp
)
target_link_libraries(${PROJECT_NAME} rogue::rogue doctest::doctest)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(rogue PUBLIC -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(rogue PUBLIC /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add GreeterTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(rogue PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(rogue PUBLIC -fprofile-arcs -ftest-coverage)
endif()
