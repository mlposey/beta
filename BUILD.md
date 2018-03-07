# Build Instructions
These instructions are designed to produce static binaries as well as
a development/testing environment. They have been tested with Ubuntu
16.04 but will likely work in similar Unix systems with minor changes.

# Setup

## Get the source.
The [releases](https://github.com/mlposey/beta/releases) page contains stable
archives of the source code. Download the latest archive and enter the root
project directory.
## Install required packages.
```
$ sudo apt install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev cmake wget curl autoconf libtool autogen unzip zlib1g-dev libssl-dev git
```
## Install Boost.
```
$ wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
$ tar -xzf boost_1_66_0.tar.gz && cd boost_1_66_0
$ ./bootstrap.sh --prefix=/usr/local
$ sudo ./b2 -j $(grep -c ^processor /proc/cpuinfo) link=static install
$ cd ..
```
## Install gRPC.
```
$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
$ cd grpc
$ git submodule update --init
$ cd third_party/protobuf
$ ./autogen.sh && ./configure && make -j $(grep -c ^processor /proc/cpuinfo) && make install && cd ../..
$ ldconfig
$ make -j $(grep -c ^processor /proc/cpuinfo) static
$ sudo make install
$ cd ..
```
## Build the app.
```
$ cmake .
$ make -j $(grep -c ^processor /proc/cpuinfo)
```
The release artifacts are located in `build/release`.