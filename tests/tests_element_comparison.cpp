//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"


template<typename T>
class ElementComparison : public testing::Test {};
TYPED_TEST_SUITE(ElementComparison, test_element);

TYPED_TEST (ElementComparison, EQ) {
	TypeParam t1 {};
	TypeParam t2 = t1;
	ASSERT_EQ(t1, t2);
	ASSERT_EQ(t2, t1);
}

TYPED_TEST (ElementComparison, NE) {
	TypeParam t1 {};
	TypeParam t2 {};
	ASSERT_NE(t1, t2);
	ASSERT_NE(t2, t1);
}

TYPED_TEST (ElementComparison, LT) {
	ASSERT_TRUE(false);
}

TYPED_TEST (ElementComparison, GT) {
	ASSERT_TRUE(false);
}

TYPED_TEST (ElementComparison, LE) {
	ASSERT_TRUE(false);
}

TYPED_TEST (ElementComparison, GE) {
	ASSERT_TRUE(false);
}
