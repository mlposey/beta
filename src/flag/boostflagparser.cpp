#include <iostream>
#include "boostflagparser.h"

namespace beta {

BoostFlagParser::BoostFlagParser()
    : flagsDefinition("options") {
    flagsDefinition.add_options()
        ("help", "show usage information")
        ("provider", po::value<std::string>(), "specify provider (e.g., gmaps)");
}

void BoostFlagParser::parse(int argc, char **argv) {
    po::store(po::parse_command_line(argc, argv, flagsDefinition), userInput);
    po::notify(userInput);
}

bool BoostFlagParser::wantsHelp() const {
    return userInput.count("help");
}

void BoostFlagParser::showHelp() const {
    std::cout << flagsDefinition << '\n';
}

std::string BoostFlagParser::provider() const {
    return userInput.count("provider") ? userInput["provider"].as<std::string>() : "";
}

}  // namespace beta