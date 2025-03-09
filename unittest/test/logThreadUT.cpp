/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#include "gtest/gtest.h"
#include "log.h"

using namespace testing;

int countLines(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return -1;  // Return -1 to indicate error
    }

    int lineCount = 0;
    std::string line;
    while (std::getline(file, line))
    {
        ++lineCount;
    }

    file.close();
    return lineCount;
}

int countStringInFile(const std::string& filename, const std::string& str)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return -1;  // Return -1 to indicate error
    }

    int count = 0;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(str) != std::string::npos)
        {
            ++count;
        }
    }
    file.close();
    return count;
}

int countFilesInFolder(const std::string& folder)
{
    return std::distance(std::filesystem::directory_iterator(folder), std::filesystem::directory_iterator{});
}

TEST(Log, four_thread_simple)
{
    std::filesystem::remove("./log/mylog.log");
    std::filesystem::remove_all("./archive/");
    Log::Instance().logConfig._enableWriteToFile = true;
    Log::Instance().logConfig._enabled = true;
    Log::Instance().logDepot.createFileIfNotExist();

    std::thread t1([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           LOG(debug, "thread1") << i << "test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1";
                           std::this_thread::sleep_for(std::chrono::milliseconds(1));
                       }
                   });
    std::thread t2([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           LOG(debug, "thread2") << i << "test2-test2-test2-test2-test2" << "-test2-test2-test2-test2-test2" << "-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2";
                           std::this_thread::sleep_for(std::chrono::milliseconds(1));
                       }
                   });
    std::thread t3([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           LOG(debug, "thread3") << i << "test3-test3-test3-test3-test3" << "-test3-test3-test3-test3-test3" << "-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3";
                           std::this_thread::sleep_for(std::chrono::milliseconds(1));
                       }
                   });
    std::thread t4([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           LOG(debug, "thread4") << i << "test4-test4-test4-test4-test4" << "-test4-test4-test4-test4-test4" << "-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4";
                           std::this_thread::sleep_for(std::chrono::milliseconds(1));
                       }
                   });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    EXPECT_EQ(40000, countLines("./log/mylog.log"));
    EXPECT_EQ(10000, countStringInFile("./log/mylog.log", "test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1"));
    EXPECT_EQ(10000, countStringInFile("./log/mylog.log", "test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2-test2"));
    EXPECT_EQ(10000, countStringInFile("./log/mylog.log", "test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3-test3"));
    EXPECT_EQ(10000, countStringInFile("./log/mylog.log", "test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4-test4"));
}

TEST(Log, archive_simple_rotation_for_size)
{
    std::filesystem::remove("./log/mylog.log");
    std::filesystem::remove_all("./archive/");
    Log::Instance().logConfig._enableWriteToFile = true;
    Log::Instance().logConfig._enabled = true;
    Log::Instance().logConfig._enableRotation = true;
    Log::Instance().logConfig._enableSizeRotation = true;
    Log::Instance().logConfig._maxFileSize = 1000;
    Log::Instance().logDepot.createFileIfNotExist();

    std::thread t1([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           LOG(debug, "thread1") << i << "test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1";
                           std::this_thread::sleep_for(std::chrono::milliseconds(1));
                       }
                   });
    t1.join();

    EXPECT_EQ(2, countFilesInFolder("./log"));  // also signal.log is present
    EXPECT_GT(countFilesInFolder("./archive"), 10);
}

TEST(Log, archive_simple_rotation_for_time)
{
    std::filesystem::remove("./log/mylog.log");
    std::filesystem::remove_all("./archive/");
    Log::Instance().logConfig._enableWriteToFile = true;
    Log::Instance().logConfig._enabled = true;
    Log::Instance().logConfig._enableRotation = true;
    Log::Instance().logConfig._enableSizeRotation = false;
    Log::Instance().logConfig._enableTimeRotation = true;

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* timeinfo = std::localtime(&currentTime);
    Log::Instance().logConfig._rotationTime = std::make_tuple(timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec + 3);

    Log::Instance().logDepot.createFileIfNotExist();

    std::thread t1([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           LOG(debug, "thread1") << i << "test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1";
                           std::this_thread::sleep_for(std::chrono::milliseconds(1));
                       }
                   });
    t1.join();

    EXPECT_EQ(2, countFilesInFolder("./log"));  // also signal.log is present
    EXPECT_EQ(1, countFilesInFolder("./archive"));
}

TEST(Log, archive_simple_rotation_for_close_application)
{
    std::filesystem::remove("./log/mylog.log");
    std::filesystem::remove_all("./archive/");

    {
        LogConfig logConfig;
        logConfig._enableWriteToFile = true;
        logConfig._enabled = true;
        logConfig._enableRotation = true;
        logConfig._enableSizeRotation = false;
        logConfig._enableTimeRotation = false;
        logConfig._enableCloseRotation = true;

        LogDepot logDepot(logConfig);
        logDepot.createFileIfNotExist();

        std::thread t1([&]()
                       {
                           for (int i = 0; i < 10000; i++)
                           {
                               Line(logDepot, logConfig, info, "xx") << i << "test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1" << "-test1-test1-test1-test1-test1-test1-test1-test1-test1-test1";
                               std::this_thread::sleep_for(std::chrono::milliseconds(1));
                           }
                       });
        t1.join();
    }
    EXPECT_EQ(2, countFilesInFolder("./log"));  // also signal.log is present
    EXPECT_EQ(1, countFilesInFolder("./archive"));
}