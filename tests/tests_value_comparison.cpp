//
// Created by Andrey Solovyev on 24/01/2023.
//


#include "typed_tests_list.h"
#include <type_traits>

template<typename T>
class ValueCompareSingletons : public testing::Test {};
TYPED_TEST_SUITE(ValueCompareSingletons, value_single_field);

template<typename T>
class ValueCompareMultiField : public testing::Test {};
TYPED_TEST_SUITE(ValueCompareMultiField, value_multifield);


TYPED_TEST(ValueCompareSingletons, Equal) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.0);
		double v3{2.0};
		double v4{-1.5};
		int v5{2};
		ASSERT_TRUE(v1==v1);
		ASSERT_TRUE(v2==v2);
		ASSERT_TRUE(v1==v4);
		ASSERT_TRUE(v4==v1);
		ASSERT_TRUE(v2==v3);
		ASSERT_TRUE(v3==v2);
		ASSERT_TRUE(v2==v5);
		ASSERT_TRUE(v5==v2);
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1), v2(2);
		double v3{2.0};
		double v4{-1.5};
		int v5{2};
		ASSERT_TRUE(v1==v1);
		ASSERT_TRUE(v2==v2);
		ASSERT_TRUE(v1==-1);
		ASSERT_TRUE(v2==v3);
		ASSERT_TRUE(v3==v2);
		ASSERT_TRUE(v2==v5);
		ASSERT_TRUE(v5==v2);
	}
}
TYPED_TEST(ValueCompareSingletons, NotEqual) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.0);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v2 != v1);
		ASSERT_TRUE(v1 != v2);
		ASSERT_TRUE(v1 != v3);
		ASSERT_TRUE(v1 != v5);
		ASSERT_TRUE(v3 != v1);
		ASSERT_TRUE(v5 != v1);
		ASSERT_TRUE(v2 != v4);
		ASSERT_TRUE(v4 != v2);
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1), v2(2);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v2 != v1);
		ASSERT_TRUE(v1 != v2);
		ASSERT_TRUE(v1 != v3);
		ASSERT_TRUE(v1 != v5);
		ASSERT_TRUE(v3 != v1);
		ASSERT_TRUE(v5 != v1);
		ASSERT_TRUE(v2 != v4);
		ASSERT_TRUE(v4 != v2);
	}
}
TYPED_TEST(ValueCompareSingletons, Less) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.0);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v1 < v2);
		ASSERT_TRUE(!(v2 < v1));
		ASSERT_TRUE(v1 < v3);
		ASSERT_TRUE(v1 < v5);
		ASSERT_TRUE(!(v3 < v1));
		ASSERT_TRUE(!(v5 < v1));
		ASSERT_TRUE(!(v2 < v4));
		ASSERT_TRUE(v4 < v2);
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1), v2(2);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v1 < v2);
		ASSERT_TRUE(!(v2 < v1));
		ASSERT_TRUE(v1 < v3);
		ASSERT_TRUE(v1 < v5);
		ASSERT_TRUE(!(v3 < v1));
		ASSERT_TRUE(!(v5 < v1));
		ASSERT_TRUE(!(v2 < v4));
		ASSERT_TRUE(v4 < v2);
	}
}
TYPED_TEST(ValueCompareSingletons, LessOrEqual) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.0);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v1 <= v2);
		ASSERT_TRUE(!(v2 <= v1));
		ASSERT_TRUE(v1 <= v3);
		ASSERT_TRUE(v1 <= v5);
		ASSERT_TRUE(v1 <= v4);
		ASSERT_TRUE(!(v3 <= v1));
		ASSERT_TRUE(!(v5 <= v1));
		ASSERT_TRUE(v4 <= v1);
		ASSERT_TRUE(!(v2 <= v4));
		ASSERT_TRUE(v4 <= v2);
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1), v2(2);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v1 <= v2);
		ASSERT_TRUE(!(v2 <= v1));
		ASSERT_TRUE(v1 <= v3);
		ASSERT_TRUE(v1 <= v5);
		ASSERT_TRUE(v1 <= 0);
		ASSERT_TRUE(v1 <= -1);
		ASSERT_TRUE(!(v3 <= v1));
		ASSERT_TRUE(!(v5 <= v1));
		ASSERT_TRUE(v4 <= v1);
		ASSERT_TRUE(!(v2 <= v4));
		ASSERT_TRUE(v4 <= v2);
	}
}
TYPED_TEST(ValueCompareSingletons, Greater) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.0);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v2 > v1);
		ASSERT_TRUE(!(v1 > v2));
		ASSERT_TRUE(v3 > v1);
		ASSERT_TRUE(v5 > v1);
		ASSERT_TRUE(!(v1 > v3));
		ASSERT_TRUE(!(v1 > v5));
		ASSERT_TRUE(v2 > v4);
		ASSERT_TRUE(!(v4 > v2));
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1), v2(2);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v2 > v1);
		ASSERT_TRUE(!(v1 > v2));
		ASSERT_TRUE(v3 > v1);
		ASSERT_TRUE(v5 > v1);
		ASSERT_TRUE(!(v1 > v3));
		ASSERT_TRUE(!(v1 > v5));
		ASSERT_TRUE(v2 > v4);
		ASSERT_TRUE(!(v4 > v2));
	}
}
TYPED_TEST(ValueCompareSingletons, GreaterOrEqual) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1(-1.5), v2(2.0);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v2 >= v1);
		ASSERT_TRUE(!(v1 >= v2));
		ASSERT_TRUE(v3 >= v1);
		ASSERT_TRUE(v5 >= v1);
		ASSERT_TRUE(v4 >= v1);
		ASSERT_TRUE(!(v1 >= v3));
		ASSERT_TRUE(!(v1 >= v5));
		ASSERT_TRUE(v1 >= v4);
		ASSERT_TRUE(!(v4 >= v2));
		ASSERT_TRUE(v2 >= v4);
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1(-1), v2(2);
		double v3 {2.0};
		double v4 {-1.5};
		int v5 {2};
		ASSERT_TRUE(v2 >= v1);
		ASSERT_TRUE(!(v1 >= v2));
		ASSERT_TRUE(v3 >= v1);
		ASSERT_TRUE(v5 >= v1);
		ASSERT_TRUE(!(v1 >= v3));
		ASSERT_TRUE(!(v1 >= v5));
		ASSERT_TRUE(v1 >= v4);
		ASSERT_TRUE(!(v4 >= v2));
		ASSERT_TRUE(v2 >= v4);
	}
}

