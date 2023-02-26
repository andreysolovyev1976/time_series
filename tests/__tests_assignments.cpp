//
// Created by Andrey Solovyev on 24/01/2023.
//

#include <gtest/gtest.h>
#include "typed_tests_list.h"

#include "time_series/value.hpp"
#include "financial_data_structures/single_quote.h"
#include "financial_data_structures/ohlcv.h"
#include "financial_data_structures/bid_ask.h"


TEST(CtorsBase, ValueDefault) {
	ASSERT_NO_THROW(base::Value<base::Seconds> ());
}
TEST(CtorsBase, ValueInt) {
	ASSERT_NO_THROW(base::Value<base::Seconds> (42));
	ASSERT_NO_THROW(base::Value<base::Seconds> (42u));
	ASSERT_NO_THROW(base::Value<base::Seconds> (42l));
	ASSERT_NO_THROW(base::Value<base::Seconds> (42ll));
	ASSERT_NO_THROW(base::Value<base::Seconds> (42ul));
	ASSERT_NO_THROW(base::Value<base::Seconds> (42ull));
	ASSERT_NO_THROW(base::Value<base::Seconds> (-1));
	ASSERT_NO_THROW(base::Value<base::Seconds> (0));
	ASSERT_NO_THROW(base::Value<base::Seconds> (1));
	ASSERT_NO_THROW(base::Value<base::Seconds> (+1));
	ASSERT_NO_THROW(base::Value<base::Seconds> (1+1));
	ASSERT_NO_THROW(base::Value<base::Seconds> ( [](){return 42;}() ));
	ASSERT_NO_THROW(base::Value<base::Seconds> (3'000'000'000));
}
TEST(CtorsBase, ValueFloat) {
	ASSERT_NO_THROW(base::Value<base::Seconds> (42.0f));
	ASSERT_NO_THROW(base::Value<base::Seconds> (-1.42f));
	ASSERT_NO_THROW(base::Value<base::Seconds> (0.0f));
	ASSERT_NO_THROW(base::Value<base::Seconds> (0.0005f));
	ASSERT_NO_THROW(base::Value<base::Seconds> (1.42f));
	ASSERT_NO_THROW(base::Value<base::Seconds> (42.0));
	ASSERT_NO_THROW(base::Value<base::Seconds> (-1.42));
	ASSERT_NO_THROW(base::Value<base::Seconds> (0.0));
	ASSERT_NO_THROW(base::Value<base::Seconds> (0.0005));
	ASSERT_NO_THROW(base::Value<base::Seconds> (1.42));
	ASSERT_NO_THROW(base::Value<base::Seconds> (+1.42));
	ASSERT_NO_THROW(base::Value<base::Seconds> (1.42+1.42));
	ASSERT_NO_THROW(base::Value<base::Seconds> (1.42+1));
	ASSERT_NO_THROW(base::Value<base::Seconds> ( [](){return 42.42;}() ));
//	ASSERT_EQ(s2, TypeParam ("42"s));
}
TEST(CtorsBase, ValueString) {
	using namespace std::string_literals;
	ASSERT_NO_THROW(base::Value<base::Seconds> ("42"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("-1"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("0"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("1"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("+1"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("42.0"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("-1.42"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("0.0"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("0.0005"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("1.42"s));
	ASSERT_NO_THROW(base::Value<base::Seconds> ("+1.42"s));
}
TEST(CtorsBase, CompileError) {
	struct S {};
	[[maybe_unused]] S s;
	// base::Value<base::Seconds> d (s); //todo: make a compile time test

}

TEST(CtorsFinancial, SingleQuoteDefault) {
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ());
}
TEST(CtorsFinancial, SingleQuoteInt) {
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42u));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42l));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42ll));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42ul));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42ull));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (-1));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (0));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (1));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (+1));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (1+1));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ( [](){return 42;}() ));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (3'000'000'000));
}
TEST(CtorsFinancial, SingleQuoteFloat) {
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42.0f));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (-1.42f));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (0.0f));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (0.0005f));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (1.42f));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (42.0));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (-1.42));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (0.0));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (0.0005));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (1.42));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (+1.42));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (1.42+1.42));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> (1.42+1));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ( [](){return 42.42;}() ));
//	ASSERT_EQ(s2, TypeParam ("42"s));
}
TEST(CtorsFinancial, SingleQuoteString) {
	using namespace std::string_literals;
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("42"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("-1"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("0"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("1"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("+1"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("42.0"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("-1.42"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("0.0"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("0.0005"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("1.42"s));
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds> ("+1.42"s));
}

TEST(CtorsFinancial, OHLCV){
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds>())
}
TEST(CtorsFinancial, OHLCV) {
	ASSERT_NO_THROW(financial::SingleQuote<base::Seconds>())
}
TEST(CtorsFinancial, BidAsk) {}

TYPED_TEST(CtorsOfElementOk, DefaultCtors) {
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d);
}


#if 0

TYPED_TEST(CtorsOfElementThrow, Floats) {
	double a {42.42};
	ASSERT_NO_THROW(TypeParam (a));
}


TYPED_TEST(Ctors, Strings) {
	using namespace std::literals;
	ASSERT_NO_THROW(TypeParam s3 ("42.5"s));
	ASSERT_NO_THROW(TypeParam ("-1.42"s));
	ASSERT_NO_THROW(TypeParam ("0.0"s));
	ASSERT_NO_THROW(TypeParam ("1.42"s));
	ASSERT_NO_THROW(TypeParam ("+1.42"s));
}
TYPED_TEST(Ctors, CopyCtor) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 (i1));
	TypeParam i2 (i1);
	ASSERT_EQ(i1, i2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2 (d1));
	TypeParam d2 (d1);
	ASSERT_EQ(d1, d2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 (s1));
	TypeParam s2 (s1);
	ASSERT_EQ(s1, s2);
}
TYPED_TEST(Ctors, MoveCtor) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 (std::move(i1)));
	TypeParam i2 (std::move(i1));
	ASSERT_EQ(i2, TypeParam (42));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2 (std::move(d1)));
	TypeParam d2 (std::move(d1));
	ASSERT_EQ(d2, TypeParam (42.42, 5));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 (std::move(s1)));
	TypeParam s2 (std::move(s1));
	ASSERT_EQ(s2, TypeParam ("42"s));
}
TYPED_TEST(Ctors, Throws) {
	using namespace std::literals;
	ASSERT_ANY_THROW(TypeParam s4 (42.42, -1));
	ASSERT_ANY_THROW(TypeParam s4 (42.42, 51));

	struct S {};
	[[maybe_unused]] S s;
//	ASSERT_ANY_THROW(isConstructable<TypeParam, S>(s));

	ASSERT_ANY_THROW(TypeParam s4 ("42,5"s));
	ASSERT_ANY_THROW(TypeParam s1 ("fuck"s));
}

TYPED_TEST(Assignments, Copy) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 = i1);
	TypeParam i2 = i1;
	ASSERT_EQ(i1, i2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2 = d1);
	TypeParam d2 = d1;
	ASSERT_EQ(d1, d2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 = s1);
	TypeParam s2 = s1;
	ASSERT_EQ(s1, s2);
}
TYPED_TEST(Assignments, Move) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 = std::move(i1));
	TypeParam i2 = std::move(i1);
	ASSERT_EQ(i2, TypeParam (42));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2  = std::move(d1));
	TypeParam d2 = std::move(d1);
	ASSERT_EQ(d2, TypeParam (42.42, 5));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 = std::move(s1));
	TypeParam s2 = std::move(s1);
}

#endif