/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#include "log.h"

#include <sys/time.h>

#include <atomic>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <mutex>

#include "config.h"

void Log::init()
{
    if (logDepot._enableSignalManager)
    {
        signal(SIGTERM, [](int signum)
               {
                   Log::Instance().logDepot.signalRecived(signum);
               });

        signal(SIGINT, [](int signum)
               {
                   Log::Instance().logDepot.signalRecived(signum);
               });

        signal(SIGKILL, [](int signum)
               {
                   Log::Instance().logDepot.signalRecived(signum);
               });

        signal(SIGABRT, [](int signum)
               {
                   Log::Instance().logDepot.signalRecived(signum);
               });
        signal(SIGSEGV, [](int signum)
               {
                   Log::Instance().logDepot.signalRecived(signum);
               });
    }
}

std::string Log::getVersion() const
{
    return PROJECT_VER;
}
