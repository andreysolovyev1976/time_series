//
// Created by Andrey Solovyev on 23/02/2023.
//

#include <gtest/gtest.h>
#include "typed_tests_list.h"

template<typename T>
class ValueConversionTraits : public testing::Test {};
TYPED_TEST_SUITE(ValueConversionTraits, test_values_single_and_multi_field);

static bool expect_int {false};

TYPED_TEST(ValueConversionTraits, Traits) {
	auto value = TypeParam();
	using Value = decltype(value);
	constexpr bool v_int { std::is_same_v<std::int64_t, typename Value::value_type> };
	constexpr bool v_double { std::is_same_v<double, typename Value::value_type> };

	// list of TypeParam - see test_values_single_and_multi_field in the typed_tests_list.h
	// goes int - double - int ... and so on
	if (expect_int) {
		ASSERT_TRUE(v_int);
		expect_int = false;
	} else {
		ASSERT_TRUE(v_double);
		expect_int = true;
	}
}

template<typename T>
class ValueConversionOperators : public testing::Test {};
TYPED_TEST_SUITE(ValueConversionOperators, value_single_field);


TYPED_TEST(ValueConversionOperators, ToValueType) {
	auto value = TypeParam();
	using Value = decltype(value);
	auto value_copy = typename Value::value_type(value);

	ASSERT_EQ(value, value_copy);
}
TYPED_TEST(ValueConversionOperators, OperatorBrackets) {
	auto value = TypeParam();
	auto value_copy = value();

	ASSERT_EQ(value, value_copy);
}
