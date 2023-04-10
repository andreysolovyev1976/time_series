//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

using namespace std::string_literals;

template <typename T, typename U>
void modifyValue (T& t, U value) {
	t += value;
}

template <typename T, typename U>
T addValue (const T& t, U value) {
	T res = t;
	res += value;
	return res;
}

template <typename T, typename U>
void modifyValuesChaining (T& t, U value) {
	t += value;
}

template<typename T>
class ElementFnApplication : public testing::Test {};
TYPED_TEST_SUITE(ElementFnApplication, test_element);


static int const init_value {10};

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlace) {
	if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
		TypeParam t1 (init_value);
		const int value = rand()%100;
		t1.applyFunction(modifyValue<std::decay_t<decltype(t1.value)>, decltype(value)>, value);
		ASSERT_EQ(t1, (value + init_value));
	}else if constexpr (std::is_same_v<std::string, typename TypeParam::mapped_type>) {
		// do nothing
	} else {
		TypeParam t1 ("10"s);
		const int value = rand()%100;
		t1.applyFunction(modifyValue<std::decay_t<decltype(t1.value)>, decltype(value)>, value);
		ASSERT_EQ(t1, (value + init_value));
	}
}

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceChaining)
{
	if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
		TypeParam t1 (init_value);
		const int value = rand()%100;
		t1
				.applyFunction(modifyValuesChaining<std::decay_t<decltype(t1.value)>, decltype(value)>, value)
				.applyFunction(modifyValuesChaining<std::decay_t<decltype(t1.value)>, decltype(value)>, value);

		ASSERT_EQ(t1, (value*2 + init_value));
	}else if constexpr (std::is_same_v<std::string, typename TypeParam::mapped_type>) {
		// do nothing
	} else {
		TypeParam t1 ("10"s);
		const int value = rand()%100;
		t1
				.applyFunction(modifyValuesChaining<std::decay_t<decltype(t1.value)>, decltype(value)>, value)
				.applyFunction(modifyValuesChaining<std::decay_t<decltype(t1.value)>, decltype(value)>, value);

		ASSERT_EQ(t1, (value*2 + init_value));
	}
}

TYPED_TEST (ElementFnApplication, ApplyFunctionCreateNew) {
		if constexpr (std::is_arithmetic_v<typename TypeParam::mapped_type>) {
			TypeParam t1(init_value);
			const int value = rand()%100;

			TypeParam t2(t1.applyFunction(addValue<std::decay_t<decltype(t1.value)>, decltype(value)>, value));
			ASSERT_EQ(t1, init_value);
			ASSERT_EQ(t2, (value+init_value));
		}else if constexpr (std::is_same_v<std::string, typename TypeParam::mapped_type>) {
			// do nothing
		} else {
			TypeParam t1 ("10"s);
			const int value = rand()%100;

			TypeParam t2(t1.applyFunction(addValue<std::decay_t<decltype(t1.value)>, decltype(value)>, value));
			ASSERT_EQ(t1, init_value);
			ASSERT_EQ(t2, (value + init_value));
		}
	}


TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceFailConst) {
}
TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceFailNonInvokable) {
}
TYPED_TEST (ElementFnApplication, ApplyFunctionCreateNewFailNonInvokable) {
}
