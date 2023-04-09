//
// Created by Andrey Solovyev on 26/01/2023.
//

#pragma once

#include <gtest/gtest.h>
#include "time_series/value.hpp"
#include "financial/data_structures/single_quote.h"
#include "financial/data_structures/ohlcv.hpp"
#include "financial/data_structures/bid_ask.hpp"
#include "time_series/element.hpp"

#include <cstdint>

#ifndef FIN_VALUE_TYPED_TESTS_LIST_H
#define FIN_VALUE_TYPED_TESTS_LIST_H

/**
 * @brief
 * Commonly used types lists
 */

using value_single_field = testing::Types<
		time_series::Value<time_series::value::traits::ValueTypeDefault>,
		time_series::Value<int64_t>,
		time_series::Value<long double>,
		time_series::financial::SingleQuote<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::SingleQuote<std::int64_t>
>;
using value_multifield = testing::Types<
		time_series::financial::OHLCV<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::OHLCV<std::int64_t>,
		time_series::financial::BidAsk<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::BidAsk<std::int64_t>
>;

//preserve order inside of this - some tests depend on it
using test_values_single_and_multi_field = testing::Types<
		time_series::Value<time_series::value::traits::ValueTypeDefault>,
		time_series::Value<std::int64_t>,
		time_series::financial::SingleQuote<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::SingleQuote<std::int64_t>,
		time_series::financial::OHLCV<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::OHLCV<std::int64_t>,
		time_series::financial::BidAsk<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::BidAsk<std::int64_t>
>;



struct UserDefined{};
inline bool operator == (UserDefined, UserDefined) { return true; } //for tests
using test_value = testing::Types<
		double,
		int,
		time_series::Value<time_series::value::traits::ValueTypeDefault>,
		time_series::Value<std::int64_t>,
		time_series::financial::SingleQuote<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::SingleQuote<std::int64_t>,
		time_series::financial::OHLCV<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::OHLCV<std::int64_t>,
		time_series::financial::BidAsk<time_series::value::traits::ValueTypeDefault>,
		time_series::financial::BidAsk<std::int64_t>,
		std::string,
		std::vector<int>,
		UserDefined
>;


using test_element = testing::Types<
		  time_series::Element<culib::time::Microseconds, double>
		, time_series::Element<culib::time::Microseconds, int>
		, time_series::Element<culib::time::Microseconds, time_series::Value<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::Value<std::int64_t>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::SingleQuote<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::SingleQuote<std::int64_t>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::OHLCV<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::OHLCV<std::int64_t>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::BidAsk<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::BidAsk<std::int64_t>>
		>;

using element_single_field = testing::Types<
		time_series::Element<culib::time::Microseconds, double>
		, time_series::Element<culib::time::Microseconds, int>
		, time_series::Element<culib::time::Microseconds, time_series::Value<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::Value<std::int64_t>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::SingleQuote<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::SingleQuote<std::int64_t>>
>;
using element_multifield = testing::Types<
		time_series::Element<culib::time::Microseconds, time_series::financial::OHLCV<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::OHLCV<std::int64_t>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::BidAsk<time_series::value::traits::ValueTypeDefault>>
		, time_series::Element<culib::time::Microseconds, time_series::financial::BidAsk<std::int64_t>>
>;


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
