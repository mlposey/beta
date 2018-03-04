#pragma once
#include <string>

namespace beta {

/** Describes a type that parses command line flags */
struct FlagParser {
    virtual ~FlagParser() = delete;
    virtual void parse(int argc, char **argv) = 0;
    virtual std::string provider() const = 0;
};

}  // namespace beta