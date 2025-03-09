# 1. Unit tests

A set of unit tests is present in the library folder. The unit tests use the gmock and gtest frameworks from Google. It
is possible to use and compile the unit tests on the X86 and ARM platform.

## 1.1. X86 compile

It is possible to compile on X86 architecture.
This compilation is done for testing purposes. The compilation flag is `X86`.

## 1.2. Prerequisite

Install the following libraries.

```bash
sudo apt install build-essential cmake libboost-all-dev libfmt-dev libssl-dev cmake-curses-gui git libiio-dev libsystemd-dev libarchive-dev libfmt-dev libcurlpp-dev geninfo genhtml gcovr firefox
```

## 1.3. Compilation and Run unittest

### 1.3.1. To compile and execute the tests on X86

```bash
cd <repository folder>
mkdir build
cd build
cmake -DX86="ON" -DUNITTEST="ON" -DCMAKE_BUILD_TYPE="Debug" ..
make RunTest
```

### 1.3.2. To compile and execute the tests on ARM

copy bin in /usr/local/bin and library in /usr/lib
copy runInScreen.sh in /usr/local/bin

```bash
<build with visualstudio code>
cmake -DX86="ON" -DUNITTEST="ON" -DCMAKE_BUILD_TYPE="Debug" ..
./runInScreen.sh
```

The result is something like this:

```bash
[       OK ] CallBack.simple_with_mock (6006 ms)
[----------] 3 tests from CallBack (8011 ms total)

[----------] Global test environment tear-down
[==========] 70 tests from 9 test suites ran. (8046 ms total)
[  PASSED  ] 70 tests.
```

## 1.4. Folder structure

```
unittest---
           |- mock
           |- test
           |- script
```

## 1.5. Unit tests summary

| file name | description | note |
| --------- | ----------- | ---- | --- | --- | --- |
|           |             |      |     |     |     |

## 1.6. Coverage

Create coverage map:

```bash
make CreateCoverage
```

This opens firefox with coverage info.
