#include <fstream>
#include "config.h"

namespace beta {

Config::Config() {
    loadParams();
}

std::string Config::get(std::string key) {
    return params[key];
}

void Config::loadParams() {
    std::ifstream configFile("./config.json");
    configFile >> params;
}

}  // namespace beta