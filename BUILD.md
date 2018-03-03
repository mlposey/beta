# Build Instructions
These instructions are designed to produce static binaries as well as
a development/testing environment. They have been tested with Ubuntu
16.04 but will likely work in similar Unix systems with minor changes.

# Setup

## Get the source.
```
$ sudo apt install git
$ git clone https://github.com/mlposey/beta.git && cd beta
```
## Install required packages.
```
$ sudo apt install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev cmake wget curl autoconf libtool autogen unzip zlib1g-dev libssl-dev
```
## Install Boost.
```
$ wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
$ tar -xzf boost_1_66_0.tar.gz && cd boost_1_66_0
$ ./bootstrap.sh --prefix=/usr/local
$ sudo ./b2 -jN link=static install
$ cd ..
```
The value of `N` in the b2 invocation should be the number of threads to use.
Building Boost will take a while. I'm sorry.
## Install gRPC.
```
$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
$ cd grpc
$ git submodule update --init
$ cd third_party/protobuf && ./autogen.sh && ./configure && make -jN && make install && cd ../..
$ ldconfig
$ make -jN static
$ sudo make install
$ cd ..
```
`N` should assume the value it was previously given.
## Build the app.
```
$ cmake .
$ make -jN
```
The release artifacts are located in `build/release`.