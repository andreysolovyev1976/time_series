//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"
#include <type_traits>
#include <string>
#include <vector>

template<typename T>
class ValueCtorsSingletons : public testing::Test {};
TYPED_TEST_SUITE(ValueCtorsSingletons, value_single_field);

template<typename T>
class ValueCtorsMultiField : public testing::Test {};
TYPED_TEST_SUITE(ValueCtorsMultiField, value_multifield);


TYPED_TEST(ValueCtorsSingletons, DefaultCtor) {
	ASSERT_NO_THROW(TypeParam ());
}
TYPED_TEST(ValueCtorsSingletons, CtorFromInt) {
	ASSERT_NO_THROW(TypeParam (42));
	ASSERT_NO_THROW(TypeParam (42u));
	ASSERT_NO_THROW(TypeParam (42l));
	ASSERT_NO_THROW(TypeParam (42ll));
	ASSERT_NO_THROW(TypeParam (42ul));
	ASSERT_NO_THROW(TypeParam (42ull));
	ASSERT_NO_THROW(TypeParam (-1));
	ASSERT_NO_THROW(TypeParam (0));
	ASSERT_NO_THROW(TypeParam (1));
	ASSERT_NO_THROW(TypeParam (+1));
	ASSERT_NO_THROW(TypeParam (1+1));
	ASSERT_NO_THROW(TypeParam ( [](){return 42;}() ));
	ASSERT_NO_THROW(TypeParam (3'000'000'000));
}
TYPED_TEST(ValueCtorsSingletons, CtorFromFloat) {
	ASSERT_NO_THROW(TypeParam (42.0f));
	ASSERT_NO_THROW(TypeParam (-1.42f));
	ASSERT_NO_THROW(TypeParam (0.0f));
	ASSERT_NO_THROW(TypeParam (0.0005f));
	ASSERT_NO_THROW(TypeParam (1.42f));
	ASSERT_NO_THROW(TypeParam (42.0));
	ASSERT_NO_THROW(TypeParam (-1.42));
	ASSERT_NO_THROW(TypeParam (0.0));
	ASSERT_NO_THROW(TypeParam (0.0005));
	ASSERT_NO_THROW(TypeParam (1.42));
	ASSERT_NO_THROW(TypeParam (+1.42));
	ASSERT_NO_THROW(TypeParam (1.42+1.42));
	ASSERT_NO_THROW(TypeParam (1.42+1));
	ASSERT_NO_THROW(TypeParam ( [](){return 42.42;}() ));
}
TYPED_TEST(ValueCtorsSingletons, CtorFromString) {
	using namespace std::string_literals;
	ASSERT_NO_THROW(TypeParam ("42"s));
	ASSERT_NO_THROW(TypeParam ("-1"s));
	ASSERT_NO_THROW(TypeParam ("0"s));
	ASSERT_NO_THROW(TypeParam ("1"s));
	if constexpr(std::is_floating_point_v<typename TypeParam::value_type>) {
		ASSERT_NO_THROW(TypeParam("42.0"s));
		ASSERT_NO_THROW(TypeParam("-1.42"s));
		ASSERT_NO_THROW(TypeParam("0.0"s));
		ASSERT_NO_THROW(TypeParam("0.0005"s));
		ASSERT_NO_THROW(TypeParam("1.42"s));
	}
	else {
		ASSERT_ANY_THROW(TypeParam("42.0"s));
		ASSERT_ANY_THROW(TypeParam("-1.42"s));
		ASSERT_ANY_THROW(TypeParam("0.0"s));
		ASSERT_ANY_THROW(TypeParam("0.0005"s));
		ASSERT_ANY_THROW(TypeParam("1.42"s));
	}
	ASSERT_ANY_THROW(TypeParam ("+1"s));
	ASSERT_ANY_THROW(TypeParam ("+1.42"s));
}
TYPED_TEST(ValueCtorsSingletons, CopyCtor) {
	TypeParam v (42.5);
	auto copy = v;
	ASSERT_EQ(copy.value, v.value);
	if (std::is_integral_v<typename TypeParam::value_type>) {
		ASSERT_EQ(copy.value, 42);
	} else {
		ASSERT_EQ(copy.value, 42.5);
	}
}
TYPED_TEST(ValueCtorsSingletons, MoveCtor) {
	TypeParam v (42.5);
	auto move__ = std::move(v);
	if (std::is_integral_v<typename TypeParam::value_type>) {
		ASSERT_EQ(move__.value, 42);
	} else {
		ASSERT_EQ(move__.value, 42.5);
	}
}
TYPED_TEST(ValueCtorsSingletons, CompileError) {
	struct S {};
	[[maybe_unused]] S s;
#if defined (WRONG_VALUE_CTOR_COMPILE_FAILURE)
	TypeParam d (s);
#endif
	ASSERT_TRUE(true);
}


/**
 * @details
 * Tests for OHLCV and BidAsk
 *
 * */

TYPED_TEST(ValueCtorsMultiField, ValueDefault) {
	ASSERT_NO_THROW(TypeParam ());
}
TYPED_TEST(ValueCtorsMultiField, CtorInitializerList) {
	using v_type = typename TypeParam::value_type;
	time_series::Value<v_type> one (42.5);
	time_series::Value<v_type> two (42.5);
	ASSERT_ANY_THROW([[maybe_unused]] TypeParam d ({one, two}));
	time_series::Value<v_type> three (42.5);
	time_series::Value<v_type> four (42.5);
	time_series::Value<v_type> five (42.5);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d ({one, two, three, four, five}));
}

TYPED_TEST(ValueCtorsMultiField, ValueCopyCtor) {
	TypeParam d ({12.2, 12.3, 12.4, 12.5, 12.7});
	auto copy = d;
	ASSERT_EQ(copy, d);
}
TYPED_TEST(ValueCtorsMultiField, ValueMoveCtor) {
	TypeParam d ({12.2, 12.3, 12.4, 12.5, 12.7});
	TypeParam copy ({12.2, 12.3, 12.4, 12.5, 12.7});
	auto move__ = std::move(d);
	ASSERT_EQ(move__, copy);
}
TYPED_TEST(ValueCtorsMultiField, CompileError) {
	struct S {};
	[[maybe_unused]] S s;
#if defined (WRONG_VALUE_CTOR_COMPILE_FAILURE)
	TypeParam d (s);
#endif
	ASSERT_TRUE(true);

}

/**
 * @details
 * Other options for value
 * */

TEST (ValueCtorsOtherOption, UserDefined) {
	struct UserDefinedDummy {};
	ASSERT_NO_THROW ([[maybe_unused]] time_series::Value<UserDefinedDummy> d);
}
TEST (ValueCtorsOtherOption, String) {
	ASSERT_NO_THROW ([[maybe_unused]] time_series::Value<std::string> d);
}
TEST (ValueCtorsOtherOption, Vector) {
	ASSERT_NO_THROW ([[maybe_unused]] time_series::Value<std::vector<int>> d);
}