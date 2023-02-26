//
// Created by Andrey Solovyev on 24/01/2023.
//

//#define NESTED_OPERATORS
//#define OPERATORS_EXISTANCE
//#define CPP_STANDARD
//#define CRTP_CORE
//#define LVALUE_RVALUE_RTTI
//#define APPLY_FUNC
//#define CONSTRUCTIBLE
//#define COMPILE_TIME_TEST_EXAMPLE
//#define SERIE_FACTORY_OPTIONS

#ifdef NESTED_OPERATORS
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

#ifdef OPERATORS_EXISTANCE
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

#ifdef CPP_STANDARD
#include <iostream>
#include "utils/cpp_version.h"

int main () {
	std::cout << (int)cpp_standard::get_version<cpp_standard::version>() << '\n';
	std::cout << cpp_standard::get_version<std::string_view>() << '\n';
	cpp_standard::get_version<double>();
}
#endif

#ifdef CRTP_CORE
#include <iostream>
#include <type_traits>

template <typename Derived>
struct Base {
		using type = Derived;
		virtual ~Base () {}
	};


template <typename T>
struct D : Base <D<T>> {
	T value;
};

struct S {};

int main () {
	D<S> d1;
	D<S>::Base::type d2;

	std::cout << typeid(decltype(d1)).name() << '\n';
	std::cout << typeid(decltype(d2)).name() << '\n';
}
#endif

#ifdef LVALUE_RVALUE_RTTI
#include <cassert>
#include <utility>
#include <functional>
#include <iostream>
#include "utils/arg_types_traits.h"
#include "boost/type_index.hpp"

template <typename T, typename U>
void lvalue_func (T& t, U b) { t += b; }
template <typename T, typename U>
T const_lvalue_func (const T& t, U b) { T res = t + b;return std::move(res); }

struct S {int i {42};};

int main () {
	std::cout << std::boolalpha;
	namespace ti = boost::typeindex;
	using namespace arg_traits;

	S s;
	const S& s1 = s;
	S& s2 = s;
	S&& s3 = S{.i = 12};
	const S s4;
	[[maybe_unused]] const S* s5 = &s4;
	[[maybe_unused]] const S* const s6 = &s4;
	[[maybe_unused]] S* const s7 = &s;
	[[maybe_unused]] S* s8 = &s;

	std:: cout
	<< "object: " << isObjectMutable(s) << ' ' << isObjectMutable(s1)<< ' ' << isObjectMutable(s2)<< ' ' << isObjectMutable(s3)<< ' ' << isObjectMutable(s4) << '\n'
	<< "const object: " << isObjectConst(s4) << ' ' << isObjectConst(s) << ' ' << isObjectConst(s1) << ' ' << isObjectConst(s2)  << ' ' << isObjectConst(s3) << '\n'
	<< "const lref: " << isLValueRefConst(s1) << ' ' << isLValueRefConst(s)  << ' ' << isLValueRefConst(s2) << ' ' << isLValueRefConst(s3) << ' ' << isLValueRefConst(s4) << '\n'
	<< "lref: " << isLValueRefMutable(s2) << ' ' << isLValueRefMutable(s) << ' ' << isLValueRefMutable(s1) << ' ' << isLValueRefMutable(s3) << ' ' << isLValueRefMutable(s4) << '\n'
	<< "rref: " << isRValueRef(s3) << ' ' << isRValueRef(s) << ' ' << isRValueRef(s1) << ' ' << isRValueRef(s2) << ' ' << isRValueRef(s4) << '\n';

#if 0
	std::cout << ti::type_id_with_cvr<decltype(s.i)>().pretty_name() << '\n';
	typename std::add_lvalue_reference_t<std::decay_t<decltype(s.i)>> lref (s.i);
	std::cout << ti::type_id_with_cvr<decltype(lref)>().pretty_name() << '\n';
	typename std::add_lvalue_reference_t<std::add_const_t<std::decay_t<decltype(s.i)>>> clref (s.i);
	std::cout << ti::type_id_with_cvr<decltype(clref)>().pretty_name() << '\n';

	std::cout
			<< func_modifies_elem_in_place<decltype(lvalue_func<int&, int>), int&, int> << ' '
			<< func_modifies_elem_in_place<decltype(lvalue_func<decltype(lref), int>), decltype(lref), int> << ' '
			<< func_modifies_elem_in_place<decltype(lvalue_func<int, int>), int, int> << ' '
			<< func_modifies_elem_in_place<decltype(const_lvalue_func<int, int>), int, int> << ' '
//			<< func_modifies_elem_in_place<decltype(const_lvalue_func<decltype(lref), int>), decltype(lref), int> << ' '
			<< '\n'
			<< func_creates_new_elem<decltype(const_lvalue_func<const int&, int>), const int&, int> << ' '
			<< func_creates_new_elem<decltype(const_lvalue_func<decltype(clref), int>), decltype(clref), int> << ' '
			<< func_creates_new_elem<decltype(const_lvalue_func<int, int>), int, int> << ' '
			<< func_creates_new_elem<decltype(lvalue_func<int&, int>), int&, int> << ' '
			<< '\n';

	s.i = 2;
	lvalue_func(lref, 2);
	std::cout << lref << '\n';

	int k = const_lvalue_func(clref, 2);
	std::cout << clref << ' ' << k << '\n';
#endif

	return 0;
}
#endif

