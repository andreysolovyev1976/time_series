//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

TYPED_TEST(ArithmeticsSingletons, OperatorPlus) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	ASSERT_EQ(v3, v1 + v2);
	ASSERT_EQ(v5, v1 + v4);

	t = v2 + v7;
	ASSERT_EQ(t, v3);
	t = t + v6;
	ASSERT_EQ(t, 3);
	t = t + v8;
	ASSERT_EQ(t, 5);
}
TYPED_TEST(ArithmeticsSingletons, OperatorEqualPlus) {
	[[maybe_unused]] TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	[[maybe_unused]] TypeParam t;
	[[maybe_unused]] double v6 {2.0};
	[[maybe_unused]] double v7 {-1.5};
	[[maybe_unused]] int v8 {2};

	ASSERT_EQ(v3, v1 + v2);
	ASSERT_EQ(v5, v1 + v4);

	t += v3;
	ASSERT_EQ(t, v3);
	t += v5;
	ASSERT_EQ(t, v1);
	t += v6;
	t = t + v8;
	ASSERT_EQ(t, v2);

	t += TypeParam (-1.5);
	ASSERT_EQ(t, v3);
}
TYPED_TEST(ArithmeticsSingletons, AdditionSameType) {
	[[maybe_unused]] TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	[[maybe_unused]] TypeParam t;
	[[maybe_unused]] double v6 {2.0};
	[[maybe_unused]] double v7 {-1.5};
	[[maybe_unused]] int v8 {2};

	ASSERT_EQ(v3, v1 + v2);
	ASSERT_EQ(v5, v1 + v4);

	t += v3;
	ASSERT_EQ(t, v3);
	t += v5;
	ASSERT_EQ(t, v1);
	t += 4;
	ASSERT_EQ(t, v2);
	t += TypeParam (-1.5);
	ASSERT_EQ(t, v3);
}
TYPED_TEST(ArithmeticsSingletons, Subtraction) {
	[[maybe_unused]] TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	[[maybe_unused]] TypeParam t;
	[[maybe_unused]] double v6 {2.0};
	[[maybe_unused]] double v7 {-1.5};
	[[maybe_unused]] int v8 {2};

	ASSERT_EQ(v3, v1 - v5);
	ASSERT_EQ(v1, v3 - v2);

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
TYPED_TEST(ArithmeticsSingletons, Division) {
	[[maybe_unused]] TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	[[maybe_unused]] TypeParam t;
	[[maybe_unused]] double v6 {2.0};
	[[maybe_unused]] double v7 {-1.5};
	[[maybe_unused]] int v8 {2};

	ASSERT_EQ(v2, v2 / v3);
	ASSERT_EQ(v5, v2 / v4);

	t /= v5;
	ASSERT_EQ(t, -2);
	t /= v4;
	ASSERT_EQ(t, 2);
	t /= 2;
	ASSERT_EQ(t, v3);
	t /= TypeParam (-0.4);
	ASSERT_EQ(t, v5);
}
TYPED_TEST(ArithmeticsSingletons, Multiplication) {
	[[maybe_unused]] TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	[[maybe_unused]] TypeParam t;
	[[maybe_unused]] double v6 {2.0};
	[[maybe_unused]] double v7 {-1.5};
	[[maybe_unused]] int v8 {2};

	ASSERT_EQ(v2, v2 * v3);
	ASSERT_EQ(v5, v2 * v4);

	t *= v2;
	ASSERT_EQ(t, v3);
	t *= v4;
	ASSERT_EQ(t, v4);
	t *= TypeParam (-2.5);
	ASSERT_EQ(t, v2);
	t *= 2;
	ASSERT_EQ(t, 5);
}



TYPED_TEST(ArithmeticsMultiField, GreaterOrEqual) {
	TypeParam v1 ({12.2, 12.3, 12.0, 11.5, 12.7});
	TypeParam v2 ({22.2, 22.3, 12.5, 12.0, 12.8});
	TypeParam v3 ({22.2, 22.3, 12.5, 12.0, 12.9});
	TypeParam v6 ({22.2, 22.3, 22.5, 12.9, 15.9});
	ASSERT_TRUE(v3 >= v2);
	ASSERT_TRUE(v2 >= v1);
	ASSERT_TRUE(!(v1 >= v6));
	ASSERT_TRUE(!(v2 >= v6));
	ASSERT_TRUE(!(v3 >= v6));

	double v4 {12.0};
	ASSERT_TRUE(v4 >= v1);
	ASSERT_TRUE(v4 >= v3);
	ASSERT_TRUE(v6 >= v4);
	ASSERT_TRUE(v4 >= v2);
	ASSERT_TRUE(v2 >= v4);
	ASSERT_TRUE(!(v4 >= v6));

	int v5 {12};
	ASSERT_TRUE(v5 >= v1);
	ASSERT_TRUE(v5 >= v2);
	ASSERT_TRUE(v2 >= v5);
	ASSERT_TRUE(v6 >= v5);
	ASSERT_TRUE(!(v5 >= v6));
}
