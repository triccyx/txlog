/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 05-2024
 */

#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>



// color
enum LogColor_e
{
    NORMAL,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

inline static std::unordered_map<LogColor_e, std::string> colorToString{
    {NORMAL, "\x1B[0m"},
    {RED, "\x1B[31m"},
    {GREEN, "\x1B[32m"},
    {YELLOW, "\x1B[33m"},
    {BLUE, "\x1B[34m"},
    {MAGENTA, "\x1B[35m"},
    {CYAN, "\x1B[36m"},
    {WHITE, "\x1B[37m"}};

// severity
enum Severity : int
{
    critical = 7,
    exception = 6,
    error = 5,  // todo luca publish error
    warning = 4,
    info = 3,
    trace = 2,
    debug = 1,
    none = 0
};
// modules
enum Modules : int
{
    core,
    app,
    communication,
    dynamicloading,
    test,
    util,
    inout,
    i2c,
    config,
    unknown
};

inline std::string moduleToString(Modules module)
{
    switch (module)
    {
        case Modules::core:
            return "CORE";
        case Modules::app:
            return "APPL";
        case Modules::communication:
            return "COMM";
        case Modules::dynamicloading:
            return "DYNA";
        case Modules::test:
            return "TEST";
        case Modules::util:
            return "UTIL";
        case Modules::inout:
            return "INOU";
        case Modules::i2c:
            return "I2C_";
        case Modules::config:
            return "CONF";
        default:
            return "UNKN";
    }
}

inline std::string severityToString(Severity error)
{
    switch (error)
    {
        case Severity::critical:
            return "\033[31mCritical*\033[0m";
        case Severity::exception:
            return "\033[31mException\033[0m";
        case Severity::error:
            return "\033[31mError****\033[0m";
        case Severity::warning:
            return "\033[93mWarning**\033[0m";
        case Severity::info:
            return "\033[34mInfo*****\033[0m";
        case Severity::trace:
            return "Trace****";
        case Severity::debug:
            return "Debug****";
        case Severity::none:
        default:
            return "None*****";
    }
}

inline std::string severityToStringNoColor(Severity error)
{
    switch (error)
    {
        case Severity::critical:
            return "Critical*";
        case Severity::exception:
            return "Exception";
        case Severity::error:
            return "Error****";
        case Severity::warning:
            return "Warning**";
        case Severity::info:
            return "Info*****";
        case Severity::trace:
            return "Trace****";
        case Severity::debug:
            return "Debug****";
        case Severity::none:
        default:
            return "None*****";
    }
}

inline std::string getDateWithMsecNow()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream ss;
    ss << std::put_time(&tm, "%d-%m-%Y_%H:%M:%S.") << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

inline std::string getDateWithoutMsecNow()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}