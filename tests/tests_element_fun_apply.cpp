//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

template <typename Type, typename U>
Type initWithValue (const U init_value) {
	using T = typename Type::elem_type;
	T t = init_value;
	return Type(t);
}

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

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlace) {
	const int init_value {10};
	TypeParam t1 = initWithValue<TypeParam>(init_value);
	const int value = rand()%100;
	t1.applyFunction(modifyValue<std::decay_t<decltype(t1.value)>, decltype(value)>, value);
	ASSERT_EQ(t1, (value + init_value));
}

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceChaining) {
	const int init_value {10};
	TypeParam t1 = initWithValue<TypeParam>(init_value);
	const int value = rand()%100;
	t1
	.applyFunction(modifyValuesChaining<std::decay_t<decltype(t1.value)>, decltype(value)>, value)
	.applyFunction(modifyValuesChaining<std::decay_t<decltype(t1.value)>, decltype(value)>, value);

	ASSERT_EQ(t1, (value * 2 + init_value));
}

TYPED_TEST (ElementFnApplication, ApplyFunctionCreateNew) {
	const int init_value {10};
	TypeParam t1 = initWithValue<TypeParam>(init_value);
	const int value = rand()%100;

	TypeParam t2 (t1.applyFunction(addValue<std::decay_t<decltype(t1.value)>, decltype(value)>, value));
	ASSERT_EQ(t1, init_value);
	ASSERT_EQ(t2, (value + init_value));
}

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceFailConst) {
}
TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceFailNonInvokable) {
}
TYPED_TEST (ElementFnApplication, ApplyFunctionCreateNewFailNonInvokable) {
}
