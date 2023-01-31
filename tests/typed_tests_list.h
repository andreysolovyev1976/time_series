//
// Created by Andrey Solovyev on 26/01/2023.
//

#pragma once

#include "time_series/value.h"
#include "financial_data_structures/single_quote.h"
#include "financial_data_structures/ohlcv.h"
#include "financial_data_structures/bid_ask.h"
#include "time_series/element.h"

#include <gtest/gtest.h>

#ifndef FIN_VALUE_TYPED_TESTS_LIST_H
#define FIN_VALUE_TYPED_TESTS_LIST_H


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
