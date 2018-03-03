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

    /**
     * Gets the configuration value for a key that maps to a storage size
     * 
     * A storage size is a string type that contains a positive number
     * followed by a unit. Units may be
     *          not included  -  indicates the unit is bytes
     *                    kB  -  kilobytes
     *                    MB  -  megabytes
     *                    GB  -  gigabytes
     * Examples:
     *      "100"   = 100 bytes
     *      "250MB" = 250 megabytes
     * 
     * @returns the size converted to bytes or 0 if the key is unrecognized
     */
    size_t getBytes(std::string key);

protected:
    Config();

private:
    void loadParams();
    size_t getByteMultiplier(std::string param);

    nlohmann::json params;
};

}  // namespace beta