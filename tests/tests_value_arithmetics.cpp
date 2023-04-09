//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

using namespace culib;
using namespace time_series;

template<typename T>
class ValueArithmeticsSingletons : public testing::Test {};
TYPED_TEST_SUITE(ValueArithmeticsSingletons, value_single_field);

template<typename T>
class ValueArithmeticsMultiField : public testing::Test {};
TYPED_TEST_SUITE(ValueArithmeticsMultiField, value_multifield);


TYPED_TEST(ValueArithmeticsSingletons, Addition) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {

		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		ASSERT_EQ(v3, v1+v2);
		ASSERT_EQ(v5, v1+v4);
		t = v2+v7;
		ASSERT_EQ(t, v3);
		t = t+v6;
		ASSERT_EQ(t, 3);
		t = t+v8+2;
		ASSERT_EQ(t, 7);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>){
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		ASSERT_EQ(v3, v1+v2);
		ASSERT_EQ(v5, v1+v4);

		t = v2+v7;
		ASSERT_EQ(t, 0);
		t = t+v6;
		ASSERT_EQ(t, 2);
		t = t+v8+2;
		ASSERT_EQ(t, 6);
	}
}
TYPED_TEST(ValueArithmeticsSingletons, AssignAddition)
{
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		t += v1;
		t += v2;
		ASSERT_EQ(v3, v1+v2);

		t = 0;
		t += v1;
		t += v4;
		ASSERT_EQ(v5, v1+v4);

		t = 0;
		t += v3;
		ASSERT_EQ(t, v3);

		t = v2;
		t += v7;
		ASSERT_EQ(t, v3);
		t += v6;
		ASSERT_EQ(t, 3);
		t += v8;
		ASSERT_EQ(t, 5);
		t += v5;
		ASSERT_EQ(t, v2);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>){
		TypeParam v1(-1), v2(2), v3(1), v5(-2);
		TypeParam t;

		t += v1;
		t += v2;
		ASSERT_EQ(v3, v1+v2);

		t = 0;
		t += v1;
		t += v1;
		ASSERT_EQ(v5, v1+v1);

		t = 0;
		t += v3;
		ASSERT_EQ(t, 1);

		t = v2;
		t += v5;
		ASSERT_EQ(t, 0);
		t += v2;
		ASSERT_EQ(t, 2);
		t += v2;
		ASSERT_EQ(t, 4);
		t += v5;
		ASSERT_EQ(t, 2);
	}
}
TYPED_TEST(ValueArithmeticsSingletons, Subtraction) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		ASSERT_EQ(v3, v1-v5);
		ASSERT_EQ(v1, v3-v2);

		t = t-v3;
		ASSERT_EQ(t, v4);
		t = t-v5;
		t = t-v4;
		ASSERT_EQ(t, v2);
		t = t-v6;
		t = t-v7;
		t = t-v8;
		ASSERT_EQ(t, 0);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		ASSERT_EQ(v3, v1-v5);
		ASSERT_EQ(v1, v3-v2);

		t = t-v3;
		ASSERT_EQ(t, v4);
		t = t-v5;
		t = t-v4;
		ASSERT_EQ(t, v2);
		t = t-v6;
		t = t-v7;
		t = t-v8;
		ASSERT_EQ(t, -1);
	}
}
TYPED_TEST(ValueArithmeticsSingletons, AssignSubtraction) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		t -= v1;
		t -= v2;
		t -= v3;
		t -= v4;
		t -= v5;

		ASSERT_EQ(t, 1.5);
		t -= v6;
		t -= v7;
		t -= v8;

		ASSERT_EQ(t, -1);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		t -= v1;
		t -= v2;
		t -= v3;
		t -= v4;
		t -= v5;
		ASSERT_EQ(t, 1);
		t -= v6;
		t -= v7;
		t -= v8;
		ASSERT_EQ(t, -2);
	}
}
TYPED_TEST(ValueArithmeticsSingletons, Division) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {

		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		ASSERT_EQ(v2, v2/v3);
		ASSERT_EQ(v5, v2/v4);

		t = 12;
		t = t/v6;
		ASSERT_EQ(t, 6);
		t = t/v8;
		ASSERT_EQ(t, 3);
		t = t/2/v4;
		ASSERT_EQ(t, v1);
		t = t/v7;
		ASSERT_EQ(t, v3);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
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
		ASSERT_EQ(t, 0);
	}
}
TYPED_TEST(ValueArithmeticsSingletons, AssignDivision) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

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
		ASSERT_EQ(t, v3);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

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
		ASSERT_EQ(t, 0);
	}
}
TYPED_TEST(ValueArithmeticsSingletons, DivisionByZeroThrows) {
	TypeParam v1(-1.0);
	TypeParam zero(0.0);

	ASSERT_THROW ( v1 / 0, std::invalid_argument);
	ASSERT_THROW ( v1 / zero, std::invalid_argument);
}
TYPED_TEST(ValueArithmeticsSingletons, Multiplication) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		t = v2*v3;
		ASSERT_EQ(t, v2);
		t = v2*v4;
		ASSERT_EQ(t, v5);

		t = t*v6;
		t = t*v7;
		t = t*v8;
		ASSERT_EQ(t, 15);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>){
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		t = v2*v3;
		ASSERT_EQ(t, v2);
		t = v2*v4;
		ASSERT_EQ(t, v5);

		t = t*v6;
		t = t*v7;
		t = t*v8;
		ASSERT_EQ(t, 12);
	}
}
TYPED_TEST(ValueArithmeticsSingletons, AssignMultiplication) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		t = 1;
		t *= v2;
		t *= v3;
		ASSERT_EQ(t, v2);
		t *= v4;
		ASSERT_EQ(t, v5);

		t *= v6;
		t *= v7;
		t *= v8;
		ASSERT_EQ(t, 15);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.5), v3(1.0), v4(-1.0), v5(-2.5);
		TypeParam t;
		double v6{2.0};
		double v7{-1.5};
		int v8{2};

		t = 1;
		t *= v2;
		t *= v3;
		ASSERT_EQ(t, v2);
		t *= v4;
		ASSERT_EQ(t, v5);

		t *= v6;
		t *= v7;
		t *= v8;
		ASSERT_EQ(t, 12);
	}
}


