#include "gtest/gtest.h"

#include "tuple/FlatTuple.h"

struct PODImitator
{
    float val1{ 10.f };
    int val2{ 11 };
};

TEST(TupleTest, test1)
{
    constexpr auto tuple = MakeFlatTuple(1.f, std::string_view("string"), PODImitator{});
    ASSERT_EQ(tuple.Get<0>(), tuple.Get<float>());
}

TEST(TupleTest, test2)
{

}

TEST(TupleTest, test3)
{

}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
