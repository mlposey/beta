#pragma once
#include <string>
#include <memory>
#include "nlohmann/json.hpp"

namespace beta {

/** Provides access to global application parameters */
class Config {
public:
    /**
     * Gets the singleton instance of Config
     * @throws std::exception if the config file is missing
     */
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    /** Gets the configuration value of a given key or "" if the key is unrecognized */
    std::string getString(std::string key);

protected:
    Config();

private:
    void loadParams();
    nlohmann::json params;
};

}  // namespace beta