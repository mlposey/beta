#pragma once
#include <string>

namespace beta {

/** Describes a type that parses command line flags */
struct FlagParser {
    virtual ~FlagParser() = default;
    virtual void parse(int argc, char **argv) = 0;
    virtual bool wantsHelp() const = 0;
    virtual void showHelp() const = 0;
    virtual std::string provider() const = 0;
};

}  // namespace beta