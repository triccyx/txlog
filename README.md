# 1. Abstract

**txlog** library provides straightforward logging capabilities for C++ applications. It focuses on minimizing complexity while offering essential features such as configurable log levels and streamlined message formatting. Designed for ease of integration, it allows developers to quickly enable logging without introducing extensive dependencies or performance overhead. The architecture is extensible, permitting further customization and enhancements to suit diverse project requirements.

# 2. Usage

1. Include the header file:

```cpp
#include "log.h"
```

2. Use the method `LOG` to log messages:

```cpp
void myFunction()
{
    LOG(info, "mymodule");
}
```

3. Add to cmake the library:

```cmake
include(FetchContent)
FetchContent_Declare(
  txlog
  GIT_REPOSITORY git@github.com:triccyx/txlog.git
  GIT_TAG main
)
FetchContent_MakeAvailable(txlog)
```

4. Use the library in your cmake Project:

```cmake
target_link_libraries(mytarget PRIVATE txlog)
```

## 2.1. Severity

The severity levels are as follows:

-   critical
-   exception
-   error
-   warning
-   info
-   trace
-   debug
-   none

## 2.2. Module

1. Define the module as a string:
   The module is the name of the module that is logging the message.

```cpp
LOG(info,"mymodule");
```

2. Define the module as an enum:
   You can use an enum to define the module:

```cpp
enum Module
{
    MYMODULE
};

LOG(info, MYMODULE);

enum class Module
{
    MYMODULE
};

LOG(info, Module::MYMODULE);
```

This solution is only available if you use at least **c++17 compiler**

# 3. Prerequisites

-   Linux system
-   c++11 compiler
-   cmake

# 4. Configuration file

The `./configuration/logconfig.json` file is used to configure the logging system.

| Configuration Option    | Description                                  | Default Value                                | Development Status |
| ----------------------- | -------------------------------------------- | -------------------------------------------- | ------------------ |
| `enabled`               | Enables or disables logging                  | `true`                                       | working            |
| `enableSignalManager`   | Enables signal handling for log rotation     | `false`                                      | working            |
| `enableSizeRotation`    | Enables log rotation based on file size      | `false`                                      | working            |
| `enableTimeRotation`    | Enables log rotation based on time           | `false`                                      | working            |
| `enableCloseRotation`   | Enables log rotation when application closes | `false`                                      | working            |
| `enableCrashStackPrint` | Enables crash stack printing                 | `false`                                      | on developing      |
| `enableRotation`        | Enables all rotation mechanisms              | `false`                                      | working            |
| `enableTime`            | Enables timestamp in log messages            | `true`                                       | working            |
| `enableWriteToFile`     | Enables writing logs to file                 | `true`                                       | working            |
| `maxFileSize`           | Maximum file size before rotation (in bytes) | `2097152`                                    | working            |
| `rotationTime`          | Time for daily rotation                      | `{"hour": 23, "minutes": 59, "seconds": 59}` | working            |
| `signalFile`            | Name of the signal file                      | `"signal.log"`                               | working            |
| `fileName`              | Name of the log file                         | `"mylog.log"`                                | working            |
| `basePath`              | Base directory for log files                 | `"/usr/local/logs"`                          | working            |
| `archivePath`           | Directory for archived log files             | `"/usr/local/archive"`                       | working            |

If the configuration file **is not present**, the default values are used.

# 5. Advanced usage

## 5.1. File rotation for size

To enable file rotation based on size, set the `enableSizeRotation` option to `true` and specify the `maxFileSize` in the configuration file. The log file will rotate when it reaches the specified size.

## 5.2. File rotation for time

To enable file rotation based on time, set the `enableTimeRotation` option to `true` and specify the `rotationTime` in the configuration file. The log file will rotate at the specified time each day.

## 5.3. File rotation for close

To enable file rotation when the application closes, set the `enableCloseRotation` option to `true`. The log file will rotate when the application closes.

## 5.4. File rotation for signal

To enable file rotation when a signal is received, set the `enableSignalRotation` option to `true`. The log file will rotate when the signal is received.

## 5.5. Stack trace print

Not available for now

# 6. Log your type

To log your type, you need to overload the `operator<<` for your type.

```cpp
LOG(info, "mytype") << mytype;
```

# 7. Coverage and Unittest

To execute the unittest, run the following command:

```bash
cmake -DUNITTEST=ON ..
make RunUnitTest
```

To create the coverage report, run the following command:

```bash
cmake -DCOVERAGE=ON -DUNITTEST=ON ..
make EvaluateCoverage
```

# 8. License

The software is licensed under the MIT License:

Copyright © 2024-2025 Luca Tricerri

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
