FROM debian:8 as builder
LABEL maintainer="Marcus Posey contact@marcusposey.com"

WORKDIR /app

RUN apt-get update && apt-get install -y \
    git build-essential g++ python-dev autotools-dev \
    libicu-dev build-essential libbz2-dev cmake wget \
    curl autoconf libtool autogen unzip

RUN clone https://github.com/mlposey/beta.git && cd beta

# Install Boost.
# It is likely the default apt version is not high enough to support
# Boost.DLL. Thus, we build from source.
RUN wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz \
 && tar -xzf boost_1_66_0.tar.gz && cd boost_1_66_0 \
 && ./bootstrap.sh --prefix=/usr/local \
 && ./b2 -j $(grep -c ^processor /proc/cpuinfo) link=static install \
 && cd ..

# Install gRPC.
RUN git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc \
 && cd grpc \
 && git submodule update --init \
 && cd third_party/protobuf && ./autogen.sh && ./configure && make && make install && cd ../.. \
 && make -j $(grep -c ^processor /proc/cpuinfo) static \
 && make install \
 && cd ..

RUN cmake . && make -j $(grep -c ^processor /proc/cpuinfo)

# -----------------------------------------------------------------------------
FROM alpine:3.7

WORKDIR /app
COPY --from=builder /app/build/release/ .

ENTRYPOINT ./beta
