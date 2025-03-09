
/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 05-2024
 */

#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>

#include "logConfig.h"

class LogDepot
{
   public:
    bool log(const std::string& toLog, const std::string& toLogToFile);
    void signalRecived(int signal);
    const int _enableSignalManager{true};
    LogDepot(LogConfig& config);
    ~LogDepot();
    void createFileIfNotExist();

   protected:
    void createFolderIfNotExist(const std::string& path);

    bool rotateSingle(int signum);
    bool rotateIfSizeExceeded();  // return true if rotation is done
    void rotateIfTimeIsReached();
    virtual bool canRotate(const std::string& fileName) const;
    void rotateFileInFs(const std::string& fileName);
    bool _rotationCheckTimeIsActive{true};

    void logInsideLogger(const std::string& toWrite);

    int secondsToMidnight() const;
    bool checkIfItIsTimeToRotate();

    std::string myBacktrace(int skip = 1);

    bool _enableSizeRotation{true};
    bool _enableTimeRotation{true};
    bool _enableCloseRotation{true};
    bool _enablePrintCrashStack{true};
    bool _disableRotation{true};

    const int _signalTimeOut{1000};
    const int _signalCloseApplication{1001};
    const int _signalSizeExceed{1002};

    int _maxFileSize = 2 * 1024 * 1024;
    std::tuple<int /*hour*/, int /*minutes*/, int /*seconds*/> _rotationTime{0, 0, 0};  // here midnight
    std::ofstream _fs;
    mutable std::mutex _mutex;
    std::string _fileName;
    std::string _basePath;
    std::string _archivePath;

    std::string _signalFile{"/usr/local/logs/signal.log"};
    std::ofstream _ofs;

    std::unique_ptr<std::thread> _rotateInTimeThread;

    LogConfig& _logConfig;

   private:
    std::string calculateArchieveFileName();
    bool _logDisabled{false};
};
