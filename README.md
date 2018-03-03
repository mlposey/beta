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
There are three options for running the app: using Docker, downloading
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