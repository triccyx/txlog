/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#pragma once

#include "logconst.h"

class LogDepot;
class LogConfig;
class Line
{
   public:
    Line(LogDepot &logDepot, LogConfig &logConfig, Severity severity, const std::string &module);
    Line(const Line &) = delete;
    Line(const Line &&);
    ~Line();

    template <typename T>
    Line &operator<<(const T &data)
    {
        _currentLogLine << data;
        _currentLogLineToFile << data;
        return *this;
    };

    using endl_type = std::ostream &(std::ostream &);
    Line &operator<<(endl_type endl)
    {
        // nothing to do
        return *this;
    }

   private:
    std::stringstream _currentLogLine;
    std::stringstream _currentLogLineToFile;
    LogDepot &_logDepot;
    LogConfig &_logConfig;
    Severity _severity;
    std::string _module;
};