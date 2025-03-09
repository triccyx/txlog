/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 05-2024
 */

#include "logDepot.h"

#include <cxxabi.h>  // for __cxa_demangle
#include <dlfcn.h>   // for dladdr
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include "logconst.h"

//#include "backward.hpp"

LogDepot::LogDepot(LogConfig& logConfig)
    : _logConfig(logConfig)
{
    if (logConfig.isPresent())
    {
        _enableSizeRotation = logConfig.enableSizeRotation;
        _enableTimeRotation = logConfig.enableTimeRotation;
        _enableCloseRotation = logConfig.enableCloseRotation;
        _enablePrintCrashStack = logConfig.enableCrashStackPrint;
        _maxFileSize = logConfig.maxFileSize;
        _rotationTime = logConfig.rotationTime;
        _signalFile = logConfig.signalFile;
        _fileName = logConfig.fileName;
        _basePath = logConfig.basePath;
        _archivePath = logConfig.archivePath;
        std::cerr << "LogConfig found" << std::endl;
    }
    else
    {
        std::cerr << "ERROR LogConfig not found" << std::endl;

        _logDisabled = true;
        return;
    }

    createFolderIfNotExist(_basePath);
    createFolderIfNotExist(_archivePath);
    createFileIfNotExist();

    _rotateInTimeThread = std::make_unique<std::thread>(&LogDepot::rotateIfTimeIsReached, this);
}

void LogDepot::createFileIfNotExist()
{
    _fs = std::ofstream(_basePath + "/" + _fileName, std::ofstream::out | std::ofstream::app);
}

LogDepot::~LogDepot()
{
    if (_enableCloseRotation)
    {
        std::cerr << "Do close rotation" << std::endl;
        logInsideLogger(" | ROTATION   | Do rotation for close application");
        rotateSingle(_signalCloseApplication);
    }

    _rotationCheckTimeIsActive = false;
    _rotateInTimeThread->join();
}

bool LogDepot::log(const std::string& toLog, const std::string& toLogToFile)
{
    if (!_logConfig.enabled)
    {
        return true;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    std::cerr << toLog << std::flush << std::endl;
    _fs << toLogToFile << std::flush << std::endl;

    // rotateIfSizeExceeded();
    return true;
}

void LogDepot::createFolderIfNotExist(const std::string& path)
{
    try
    {
        std::filesystem::create_directories(path);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error creating folder:" << path << " " << e.what() << std::endl;
    }
}

bool LogDepot::canRotate(const std::string& fileName) const
{
    try
    {
        if (std::filesystem::file_size(fileName) == 0)
        {
            // do not rotate empty file
            std::cerr << "Do not rotate empty file";
            return false;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error rotating file check size:" << fileName << " " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool LogDepot::rotateSingle(int signum)
{
    std::string fileToRotate = _basePath + "/" + _fileName;
    std::cerr << "Try rotateSingleFile:" << fileToRotate << std::endl;
    if (!canRotate(fileToRotate))
    {
        return false;
    }
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _fs.close();
    }

    rotateFileInFs(fileToRotate);

    std::lock_guard<std::mutex> lock(_mutex);
    _fs = std::ofstream(fileToRotate);

    // signal file
    _ofs.open(_signalFile, std::ofstream::out | std::ofstream::app);
    _ofs << getDateWithMsecNow() << " | Rotate file:" << fileToRotate << " for signal:" << signum << " |";
    _ofs.close();
    return true;
}

bool LogDepot::rotateIfSizeExceeded()
{
    if (!_enableSizeRotation)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_fs.tellp() > _maxFileSize)
    {
        std::stringstream ss;
        ss << " | ROTATION   | Do rotation for dimension exceeded";
        logInsideLogger(ss.str());
        rotateSingle(_signalSizeExceed);
        return true;
    }
    return false;
}

void LogDepot::rotateIfTimeIsReached()
{
    while (_rotationCheckTimeIsActive)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (!_enableTimeRotation)
        {
            continue;
        }

        if (checkIfItIsTimeToRotate())
        {
            rotateSingle(_signalTimeOut);
        }
    }
}

bool LogDepot::checkIfItIsTimeToRotate()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // Extract the components of the current time
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* timeinfo = std::localtime(&currentTime);

    // Check if it's time to rotate
    if (timeinfo->tm_hour == std::get<0>(_rotationTime) && timeinfo->tm_min == std::get<1>(_rotationTime) && timeinfo->tm_sec == std::get<2>(_rotationTime))
    {
        std::cout << "It is midnight, rotate!";
        logInsideLogger("| Do rotation for timeout\n");
        return true;
    }

    return false;
}

int LogDepot::secondsToMidnight() const  // Not used for now
{
    // Get the current time
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // Calculate the time until the next midnight
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm midnight_tm = *std::localtime(&currentTime);

    midnight_tm.tm_hour = 24;
    midnight_tm.tm_min = 0;
    midnight_tm.tm_sec = 0;

    std::chrono::system_clock::time_point midnight = std::chrono::system_clock::from_time_t(std::mktime(&midnight_tm));
    std::chrono::seconds time_until_midnight = std::chrono::duration_cast<std::chrono::seconds>(midnight - now);

    // Get the number of seconds remaining until midnight
    int seconds_until_midnight = time_until_midnight.count();

    std::cout << "Seconds remaining until midnight: " << seconds_until_midnight << " seconds";
    return seconds_until_midnight;
}

void LogDepot::signalRecived(int signal)
{
    if (_enablePrintCrashStack)
    {
        logInsideLogger(myBacktrace());
    }
    std::stringstream ss;
    ss << " | ROTATION   | Signal received:" << signal << " | Do rotation for signal";
    logInsideLogger(ss.str());
    std::cerr << ss.str() << std::endl;

    rotateSingle(signal);
    // kill(getpid(), signum);
    std::quick_exit(0);
}

void LogDepot::rotateFileInFs(const std::string& fileName)
{
    try
    {
        std::string archieveName = calculateArchieveFileName();

        // std::filesystem::make_directory(archieveName);
        if (!_disableRotation)
        {
            std::filesystem::copy(fileName, archieveName);
        }
        std::filesystem::remove(fileName);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error rotating file:" << fileName << " " << e.what() << std::endl;
    }
}

void LogDepot::logInsideLogger(const std::string& toWrite)
{
    std::cerr << toWrite << std::endl;
    ;
    std::lock_guard<std::mutex> lock(_mutex);

    _fs << getDateWithMsecNow() << toWrite
        << std::flush;
}

std::string LogDepot::myBacktrace(int skip)
{
    return "";
    /*
    using namespace backward;
    StackTrace st;
    st.load_here(32);
    Printer p;
    std::stringstream ss;
    p.print(st, ss);
    return ss.str();
    */
}

std::string LogDepot::calculateArchieveFileName()
{
    std::stringstream ss;
    ss << _archivePath << "/" << getDateWithoutMsecNow() + "_" + _fileName;
    return ss.str();
}