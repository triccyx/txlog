#include "log.h"

#include <sys/time.h>

#include <atomic>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <mutex>

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
