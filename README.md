# BeTA
[![Build Status](http://jenkins.marcusposey.com:8081/buildStatus/icon?job=mlposey/beta/master)](http://jenkins.marcusposey.com:8081/job/mlposey/job/beta/job/master/)

BeTA is a caching system that manages geospatial paths. It uses [gRPC](https://grpc.io/about/)
for networking and lets developers write plugins that fetch external paths
in the event of a cache miss.

# Status
This project is in a very early stage. As such, it is not recommended
for production use due to both the scope of testing and stability of
the gRPC and Provider APIs. The name may even change; *SearchyFindyThing*
sounds better every day.

# Documentation
Doxygen produces the documentation in the `docs` folder. You can find a live
version of the docs [here](https://mlposey.github.io/beta/annotated.html).

# Running the App
There are three options for running the app: using [Docker](https://docs.docker.com/engine/docker-overview/), downloading
binaries, and building from source.

## Using Docker
A pre-built image of the latest release exists at `mlposey/beta`. If you wish
to perform a manual build, you can do so using [the Dockerfile](Dockerfile).
Simply run `docker build -t beta .` within the root repository directory. The
image exposes the default port `1995`.

## Downloading Binaries
Each release contains an archive with pre-built binaries. You can find all
releases [here](https://github.com/mlposey/beta/releases).

## Building from Source
[BUILD](BUILD.md) contains detailed instructions for building binaries in a
Unix environment.
