//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"
#include <utility>

using namespace std::string_literals;


template<typename T>
class ElementCtorsOk : public testing::Test {};
TYPED_TEST_SUITE(ElementCtorsOk, test_value);

using duration_t = typename culib::time::Microseconds;
using ts_t = typename culib::time::Timestamp<duration_t>;

template <typename V>
using elem_ms_t = typename time_series::Element<culib::time::Microseconds, V>;

TYPED_TEST(ElementCtorsOk, CtorFromElemTypes) {
	[[maybe_unused]] elem_ms_t<TypeParam> d;

	[[maybe_unused]] TypeParam value {};
	[[maybe_unused]] elem_ms_t<TypeParam> d_value(value);
	[[maybe_unused]] elem_ms_t<TypeParam> d_value_rvalue(TypeParam{});

	[[maybe_unused]] ts_t ts;
	[[maybe_unused]] elem_ms_t<TypeParam> d_ts_value(ts, value);
	[[maybe_unused]] elem_ms_t<TypeParam> d_ts_valuervalue(ts, TypeParam{});

	using value_pair_t = typename std::pair<ts_t, TypeParam>;
	[[maybe_unused]] value_pair_t value_pair;
	[[maybe_unused]] elem_ms_t<TypeParam> d_pair(value_pair);
	[[maybe_unused]] elem_ms_t<TypeParam> d_pair_rvalue(value_pair_t{});
}

TYPED_TEST(ElementCtorsOk, CtorFromString) {
	if constexpr (std::is_arithmetic_v<TypeParam>
	|| std::is_same_v<TypeParam, std::vector<int>>
	|| std::is_same_v<TypeParam, UserDefined>
	) {
		// do nothing, as there are no ctors from string for the types above
	}
	else {
		[[maybe_unused]] std::string initial_value{"10"};
		[[maybe_unused]] elem_ms_t<TypeParam> d{TypeParam(initial_value)};

		[[maybe_unused]] elem_ms_t<TypeParam> d1{TypeParam("10"s)};
		[[maybe_unused]] elem_ms_t<TypeParam> d2{TypeParam("10")};
	}
}



TEST (ElementCtorsNotOk, CtorsFailRequirements) {

#if defined (WRONG_ELEMENT_CTOR_COMPILE_FAILURE)
	time_series::Element<int, int> fail_duration_requirements;
#endif
}

TYPED_TEST(ElementCtorsOk, CopyCtor) {
	TypeParam value{};
	ts_t ts;
	elem_ms_t<TypeParam> d_value(ts, value);
	auto copy_value = d_value;

	//somehow gtest requires ostream operator for vector
	if constexpr (!std::is_same_v<TypeParam, std::vector<int>>) {
		ASSERT_EQ(copy_value, d_value);
	}
}
TYPED_TEST(ElementCtorsOk, MoveCtor) {
	TypeParam value{};
	ts_t ts;
	elem_ms_t<TypeParam> d_value(ts, value);
	auto move_value = std::move(d_value);

	//somehow gtest requires ostream operator for vector
	if constexpr (!std::is_same_v<TypeParam, std::vector<int>>) {
		ASSERT_EQ(move_value, d_value);
	}
}
