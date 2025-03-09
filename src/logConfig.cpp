/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 05-2024
 */

#include "logConfig.h"

#include <fstream>
#include <iostream>

LogConfig::LogConfig()
{
    std::ifstream ifs(_configFile);
    if (!ifs.is_open())
    {
        std::cerr << "ERROR Cannot open logConfig.json";
        _isPresent = false;
        return;
    }
    std::cerr << "***Use log configuration from " << _configFile;
    nlohmann::json j = nlohmann::json::parse(ifs);
    fromJson(j);
}

void LogConfig::fromJson(const nlohmann::json& j)
{
    if (j.count("enableSignalManager"))
    {
        j.at("enableSignalManager").get_to(enableSignalManager);
    }
    if (j.count("enableSizeRotation"))
    {
        j.at("enableSizeRotation").get_to(enableSizeRotation);
    }
    if (j.count("enableTimeRotation"))
    {
        j.at("enableTimeRotation").get_to(enableTimeRotation);
    }
    if (j.count("enableCloseRotation"))
    {
        j.at("enableCloseRotation").get_to(enableCloseRotation);
    }
    if (j.count("enableCrashStackPrint"))
    {
        j.at("enableCrashStackPrint").get_to(enableCrashStackPrint);
    }
    if (j.count("disableRotation"))
    {
        j.at("disableRotation").get_to(disableRotation);
    }
    if (j.count("maxFileSize"))
    {
        j.at("maxFileSize").get_to(maxFileSize);
    }
    if (j.count("rotationTime"))
    {
        auto rotationTimeObj = j.at("rotationTime");
        rotationTime = std::make_tuple(rotationTimeObj.at("hour"), rotationTimeObj.at("minutes"), rotationTimeObj.at("seconds"));
    }
    if (j.count("signalFile"))
    {
        j.at("signalFile").get_to(signalFile);
    }
    if (j.count("fileName"))
    {
        j.at("fileName").get_to(fileName);
    }
    if (j.count("basePath"))
    {
        j.at("basePath").get_to(basePath);
    }
    if (j.count("archivePath"))
    {
        j.at("archivePath").get_to(archivePath);
    }
    if (j.count("enabled"))
    {
        j.at("enabled").get_to(enabled);
    }
}

bool LogConfig::isPresent() const
{
    return _isPresent;
}