#ifdef APPLY_FUNC
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include "boost/type_index.hpp"

template <typename T>
struct S {
	T value {1};
	S () = default;
	S (T t) : value (t) {}

	template <typename Fn, typename... Args>
	decltype(auto) apply (Fn&& fn, Args&& ...args) {
		if constexpr (std::is_same_v<std::invoke_result_t<Fn, T&, Args...>, void>) {
			std::invoke(std::forward<Fn>(fn), value, std::forward<Args...>(args...));
			return *this;
		}
		else if constexpr (std::is_same_v<std::invoke_result_t<Fn, T, Args...>, T>) {
			return S(std::invoke(std::forward<Fn>(fn), value, std::forward<Args...>(args...)));
		} else {
			static_assert(sizeof(Fn) == 0, "Unexpected callable while applying over element");
		}
	}
};


template <typename T>
void func_ret_void (T& t, int param) { t += param; }

template <typename T>
T func_ret_value (const T& t, int param) {T res{}; res = t + param; return res;}


int main (){
	namespace ti = boost::typeindex;
	std::cout << ti::type_id_with_cvr<decltype(func_ret_void<int>)>().pretty_name() << '\n';
	std::cout << ti::type_id_with_cvr<decltype(func_ret_value<int>)>().pretty_name() << '\n';

	S<int> s1;
	s1.apply<decltype(func_ret_void<int>), int>(func_ret_void, 1);
	assert(s1.value==2);

	S<int> s2 = s1.apply<decltype(func_ret_value<int>), int>(func_ret_value, 1);
	assert(s1.value == 2);
	assert(s2.value == 3);
}
#endif

#ifdef CONSTRUCTIBLE
#include "time_series/types_requirements/container.h"
#include <iostream>
#include <cassert>
#include <cstddef>
#include <vector>
#include <map>
#include <unordered_map>

struct S1 {int value {42};};
struct SHasher {
	std::size_t operator () (const S1& s) const {
		return s.value;
	}
};
bool operator == (const S1& lhs, const S1& rhs) {
	return lhs.value == rhs.value;
}
bool operator < (const S1& lhs, const S1& rhs) {
	return lhs.value < rhs.value;
}
struct SComparator {
	bool operator () (const S1& lhs, const S1& rhs) const {
		return lhs.value < rhs.value;
	}
};

