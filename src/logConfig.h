/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#pragma once

#include <nlohmann/json.hpp>

class LogConfig
{
   public:
    LogConfig();

    bool _enabled{true};
    bool _enableSignalManager{false};
    bool _enableSizeRotation{false};
    bool _enableTimeRotation{false};
    bool _enableCloseRotation{false};
    bool _enableCrashStackPrint{false};
    bool _enableRotation{false};
    bool _enableTime{true};
    bool _enableWriteToFile{true};
    int _maxFileSize{2097152};
    std::tuple<int /*hour*/, int /*minutes*/, int /*seconds*/> _rotationTime{23, 59, 59};
    std::string _signalFile{"signal.log"};
    std::string _fileName{"mylog.log"};
    std::string _basePath{"/usr/local/logs"};
    std::string _archivePath{"/usr/local/archive"};

    bool isPresent() const;

   private:
    bool _isPresent{true};
    void fromJson(const nlohmann::json& j);
    const std::string _configFile{"./configuration/logconfig.json"};
};