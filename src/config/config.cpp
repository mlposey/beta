#include <fstream>
#include "config.h"

namespace beta {

Config::Config() {
    loadParams();
}

std::string Config::getString(std::string key) {
    auto it = params.find(key);
    if (it == params.end()) {
        return "";
    }
    return *it;
}

void Config::loadParams() {
    std::ifstream configFile("./config.json");
    configFile >> params;
}

}  // namespace beta