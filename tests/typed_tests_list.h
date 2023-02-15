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
		financial::SingleQuote<base::traits::ValueTypeDefault>,
		financial::SingleQuote<std::int64_t>
>;
using value_multifield = testing::Types<
		financial::OHLCV<base::traits::ValueTypeDefault>,
		financial::OHLCV<std::int64_t>,
		financial::BidAsk<base::traits::ValueTypeDefault>,
		financial::BidAsk<std::int64_t>
>;

/**
 * @brief
 * Ctors
 */

template<typename T>
class ValueCtorsSingletons : public testing::Test {};
TYPED_TEST_SUITE(ValueCtorsSingletons, value_single_field);

template<typename T>
class ValueCtorsMultiField : public testing::Test {};
TYPED_TEST_SUITE(ValueCtorsMultiField, value_multifield);

using test_value = testing::Types<
		double,
		int,
		base::Value<base::traits::ValueTypeDefault>,
		base::Value<std::int64_t>,
		financial::SingleQuote<base::traits::ValueTypeDefault>,
		financial::SingleQuote<std::int64_t>,
		financial::OHLCV<base::traits::ValueTypeDefault>,
		financial::OHLCV<std::int64_t>,
		financial::BidAsk<base::traits::ValueTypeDefault>,
		financial::BidAsk<std::int64_t>
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
class ValueCompareSingletons : public testing::Test {};
TYPED_TEST_SUITE(ValueCompareSingletons, value_single_field);

template<typename T>
class ValueCompareMultiField : public testing::Test {};
TYPED_TEST_SUITE(ValueCompareMultiField, value_multifield);


/**
 * @brief
 * Arithmetics
 */

template<typename T>
class ValueArithmeticsSingletons : public testing::Test {};
TYPED_TEST_SUITE(ValueArithmeticsSingletons, value_single_field);

template<typename T>
class ValueArithmeticsMultiField : public testing::Test {};
TYPED_TEST_SUITE(ValueArithmeticsMultiField, value_multifield);


/**
 * @brief
 * Element Comparison
 */

using test_element = testing::Types<
		  time_series::Element<base::Seconds, double>
		, time_series::Element<base::Seconds, int>
		, time_series::Element<base::Seconds, base::Value<base::traits::ValueTypeDefault>>
		, time_series::Element<base::Seconds, base::Value<std::int64_t>>
		, time_series::Element<base::Seconds, financial::SingleQuote<base::traits::ValueTypeDefault>>
		, time_series::Element<base::Seconds, financial::SingleQuote<std::int64_t>>
		, time_series::Element<base::Seconds, financial::OHLCV<base::traits::ValueTypeDefault>>
		, time_series::Element<base::Seconds, financial::OHLCV<std::int64_t>>
		, time_series::Element<base::Seconds, financial::BidAsk<base::traits::ValueTypeDefault>>
		, time_series::Element<base::Seconds, financial::BidAsk<std::int64_t>>
		>;

template<typename T>
class ElementComparison : public testing::Test {};
TYPED_TEST_SUITE(ElementComparison, test_element);

template<typename T>
class ElementFnApplication : public testing::Test {};
TYPED_TEST_SUITE(ElementFnApplication, test_element);


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
