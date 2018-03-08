#pragma once
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>
#include "protobuf/beta.grpc.pb.h"
#include "cache/pathcache.h"
#include "provider/provideradapter.h"

using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using betagrpc::Beta;
using betagrpc::RouteQuery;
using betagrpc::Route;
using betagrpc::AddResponse;

namespace beta {

/** Routes gRPC requests to the internal cache */
class Server final : public Beta::Service {
public:
    /**
     * Creates a new server instance
     * @param cache the cache used to answer requests
     * @param provider an optional provider to handle cache misses; may be nullptr
     */
    Server(PathCache cache, std::shared_ptr<ProviderAdapter> provider);

    /** Starts a blocking instance of the server at the given port */
    void listen(std::string port);

    Status GetRoute(ServerContext *ctx, const RouteQuery *query, Route *route) override;
    Status MustGetRoute(ServerContext *ctx, const RouteQuery *query, Route *route) override;
    Status AddRoute(ServerContext *ctx, const Route *route, AddResponse *res) override;

private:
    PathCache cache;
    std::shared_ptr<ProviderAdapter> provider;
};

}  // namespace beta
