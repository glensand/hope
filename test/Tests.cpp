#include "gtest/gtest.h"

#include "commontests/customtraitstest.h"

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
