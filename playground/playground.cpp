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


#if 0
#include "financial_data_structures/ohlcv.h"
#include "financial_data_structures/bid_ask.h"
#include "time_series/element.h"
#include "time_series/value.h"

#include <iostream>


template <typename T>
T sq (T a) {
	return a * a;
}


/// Create compile-time tests that allow checking a specific function's type
#define COMPILE_TIME_TEST(func) COMPILE_TIME_TEST_FUNCTION(func, func)
#define COMPILE_TIME_TEST_FUNCTION(name, func)                                                              \
namespace detail {                                                                                          \
  template<typename R, auto... args> struct name ## FromArgs:std::false_type{};                             \
  template<auto... args> struct name ## FromArgs<decltype(func(args...)), args...> : std::true_type{};      \
  template<typename R, typename... Args> struct name ## FromType:std::false_type{};                         \
  template<typename... Args> struct name ## FromType<decltype(func(std::declval<Args>()...)), Args...> : std::true_type{};\
}                                                                                                           \
template<typename R, auto ...Args>                                                                          \
static constexpr auto name ## _compiles = detail::name ## FromArgs<R, Args...>::value;                      \
template<typename ...Args> \
static constexpr auto name ## _compiles_from_type = detail::name ## FromType<Args...>::value;\


int func();
template <typename T> void func(T);
void func(double);
void func(double, double );
void func(double, double, double);

// create the structs from above macros for the function `func`
COMPILE_TIME_TEST(func);


template <typename T, typename U>
auto sum (T a, U b) {return a + b;}

int main () {

	static_assert(!func_compiles<void>);
	static_assert(func_compiles<int>);
	static_assert(func_compiles_from_type<void, double, double>);
	static_assert(!func_compiles_from_type<void, double, double, double, double>);

	static_assert(func_compiles<void, 1>);
	static_assert(!func_compiles<void, 1, nullptr>);



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

	time_series::Element<base::Seconds, financial::BidAsk<base::Seconds>> element (b);
	s = element.toString();
	std::cout << s << '\n';
	std::cout << element << '\n';

	time_series::Element<base::Seconds, double> element_double (42.12);
	std::cout << element_double << '\n';

	time_series::Element<base::Seconds, int> element_int (42);
	std::cout << element_int << '\n';
}

#endif


#if 0
#include <iostream>
#include "time_series/type_requirements.h"
#include "time_series/value.h"

template <typename L, typename R, requirements::BinOperatorsExist<L, R> = true>
constexpr bool opsExists () {return true;}

int main () {
	using namespace std;
	cout << boolalpha;
	cout << opsExists<char, int>() << '\n';

	base::Value<base::Seconds> value (42.2);
	bool b = value < 12.2;
	b = 12.2 < value;
	b = value == 42.2;
	b = 42.2 == value;

	cout << opsExists<decltype(value), int>() << '\n';
	cout << opsExists<int, decltype(value)>() << '\n';
	cout << opsExists<decltype(value), double>() << '\n';
	cout << opsExists<double, decltype(value)>() << '\n';

	base::Value<base::Seconds> t;
	cout << t << '\n';
	t = value - 12.2;
	t = 52.2 - t;
	cout << t << '\n';

	cout << (t < 25.5) << '\n';
	cout << (25.5 < t) << '\n';

	auto r = value + t;
	cout << r << '\n';


	base::Value<base::Seconds, int> i1 (-1.5);
	base::Value<base::Seconds, int> i2 (2.5);
	cout << i1 + i2 << '\n';
	cout << i2 - 1.5 << '\n';
}

#endif

#if 0

#include <iostream>
#include "cpp_version.h"

int main () {
	std::cout << (int)cpp_standard::get_version<cpp_standard::version>() << '\n';
	std::cout << cpp_standard::get_version<std::string_view>() << '\n';
	cpp_standard::get_version<double>();
}

#endif