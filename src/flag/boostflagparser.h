#pragma once
#include <boost/program_options.hpp>
#include "flagparser.h"

namespace po = boost::program_options;

namespace beta {

/** Uses Boost.ProgramOptions to parse command-line flags */
class BoostFlagParser : public FlagParser {
public:
    BoostFlagParser();

    /** Parses all command-line arguments */
    void parse(int argc, char **argv) override;

    /** Determines if the --help flag was provided */
    bool wantsHelp() const override;
    /** Displays a help message */
    void showHelp() const override;

    /** Returns the value passed to --provider or "" if missing */
    std::string provider() const override;

private:
    po::options_description flagsDefinition;
    po::variables_map userInput;
};

}  // namespace beta