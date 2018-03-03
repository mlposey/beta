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

size_t Config::getBytes(std::string key) {
    std::string raw = getString(key);
    if (raw.empty()) return 0;
    else if (raw.length() < 3) return atoi(raw.c_str());

    size_t multiplier = getByteMultiplier(raw);
    if (multiplier == 1) return atoi(raw.c_str());

    std::string amount = raw.substr(0, raw.size() - 1);
    return atoi(amount.c_str()) * multiplier;
}

size_t Config::getByteMultiplier(std::string param) {
    std::string unit = param.substr(param.size() - 2);    
    if (unit == "kB") return 1'000;
    else if (unit == "MB") return 1'000'000;
    else if (unit == "GB") return 1'000'000'000;
    else return 1;
}

void Config::loadParams() {
    std::ifstream configFile("./config.json");
    configFile >> params;
}

}  // namespace beta