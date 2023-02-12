//
// Created by Andrey Solovyev on 24/01/2023.
//

#include <gtest/gtest.h>
#include "floating_point_comp.h"


using namespace comp;


TEST (CompareBasics, EqualSame) {
	double v1 {0.000005};
	double v2 {0.000006};
	double v3 {0.000005};
	ASSERT_TRUE(eq(v1, v1));
	ASSERT_TRUE(eq(v2, v2));
	ASSERT_TRUE(eq(v1, v3));
	ASSERT_TRUE(eq(v3, v1));
}

TEST (CompareBasics, EqualDifferent) {
	double v1 {42.0};
	int v2 {42};
	ASSERT_TRUE(eq(v1, v2));
	ASSERT_TRUE(eq(v2, v1));
}

TEST (CompareBasics, NotEqualSame) {
	double v1 {0.00005};
	double v2 {0.0001};
	ASSERT_TRUE(ne(v1, v2));
	ASSERT_TRUE(ne(v2, v1));
}

TEST (CompareBasics, NotEqualDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(ne(v1, v2));
	ASSERT_TRUE(ne(v2, v1));
}

TEST (CompareBasics, LessSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(lt(v2, v1));
	ASSERT_TRUE(!lt(v1, v2));
}

TEST (CompareBasics, LessDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(lt(v2, v1));
	ASSERT_TRUE(!lt(v1, v2));
}
TEST (CompareBasics, LessOrEqualSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(le(v2, v1));
	ASSERT_TRUE(!le(v1, v2));
}

TEST (CompareBasics, LessOrEqualDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(le(v2, v1));
	ASSERT_TRUE(!le(v1, v2));
}

TEST (CompareBasics, GreaterSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(gt(v1, v2));
	ASSERT_TRUE(!gt(v2, v1));
}

TEST (CompareBasics, GreaterDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(gt(v1, v2));
	ASSERT_TRUE(!gt(v2, v1));
}

TEST (CompareBasics, GreaterOrEqualSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(ge(v1, v2));
	ASSERT_TRUE(!ge(v2, v1));
}

TEST (CompareBasics, GreaterOrEqualDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(ge(v1, v2));
	ASSERT_TRUE(!ge(v2, v1));
}