#include "gtest/gtest.h"

#include "commontests/customtraitstest.h"
#include "commontests/fieldescounttest.h"

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
