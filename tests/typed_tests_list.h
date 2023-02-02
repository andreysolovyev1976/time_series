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


#ifndef FIN_VALUE_TYPED_TESTS_LIST_H
#define FIN_VALUE_TYPED_TESTS_LIST_H

/**
 * @brief
 * Commonly used types lists
 */

using test_types_singletons = testing::Types<
		base::Value<base::Seconds>,
		financial::SingleQuote<base::Seconds>
>;
using test_types_multifield = testing::Types<
		financial::OHLCV<base::Seconds>,
		financial::BidAsk<base::Seconds>
>;


/**
 * @brief
 * Ctors
 */

template<typename T>
class CtorsSingletons : public testing::Test {};
TYPED_TEST_SUITE(CtorsSingletons, test_types_singletons);

template<typename T>
class CtorsMultiField : public testing::Test {};
TYPED_TEST_SUITE(CtorsMultiField, test_types_multifield);

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


/**
 * @brief
 * Comparison
 */

template<typename T>
class CompareSingletons : public testing::Test {};
TYPED_TEST_SUITE(CompareSingletons, test_types_singletons);

template<typename T>
class CompareMultiField : public testing::Test {};
TYPED_TEST_SUITE(CompareMultiField, test_types_multifield);


/**
 * @brief
 * Arithmetics
 */

template<typename T>
class ArithmeticsSingletons : public testing::Test {};
TYPED_TEST_SUITE(ArithmeticsSingletons, test_types_singletons);

template<typename T>
class ArithmeticsMultiField : public testing::Test {};
TYPED_TEST_SUITE(ArithmeticsMultiField, test_types_multifield);


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
