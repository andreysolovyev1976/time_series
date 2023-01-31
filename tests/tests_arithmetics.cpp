//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

TYPED_TEST(Arithmetics, AdditionPODType) {
	TypeParam v1(-1.5, 1), v2(2.5, 2), v3(1.0), v4(-1.0), v5(-2.5, 3);
	ASSERT_EQ(v3, v1 + v2);
	ASSERT_EQ(v5, v1 + v4);
	TypeParam t;
	t += v3;
	ASSERT_EQ(t, v3);
	t += v5;
	ASSERT_EQ(t, v1);
	t += 4;
	ASSERT_EQ(t, v2);
	t += TypeParam (-1.5);
	ASSERT_EQ(t, v3);
}
TYPED_TEST(Arithmetics, AdditionSameType) {
	TypeParam v1(-1.5, 1), v2(2.5, 2), v3(1.0), v4(-1.0), v5(-2.5, 3);
	ASSERT_EQ(v3, v1 + v2);
	ASSERT_EQ(v5, v1 + v4);
	TypeParam t;
	t += v3;
	ASSERT_EQ(t, v3);
	t += v5;
	ASSERT_EQ(t, v1);
	t += 4;
	ASSERT_EQ(t, v2);
	t += TypeParam (-1.5);
	ASSERT_EQ(t, v3);
}
TYPED_TEST(Arithmetics, Subtraction) {
	TypeParam v1(-1.5, 1), v2(2.5, 2), v3(1.0), v4(-1.0), v5(-2.5, 3);
	ASSERT_EQ(v3, v1 - v5);
	ASSERT_EQ(v1, v3 - v2);
	TypeParam t;
	t -= v3;
	ASSERT_EQ(t, v4);
	t -= v5;
	t -= v4;
	ASSERT_EQ(t, v2);
	t -= 5;
	ASSERT_EQ(t, v5);
	t -= TypeParam (-3.5);
	ASSERT_EQ(t, v3);
}
TYPED_TEST(Arithmetics, Division) {
	TypeParam v1(-1.5, 1), v2(2.5, 2), v3(1.0), v4(-1.0), v5(-2.5, 3);
	ASSERT_EQ(v2, v2 / v3);
	ASSERT_EQ(v5, v2 / v4);
	TypeParam t (5);
	t /= v5;
	ASSERT_EQ(t, -2);
	t /= v4;
	ASSERT_EQ(t, 2);
	t /= 2;
	ASSERT_EQ(t, v3);
	t /= TypeParam (-0.4);
	ASSERT_EQ(t, v5);
}
TYPED_TEST(Arithmetics, Multiplication) {
	TypeParam v1(-1.5, 1), v2(2.5, 2), v3(1.0), v4(-1.0), v5(-2.5, 3);
	ASSERT_EQ(v2, v2 * v3);
	ASSERT_EQ(v5, v2 * v4);
	TypeParam t (0.4);
	t *= v2;
	ASSERT_EQ(t, v3);
	t *= v4;
	ASSERT_EQ(t, v4);
	t *= TypeParam (-2.5);
	ASSERT_EQ(t, v2);
	t *= 2;
	ASSERT_EQ(t, 5);
}
