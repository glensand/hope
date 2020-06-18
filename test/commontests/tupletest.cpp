#include "gtest/gtest.h"

#include "tuple/FlatTuple.h"
#include "tuple/TupleFromStruct.h"

struct PODImitator
{
    constexpr static int DefaultInt{ 11 };
    float val1{ 10.f };
    int val2{ DefaultInt };
};

struct ComplicatedStruct
{
    std::string_view Name;
    int Index;
};

TEST(TupleTest, ConstexprInitialization)
{
    constexpr auto tuple = MakeFlatTuple(1.f, std::string_view("string"), PODImitator{});

    static_assert(tuple.Get<0>() == tuple.Get<float>());
    static_assert(tuple.Get<1>() == tuple.Get<std::string_view>());
    static_assert(tuple.Get<2>().val2 == PODImitator::DefaultInt);
}

TEST(TupleTest, ValueChanging)
{
    auto tuple = MakeFlatTuple(PODImitator{}, PODImitator{ 0.1, 1 });
    auto&& secondInt = tuple.Get<1>().val2;
    ASSERT_EQ(secondInt, 1);
    secondInt = 12;
    ASSERT_EQ(secondInt, 12);
}

struct TestStruct3
{
    double i;
    float g;
    int k;
};

TEST(TupleTest, TupleFromStruct)
{

}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
