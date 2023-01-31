//
// Created by Andrey Solovyev on 24/01/2023.
//

#include <gtest/gtest.h>

TEST (CompareBasics, EqualSame) {
	double v1 {0.000005};
	double v2 {0.000006};
	double v3 {0.000005};
	ASSERT_TRUE(v1 == v1);
	ASSERT_TRUE(v2 == v2);
	ASSERT_TRUE(v1 == v3);
	ASSERT_TRUE(v3 == v1);
}

TEST (CompareBasics, EqualDifferent) {
	double v1 {42.0};
	int v2 {42};
	ASSERT_TRUE(v1 == v1);
	ASSERT_TRUE(v2 == v2);
	ASSERT_TRUE(v1 == v2);
	ASSERT_TRUE(v2 == v1);
}

TEST (CompareBasics, NotEqualSame) {
	double v1 {0.000005};
	double v2 {0.000006};
	ASSERT_TRUE(v1 != v2);
	ASSERT_TRUE(v2 != v1);
}

TEST (CompareBasics, NotEqualDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(v1 != v2);
	ASSERT_TRUE(v2 != v1);
}

TEST (CompareBasics, LessSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(v2 < v1);
	ASSERT_TRUE(!(v1 < v2));
}

TEST (CompareBasics, LessDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(v2 < v1);
	ASSERT_TRUE(!(v1 < v2));
}
TEST (CompareBasics, LessOrEqualSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(v2 <= v1);
	ASSERT_TRUE(!(v1 <= v2));
}

TEST (CompareBasics, LessOrEqualDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(v2 <= v1);
	ASSERT_TRUE(!(v1 <= v2));
}

TEST (CompareBasics, GreaterSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(v1 > v2);
	ASSERT_TRUE(!(v2 > v1));
}

TEST (CompareBasics, GreaterDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(v1 > v2);
	ASSERT_TRUE(!(v2 > v1));
}

TEST (CompareBasics, GreaterOrEqualSame) {
	double v1 {42.5};
	double v2 {42.0};
	ASSERT_TRUE(v1 >= v2);
	ASSERT_TRUE(!(v2 >= v1));
}

TEST (CompareBasics, GreaterOrEqualDifferent) {
	double v1 {42.5};
	int v2 {42};
	ASSERT_TRUE(v1 >= v2);
	ASSERT_TRUE(!(v2 >= v1));
}