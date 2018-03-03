FROM debian:9 as builder
LABEL maintainer="Marcus Posey contact@marcusposey.com"

WORKDIR /app

RUN apt-get update && apt-get install -y \
    git build-essential g++ python-dev autotools-dev   \
    libicu-dev build-essential libbz2-dev cmake wget   \
    curl autoconf libtool autogen unzip git zlib1g-dev \
    libssl-dev

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
 && cd third_party/protobuf && ./autogen.sh && ./configure \
 && make -j $(grep -c ^processor /proc/cpuinfo) && make install && cd ../.. \
 && ldconfig \
 && make -j $(grep -c ^processor /proc/cpuinfo) static \
 && make install \
 && cd ..

WORKDIR beta
COPY . .

RUN cmake . && make -j $(grep -c ^processor /proc/cpuinfo)

# -----------------------------------------------------------------------------

FROM debian:9-slim

WORKDIR /app
COPY --from=builder /app/beta/build/release/ .

ENTRYPOINT ./beta
