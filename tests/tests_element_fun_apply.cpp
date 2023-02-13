//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"
#include "boost/type_index.hpp"

template <typename Type, typename U>
Type initWithValue (const U init_value) {
	using T = typename Type::elem_type;
	T t;
	if constexpr (std::is_arithmetic_v<T>) {
		t = init_value;
	}
	else if constexpr (
			std::disjunction_v<
					std::is_same<T,financial::OHLCV<base::traits::ValueTypeDefault>>,
					std::is_same<T,financial::OHLCV<std::int64_t>>
			>)
	{
		t.open = init_value;
		t.high = init_value;
		t.low = init_value;
		t.close = init_value;
		t.volume = init_value;
	}
	else if constexpr (
			std::disjunction_v<
					std::is_same<T,financial::BidAsk<base::traits::ValueTypeDefault>>,
					std::is_same<T,financial::BidAsk<std::int64_t>>
			>)
	{
		t.bid = init_value;
		t.ask = init_value;
		t.middle = init_value;
		t.price = init_value;
		t.volume = init_value;
	}
	else if constexpr (
			std::disjunction_v<
					std::is_same<T,financial::SingleQuote<base::traits::ValueTypeDefault>>,
					std::is_same<T,financial::SingleQuote<std::int64_t>>
			>)
	{
		t.value = init_value;
	}
	else {
		throw std::runtime_error("unexpected TypeParam: " + std::string(typeid(T).name()));
	}
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


TYPED_TEST (ElementFnApplication, ApplyFunctionInPlace) {
	const int init_value {10};
	TypeParam t1 = initWithValue<TypeParam>(init_value);
	const int value = rand()%100;
	t1.applyFunction(modifyValue<decltype(t1.value), const int>, value);
	ASSERT_EQ(t1, (value + init_value));
}

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceChaining) {
}

TYPED_TEST (ElementFnApplication, ApplyFunctionCreateNew) {
	namespace ti = boost::typeindex;

	const int init_value {10};
	TypeParam t1 = initWithValue<TypeParam>(init_value);
	const int value = rand()%100;

	std::cout << ti::type_id_with_cvr<decltype(t1.value)>().pretty_name() << '\n';
	std::cout << ti::type_id_with_cvr<decltype(addValue<decltype(t1.value), const int>)>().pretty_name() << '\n';

	TypeParam t2 (t1.applyFunction(addValue<decltype(t1.value), const int>, value));
	ASSERT_EQ(t1, init_value);
	ASSERT_EQ(t2, (value + init_value));
}

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceFailConst) {
}
TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceFailNonInvokable) {
}
TYPED_TEST (ElementFnApplication, ApplyFunctionCreateNewFailNonInvokable) {
}
