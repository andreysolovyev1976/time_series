//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

TYPED_TEST(ArithmeticsSingletons, Addition) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	ASSERT_EQ(v3, v1 + v2);
	ASSERT_EQ(v5, v1 + v4);

	if (std::is_integral_v<typename TypeParam::type>) {
		t = v2+v7;
		ASSERT_EQ(t, 0);
		t = t+v6;
		ASSERT_EQ(t, 2);
		t = t+v8+2;
		ASSERT_EQ(t, 6);
	}
	else {
		t = v2+v7;
		ASSERT_EQ(t, v3);
		t = t+v6;
		ASSERT_EQ(t, 3);
		t = t+v8+2;
		ASSERT_EQ(t, 7);
	}
}
TYPED_TEST(ArithmeticsSingletons, AssignAddition) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	t += v1;
	t += v2;
	ASSERT_EQ(v3, v1 + v2);

	t = 0;
	t += v1;
	t += v4;
	ASSERT_EQ(v5, v1 + v4);

	t = 0;
	t += v3;
	ASSERT_EQ(t, v3);

	t = v2;
	t += v7;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, 0);
		t += v6;
		ASSERT_EQ(t, 2);
		t += v8;
		ASSERT_EQ(t, 4);
		t += v5;
		ASSERT_EQ(t, v6);

	} else {
		ASSERT_EQ(t, v3);
		t += v6;
		ASSERT_EQ(t, 3);
		t += v8;
		ASSERT_EQ(t, 5);
		t += v5;
		ASSERT_EQ(t, v2);
	}
}
TYPED_TEST(ArithmeticsSingletons, Subtraction) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	ASSERT_EQ(v3, v1 - v5);
	ASSERT_EQ(v1, v3 - v2);

	t = t - v3;
	ASSERT_EQ(t, v4);
	t = t - v5;
	t = t - v4;
	ASSERT_EQ(t, v2);
	t = t - v6;
	t = t - v7;
	t = t - v8;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, -1);
	} else {
		ASSERT_EQ(t, 0);
	}
}
TYPED_TEST(ArithmeticsSingletons, AssignSubtraction) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	t -= v1;
	t -= v2;
	t -= v3;
	t -= v4;
	t -= v5;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, 1);
	} else {
		ASSERT_EQ(t, 1.5);
	}
	t -= v6;
	t -= v7;
	t -= v8;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, -2);
	} else {
		ASSERT_EQ(t, -1);
	}
}
TYPED_TEST(ArithmeticsSingletons, Division) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	ASSERT_EQ(v2, v2 / v3);
	ASSERT_EQ(v5, v2 / v4);

	t = 12;
	t = t / v6;
	ASSERT_EQ(t, 6);
	t = t / v8;
	ASSERT_EQ(t, 3);
	t = t / 2 / v4;
	ASSERT_EQ(t, v1);
	t = t / v7;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, 0);
	} else {
		ASSERT_EQ(t, v3);
	}
}
TYPED_TEST(ArithmeticsSingletons, AssignDivision) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	t = v2;
	t /= v3;
	ASSERT_EQ(t, v2);
	t /= v4;
	ASSERT_EQ(t, v5);

	t = 12;
	t /= v6;
	ASSERT_EQ(t, 6);
	t /= v8;
	ASSERT_EQ(t, 3);
	t /= 2;
	t /= v4;
	ASSERT_EQ(t, v1);
	t /= v7;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, 0);
	} else {
		ASSERT_EQ(t, v3);
	}
}
TYPED_TEST(ArithmeticsSingletons, Multiplication) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	t = v2 * v3;
	ASSERT_EQ(t, v2);
	t = v2 * v4;
	ASSERT_EQ(t, v5);

	t = t * v6;
	t = t * v7;
	t = t * v8;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, 12);
	} else {
		ASSERT_EQ(t, 15);
	}
}
TYPED_TEST(ArithmeticsSingletons, AssignMultiplication) {
	TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
	TypeParam t;
	double v6 {2.0};
	double v7 {-1.5};
	int v8 {2};

	t = 1;
	t *= v2;
	t *= v3;
	ASSERT_EQ(t, v2);
	t *= v4;
	ASSERT_EQ(t, v5);

	t *= v6;
	t *= v7;
	t *= v8;
	if (std::is_integral_v<typename TypeParam::type>) {
		ASSERT_EQ(t, 12);
	} else {
		ASSERT_EQ(t, 15);
	}
}


