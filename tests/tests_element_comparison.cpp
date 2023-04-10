//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"
#include <thread>
#include <chrono>

//#define WRONG_ELEMENT_COMPARE_COMPILE_FAILURE

using namespace std::chrono_literals;

template<typename T>
class ElementComparison : public testing::Test {};
TYPED_TEST_SUITE(ElementComparison, test_element);

using duration_t = culib::time::Microseconds;
using ts_t = typename culib::time::Timestamp<duration_t>;
ts_t first;

TYPED_TEST (ElementComparison, EQ) {
	TypeParam t1 {};
	TypeParam t2 = t1;
	ASSERT_TRUE(t1 == t2);
	ASSERT_TRUE(t2 == t1);
}

TYPED_TEST (ElementComparison, NE_By_Timestamp) {
	TypeParam t1(first, {});
	std::this_thread::sleep_for(10us);
	TypeParam t2 {};
	ASSERT_TRUE(t1 != t2);
	ASSERT_TRUE(t2 != t1);
}

TYPED_TEST (ElementComparison, NE_By_Value) {
	if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
		TypeParam t1(first, 42);
		std::this_thread::sleep_for(10us);
		TypeParam t2(first, {});
		ASSERT_TRUE(t1 != t2);
		ASSERT_TRUE(t2 != t1);
	} else {
		TypeParam t3(first, {"42"});
		std::this_thread::sleep_for(10us);
		TypeParam t4(first, {});
		ASSERT_TRUE(t3 != t4);
		ASSERT_TRUE(t4 != t3);
	}
}

#if 0
TYPED_TEST (ElementComparison, LT_By_Timestamp) {
	TypeParam t1(first, {});
	std::this_thread::sleep_for(10us);
	TypeParam t2 {};
	ASSERT_TRUE(t1 < t2);
	ASSERT_FALSE(t2 < t1);
}

TYPED_TEST (ElementComparison, LT_By_Value) {
	if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
		TypeParam t1(first, 42);
		std::this_thread::sleep_for(10us);
		TypeParam t2(first, 0);
		ASSERT_TRUE(t2 < t1);
		ASSERT_FALSE(t1 < t2);
	} else {
		TypeParam t3(first, {"42"});
		std::this_thread::sleep_for(10us);
		TypeParam t4(first, {"0"});
		ASSERT_TRUE(t4 < t3);
		ASSERT_FALSE(t3 < t4);
	}
}

TYPED_TEST (ElementComparison, LE_By_Timestamp) {
	TypeParam t1(first, {});
	std::this_thread::sleep_for(10us);
	TypeParam t2 (first, {});
	ASSERT_TRUE(t1 <= t2);
	ASSERT_TRUE(t2 <= t1);
}

TYPED_TEST (ElementComparison, LE_By_Value) {
	if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
		TypeParam t1(first, 42);
		std::this_thread::sleep_for(10us);
		TypeParam t2(first, 0);
		TypeParam t3(first, 42);
		ASSERT_TRUE(t2 <= t1);
		ASSERT_TRUE(t3 <= t1);
	} else {
		TypeParam t4(first, {"42"});
		std::this_thread::sleep_for(10us);
		TypeParam t5(first, {"0"});
		TypeParam t6(first, {"42"});
		ASSERT_TRUE(t5 <= t4);
		ASSERT_TRUE(t6 <= t4);
	}
}



TYPED_TEST (ElementComparison, GT_By_Timestamp) {
	TypeParam t1(first, {});
	std::this_thread::sleep_for(10us);
	TypeParam t2 {};
	ASSERT_TRUE(t2 > t1);
	ASSERT_FALSE(t1 > t2);
}

TYPED_TEST (ElementComparison, GT_By_Value) {
	if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
		TypeParam t1(first, 42);
		std::this_thread::sleep_for(10us);
		TypeParam t2(first, 0);
		ASSERT_TRUE(t1 > t2);
		ASSERT_FALSE(t2 > t1);
	} else {
		TypeParam t3(first, {"42"});
		std::this_thread::sleep_for(10us);
		TypeParam t4(first, {"0"});
		ASSERT_TRUE(t3 > t4);
		ASSERT_FALSE(t4 > t3);
	}
}

TYPED_TEST (ElementComparison, GE_By_Timestamp) {
	TypeParam t1(first, {});
	std::this_thread::sleep_for(10us);
	TypeParam t2 (first, {});
	ASSERT_TRUE(t1 >= t2);
	ASSERT_TRUE(t2 >= t1);
}

TYPED_TEST (ElementComparison, GE_By_Value) {
	if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
		TypeParam t1(first, 42);
		std::this_thread::sleep_for(10us);
		TypeParam t2(first, 0);
		TypeParam t3(first, 42);
		ASSERT_TRUE(t1 >= t2);
		ASSERT_TRUE(t3 >= t1);
	} else {
		TypeParam t4(first, {"42"});
		std::this_thread::sleep_for(10us);
		TypeParam t5(first, {"0"});
		TypeParam t6(first, {"42"});
		ASSERT_TRUE(t4 >= t5);
		ASSERT_TRUE(t6 >= t4);
	}
}
#endif


TEST (ElementComparison, CompileTimeErrors) {
	struct UserDefinedDummyNoComparison {};
	time_series::Element<duration_t, UserDefinedDummyNoComparison> elem1;
	std::this_thread::sleep_for(10us);
	time_series::Element<duration_t, UserDefinedDummyNoComparison> elem2;

#ifdef WRONG_ELEMENT_COMPARE_COMPILE_FAILURE
	ASSERT_TRUE(elem1 < elem2); //fails to compile
#endif
}