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
There are two options for running the app: using Docker and building from source.
## Using Docker
The Dockerfile will produce a minimal application image to deploy. Simply
run `docker build -t beta .` within the root repository directory. The image
exposes the default port `1995`.

## Building from Source
[BUILD](BUILD.md) contains detailed instructions for building binaries in a
Unix environment.