TYPED_TEST(ArithmeticsMultiField, Addition) {
	TypeParam v1 ({-12.2, 12.3, 12.0, 11.5, 12.7});
	TypeParam v2 ({22.2, 22.3, 22.5, 12.9, 15.9});
	auto t = v1 + v2;
	TypeParam check ({10.0, 34.6, 34.5, 24.4, 28.6});

	ASSERT_TRUE(t == check);
	ASSERT_TRUE(t != v1);
	ASSERT_TRUE(t != v2);

	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t = t + v3;
	t = v4 + t;
	t = t + v5;

	/*volume is ignored*/
	check = TypeParam ({12.5, 37.1, 37.0, 26.9, 0});
	ASSERT_EQ(t, check);
}
TYPED_TEST(ArithmeticsMultiField, AssignAddition) {
	TypeParam v1 ({-12.2, 12.3, 12.0, 11.5, 12.7});
	TypeParam v2 ({22.2, 22.3, 22.5, 12.9, 15.9});
	auto t = v1;
	t += v2;
	TypeParam check ({10.0, 34.6, 34.5, 24.4, 28.6});

	ASSERT_TRUE(t == check);
	ASSERT_TRUE(t != v1);
	ASSERT_TRUE(t != v2);

	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t += v3;
	t += v4;
	t += v5;

	/*volume is ignored*/
	check = TypeParam ({12.5, 37.1, 37.0, 26.9, 28.6});
	ASSERT_EQ(t, check);
}
TYPED_TEST(ArithmeticsMultiField, Subtraction) {
	TypeParam v1 ({-12.2, 12.3, 12.0, 11.5, 12.7});
	TypeParam v2 ({22.2, 22.3, 22.5, 12.9, 15.9});
	auto t = v2 - v1;
	TypeParam check ({34.4, 10, 10.5, 1.4, 3.2});

	ASSERT_EQ(t, check);
	ASSERT_TRUE(t != v1);
	ASSERT_TRUE(t != v2);

	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t = t - v3;
	t = v4 - t;
	t = t - v5;

	check = TypeParam ({-39.9, -5.5, -16, -6.9, -8.7});
	ASSERT_EQ(t, check);
}
TYPED_TEST(ArithmeticsMultiField, AssignSubtraction) {
	TypeParam v1 ({-12.2, 12.3, 12.0, 11.5, 12.7});
	TypeParam v2 ({22.2, 22.3, 22.5, 12.9, 15.9});
	auto t = v2;
	t -= v1;
	TypeParam check ({34.4, 10, 10.5, 1.4, 3.2});

	ASSERT_EQ(t, check);
	ASSERT_TRUE(t != v1);
	ASSERT_TRUE(t != v2);

	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t -= v3;
	t -= v4;
	t -= v5;

	check = TypeParam ({-39.9, -5.5, -16, -6.9, -8.7});
	ASSERT_EQ(t, check);
}
TYPED_TEST(ArithmeticsMultiField, Division) {
	TypeParam v1 ({-12.3, 12.3, 12.0, 11.1, 12.12});
	TypeParam v2 ({36.9, 36.9, 54.0, 44.4, 54.54});
	auto t = v2 / v1;
	TypeParam check ({-3, 3, 4.5, 4, 4.5});

	ASSERT_EQ(t, check);
	ASSERT_TRUE(t != v1);
	ASSERT_TRUE(t != v2);

	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t = t / v3;
	t = v4 / t;
	t = t / v5;

	check = TypeParam ({0.5, -0.5, -0.75, -0.666667, -0.75});
	ASSERT_EQ(t, check);
}
TYPED_TEST(ArithmeticsMultiField, AssignDivision) {
	TypeParam v1 ({-12.3, 12.3, 12.0, 11.1, 12.12});
	TypeParam v2 ({36.9, 36.9, 54.0, 44.4, 54.54});
	auto t = v2;
	t /= v1;
	TypeParam check ({-3, 3, 4.5, 4, 4.5});

	ASSERT_EQ(t, check);
	ASSERT_TRUE(t != v1);
	ASSERT_TRUE(t != v2);

	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t /= v3;
	t /= v4;
	t /= v5;

	check = TypeParam ({0.5, -0.5, -0.75, -0.666667, -0.75});
	ASSERT_EQ(t, check);
}
TYPED_TEST(ArithmeticsMultiField, Multiplication) {
	TypeParam t ({0.5, -0.5, -0.75, -0.666667, -0.75});
	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t = t * v3;
	t = v4 * t;
	t = t * v5;

	/*volume is ignored*/
	TypeParam check ({-3, 3, 4.5, 4, 0});
	ASSERT_EQ(t, check);

	TypeParam v1 ({-12.3, 12.3, 12.0, 11.1, 12.12});
	t = t * v1;

	check = TypeParam ({36.9, 36.9, 54.0, 44.4, 54.54});
	ASSERT_EQ(t, check);
	ASSERT_TRUE(t != v1);
}
TYPED_TEST(ArithmeticsMultiField, AssignMultiplication) {
	TypeParam t ({0.5, -0.5, -0.75, -0.666667, -0.75});
	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t *= v3;
	t *= v4;
	t *= v5;

	/*volume is ignored*/
	TypeParam check ({-3, 3, 4.5, 4, -0.75});
	ASSERT_EQ(t, check);

	TypeParam v1 ({-12.3, 12.3, 12.0, 11.1, 12.12});
	t *= v1;

	check = TypeParam ({36.9, 36.9, 54.0, 44.4, 54.54});
	ASSERT_EQ(t, check);
	ASSERT_TRUE(t != v1);
}

TYPED_TEST(ArithmeticsMultiField, DraggingVolumeAlong) {
	TypeParam t ({0.5, -0.5, -0.75, -0.666667, -0.75});
	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	t *= v3;
	t *= v4;
	t *= v5;
	/*volume is ignored*/
	TypeParam check ({-3, 3, 4.5, 4, -0.75});
	ASSERT_EQ(t, check);

	TypeParam v1 ({-12.3, 12.3, 12.0, 11.1, 12.12});
	t *= v1;

	check = TypeParam ({36.9, 36.9, 54.0, 44.4, 54.54});
	ASSERT_EQ(t, check);
	ASSERT_TRUE(t != v1);
}
