//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"


template<typename T>
class CtorsOfElementOk : public testing::Test {};
TYPED_TEST_SUITE(CtorsOfElementOk, test_element_value);

template<typename T>
class CtorsOfElementThrow : public testing::Test {};
TYPED_TEST_SUITE(CtorsOfElementThrow, test_element_throw);

TYPED_TEST(CtorsOfElementOk, CtorFromElemTypes) {
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d);
}

//todo: add copy and move ctors f