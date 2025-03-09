/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 05-2024
 */

#pragma once

#include <nlohmann/json.hpp>

class LogConfig
{
   public:
    LogConfig();

    bool enabled{true};
    bool enableSignalManager{true};
    bool enableSizeRotation{true};
    bool enableTimeRotation{true};
    bool enableCloseRotation{true};
    bool enableCrashStackPrint{true};
    bool disableRotation{true};
    int maxFileSize{2097152};
    std::tuple<int /*hour*/, int /*minutes*/, int /*seconds*/> rotationTime{23, 59, 59};
    std::string signalFile{"/usr/local/logs/signal.log"};
    std::string fileName{"mylog.log"};
    std::string basePath{"/usr/local/logs"};
    std::string archivePath{"/usr/local/logs/archive"};

    bool isPresent() const;

   private:
    bool _isPresent{true};
    void fromJson(const nlohmann::json& j);
    const std::string _configFile{"./configuration/logconfig.json"};
};