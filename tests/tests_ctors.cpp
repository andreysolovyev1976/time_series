//
// Created by Andrey Solovyev on 24/01/2023.
//

#include <gtest/gtest.h>
#include "time_series/value.h"
#include "financial_data_structures/single_quote.h"
#include "financial_data_structures/ohlcv.h"
#include "financial_data_structures/bid_ask.h"
#include "time_series/element.h"

using test_types_ctors_singletons = testing::Types<
		base::Value<base::Seconds>,
		financial::SingleQuote<base::Seconds>
>;
template<typename T>
class CtorsSingletons : public testing::Test {};
TYPED_TEST_SUITE(CtorsSingletons, test_types_ctors_singletons);

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
}
TYPED_TEST(CtorsSingletons, MoveCtor) {
	TypeParam v (42.5);
	auto move__ = std::move(v);
	ASSERT_EQ(move__.value, 42.5);
}
TYPED_TEST(CtorsSingletons, CompileError) {
	struct S {};
	[[maybe_unused]] S s;
	// base::Value<base::Seconds> d (s); //todo: make a compile time test

}



using test_types_ctors_multifield = testing::Types<
		financial::OHLCV<base::Seconds>,
		financial::BidAsk<base::Seconds>
>;
template<typename T>
class CtorsSingletonsMultiField : public testing::Test {};
TYPED_TEST_SUITE(CtorsSingletonsMultiField, test_types_ctors_multifield);

TYPED_TEST(CtorsSingletonsMultiField, ValueDefault) {
	ASSERT_NO_THROW(TypeParam ());
}
TYPED_TEST(CtorsSingletonsMultiField, CtorInitializerList) {
	base::Value<base::Seconds> one (42.5);
	base::Value<base::Seconds> two (42.5);
	ASSERT_ANY_THROW([[maybe_unused]] TypeParam d ({one, two}));
	base::Value<base::Seconds> three (42.5);
	base::Value<base::Seconds> four (42.5);
	base::Value<base::Seconds> five (42.5);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d ({one, two, three, four, five}));
}

TYPED_TEST(CtorsSingletonsMultiField, ValueCopyCtor) {
	TypeParam d ({12.2, 12.3, 12.4, 12.5, 12.7});
	auto copy = d;
	ASSERT_EQ(copy, d);
}
TYPED_TEST(CtorsSingletonsMultiField, ValueMoveCtor) {
	TypeParam d ({12.2, 12.3, 12.4, 12.5, 12.7});
	TypeParam copy ({12.2, 12.3, 12.4, 12.5, 12.7});
	auto move__ = std::move(d);
	ASSERT_EQ(move__, copy);
}
TYPED_TEST(CtorsSingletonsMultiField, CompileError) {
	struct S {};
	[[maybe_unused]] S s;
	// base::Value<base::Seconds> d (s); //todo: make a compile time test

}



using test_element_ok = testing::Types<
		double,
		int,
		base::Value<base::Seconds>,
		financial::SingleQuote<base::Seconds>,
		financial::OHLCV<base::Seconds>,
		financial::BidAsk<base::Seconds>
>;

struct S{};
using test_element_throw = testing::Types<
		std::string,
		S
>;

template<typename T>
class CtorsOfElementOk : public testing::Test {};
TYPED_TEST_SUITE(CtorsOfElementOk, test_element_ok);

template<typename T>
class CtorsOfElementThrow : public testing::Test {};
TYPED_TEST_SUITE(CtorsOfElementThrow, test_element_throw);

TYPED_TEST(CtorsOfElementOk, CtorFromElemTypes) {
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d);
}

//todo: add copy and move ctors f