int main () {
	std::cout << std::boolalpha;

	std::cout << "vector is constructible: "
			  << std::is_constructible_v<std::vector<S1>> << '\n';
	std::cout << "vector is constructible: "
			  << std::is_constructible_v<std::vector<int>> << '\n';

	std::cout << "unordered_map is constructible: "
	<< std::is_constructible_v<std::unordered_map<S1, int, SHasher>> << '\n';

	std::cout << requirements::ContainerArgsCount<std::vector, S1>() << '\n';

}
#endif


#ifdef COMPILE_TIME_TEST_EXAMPLE

#include <iostream>
#include <string>
#include "time_series/types_requirements/compile_time_test.h"


struct SomeStruct {int value {42};};

int func();
template <typename T> void func(T);
void func(double);
void func(double, double );
void func(double, double, double);

// create the structs from above macros for the function `func`
COMPILE_TIME_TEST(func);


void f (int) {}
int f (int, int) {return 42;}
int g (double) {return 42;}

COMPILE_TIME_TEST(f);
COMPILE_TIME_TEST(g);


void call_vec (std::vector<int>&) {}
COMPILE_TIME_TEST(call_vec);


int main () {

	using namespace std::string_literals;

	static_assert(!func_compiles<void>);
	static_assert(func_compiles<int>);
	static_assert(func_compiles_from_type<void, double, double>);
	static_assert(!func_compiles_from_type<void, double, double, double, double>);

	static_assert(func_compiles<void, 1>);
	static_assert(!func_compiles<void, 1, nullptr>);

	static_assert(f_compiles<void, 1>);
	static_assert(!f_compiles<void, 1, nullptr>);


	[[maybe_unused]] SomeStruct s;
	std::cout << std::boolalpha;
	std::cout << Compilable<decltype(g), int, double>() << '\n';
//	std::cout << CompileTimeTest(f, 12) << '\n';
//	std::cout << CompileTimeTest(f, 12, 12) << '\n';
//	std::cout << CompileTimeTest(f, 12, 12, 12) << '\n';
//	std::cout << CompileTimeTest(f, 12, "fail"s, 12) << '\n';
//	std::cout << CompileTimeTest<int>(g, 12.2) << '\n';
//	std::cout << CompileTimeTest(&func, 12.2, 12.2, 12.2) << '\n';

	std::cout << std::is_invocable_r_v<int, decltype(g), decltype(12.2)> << '\n';

}

#endif

#ifdef SERIE_FACTORY_OPTIONS

#include "time_series/element.hpp"

#include <iostream>
#include <type_traits>
#include <cstddef>

#include <map>
#include <vector>
#include <unordered_map>


#include "boost/type_index.hpp"

using namespace time_series;
using namespace boost::typeindex;
using namespace requirements;

#if 1

template<typename Container, typename = void>
struct MaybeContainer : std::false_type {};

template<typename Container>
struct MaybeContainer<
		Container,
		std::void_t<
				decltype(std::declval<Container>().begin()),
				decltype(std::declval<Container>().end())
		>
> : std::true_type {};

template<typename Container>
inline constexpr bool is_container_v { MaybeContainer<Container>::value };

//template<typename Container>
//using IsContainer = std::enable_if_t<isContainer_v<Container>, bool>;

#endif

#if 1
template <template <typename...> typename Container, typename Arg1, typename = void>
struct ContainerHasSingleArg : std::false_type {};
template <template <typename...> typename Container, typename Arg1>
struct ContainerHasSingleArg <Container,
		Arg1, std::void_t<std::is_constructible<Container<Arg1>>>
> : std::true_type {};
template <template <typename...> typename Container, typename Arg1>
constexpr bool ContainerHasSingleArg_v () {return ContainerHasSingleArg<Container, Arg1>::value;}

template <template <typename...> typename Container, typename Arg1, typename Arg2, typename = void>
struct ContainerHasTwoArgs : std::false_type {};
template <template <typename...> typename Container, typename Arg1, typename Arg2>
struct ContainerHasTwoArgs <Container,
							Arg1, Arg2,
							std::void_t<std::is_constructible<Container<Arg1, Arg2>>>
