#include "gtest/gtest.h"

#include "commontests/customtraitstest.h"
#include "commontests/fieldescounttest.h"
#include "core/fastpimpl.h"

struct TestStruct
{
    double a{ }, b{ }, c{ };
    int d{ };
};

TEST(TupleTest, ValueChanging)
{
    FastPImpl<TestStruct, sizeof(TestStruct), alignof(TestStruct)> p;
    auto& n = *p;
    n.d = 1;
    ASSERT_EQ(p->d, n.d);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
