/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
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

// #include "backward.hpp"

LogDepot::LogDepot(LogConfig& logConfig)
    : _logConfig(logConfig)
{
    if (logConfig.isPresent())
    {
        _signalFile = logConfig._signalFile;
        _fileName = logConfig._fileName;
        _basePath = logConfig._basePath;
        _archivePath = logConfig._archivePath;
    }
    else
    {
        std::cerr << "WARNING LogConfig not found" << std::endl;

        _logDisabled = true;
        return;
    }

    createFileIfNotExist();

    _rotateInTimeThread = std::make_unique<std::thread>(&LogDepot::rotateIfTimeIsReached, this);
}

void LogDepot::createFileIfNotExist()
{
    createFolderIfNotExist(_basePath);
    createFolderIfNotExist(_archivePath);
    _fs = std::ofstream(_basePath + "/" + _fileName, std::ofstream::out | std::ofstream::app);
}

LogDepot::~LogDepot()
{
    if (_logConfig._enableCloseRotation)
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
    if (!_logConfig._enabled)
    {
        return true;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    std::cerr << toLog << std::flush << std::endl;

    if (_logConfig._enableWriteToFile)
    {
        _fs << toLogToFile << std::flush << std::endl;
        rotateIfSizeExceeded();
    }
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
        _fs.close();
    }

    rotateFileInFs(fileToRotate);

    _fs = std::ofstream(fileToRotate);

    // update signal file
    _ofs = std::ofstream(_basePath + "/" + _signalFile, std::ofstream::out | std::ofstream::app);
    _ofs << getDateWithMsecNow() << " | Rotate file:" << fileToRotate << " for signal:" << signum << " |"<<std::endl;
    return true;
}

bool LogDepot::rotateIfSizeExceeded()
{
    if (!_logConfig._enableSizeRotation)
    {
        return true;
    }

    if (_fs.tellp() > _logConfig._maxFileSize)
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
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (!_logConfig._enableTimeRotation)
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
    if (timeinfo->tm_hour == std::get<0>(_logConfig._rotationTime) && timeinfo->tm_min == std::get<1>(_logConfig._rotationTime) && timeinfo->tm_sec == std::get<2>(_logConfig._rotationTime))
    {
        logInsideLogger("| Do rotation for timeout\n");
        return true;
    }

    return false;
}

void LogDepot::signalRecived(int signal)
{
    if (_logConfig._enableCrashStackPrint)
    {
        logInsideLogger(myBacktrace());
    }

    // Convert signal number to string representation
    std::string signalName;
    switch (signal) {
        case SIGINT:
            signalName = "SIGINT";
            break;
        case SIGTERM:
            signalName = "SIGTERM";
            break;
        case SIGSEGV:
            signalName = "SIGSEGV";
            break;
        case SIGABRT:
            signalName = "SIGABRT";
            break;
        case SIGFPE:
            signalName = "SIGFPE";
            break;
        case SIGILL:
            signalName = "SIGILL";
            break;
        case SIGHUP:
            signalName = "SIGHUP";
            break;
        default:
            signalName = "Signal " + std::to_string(signal);
            break;
    }

    std::stringstream ss;
    ss << " | ROTATION   | Signal received:" << signalName << "(" << signal << ")" << " | Do rotation for signal";
    logInsideLogger(ss.str());
    std::cerr << ss.str() << std::endl;

    rotateSingle(signal);
    std::quick_exit(0);
}

void LogDepot::rotateFileInFs(const std::string& fileName)
{
    try
    {
        std::string archieveName = calculateArchiveFileName();

        // std::filesystem::make_directory(archieveName);
        if (_logConfig._enableRotation)
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

    _fs << getDateWithMsecNow() << toWrite
        << std::flush;
}

std::string LogDepot::calculateArchiveFileName() const
{
    std::stringstream ss;
    ss << _archivePath << "/" << getDateWithMsecNow() + "_" + _fileName;
    return ss.str();
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
