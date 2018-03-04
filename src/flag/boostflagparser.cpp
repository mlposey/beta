#include <iostream>
#include "boostflagparser.h"

namespace beta {

BoostFlagParser::BoostFlagParser()
    : flagsDefinition("options") {
    flagsDefinition.add_options()
        ("help", "show usage information")
        (providerKey, po::value<std::string>(), "optionally specify a provider\n"
                                               "This should not include the file extension.\n"
                                               "E.g, use gmaps -- not gmaps.so\n"
                                               "To change the provider search path, alter\n"
                                               "the provider_dir setting in config.json.");
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
    return userInput.count(providerKey) ? userInput[providerKey].as<std::string>() : "";
}

}  // namespace beta