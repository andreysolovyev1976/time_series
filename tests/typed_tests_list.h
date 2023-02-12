//
// Created by Andrey Solovyev on 26/01/2023.
//

#pragma once

#include <gtest/gtest.h>
#include "time_series/value.h"
#include "financial_data_structures/single_quote.h"
#include "financial_data_structures/ohlcv.h"
#include "financial_data_structures/bid_ask.h"
#include "time_series/element.h"

#include <cstdint>

#ifndef FIN_VALUE_TYPED_TESTS_LIST_H
#define FIN_VALUE_TYPED_TESTS_LIST_H

/**
 * @brief
 * Commonly used types lists
 */

using value_single_field = testing::Types<
		base::Value<base::traits::ValueTypeDefault>,
		base::Value<int64_t>,
		base::Value<long double>,
		financial::SingleQuote
>;
//todo: add here ValueType
using value_multifield = testing::Types<
		financial::OHLCV<base::traits::ValueTypeDefault>,
		financial::BidAsk<base::traits::ValueTypeDefault>,
		financial::OHLCV<std::int64_t>,
		financial::BidAsk<std::int64_t>
>;

/**
 * @brief
 * Ctors
 */

template<typename T>
class CtorsSingletons : public testing::Test {};
TYPED_TEST_SUITE(CtorsSingletons, value_single_field);

template<typename T>
class CtorsMultiField : public testing::Test {};
TYPED_TEST_SUITE(CtorsMultiField, value_multifield);



using test_element_value = testing::Types<
		double,
		int,
		base::Value<base::traits::ValueTypeDefault>,
		base::Value<std::int64_t>,
		financial::SingleQuote,
		financial::OHLCV<base::traits::ValueTypeDefault>,
		financial::BidAsk<base::traits::ValueTypeDefault>
>;

struct S{};
using test_element_throw = testing::Types<
		std::string,
		S
>;


/**
 * @brief
 * Comparison
 */

template<typename T>
class CompareSingletons : public testing::Test {};
TYPED_TEST_SUITE(CompareSingletons, value_single_field);

template<typename T>
class CompareMultiField : public testing::Test {};
TYPED_TEST_SUITE(CompareMultiField, value_multifield);


/**
 * @brief
 * Arithmetics
 */

template<typename T>
class ArithmeticsSingletons : public testing::Test {};
TYPED_TEST_SUITE(ArithmeticsSingletons, value_single_field);

template<typename T>
class ArithmeticsMultiField : public testing::Test {};
TYPED_TEST_SUITE(ArithmeticsMultiField, value_multifield);


/**
 * @brief
 * Element Comparison
 */

template<typename T>
class ElementComparison : public testing::Test {};
TYPED_TEST_SUITE(ElementComparison, test_element_value);

template<typename T>
class ElementFnApplication : public testing::Test {};
TYPED_TEST_SUITE(ElementFnApplication, test_element_value);


#if 0

using test_types = testing::Types<int>;

template<typename T>
class Arithmetics : public testing::Test {};
TYPED_TEST_SUITE(Arithmetics, test_types);

template<typename T>
class Constructible : public testing::Test {};
TYPED_TEST_SUITE(Constructible, test_types);

template<typename T>
class Assignments : public testing::Test {};
TYPED_TEST_SUITE(Assignments, test_types);

template<typename T>
class Precision : public testing::Test {};
TYPED_TEST_SUITE(Precision, test_types);

#endif


#endif //FIN_VALUE_TYPED_TESTS_LIST_H
