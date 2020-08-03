#include "gtest/gtest.h"

#include "tuple/generated.h"
#include "tuple/flat_tuple.h"

struct POD_imitator {
    constexpr static int DefaultInt{ 11 };
    float val1{ 10.f };
    int val2{ DefaultInt };
};

struct complicated_struct {
    std::string_view name;
    int index;
};

TEST(TupleTest, ConstexprInitialization)
{
    constexpr auto tuple = hope::make_flat_tuple(1.f, std::string_view("string"), POD_imitator{});

    static_assert(tuple.get<0>() == tuple.get<float>());
    static_assert(tuple.get<1>() == tuple.get<std::string_view>());
    static_assert(tuple.get<2>().val2 == POD_imitator::DefaultInt);
}

TEST(TupleTest, ValueChanging)
{
    auto tuple = hope::make_flat_tuple(POD_imitator{}, POD_imitator{ 0.1, 1 });
    auto&& secondInt = tuple.get<1>().val2;
    ASSERT_EQ(secondInt, 1);
    secondInt = 12;
    ASSERT_EQ(secondInt, 12);
}

struct test_struct_3 {
    double i;
    float g;
    int k;
    bool b;
};

TEST(TupleTest, TupleFromStruct)
{
    constexpr test_struct_3 ts3{ 0.1, 0.1f, 11, true};
    constexpr auto ts3Tuple = hope::tuple_from_struct(ts3);

    static_assert(ts3Tuple.get<2>() == ts3.k);
    static_assert(ts3Tuple.get<3>() == ts3.b);

    ASSERT_TRUE(std::abs(ts3Tuple.Get<0>() - ts3.i) < std::numeric_limits<double>::epsilon());
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
