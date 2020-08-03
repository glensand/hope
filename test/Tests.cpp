#include "gtest/gtest.h"

#include "commontests/fieldescounttest.h"
#include "core/fast_pimpl.h"

struct test_struct{
    double a{ }, b{ }, c{ };
    int d{ };
};

TEST(TupleTest, ValueChanging)
{
    hope::fast_pimpl<test_struct, sizeof(test_struct), alignof(test_struct)> p;
    auto& n = *p;
    n.d = 1;
    ASSERT_EQ(p->d, n.d);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
