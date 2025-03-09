/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 03-2025
 */

#include "gtest/gtest.h"
#include "log.h"

using namespace testing;

TEST(Log, simple)
{
    testing::internal::CaptureStderr();
    LOG(debug, "test");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, " | (Debug****) | (test) | \n");
}

TEST(Log, simple_enum)
{
    enum Module
    {
        TEST
    };
    testing::internal::CaptureStderr();
    LOG(debug, TEST);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, " | (Debug****) | (TEST) | \n");
}

TEST(Log, simple_enum_class)
{
    testing::internal::CaptureStderr();

    enum class Module2
    {
        TEST2
    };

    LOG(debug, Module2::TEST2);
    std::string output2 = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output2, " | (Debug****) | (TEST2) | \n");
}

TEST(Log, write_to_file_yes)
{
    std::filesystem::remove("./log/mylog.log");
    testing::internal::CaptureStderr();
    Log::Instance().logConfig._enableWriteToFile = true;
    Log::Instance().logDepot.createFileIfNotExist();


    LOG(debug, "test");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, " | (Debug****) | (test) | \n");

    std::ifstream file("./log/mylog.log");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_EQ(content, " | (Debug****) | (test) | \n");
}

TEST(Log, write_to_file_no)
{
    std::filesystem::remove("./log/mylog.log");
    testing::internal::CaptureStderr();
    Log::Instance().logConfig._enableWriteToFile = false;
    Log::Instance().logDepot.createFileIfNotExist();
 
    LOG(debug, "test");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, " | (Debug****) | (test) | \n");

    std::ifstream file("./log/mylog.log");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "");
}

TEST(Log,no_logging)
{
    std::filesystem::remove("./log/mylog.log");
    testing::internal::CaptureStderr();
    Log::Instance().logConfig._enableWriteToFile = false;
    Log::Instance().logConfig._enabled = false;
    Log::Instance().logDepot.createFileIfNotExist();

    LOG(debug, "test");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "");

    std::ifstream file("./log/mylog.log");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "");
}

TEST(Log, get_version)
{
    std::string version = Log::Instance().getVersion();
    EXPECT_TRUE(version.size()>3); // Assuming the version is "1.0.0", replace with the actual version if different
}


