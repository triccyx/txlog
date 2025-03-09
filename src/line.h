/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 05-2024
 */

#pragma once

#include "logconst.h"

class LogDepot;

class Line
{
   public:
    Line(LogDepot &logDepot, Severity severity, Modules module);
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
    Severity _severity;
    Modules _module;
};