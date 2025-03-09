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
        /*
        signal(SIGTERM, [&](int signum)
               {
                   logDepot.signalRecived(signum);
               });

        signal(SIGINT, [&logDepot](int signum)
               {
                   logDepot.signalRecived(signum);
               });

        signal(SIGKILL, [&logDepot](int signum)
               {
                   logDepot.signalRecived(signum);
               });

        signal(SIGABRT, [&logDepot](int signum)
               {
                   logDepot.signalRecived(signum);
               });
        signal(SIGSEGV, [&logDepot](int signum)
               {
                   logDepot.signalRecived(signum);
               });
               */
    }
}

std::string Log::getVersion() const
{
    return PROJECT_VER;
}