> : std::true_type {};
template <template <typename...> typename Container, typename Arg1, typename Arg2>
constexpr bool ContainerHasTwoArgs_v () {return ContainerHasTwoArgs<Container, Arg1, Arg2>::value;}



template<typename C>
struct TypeHolder {
	using container_type = C;
};

template<
		typename Duration,
		typename ElemType,
		template<typename...> typename Container,
		typename... Args
>
static
constexpr auto getContainer()
{
	using namespace requirements;
	using Elem = Element<Duration, ElemType>;
	using Timestamp = base::Timestamp<Duration>;

	if constexpr (ContainerHasSingleArg_v<Container, Elem>()) {
		if constexpr (is_container_v<Container<Elem, Args...>>) {
			return TypeHolder<Container<Elem, Args...>>{};
		}
	}
	else if constexpr (ContainerHasTwoArgs_v<Container, Timestamp, ElemType>()) {
		if constexpr (is_container_v<Container<Timestamp, ElemType, Args...>>) {
			return TypeHolder<Container<Timestamp, ElemType, Args...>>{};
		}
	}
	else {
		throw std::invalid_argument("Unexpected parameters");
	}
}
#endif

#if 0
namespace belts {
// Inspired by
// * https://www.youtube.com/watch?v=U3jGdnRL3KI
// * http://cppconf.ru/talks/ivan-cukic?lang=ru

  struct nonesuch{};

  template <
		  typename Def,
		  typename Void,
		  template<typename...> typename Op,
		  typename... Args
  >
  struct DETECTOR {
	  using value_t = std::false_type;
	  using type = Def;
  };

  template <
		  typename Def,
		  template<typename...> typename Op,
		  typename... Args
  >
  struct DETECTOR<Def, std::void_t<Op<Args...>>, Op, Args...> {
	  using value_t = std::true_type;
	  using type = Op<Args...>;
  };

  template <template<typename...> typename Op, typename... Args>
  using is_detected = typename DETECTOR<nonesuch, void, Op, Args...>::value_t;

  template <template<typename...> typename Op, typename... Args>
  [[maybe_unused]]
  constexpr bool is_detected_v = is_detected<Op, Args...>::value;

  template <template<typename...> typename Op, typename... Args>
  using detected_t = typename DETECTOR<nonesuch, void, Op, Args...>::type;

// https://stackoverflow.com/a/39818497
	template<typename T, typename = void>
  [[maybe_unused]]
  constexpr bool is_defined = false;

  template<typename T>
  [[maybe_unused]]
  constexpr bool is_defined<T, decltype(sizeof(T), void())> = true;
}
#endif

template <typename T>
struct S11 {T value;};

int main(){
	[[maybe_unused]] S11<int> s11;
	std::cout << std::boolalpha;


	base::Timestamp<base::Nanoseconds> ts;
	std::cerr << ts.time_point.time_since_epoch().count() << '\n';


	auto v = getContainer<base::Nanoseconds, int, std::vector>();
	std::cerr << type_id_with_cvr<typename decltype(v)::container_type>().pretty_name() << '\n';
	auto m = getContainer<base::Nanoseconds, int, std::map>();
	std::cerr << type_id_with_cvr<typename decltype(m)::container_type>().pretty_name() << '\n';


	using H = typename base::TimestampHasher<base::Nanoseconds>;
	std::cerr << std::is_copy_constructible<H>::value << '\n';
	std::cerr << std::is_move_constructible<H>::value << '\n';
	std::cerr << std::is_invocable_r<std::size_t, H, base::Timestamp<base::Nanoseconds> const&>::value << '\n';
	std::cerr << std::__check_hash_requirements<base::Timestamp<base::Nanoseconds>, H>::value << '\n';
	auto h = getContainer<base::Nanoseconds, int, std::unordered_map, H>();
	std::cerr << type_id_with_cvr<typename decltype(h)::container_type>().pretty_name() << '\n';
}
#endif

#if 1
#include <iostream>

int main () {

}

#endif