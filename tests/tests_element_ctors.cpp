//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"
#include <utility>

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


TEST (ElementCtorsNotOk, CtorsFailRequirements) {

#if defined (WRONG_ELEMENT_CTOR_COMPILE_FAILURE)
	time_series::Element<int, int> fail_duration_requirements;
#endif
}



//todo: add copy and move ctors f