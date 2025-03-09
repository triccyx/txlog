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

   public:
    static Log &Instance()
    {
        static Log instance;
        return instance;
    }

   private:
    Log() = default;
    Log *operator=(Log const &) = delete;
    Log(Log const &) = delete;
};

template <typename SEVERITY, typename MODULE>
constexpr auto LOG(SEVERITY severity, MODULE module)
{
    return Line(Log::Instance().logDepot, severity, module);
}