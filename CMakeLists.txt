cmake_minimum_required(VERSION 2.8.2)
project(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
INCLUDE_DIRECTORIES(src external)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-Wall -Werror -Ofast -fno-omit-frame-pointer")

set(FLAG_SRC
        src/flag/flagparser.h
        src/flag/boostflagparser.cpp)

set(PATH_SRC
        src/path/node.cpp
        src/path/path.h
        src/path/pathquery.h
        src/path/path2d.cpp
        src/path/shareablepath.cpp)

set(CACHE_SRC
        src/cache/cacheentry.h
        src/cache/pathcache.cpp
        src/cache/optimize/optimizer.cpp
        src/cache/index/pathindex.h
        src/cache/index/pathrtree.cpp)

set(PROVIDER_SRC
        src/provider/pathprovider.h
        src/provider/providerloader.cpp
        src/provider/provideradapter.cpp)

set(NET_SRC
        src/net/protobuf/beta.grpc.pb.cc src/net/protobuf/beta.grpc.pb.h
        src/net/protobuf/beta.pb.cc src/net/protobuf/beta.pb.h
        src/net/messageconverter.cpp src/net/messageconverter.h
        src/net/server.cpp)

set(CONFIG_SRC src/config/config.cpp)
set(EXTERNAL_SRC external/nlohmann/json.hpp)

set(SOURCE_FILES
        ${FLAG_SRC} ${PATH_SRC} ${CACHE_SRC} ${PROVIDER_SRC} ${NET_SRC}
        ${CONFIG_SRC} ${EXTERNAL_SRC}
        src/beta.cpp)

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
        tests/flag/boostflagparser.cpp
        tests/config/config.cpp
        tests/path/path2d.cpp
        tests/path/shareablepath.cpp
        tests/cache/cacheentry.cpp
        tests/cache/pathcache.cpp
        tests/cache/index/pathrtree.cpp
        tests/provider/providerloader.cpp)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests gtest ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})
