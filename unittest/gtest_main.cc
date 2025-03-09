/**
 * @author Luca Tricerri <triccyx@gmail.com>
 * @date 08-2024
 */

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    //::testing::GTEST_FLAG(filter) = "Log.archive_simple_rotation_for_close_application";

    return RUN_ALL_TESTS();
}