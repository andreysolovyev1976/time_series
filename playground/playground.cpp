//
// Created by Andrey Solovyev on 24/01/2023.
//


#if 0
#include <iostream>
#include <iomanip>
#include <type_traits>



struct S{
	int value {42};
};

bool operator == (const S &lhs, const S &rhs) {
	std::cout << std::string(__PRETTY_FUNCTION__ ) << '\n';
	return lhs.value == rhs.value;
}
bool operator == (const S &lhs, int rhs) {
	std::cout << std::string(__PRETTY_FUNCTION__ ) << '\n';
	return lhs.value == rhs;
}

struct D{
	int value {42};
};
bool operator == (const D &lhs, const D &rhs) {
	std::cout << std::string(__PRETTY_FUNCTION__ ) << '\n';
	return lhs.value == rhs.value;
}
bool operator == (const D &lhs, int rhs) {
	std::cout << std::string(__PRETTY_FUNCTION__ ) << '\n';
	return lhs.value == rhs;
}


template <typename T>
using IsOk = std::enable_if_t<
        std::disjunction_v<
                std::is_same<T, S>,
				std::is_same<T, D>
                >
        , bool>;

template <typename Value, IsOk<Value> = true>
struct Compound {
	Value value;
};

bool operator == (const S &lhs, const D &rhs) {
	std::cout << std::string(__PRETTY_FUNCTION__ ) << '\n';
	return lhs.value == rhs.value;
}


template <typename Value, IsOk<Value> = true>
bool operator == (const Compound<Value> &lhs, const Compound<Value> &rhs){
	std::cout << std::string(__PRETTY_FUNCTION__ ) << '\n';
	return lhs.value == rhs.value;
}

template <typename Value1, typename Value2, IsOk<Value1> = true, IsOk<Value2> = true>
bool operator == (const Compound<Value1> &lhs, const Compound<Value2> &rhs){
	std::cout << std::string(__PRETTY_FUNCTION__ ) << '\n';
	return lhs.value == rhs.value;
}


int main () {
	std::cout << std::boolalpha;
	Compound<S> fst;
	fst.value.value = 12;

	Compound<D> snd;
	snd.value.value = 12;

	std::cout << "First call: \n";
	std::cout << (fst == snd);

	std::cout << "\nSecond call: \n";
	std::cout << (fst == fst);

	std::cout << "\nThird call: \n";
	std::cout << (fst.value == 42);

	std::cout << "\nFourth call: \n";
	std::cout << (42 == fst.value);

}

#endif

#include "financial_data_structures/ohlcv.h"
#include "financial_data_structures/bid_ask.h"
#include "time_series/element.h"
#include "time_series/value.h"

#include <iostream>

int main () {
	financial::OHLCV<base::Seconds> point;
	point.open = 1.0;
	point.high = 2.0;
	point.low = 3.0;
	point.close = 4.0;
	point.volume = 12.0;
	auto s = point.toString();
	std::cout << s << '\n';
	std::cout << point << '\n';


	financial::BidAsk<base::Seconds> b;
	b.bid = 1.0;
	b.ask = 2.0;
	b.middle = 3.0;
	b.price = 4.0;
	b.volume = 12.0;
	s = b.toString();
	std::cout << s << '\n';
	std::cout << b << '\n';

	time_series::Element<financial::BidAsk<base::Seconds>, base::Seconds> element (b);
	s = element.toString();
	std::cout << s << '\n';
	std::cout << element << '\n';

	time_series::Element<double, base::Seconds> element_double (42.12);
	std::cout << element_double << '\n';

	time_series::Element<int, base::Seconds> element_int (42);
	std::cout << element_int << '\n';

}