TYPED_TEST(ValueArithmeticsMultiField, Addition) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1({-12.2, 12.3, 12.0, 11.5, 12.7});
		TypeParam v2({22.2, 22.3, 22.5, 12.9, 15.9});
		auto t = v1+v2;
		TypeParam check({10.0, 34.6, 34.5, 24.4, 28.6});

		ASSERT_TRUE(t==check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};
		t = t+v3;
		t = v4+t;
		t = t+v5;

		/*volume is ignored*/
		check = TypeParam({12.5, 37.1, 37.0, 26.9, 0});
		ASSERT_EQ(t, check);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1({-12, 12, 10, 11, 12});
		TypeParam v2({22, 22, 22, 12, 15});
		auto t = v1+v2;
		TypeParam check({10, 34, 32, 23, 27});

		ASSERT_TRUE(t==check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1};
		int v5{2};
		t = t+v3;
		t = v4+t;
		t = t+v5;

		// volume is ignored
		check = TypeParam({13, 37, 35, 26, 0});
		ASSERT_EQ(t, check);
	}
}
TYPED_TEST(ValueArithmeticsMultiField, AssignAddition) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1({-12.2, 12.3, 12.0, 11.5, 12.7});
		TypeParam v2({22.2, 22.3, 22.5, 12.9, 15.9});
		auto t = v1;
		t += v2;
		TypeParam check({10.0, 34.6, 34.5, 24.4, 28.6});

		ASSERT_TRUE(t==check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};
		t += v3;
		t += v4;
		t += v5;

		/*volume is ignored*/
		check = TypeParam({12.5, 37.1, 37.0, 26.9, 28.6});
		ASSERT_EQ(t, check);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1({-12, 12, 10, 11, 12});
		TypeParam v2({22, 22, 22, 12, 15});
		auto t = v1;
		t += v2;
		TypeParam check({10, 34, 32, 23, 27});

		ASSERT_TRUE(t==check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1};
		int v5{2};
		t += v3;
		t = v4+t;
		t += v5;

		// volume is ignored
		check = TypeParam({13, 37, 35, 26, 0});
		ASSERT_EQ(t, check);
	}
}
TYPED_TEST(ValueArithmeticsMultiField, Subtraction) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1({-12.2, 12.3, 12.0, 11.5, 12.7});
		TypeParam v2({22.2, 22.3, 22.5, 12.9, 15.9});
		auto t = v2-v1;
		TypeParam check({34.4, 10, 10.5, 1.4, 3.2});

		ASSERT_EQ(t, check);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		t = financial::operator-<typename TypeParam::value_type, decltype(v3), true>(t, v3);
		check = TypeParam({32.4, 8, 8.5, -0.6, 1.2});
		ASSERT_EQ(t, check);

		t = v4-t;
		check = TypeParam({-33.9, -9.5, -10, -0.9, 0});
		ASSERT_EQ(t, check);

		t = t-v5;
		check = TypeParam({-35.9, -11.5, -12, -2.9, 0});
		ASSERT_EQ(t, check);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1({-12, 12, 10, 11, 13});
		TypeParam v2({22, 22, 22, 12, 15});

		auto t = v2-v1;
		TypeParam check({34, 10, 12, 1, 2});
		ASSERT_EQ(t, check);

		double v3{2.0};
		double v4{-1};
		int v5{2};

		t = financial::operator-<typename TypeParam::value_type, decltype(v3), true>(t, v3);
		check = TypeParam({32, 8, 10, -1, 0});
		ASSERT_EQ(t, check);

		t = v4-t;
		check = TypeParam({-33, -9, -11, 0, 0});
		ASSERT_EQ(t, check);

		t = t-v5;
		check = TypeParam({-35, -11, -13, -2, 0});
		ASSERT_EQ(t, check);
	}
}
TYPED_TEST(ValueArithmeticsMultiField, AssignSubtraction) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1({-12.2, 12.3, 12.0, 11.5, 12.7});
		TypeParam v2({22.2, 22.3, 22.5, 12.9, 15.9});
		auto t = v2;
		t -= v1;
		TypeParam check({34.4, 10, 10.5, 1.4, 3.2});

		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		t = financial::operator-=<typename TypeParam::value_type, decltype(v3), true>(t, v3);
		check = TypeParam({32.4, 8, 8.5, -0.6, 1.2});
		ASSERT_EQ(t, check);

		t -= v4;
		check = TypeParam({33.9, 9.5, 10, 0.9, 1.2});
		ASSERT_EQ(t, check);

		t -= v5;
		check = TypeParam({31.9, 7.5, 8, -1.1, 1.2});
		ASSERT_EQ(t, check);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1({-12, 12, 10, 11, 13});
		TypeParam v2({22, 22, 22, 12, 15});

		auto t = v2;
		t -= v1;
		TypeParam check({34, 10, 12, 1, 2});
		ASSERT_EQ(t, check);

		double v3{2.0};
		double v4{-1};
		int v5{2};

		t = financial::operator-<typename TypeParam::value_type, decltype(v3), true>(t, v3);
		check = TypeParam({32, 8, 10, -1, 0});
		ASSERT_EQ(t, check);

		t -= v4;
		check = TypeParam({33, 9, 11, 0, 0});
		ASSERT_EQ(t, check);

		t -= v5;
		check = TypeParam({31, 7, 9, -2, 0});
		ASSERT_EQ(t, check);
	}
}

