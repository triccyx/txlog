/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#include "line.h"

#include "logConfig.h"
#include "logDepot.h"
#include "logconst.h"

Line::Line(const Line &&other)
    : _logDepot(other._logDepot),
      _logConfig(other._logConfig),
      _severity(other._severity),
      _module(other._module)
{
    _currentLogLine << other._currentLogLine.str();
    _currentLogLineToFile << other._currentLogLineToFile.str();
}

Line::Line(LogDepot &logDepot, LogConfig &logConfig, Severity severity, const std::string &module)
    : _logDepot(logDepot),
      _logConfig(logConfig),
      _severity(severity),
      _module(module)
{
    std::stringstream ss;
    if (_logConfig._enableTime)
    {
        ss << getDateWithMsecNow();
    }
    std::string dateheader = ss.str();

    ss.str("");
    ss << "(" << severityToString(_severity) << ")";
    std::string severityheader = ss.str();

    ss.str("");
    ss << "(" << severityToStringNoColor(_severity) << ")";
    std::string severityheaderNoColor = ss.str();

    ss.str("");
    ss << "(" << module << ")";
    std::string moduleheader = ss.str();

    _currentLogLine << dateheader << " | " << severityheader << " | " << moduleheader << " | ";
    _currentLogLineToFile << dateheader << " | " << severityheaderNoColor << " | " << moduleheader << " | ";
};

Line ::~Line()
{
    std::string toWrite = _currentLogLine.str();
    std::string toWriteToFile = _currentLogLineToFile.str();
    _logDepot.log(toWrite, toWriteToFile);
};