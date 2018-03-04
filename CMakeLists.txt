cmake_minimum_required(VERSION 2.8.2)
project(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system filesystem)
INCLUDE_DIRECTORIES(src external)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        src/flag/flagparser.h
        src/beta.cpp
        external/nlohmann/json.hpp
        src/config/config.cpp src/config/config.h
        src/path/node.cpp src/path/node.h
        src/path/path.h
        src/path/path2d.cpp src/path/path2d.h
        src/path/shareablepath.cpp src/path/shareablepath.h
        src/cache/cacheentry.h
        src/cache/optimizer.cpp src/cache/optimizer.h
        src/cache/pathcache.cpp src/cache/pathcache.h
        src/provider/pathprovider.h
        src/provider/providerloader.cpp src/provider/providerloader.h
        src/net/protobuf/beta.grpc.pb.cc src/net/protobuf/beta.grpc.pb.h
        src/net/protobuf/beta.pb.cc src/net/protobuf/beta.pb.h
        src/net/messageconverter.hpp
        src/net/server.cpp src/net/server.h)

add_executable(beta ${SOURCE_FILES})
target_link_libraries(beta
        ${CMAKE_DL_LIBS}
        ${Boost_LIBRARIES}
        -lpthread -lm
        libgrpc++.a libgrpc.a libprotobuf.a
        -lz
        libssl.a libcrypto.a
        -static-libstdc++)

set(BETA_RELEASE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release)
add_custom_command(TARGET beta POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BETA_RELEASE_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BETA_RELEASE_DIRECTORY}/providers
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:beta> ${BETA_RELEASE_DIRECTORY}/.
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json ${BETA_RELEASE_DIRECTORY}/.)

file(COPY tests/provider/mock src/config/config.json
     DESTINATION build/)

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
        tests/config/config.cpp
        tests/path/path2d.cpp
        tests/path/shareablepath.cpp
        tests/cache/pathcache.cpp
        tests/provider/providerloader.cpp)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests gtest ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})