TYPED_TEST(ValueArithmeticsMultiField, VolumeTooParam) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1({-12.2, 12.3, 12.0, 11.5, 12.7});
		double v3{2.0};
		auto t = financial::operator-<typename TypeParam::value_type, decltype(v3), true>(v1, v3);
		auto check = TypeParam({-14.2, 10.3, 10, 9.5, 10.7});
		ASSERT_EQ(t, check);

	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1({-12, 12, 10, 11, 13});
		double v3{2};
		auto t = financial::operator-<typename TypeParam::value_type, decltype(v3), true>(v1, v3);
		auto check = TypeParam({-14, 10, 8, 9, 11});
		ASSERT_EQ(t, check);
	}
}

TYPED_TEST(ValueArithmeticsMultiField, Division) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {

		TypeParam v1({-12.3, 12.3, 12.0, 11.1, 12.12});
		TypeParam v2({36.9, 36.9, 54.0, 44.4, 54.54});
		auto t = v2/v1;
		TypeParam check({-3, 3, 4.5, 4, 4.5});

		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		t = t/v3;
		check = TypeParam({-1.5, 1.5, 2.25, 2, 0});
		ASSERT_EQ(t, check);

		t = TypeParam({-1.5, 1.5, 2.25, 2, 1});
		t = v4/t;
		check = TypeParam({1, -1, -0.666667, -0.75, 0});
		ASSERT_EQ(t, check);

		t = t/v5;
		check = TypeParam({0.5, -0.5, -0.333333, -0.375, 0});
		ASSERT_EQ(t, check);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1({-1, 1, 2, -1, -2});
		TypeParam v2({2, -2, 0, -1, 2});
		auto t = v2/v1;
		TypeParam check({-2, -2, 0, 1, -1});

		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		t = t/v3;
		check = TypeParam({-1, -1, 0, 0, 0});
		ASSERT_EQ(t, check);

		t = TypeParam ({6, -6, 4, -4, 2});
		t = 24/t;
		check = TypeParam({4, -4, 6, -6, 0});
		ASSERT_EQ(t, check);

		t = t/v5;
		check = TypeParam({2, -2, 3, -3, 0});
		ASSERT_EQ(t, check);
	}
}
TYPED_TEST(ValueArithmeticsMultiField, AssignDivision) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {

		TypeParam v1({-12.3, 12.3, 12.0, 11.1, 12.12});
		TypeParam v2({36.9, 36.9, 54.0, 44.4, 54.54});
		auto t = v2;
		t /= v1;
		TypeParam check({-3, 3, 4.5, 4, 4.5});

		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		t /= v3;
		check = TypeParam({-1.5, 1.5, 2.25, 2, 4.5});
		ASSERT_EQ(t, check);

		t /= v4;
		check = TypeParam({1, -1, -1.5, -1.333333, 4.5});
		ASSERT_EQ(t, check);

		t /= v5;
		check = TypeParam({0.5, -0.5, -0.75, -0.666667, 4.5});
		ASSERT_EQ(t, check);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1({-1, 1, 2, -1, -2});
		TypeParam v2({2, -2, 0, -1, 2});
		auto t = v2;
		t /= v1;
		TypeParam check({-2, -2, 0, 1, -1});

		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
		ASSERT_TRUE(t!=v2);

		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		t /= v3;
		check = TypeParam({-1, -1, 0, 0, -1});
		ASSERT_EQ(t, check);

		t = TypeParam ({6, -6, 4, -4, 2});
		t = 24/t;
		check = TypeParam({4, -4, 6, -6, 0});
		ASSERT_EQ(t, check);

		t /= v5;
		check = TypeParam({2, -2, 3, -3, 0});
		ASSERT_EQ(t, check);
	}
}
TYPED_TEST(ValueArithmeticsMultiField, DivisionByZeroThrows) {
	TypeParam v1({-1, 1, 2, -1, -2});
	TypeParam zero(0.0);

	ASSERT_THROW ( v1 / 0, std::invalid_argument);
	ASSERT_THROW ( v1 / zero, std::invalid_argument);

	TypeParam v2({-1, 0, 2, -1, -2});
	ASSERT_TRUE (v2.containsZero());

	TypeParam v3({-1, 1, 2, -1, 0});
	ASSERT_TRUE (v2.containsZero());

	ASSERT_THROW ( v1 / v3, std::invalid_argument);

}
TYPED_TEST(ValueArithmeticsMultiField, Multiplication) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {

		TypeParam t({0.5, -0.5, -0.75, -0.666667, -0.75});
		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		TypeParam check;

		t = t*v3;
		check = TypeParam({1, -1, -1.5, -1.333333, 0});
		ASSERT_EQ(t, check);

		t = v4*t;
		check = TypeParam({-1.5, 1.5, 2.25, 2, 0});
		ASSERT_EQ(t, check);

		t = t*v5;
		check = TypeParam({-3, 3, 4.5, 4, 0});
		ASSERT_EQ(t, check);

		TypeParam v1({-12.3, 12.3, 12.0, 11.1, 12.12});
		t = t*v1;

		check = TypeParam({36.9, 36.9, 54.0, 44.4, 0});

		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam t({3, -3, -2, -1, 2});
		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		TypeParam check;

		t = t * v3;
		check = TypeParam({6, -6, -4, -2, 0}); //volume is not taken into consideration
		ASSERT_EQ(t, check);

		t = t * v4;
		check = TypeParam({-9, 9, 6, 3, 0});
		ASSERT_EQ(t, check);

		t = t * v5;
		check = TypeParam({-18, 18, 12, 6, 0});
		ASSERT_EQ(t, check);

		TypeParam v1({-1, 1, 0, 2, 2});
		t = t * v1;
		check = TypeParam({18, 18, 0, 12, 0});
		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
	}
}
TYPED_TEST(ValueArithmeticsMultiField, AssignMultiplication){
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam t({0.5, -0.5, -0.75, -0.666667, -0.75});
		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		TypeParam check;

		t *= v3;
		check = TypeParam({1, -1, -1.5, -1.33333, -0.75});
		ASSERT_EQ(t, check);

		t *= v4;
		check = TypeParam({-1.5, 1.5, 2.25, 2, -0.75});
		ASSERT_EQ(t, check);

		t *= v5;
		check = TypeParam({-3, 3, 4.5, 4, -0.75});
		ASSERT_EQ(t, check);

		TypeParam v1({-12.3, 12.3, 12.0, 11.1, 12.12});
		t *= v1;
		check = TypeParam({36.9, 36.9, 54.0, 44.4, -9.09});
		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
	}
	else if (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam t({3, -3, -2, -1, 2});
		double v3{2.0};
		double v4{-1.5};
		int v5{2};

		TypeParam check;

		t *= v3;
		check = TypeParam({6, -6, -4, -2, 2}); //volume is not taken into consideration
		ASSERT_EQ(t, check);

		t *= v4;
		check = TypeParam({-9, 9, 6, 3, 2});
		ASSERT_EQ(t, check);

		t *= v5;
		check = TypeParam({-18, 18, 12, 6, 2});
		ASSERT_EQ(t, check);

		TypeParam v1({-1, 1, 0, 2, 2});
		t *= v1;
		check = TypeParam({18, 18, 0, 12, 4});
		ASSERT_EQ(t, check);
		ASSERT_TRUE(t!=v1);
	}
}


