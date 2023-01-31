//
// Created by Andrey Solovyev on 26/01/2023.
//

#pragma once

#include "element.h"
#include <gtest/gtest.h>

#ifndef FIN_VALUE_TYPED_TESTS_LIST_H
#define FIN_VALUE_TYPED_TESTS_LIST_H

using test_types = testing::Types<financial::Element>;

template<typename T>
class Arithmetics : public testing::Test {};
TYPED_TEST_SUITE(Arithmetics, test_types);

template<typename T>
class Constructible : public testing::Test {};
TYPED_TEST_SUITE(Constructible, test_types);

template<typename T>
class Compare : public testing::Test {};
TYPED_TEST_SUITE(Compare, test_types);

template<typename T>
class Ctors : public testing::Test {};
TYPED_TEST_SUITE(Ctors, test_types);

template<typename T>
class Assignments : public testing::Test {};
TYPED_TEST_SUITE(Assignments, test_types);

template<typename T>
class Precision : public testing::Test {};
TYPED_TEST_SUITE(Precision, test_types);



#endif //FIN_VALUE_TYPED_TESTS_LIST_H
