//
// Created by Andrey Solovyev on 23/02/2023.
//

#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "tslib.h"
#include "finlib.h"
#include "culib.h"
#include "typed_tests_list.h"


using namespace culib::time;
using namespace culib::requirements;

using namespace time_series;
using namespace time_series::financial;
using namespace time_series::requirements;

using duration_t = culib::time::Microseconds;

template<typename T>
class ElementConversionOperators : public testing::Test {};
TYPED_TEST_SUITE(ElementConversionOperators, test_value);


TYPED_TEST(ElementConversionOperators, CheckTraits) {
	auto elem = time_series::Element<duration_t, TypeParam>();
	using Elem = decltype(elem);
	constexpr bool k {  is_duration_v<typename Elem::key_type::duration_type> };
	constexpr bool m {  std::is_same_v<typename Elem::mapped_type, TypeParam> };
	constexpr bool v {
		std::is_same_v<
		        typename std::pair<typename Elem::key_type, typename Elem::mapped_type>,
				typename Elem::value_type
				>};

	ASSERT_EQ(true, k);
	ASSERT_EQ(true, m);
	ASSERT_EQ(true, v);
}

TYPED_TEST(ElementConversionOperators, ToKeyType) {
	auto elem = time_series::Element<duration_t, TypeParam>();
	using Elem = decltype(elem);
	auto key = typename Elem::key_type(elem);

	ASSERT_EQ(key, elem.timestamp);
}
TYPED_TEST(ElementConversionOperators, ToMappedType) {
	auto elem = time_series::Element<duration_t, TypeParam>();
	using Elem = decltype(elem);
	auto value = typename Elem::mapped_type(elem);

	ASSERT_EQ(value, elem.value);
}
TYPED_TEST(ElementConversionOperators, ToValueType) {
	auto elem = time_series::Element<duration_t, TypeParam>();
	using Elem = decltype(elem);
	auto p1 = typename Elem::value_type(elem);
	auto p2 = std::pair{elem.timestamp, elem.value};

	ASSERT_EQ(p1, p2);
}
TYPED_TEST(ElementConversionOperators, OperatorBrackets) {
//	auto elem = time_series::Element<duration_t, TypeParam>();
//	auto const& p1 = elem();
//	auto p2 = std::pair{elem.timestamp, elem.value};

//	ASSERT_EQ(p1, p2);

	//the problem here is on Element side, there is an lvalue created out of temporary object
	//Element can be reformated to std::pair

	ASSERT_TRUE(false);
}
TYPED_TEST(ElementConversionOperators, First) {
	auto elem = time_series::Element<duration_t, TypeParam>();
	auto key = elem.first();

	ASSERT_EQ(key, elem.timestamp);
}
TYPED_TEST(ElementConversionOperators, Second) {
	auto elem = time_series::Element<duration_t, TypeParam>();
	auto value = elem.second();

	ASSERT_EQ(value, elem.value);
}
