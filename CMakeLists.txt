cmake_minimum_required(VERSION 2.8.2)
project(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
INCLUDE_DIRECTORIES(src external)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        src/beta.cpp
        src/path/path.h
        src/path/path2d.cpp src/path/path2d.h
        src/path/shareablepath.cpp src/path/shareablepath.h
        src/cache/cacheentry.h
        src/cache/optimizer.cpp src/cache/optimizer.h
        src/cache/pathcache.cpp src/cache/pathcache.h)

add_executable(beta ${SOURCE_FILES})

configure_file(external/CMakeLists.txt.in external/gtest/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/gtest/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/gtest/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/external/gtest/googletest-src
                 ${CMAKE_BINARY_DIR}/external/gtest/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(TEST_FILES
        tests/main.cpp
        tests/path/path2d.cpp
        tests/path/shareablepath.cpp
        tests/cache/pathcache.cpp)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests gtest)
