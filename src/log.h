/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#pragma once

#include <locale>
#include <memory>
#include <string>
#include <type_traits>

#include "line.h"
#include "logConfig.h"
#include "logDepot.h"
#include "logconst.h"

class Log
{
   public:
    void init();

    LogConfig logConfig;
    LogDepot logDepot{logConfig};
    std::string getVersion() const;

    static Log& Instance()
    {
        static Log instance;
        return instance;
    }

   private:
    Log() = default;
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;
};

#if __cplusplus >= 201703L
#include "magic_enum.hpp"
template <typename SEVERITY, typename MODULE>
constexpr auto LOG(SEVERITY severity, MODULE module)
{
    auto moduleString = std::string(magic_enum::enum_name(module));
    return Line(Log::Instance().logDepot, Log::Instance().logConfig, severity, moduleString);
}
#endif

template <typename SEVERITY>
constexpr auto LOG(SEVERITY severity, const std::string& module)
{
    return Line(Log::Instance().logDepot, Log::Instance().logConfig, severity, module);
}

template <typename SEVERITY>
constexpr auto LOG(SEVERITY severity, const char* module)
{
    return Line(Log::Instance().logDepot, Log::Instance().logConfig, severity, module);
}