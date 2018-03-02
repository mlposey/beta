# BeTA
BeTA is a caching system that maintains an optimal set of paths
whose subpaths are used to answer queries. It uses gRPC for client
communication and allows developers to write path provider plugins
that are used to fetch and store paths in the event of a cache miss.

# Status
This project is in a very early stage. As such, it is not recommended
for production use due to both the scope of testing and stability of
the gRPC and Provider APIs. The name may even change; *SearchyFindyThing*
sounds nicer every day.