//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"


template<typename T>
class ElementCtorsOk : public testing::Test {};
TYPED_TEST_SUITE(ElementCtorsOk, test_value);

template<typename T>
class ElementCtorsThrow : public testing::Test {};
TYPED_TEST_SUITE(ElementCtorsThrow, test_element_throw);

TYPED_TEST(ElementCtorsOk, CtorFromElemTypes) {
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d);
}

//todo: add copy and move ctors f