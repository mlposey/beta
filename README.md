# BeTA
BeTA is a caching system that maintains an optimal set of paths whose
subpaths are used to answer queries. It uses gRPC for client communication
and allows developers to write path provider plugins that can fetch paths
from external services in the event of a cache miss.

# Status
This project is in a very early stage. As such, it is not recommended
for production use due to both the scope of testing and stability of
the gRPC and Provider APIs. The name may even change; *SearchyFindyThing*
sounds better every day.

# Running the App

## Using Docker
Docker can be used to build a minimal container for the application. Simply
run `docker build -t beta .` within the root repository directory. The image
exposes the default port `1995`.

## Building from Source
### Dependencies
The following dependencies are required to build the application:
* g++ with support for C++14
* CMake 2.8.2+
* Boost
* gRPC
* A Unix environment

### Setup
This process assumes an Ubuntu 16.04 environment; however, it should be similar for
other Unix-like systems.
#### Get the source.
```
$ sudo apt install git
$ git clone https://github.com/mlposey/beta.git && cd beta
```
#### Install the base build tools.
```
$ sudo apt install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev cmake wget curl autoconf libtool autogen unzip zlib1g-dev libssl-dev
```
#### Install Boost.
```
$ wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
$ tar -xzf boost_1_66_0.tar.gz && cd boost_1_66_0
$ ./bootstrap.sh --prefix=/usr/local
$ sudo ./b2 -jN link=static install
$ cd ..
```
The value of `N` in the b2 invocation should be the number of threads to use.
Building Boost will take a while. I'm sorry.
#### Install gRPC.
```
$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
$ cd grpc
$ git submodule update --init
$ cd third_party/protobuf && ./autogen.sh && ./configure && make && make install && cd ../..
$ ldconfig
$ make -jN static
$ sudo make install
$ cd ..
```
`N` should assume the value it was previously given.
### Build the app.
```
$ cmake .
$ make -jN
```
The release artifacts are located in `build/release`.