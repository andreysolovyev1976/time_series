//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

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
	[[maybe_unused]] double v1 {0.000005};
	[[maybe_unused]] double v2 {0.000006};
	[[maybe_unused]] double v3 {0.000005};
}

TYPED_TEST (ElementComparison, GT) {
	[[maybe_unused]] double v1 {0.000005};
	[[maybe_unused]] double v2 {0.000006};
	[[maybe_unused]] double v3 {0.000005};
}

TYPED_TEST (ElementComparison, LE) {
	[[maybe_unused]] double v1 {0.000005};
	[[maybe_unused]] double v2 {0.000006};
	[[maybe_unused]] double v3 {0.000005};
}

TYPED_TEST (ElementComparison, GE) {
	[[maybe_unused]] double v1 {0.000005};
	[[maybe_unused]] double v2 {0.000006};
	[[maybe_unused]] double v3 {0.000005};
}
