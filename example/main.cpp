#include <iostream>
#include "log.h"

int main(int argc, char* argv[])
{
    // Initialize the logging system
    Log::Instance().init();
    
    // Log messages with different severity levels
    LOG(info, "main") << "Application started";
    LOG(debug, "main") << "Command line arguments: " << argc;
    
    // Perform some example operations
    try {
        LOG(trace, "main") << "Performing sample operation";
        
        // Simulate some work
        int result = 42;
        LOG(info, "main") << "Operation completed with result: " << result;
    } catch (const std::exception& e) {
        LOG(error, "main") << "Exception caught: " << e.what();
        return 1;
    }

    LOG(info, "main") << "Application shutting down";
    return 0;
}
