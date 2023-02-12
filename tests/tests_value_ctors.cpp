//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

TYPED_TEST(CtorsSingletons, DefaultCtor) {
	ASSERT_NO_THROW(TypeParam ());
}
TYPED_TEST(CtorsSingletons, CtorFromInt) {
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
TYPED_TEST(CtorsSingletons, CtorFromFloat) {
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
//	ASSERT_EQ(s2, TypeParam ("42"s));
}
TYPED_TEST(CtorsSingletons, CtorFromString) {
	using namespace std::string_literals;
	ASSERT_NO_THROW(TypeParam ("42"s));
	ASSERT_NO_THROW(TypeParam ("-1"s));
	ASSERT_NO_THROW(TypeParam ("0"s));
	ASSERT_NO_THROW(TypeParam ("1"s));
	ASSERT_NO_THROW(TypeParam ("+1"s));
	ASSERT_NO_THROW(TypeParam ("42.0"s));
	ASSERT_NO_THROW(TypeParam ("-1.42"s));
	ASSERT_NO_THROW(TypeParam ("0.0"s));
	ASSERT_NO_THROW(TypeParam ("0.0005"s));
	ASSERT_NO_THROW(TypeParam ("1.42"s));
	ASSERT_NO_THROW(TypeParam ("+1.42"s));
}
TYPED_TEST(CtorsSingletons, CopyCtor) {
	TypeParam v (42.5);
	auto copy = v;
	ASSERT_EQ(copy.value, v.value);
	if (std::is_integral_v<typename TypeParam::value_type>) {
		ASSERT_EQ(copy.value, 42);
	} else {
		ASSERT_EQ(copy.value, 42.5);
	}
}
TYPED_TEST(CtorsSingletons, MoveCtor) {
	TypeParam v (42.5);
	auto move__ = std::move(v);
	if (std::is_integral_v<typename TypeParam::value_type>) {
		ASSERT_EQ(move__.value, 42);
	} else {
		ASSERT_EQ(move__.value, 42.5);
	}
}
TYPED_TEST(CtorsSingletons, CompileError) {
	struct S {};
	[[maybe_unused]] S s;
	// base::Value<base::Seconds> d (s); //todo: make a compile time test

}


/**
 * @details
 * Tests for OHLCV and BidAsk
 *
 * */

TYPED_TEST(CtorsMultiField, ValueDefault) {
	ASSERT_NO_THROW(TypeParam ());
}
TYPED_TEST(CtorsMultiField, CtorInitializerList) {
	using v_type = typename TypeParam::value_type;
	base::Value<v_type> one (42.5);
	base::Value<v_type> two (42.5);
	ASSERT_ANY_THROW([[maybe_unused]] TypeParam d ({one, two}));
	base::Value<v_type> three (42.5);
	base::Value<v_type> four (42.5);
	base::Value<v_type> five (42.5);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d ({one, two, three, four, five}));
}

TYPED_TEST(CtorsMultiField, ValueCopyCtor) {
	TypeParam d ({12.2, 12.3, 12.4, 12.5, 12.7});
	auto copy = d;
	ASSERT_EQ(copy, d);
}
TYPED_TEST(CtorsMultiField, ValueMoveCtor) {
	TypeParam d ({12.2, 12.3, 12.4, 12.5, 12.7});
	TypeParam copy ({12.2, 12.3, 12.4, 12.5, 12.7});
	auto move__ = std::move(d);
	ASSERT_EQ(move__, copy);
}
TYPED_TEST(CtorsMultiField, CompileError) {
	struct S {};
	[[maybe_unused]] S s;
	// base::Value<base::Seconds> d (s); //todo: make a compile time test
}


//todo: add copy and move ctors f