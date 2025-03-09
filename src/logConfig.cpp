/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#include "logConfig.h"

#include <fstream>
#include <iostream>

LogConfig::LogConfig()
{
    std::ifstream ifs(_configFile);
    if (!ifs.is_open())
    {
        std::cerr << "WARNING Cannot open logConfig.json";
        _isPresent = false;
        return;
    }
    nlohmann::json j = nlohmann::json::parse(ifs);
    fromJson(j);
}

void LogConfig::fromJson(const nlohmann::json& j)
{
    if (j.count("enableSignalManager"))
    {
        j.at("enableSignalManager").get_to(_enableSignalManager);
    }
    if (j.count("enableSizeRotation"))
    {
        j.at("enableSizeRotation").get_to(_enableSizeRotation);
    }
    if (j.count("enableTimeRotation"))
    {
        j.at("enableTimeRotation").get_to(_enableTimeRotation);
    }
    if (j.count("enableCloseRotation"))
    {
        j.at("enableCloseRotation").get_to(_enableCloseRotation);
    }
    if (j.count("enableCrashStackPrint"))
    {
        j.at("enableCrashStackPrint").get_to(_enableCrashStackPrint);
    }
    if (j.count("enableRotation"))
    {
        j.at("enableRotation").get_to(_enableRotation);
    }
    if (j.count("enableTime"))
    {
        j.at("enableTime").get_to(_enableTime);
    }
    if (j.count("maxFileSize"))
    {
        j.at("maxFileSize").get_to(_maxFileSize);
    }
    if (j.count("rotationTime"))
    {
        auto rotationTimeObj = j.at("rotationTime");
        _rotationTime = std::make_tuple(rotationTimeObj.at("hour"), rotationTimeObj.at("minutes"), rotationTimeObj.at("seconds"));
    }
    if (j.count("signalFile"))
    {
        j.at("signalFile").get_to(_signalFile);
    }
    if (j.count("fileName"))
    {
        j.at("fileName").get_to(_fileName);
    }
    if (j.count("basePath"))
    {
        j.at("basePath").get_to(_basePath);
    }
    if (j.count("archivePath"))
    {
        j.at("archivePath").get_to(_archivePath);
    }
    if (j.count("enabled"))
    {
        j.at("enabled").get_to(_enabled);
    }
    if (j.count("enableWriteToFile"))
    {
        j.at("enableWriteToFile").get_to(_enableWriteToFile);
    }
}

bool LogConfig::isPresent() const
{
    return _isPresent;
}