/**
 * @details
 * for OHLCV default field to compare is Close
 * for BidAsk default field to compare is Price
 * it is a pain in the ass to call a template param using GTest,
 * so v3-v5 values are chosen to pass/fail a test.
 * */

TYPED_TEST(ValueCompareMultiField, Equal) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12.2, 12.3, 12.0, 12.5, 12.7});
		TypeParam v2 ({22.2, 22.3, -0.4, 12.5, 12.7});
		ASSERT_TRUE(v1 == v1);
		ASSERT_TRUE(v2 == v2);

		double v3 {12.5};
		//Both Price and Close are equal to v3
		ASSERT_TRUE(v1==v3);
		ASSERT_TRUE(v2 == v3);
		ASSERT_TRUE(v3 == v1);
		ASSERT_TRUE(v3 == v2);

		int v5 {12};
		TypeParam v6 ({22.2, 22.3, -0.4, 12.0, 12.7});
		//Both Price and Close are equal to v5
		ASSERT_TRUE(v5 == v6);
		ASSERT_TRUE(v6 == v5);
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12, 12, 12, 12, 12});
		TypeParam v2 ({22, 22, -0, 12, 12});
		ASSERT_TRUE(v1 == v1);
		ASSERT_TRUE(v2 == v2);

		double v3 {12};
		//Both Price and Close are equal to v3
		ASSERT_TRUE(v1==v3);
		ASSERT_TRUE(v2 == v3);
		ASSERT_TRUE(v3 == v1);
		ASSERT_TRUE(v3 == v2);

		int v5 {12};
		TypeParam v6 ({22.2, 22.3, -0.4, 12.0, 12.7});
		//Both Price and Close are equal to v5
		ASSERT_TRUE(v5 == v6);
		ASSERT_TRUE(v6 == v5);
	}
}
TYPED_TEST(ValueCompareMultiField, NotEqual) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12.2, 12.3, 12.0, 12.5, 12.7});
		TypeParam v2 ({22.2, 22.3, -1.5, 12.5, 12.7});
		ASSERT_TRUE(v1 != v2);
		ASSERT_TRUE(v2 != v1);

		double v4 {-1.5};
		//Both Price and Close are equal to v3, so
		ASSERT_TRUE(v1 != v4);
		ASSERT_TRUE(v4 != v1);
		ASSERT_TRUE(v2 != v4); //note the third elem is equal
		ASSERT_TRUE(v4 != v2);

		int v5 {12};
		//Both Price and Close are equal to v5
		ASSERT_TRUE(v5 != v1);
		ASSERT_TRUE(v5 != v2);
		ASSERT_TRUE(v1 != v5);
		ASSERT_TRUE(v2 != v5);
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12, 12, 12, 12, 12});
		TypeParam v2 ({22, 22, -1, 12, 12});
		ASSERT_TRUE(v1 != v2);
		ASSERT_TRUE(v2 != v1);

		double v4 {-1.5};
		//Both Price and Close are equal to v3, so
		ASSERT_TRUE(v1 != v4);
		ASSERT_TRUE(v4 != v1);
		ASSERT_TRUE(v2 != v4); //note the third elem is equal
		ASSERT_TRUE(v4 != v2);

		int v5 {42};
		ASSERT_TRUE(v5 != v1);
		ASSERT_TRUE(v5 != v2);
		ASSERT_TRUE(v1 != v5);
		ASSERT_TRUE(v2 != v5);
	}
}
TYPED_TEST(ValueCompareMultiField, Less) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12.2, 12.3, 12.0, 11.5, 12.7}); //all of v1 < v2
		TypeParam v2 ({22.2, 22.3, 12.5, 12.7, 12.8});
		ASSERT_TRUE(v1 < v2);
		ASSERT_TRUE(!(v2 < v1));

		double v3 {12.6};
		ASSERT_TRUE(v1 < v3);
		ASSERT_TRUE(v3 < v2);
		ASSERT_TRUE(!(v3 < v1));
		ASSERT_TRUE(!(v2 < v3));

		int v5 {12};
		ASSERT_TRUE(v1 < v5);
		ASSERT_TRUE(v5 < v2);
		ASSERT_TRUE(!(v5 < v1));
		ASSERT_TRUE(!(v2 < v5));
	}
	else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12, 12, 12, 11, 12}); //all of v1 < v2
		TypeParam v2 ({22, 22, 13, 15, 13});
		ASSERT_TRUE(v1 < v2);
		ASSERT_TRUE(!(v2 < v1));

		double v3 {12};
		ASSERT_TRUE(v1 < v3);
		ASSERT_TRUE(v3 < v2);
		ASSERT_TRUE(!(v3 < v1));
		ASSERT_TRUE(!(v2 < v3));

		int v5 {14};
		ASSERT_TRUE(v1 < v5);
		ASSERT_TRUE(v5 < v2);
		ASSERT_TRUE(!(v5 < v1));
		ASSERT_TRUE(!(v2 < v5));
	}
}
TYPED_TEST(ValueCompareMultiField, LessOrEqual) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12.2, 12.3, 12.0, 11.5, 12.7});
		TypeParam v2 ({22.2, 22.3, 12.5, 12.0, 12.8});
		TypeParam v3 ({22.2, 22.3, 12.5, 12.0, 12.9});
		TypeParam v6 ({22.2, 22.3, 22.5, 12.9, 15.9});
		ASSERT_TRUE(v2 <= v3);
		ASSERT_TRUE(v1 <= v2);
		ASSERT_TRUE(v1 <= v2);
		ASSERT_TRUE(!(v6 <= v1));
		ASSERT_TRUE(!(v6 <= v2));
		ASSERT_TRUE(!(v6 <= v3));

		double v4 {12.0};
		ASSERT_TRUE(v1 <= v4);
		ASSERT_TRUE(v4 <= v2);
		ASSERT_TRUE(v2 <= v4);
		ASSERT_TRUE(!(v6 <= v4));

		int v5 {12};
		ASSERT_TRUE(v1 <= v5);
		ASSERT_TRUE(v5 <= v6);
		ASSERT_TRUE(v2 <= v5);
		ASSERT_TRUE(v5 <= v2);
		ASSERT_TRUE(!(v6 <= v5));
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12, 12, 12, 11, 12});
		TypeParam v2 ({22, 22, 12, 12, 12});
		TypeParam v3 ({22, 22, 12, 12, 12});
		TypeParam v6 ({22, 22, 22, 15, 15});
		ASSERT_TRUE(v2 <= v3);
		ASSERT_TRUE(v1 <= v2);
		ASSERT_TRUE(!(v6 <= v1));
		ASSERT_TRUE(!(v6 <= v2));
		ASSERT_TRUE(!(v6 <= v3));

		double v4 {12.0};
		ASSERT_TRUE(v1 <= v4);
		ASSERT_TRUE(v4 <= v2);
		ASSERT_TRUE(v2 <= v4);
		ASSERT_TRUE(!(v6 <= v4));

		int v5 {15};
		ASSERT_TRUE(v1 <= v5);
		ASSERT_TRUE(v5 <= v6);
		ASSERT_TRUE(v2 <= v5);
		ASSERT_TRUE((v6 <= v5));
	}
}
TYPED_TEST(ValueCompareMultiField, Greater) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12.2, 12.3, 12.0, 11.5, 12.7}); //all of v1 < v2
		TypeParam v2 ({22.2, 22.3, 12.5, 12.7, 12.8});
		ASSERT_TRUE(v2 > v1);
		ASSERT_TRUE(!(v1 > v2));

		double v3 {12.6};
		ASSERT_TRUE(v3 > v1);
		ASSERT_TRUE(v2 > v3);
		ASSERT_TRUE(!(v1 > v3));
		ASSERT_TRUE(!(v3 > v2));

		int v5 {12};
		ASSERT_TRUE(v5 > v1);
		ASSERT_TRUE(v2 > v5);
		ASSERT_TRUE(!(v1 > v5));
		ASSERT_TRUE(!(v5 > v2));
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12, 12, 12, 11, 12}); //all of v1 < v2
		TypeParam v2 ({22, 22, 13, 13, 13});
		ASSERT_TRUE(v2 > v1);
		ASSERT_TRUE(!(v1 > v2));

		double v3 {12.6};
		ASSERT_TRUE(v3 > v1);
		ASSERT_TRUE(v2 > v3);
		ASSERT_TRUE(!(v1 > v3));
		ASSERT_TRUE(!(v3 > v2));

		int v5 {12};
		ASSERT_TRUE(v5 > v1);
		ASSERT_TRUE(v2 > v5);
		ASSERT_TRUE(!(v1 > v5));
		ASSERT_TRUE(!(v5 > v2));
	}
}
TYPED_TEST(ValueCompareMultiField, GreaterOrEqual) {
	if constexpr (std::is_floating_point_v<typename TypeParam::value_type>) {
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
	} else if constexpr (std::is_integral_v<typename TypeParam::value_type>) {
		TypeParam v1 ({12, 12, 12, 11, 12});
		TypeParam v2 ({22, 22, 12, 12, 12});
		TypeParam v3 ({22, 22, 12, 12, 12});
		TypeParam v6 ({22, 22, 22, 13, 15});
		ASSERT_TRUE(v3 >= v2);
		ASSERT_TRUE(v2 >= v1);
		ASSERT_TRUE(!(v1 >= v6));
		ASSERT_TRUE(!(v2 >= v6));
		ASSERT_TRUE(!(v3 >= v6));

		double v4 {12.5};
		ASSERT_TRUE(v4 >= v1);
		ASSERT_TRUE(v4 >= v3);
		ASSERT_TRUE(v6 >= v4);
		ASSERT_TRUE(v4 >= v2);
		ASSERT_TRUE(!(v4 >= v6));

		int v5 {12};
		ASSERT_TRUE(v5 >= v1);
		ASSERT_TRUE(v5 >= v2);
		ASSERT_TRUE(v2 >= v5);
		ASSERT_TRUE(v6 >= v5);
		ASSERT_TRUE(!(v5 >= v6));
	}
}

