//
// Created by Andrey Solovyev on 24/01/2023.
//


#include <gtest/gtest.h>
#include "time_series/value.h"
#include "financial_data_structures/single_quote.h"
#include "financial_data_structures/ohlcv.h"
#include "financial_data_structures/bid_ask.h"
#include "time_series/element.h"

using test_types_comparison_singletons = testing::Types<
		base::Value<base::Seconds>,
		financial::SingleQuote<base::Seconds>
>;
template<typename T>
class CompareSingletons : public testing::Test {};
TYPED_TEST_SUITE(CompareSingletons, test_types_comparison_singletons);



TYPED_TEST(CompareSingletons, Equal) {
	TypeParam v1(-1.5), v2(2.0);
	double v3 {2.0};
	double v4 {-1.5};
	int v5 {2};
	ASSERT_TRUE(v1 == v1);
	ASSERT_TRUE(v2 == v2);
	ASSERT_TRUE(v1 == v4);
	ASSERT_TRUE(v4 == v1);
	ASSERT_TRUE(v2 == v3);
	ASSERT_TRUE(v3 == v2);
	ASSERT_TRUE(v2 == v5);
	ASSERT_TRUE(v5 == v2);
}
TYPED_TEST(CompareSingletons, NotEqual) {
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
}
TYPED_TEST(CompareSingletons, Less) {
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
}
TYPED_TEST(CompareSingletons, LessOrEqual) {
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
}
TYPED_TEST(CompareSingletons, Greater) {
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
}
TYPED_TEST(CompareSingletons, GreaterOrEqual) {
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
}


using test_types_comparison_multifield = testing::Types<
		financial::OHLCV<base::Seconds>,
		financial::BidAsk<base::Seconds>
>;
template<typename T>
class CompareMultiField : public testing::Test {};
TYPED_TEST_SUITE(CompareMultiField, test_types_comparison_multifield);

/**
 * @details
 * for OHLCV default field to compare is Close
 * for BidAsk default field to compare is Price
 * it is a pain in the ass to call a template param using GTest,
 * so v3-v5 values are chosen to pass/fail a test.
 * */

TYPED_TEST(CompareMultiField, Equal) {
	TypeParam v1 ({12.2, 12.3, 12.0, 12.5, 12.7});
	TypeParam v2 ({22.2, 22.3, -0.4, 12.5, 12.7});
	ASSERT_TRUE(v1 == v1);
	ASSERT_TRUE(v2 == v2);

	double v3 {12.5};
	//Both Price and Close are equal to v3
	ASSERT_TRUE(v1 == v3);
	ASSERT_TRUE(v2 == v3);
	ASSERT_TRUE(v3 == v1);
	ASSERT_TRUE(v3 == v2);

	int v5 {12};
	TypeParam v6 ({22.2, 22.3, -0.4, 12.0, 12.7});
	//Both Price and Close are equal to v5
	ASSERT_TRUE(v5 == v6);
	ASSERT_TRUE(v6 == v5);
}
TYPED_TEST(CompareMultiField, NotEqual) {
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
}
TYPED_TEST(CompareMultiField, Less) {
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
TYPED_TEST(CompareMultiField, LessOrEqual) {
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
}
TYPED_TEST(CompareMultiField, Greater) {
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
}
TYPED_TEST(CompareMultiField, GreaterOrEqual) {
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

