cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

project (
  "shush-cpu"
  VERSION 1.0.0
  DESCRIPTION "A CPU emulator that takes custom bytecode files and executes them."
)
set (
  SOURCES
  src/${PROJECT_NAME}.cpp
)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - LIBRARY INITIALIZATION- - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_executable(${PROJECT_NAME} ${SOURCES})

set(BUILD_TESTS OFF CACHE BOOL "Build tests")
if (BUILD_TESTS)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - GTEST INITIALIZATION- - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_subdirectory(googletest)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include_directories(${gtest_SOURCE_DIR})

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - TESTS - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set(UNIT_TESTS_NAME "run-tests-${PROJECT_NAME}")
set(UNIT_TESTS_FILE "test/test.cpp")

add_executable(${UNIT_TESTS_NAME} ${UNIT_TESTS_FILE})

# Link test executable against gtest & gtest_main
target_link_libraries(${UNIT_TESTS_NAME} gtest_main ${LIBRARY_NAME})
add_test(${UNIT_TESTS_NAME} ${UNIT_TESTS_NAME})

# For next libraries.
set(BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)

endif() # BUILD_TESTS

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - DEPENDENCIES- - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_subdirectory(submodules/shush-stack)
add_subdirectory(submodules/shush-logs)
add_subdirectory(submodules/shush-dump)
add_subdirectory(submodules/shush-format)
add_subdirectory(submodules/shush-file)

target_link_libraries(shush-logs shush-format)
target_link_libraries(shush-dump shush-format)
target_link_libraries(shush-file shush-dump)
target_link_libraries(shush-stack INTERFACE shush-logs shush-dump)

target_link_libraries(shush-cpu shush-stack shush-logs shush-dump shush-file)
include_directories(include inc)
