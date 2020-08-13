#include "gtest/gtest.h"

#include "typelist/type_list.h"
#include "sm_objects.h"
#include "memory/small_object_allocator.h"
#include <vector>
#include <algorithm>
#include <random>

namespace hope::memory {

    class reset_sm_allocator {
		public:
        static void apply() {
			small_object_allocator::instance().clear();
		}
	};

}
namespace hope::memory::testing{

	template <typename T>
	using RegisteredTypes = type_list<
		sm_object_0<T>, sm_object_1<T>, sm_object_2<T>, sm_object_3<T>, sm_object_4<T>, sm_object_5<T>
    >;

	typedef ::testing::Types
		<
		bytes_1, bytes_2, bytes_3, bytes_4, bytes_5, bytes_6, bytes_7, bytes_8, bytes_9, bytes_10, bytes_11
		>
		TestTypes;

    using sm_vector_t = std::vector<small_object*>;

	template <typename TArgumentClass>
	class sm_allocator_test : public ::testing::Test {
	protected:
		void SetUp() override {

		}

		void TearDown() override {
			reset_sm_allocator::apply();
		}

		sm_vector_t		m_sm_vector;
	};

	constexpr std::size_t ObjectsCount{ 1000 };

	template<std::size_t... Is, typename... Ts>
	void fill_vector(std::index_sequence<Is...> seq, type_list<Ts...> list, sm_vector_t& vec) {
	    for(std::size_t j{ 0 }; j < ObjectsCount; ++j) {
			small_object* br[] = { new typename decltype(get_nth_type<Is>(list))::Type... };
			vec.insert(std::end(vec), std::begin(br), std::end(br));
		}
	}

	TYPED_TEST_CASE(sm_allocator_test, TestTypes);

	TYPED_TEST(sm_allocator_test, sm_allocator_initialization)
	{
		constexpr RegisteredTypes<TypeParam> InnerTypeList{};
		fill_vector(std::make_index_sequence<size(InnerTypeList)>(), InnerTypeList, this->m_sm_vector);
		auto rng = std::default_random_engine{};
	    //std::shuffle(std::begin(this->m_sm_vector), std::end(this->m_sm_vector), rng);

		for (const auto object : this->m_sm_vector)
			delete object;
		this->m_sm_vector.clear();
	}

}
