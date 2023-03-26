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
//#define TUPLE_EXERCISES
//#define JOIN_BASICS
//#define JOIN_SQL_LIKE
//#define SET_OPERATIONS_BASICS
//#define MULTI_ARG_TEMPLATE_CHECK
//#define CPP20CHECK
//#define CONCEPTS_CIRCULAR_LINK_PROBLEM
#define INHERITANCE_SAME_NAMING_ISSUE

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

template<typename Object, typename = void>
struct MaybeContainer : std::false_type {};

template<typename Object>
struct MaybeContainer<
		Object,
		std::void_t<
				decltype(std::declval<Object>().begin()),
				decltype(std::declval<Object>().end())
		>
> : std::true_type {};

template<typename Object>
inline constexpr bool is_container_v { MaybeContainer<Object>::value };

//template<typename Object>
//using IsContainer = std::enable_if_t<isContainer_v<Object>, bool>;

#endif

#if 1
template <template <typename...> typename Object, typename Arg1, typename = void>
struct ContainerHasSingleArg : std::false_type {};
template <template <typename...> typename Object, typename Arg1>
struct ContainerHasSingleArg <Object,
		Arg1, std::void_t<std::is_constructible<Object<Arg1>>>
> : std::true_type {};
template <template <typename...> typename Object, typename Arg1>
constexpr bool ContainerHasSingleArg_v () {return ContainerHasSingleArg<Object, Arg1>::value;}

template <template <typename...> typename Object, typename Arg1, typename Arg2, typename = void>
struct ContainerHasTwoArgs : std::false_type {};
template <template <typename...> typename Object, typename Arg1, typename Arg2>
struct ContainerHasTwoArgs <Object,
							Arg1, Arg2,
							std::void_t<std::is_constructible<Object<Arg1, Arg2>>>
> : std::true_type {};
template <template <typename...> typename Object, typename Arg1, typename Arg2>
constexpr bool ContainerHasTwoArgs_v () {return ContainerHasTwoArgs<Object, Arg1, Arg2>::value;}



template<typename C>
struct TypeHolder {
	using container_type = C;
};

template<
		typename Duration,
		typename ElemType,
		template<typename...> typename Object,
		typename... Args
>
static
constexpr auto getContainer()
{
	using namespace requirements;
	using Elem = Element<Duration, ElemType>;
	using Timestamp = base::Timestamp<Duration>;

	if constexpr (ContainerHasSingleArg_v<Object, Elem>()) {
		if constexpr (is_container_v<Object<Elem, Args...>>) {
			return TypeHolder<Object<Elem, Args...>>{};
		}
	}
	else if constexpr (ContainerHasTwoArgs_v<Object, Timestamp, ElemType>()) {
		if constexpr (is_container_v<Object<Timestamp, ElemType, Args...>>) {
			return TypeHolder<Object<Timestamp, ElemType, Args...>>{};
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

#ifdef TUPLE_EXERCISES

#include "utils/itertools.hpp"
#include <iostream>
#include <tuple>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <functional>
#include "boost/type_index.hpp"

using namespace boost::typeindex;
using namespace itertools;


std::ostream& operator << (std::ostream& os, std::pair<int, std::string> const& p) {
	os << p.first << ' ' << p.second;
	return os;
}




template <typename... Args>
constexpr auto packToTuple(Args... args) {
	return std::make_tuple(std::forward<Args>(args)...);
}

template <typename... Tuples>
constexpr std::size_t getMinSize (const Tuples& ...) {
	return std::min({std::tuple_size_v<std::decay_t<Tuples>>...});
}

template <typename NewElem, typename... TupleElems>
std::tuple<TupleElems..., NewElem> tuplePushBack(std::tuple<TupleElems...> &&tup, NewElem &&el) {
	return std::tuple_cat(
			std::forward<std::tuple<TupleElems...>>(tup),
			std::make_tuple(std::forward<NewElem>(el)));
}

template <typename NewElem, typename... TupleElems>
std::tuple<TupleElems..., NewElem> tuplePushFront(std::tuple<TupleElems...> &&tup, NewElem &&el) {
	return std::tuple_cat(
			std::make_tuple(std::forward<NewElem>(el)),
			std::forward<std::tuple<TupleElems...>>(tup));
}

template <std::size_t I, typename... Tuples>
constexpr auto getSlice (Tuples&& ...tuples) {
	return std::make_tuple(std::get<I>(tuples)...);
}


template <std::size_t Index, typename Function, typename... Tuples>
constexpr void invokeAt([[maybe_unused]] Function&& func, Tuples&&... tuples) {
//	func((std::get<Index>(std::forward<Tuples>(tuples)), ...));
//	func(std::get<Index>(std::forward<Tuples>(tuples))...);
//	std::cout << std::get<Index>(std::forward<Tuples>(tuples))... << ' ';
	std::invoke(std::forward<Function>(func), (std::get<Index>(std::forward<Tuples>(tuples)),...) );
}

template <std::size_t... Indices, typename Function, typename... Tuples>
constexpr void applySequence(Function&& func,  std::index_sequence<Indices...>, Tuples&&... tuples) {
	(((void)invokeAt<Indices>(std::forward<Function>(func), std::forward<Tuples>(tuples)...), ...));
}


template <typename Function, typename... Tuples>
constexpr void tuplesForEach(Function&& func, Tuples&&... tuples) {
	static_assert(sizeof...(tuples) > 0, "Must be called with at least one tuple argument");
	constexpr auto min_length = getMinSize(std::forward<Tuples>(tuples)...);
	if constexpr (min_length != 0) {
		applySequence(std::forward<Function>(func),
				std::make_index_sequence<min_length>{},
				std::forward<Tuples>(tuples)...);
	}
	else {
//		func();
	std::cout << "doin' nothin'\n";
	}
}


template <typename ...Types>
bool compareEach (Types&&... pack1, Types&&... pack2) {
	return ((pack1,...) == (pack2,...) );
}
//template <typename ...Types>
//void printEach (Types&&... pack) {
//	((std::cout << std::forward<Types>(pack) << ' '), ...);
//	std::cout << '\n';
//}
template <typename ...Types>
void printEach (std::tuple<Types...> t) {
	std::cout << std::get<0>( std::forward<std::tuple<Types...>>(t) ) << ' ';
	std::cout << std::get<1>( std::forward<std::tuple<Types...>>(t) ) << ' ';
//	std::cout << std::get<2>( std::forward<std::tuple<Types...>>(t) ) << ' ';
	std::cout << '\n';
}
template <typename Type>
void printOne (Type&& pack) {
	std::cout << std::forward<Type>(pack) << '\n';
}
template <template <typename...> typename Fn>
struct CallableHolder {
	template <typename ...Args>
	decltype(auto) operator () (Fn<Args...>&& fn, Args&&... args) {
		return std::invoke(std::forward<Fn>(fn)(std::forward<Args>(args)...));
	}
};


template<typename... T1, typename... T2, std::size_t... I>
constexpr auto comp_impl(const std::tuple<T1...>& t1, const std::tuple<T2...>& t2, std::index_sequence<I...>) {
	bool result {false};
	((result = result || std::get<I>(t1) == std::get<I>(t2)) ,...);
	return result;
}

template<typename... T1, typename... T2>
constexpr auto comp(const std::tuple<T1...>& t1, const std::tuple<T2...>& t2) {
	static_assert(sizeof...(T1) == sizeof...(T2));
	return comp_impl(t1, t2, std::make_index_sequence<sizeof...(T1)>{});
}


template<typename Tuple2, typename... T1, std::size_t... I>
constexpr auto produceDereferencedImpl(const std::tuple<T1...>& t1, std::index_sequence<I...>) {
	return Tuple2({ ((*std::get<I>(t1)),...) });
}

template<typename Tuple2, typename... T1>
constexpr auto produceDereferenced(const std::tuple<T1...>& t1) {
	auto t2 = produceDereferencedImpl<Tuple2, T1...>(t1, std::make_index_sequence<sizeof...(T1)>{});
	return t2;
}

int main () {
	std::cout << std::boolalpha;
	std::cerr << std::boolalpha;

	[[maybe_unused]] std::tuple t1 = std::make_tuple (12, 3.5, 12);
	[[maybe_unused]] std::tuple t2 = std::make_tuple (12, 42.5, 11);
	[[maybe_unused]] std::tuple t3 = std::make_tuple (12, 3.5, 12);
	[[maybe_unused]] std::tuple t4 = std::make_tuple (12, 3.5);
	[[maybe_unused]] std::tuple t5 = std::make_tuple (15, 5.5, 12, 42);

//	printEach (std::make_tuple(12, 'c', 42.5));
//	std::cout << getMinSize(t3, t4, t1, t2, t5) << '\n';

//	auto t_res2 = getSlice<1>(t1, t2, t5, t3);
//	printEach (t_res2);
//	std::cout << std::tuple_size<decltype(t_res2)>{} << '\n';

//	std::cout << (comp(t1, t2)) << '\n';


	[[maybe_unused]] std::tuple t6 = std::make_tuple (15, 5.5, 12, 42);
	std::vector<int> v {1, 2, 3, 4, 5};
	std::map<int, std::string> m {{1, "one"}, {2, "two"}, {3, "three"}};

	[[maybe_unused]] std::tuple t_iter = std::make_tuple(v.begin(), m.begin());
	std::cout << "tuple is created with a size of " << std::tuple_size<decltype(t_iter)>{} << '\n';
	std::cout << *(std::get<0>(t_iter)) << ' ';
	std::cout << (*(std::get<1>(t_iter))).first << ' ' << (*(std::get<1>(t_iter))).second << '\n';

	std::tuple t_ref = std::make_tuple(*v.begin(), *m.begin());
	printEach(t_ref);

}

#endif

#ifdef JOIN_BASICS

#include "utils/itertools.hpp"
#include "utils/utils.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include <cstddef>
#include <algorithm>

#include "boost/type_index.hpp"

using namespace boost::typeindex;
using namespace std::string_literals;

std::ostream& operator << (std::ostream& os, std::pair<int, std::string> const& p) {
	os << p.first << ' ' << p.second;
	return os;
}

struct ValueHolder {
	int value;
	ValueHolder (int i) : value (i) {}
	ValueHolder (std::pair<const int, std::string> &p) : value (p.first) {}
};
bool operator < (ValueHolder const& lhs, ValueHolder const& rhs) { return lhs.value < rhs.value; }
bool operator == (ValueHolder const& lhs, ValueHolder const& rhs) { return lhs.value == rhs.value; }
bool operator < (ValueHolder const& lhs, std::pair<const int, std::string> const& rhs) { return lhs.value < rhs.first; }
bool operator == (ValueHolder const& lhs, std::pair<const int, std::string> const& rhs) { return lhs.value == rhs.first; }
bool operator < (std::pair<const int, std::string> const& lhs, ValueHolder const& rhs) { return lhs.first < rhs.value; }
bool operator == (std::pair<const int, std::string> const& lhs, ValueHolder const& rhs) { return lhs.first == rhs.value; }

std::ostream& operator << (std::ostream& os, std::vector<int> const& v) {
	os << "{ "s;
	for (auto const& elem: v) os << elem << ' ';
	os << '}';
	return os;
}

std::ostream& operator << (std::ostream& os, std::vector<ValueHolder> const& v) {
	os << "{ "s;
	for (auto const& elem: v) os << elem.value << ' ';
	os << '}';
	return os;
}


struct SetOperations {
	enum class Operation {
		None = 0,
		Intersection,
		Difference,
		SymmetricDifference,
		Union,
	};
	operator int () { return static_cast<int>(op); }

	Operation op;
};

#define IMPLEMENT_OPERATION_ON_SETS(c, ...) \
switch(c) \
{ case 1: std::set_intersection			(__VA_ARGS__); break; \
  case 2: std::set_difference			(__VA_ARGS__); break; \
  case 3: std::set_symmetric_difference	(__VA_ARGS__); break; \
  case 4: std::set_union				(__VA_ARGS__); break;  }


template <typename Serie>
bool isSortedAscending(Serie const& v) {
	return *v.begin() < *std::prev(v.end());
}

template <
		typename SerieRet
		, typename... SerieArgs
		, std::enable_if_t<sizeof...(SerieArgs)==2u, void>
>
void
implementSetOperation(
		SerieRet & result
		, SetOperations operation
		, std::tuple<SerieArgs...> &args) {
	//there is no guarantee resulting container is empty, therefore it is end(), not begin()
	auto ret_iter = std::inserter(result, result.end());

	bool const ascending1 { isSortedAscending(std::get<0>(args)) };
	bool const ascending2 { isSortedAscending(std::get<1>(args)) };
	using Serie1 = std::remove_reference_t<decltype(std::get<0>(args))>;
	using Serie2 = std::remove_reference_t<decltype(std::get<1>(args))>;

#define CALL_OPERATION(first_cond, second_cond) \
	auto [b1, e1] = itertools::getIterators<Serie1, first_cond>(std::get<0>(args)); \
	auto [b2, e2] = itertools::getIterators<Serie2, second_cond>(std::get<1>(args)); \
	IMPLEMENT_OPERATION_ON_SETS(operation, b1, e1, b2, e2, ret_iter);

	if 		(ascending1  && ascending2 ) { CALL_OPERATION(true,  true ) }
	else if (ascending1  && !ascending2) { CALL_OPERATION(true,  false) }
	else if (!ascending1 && ascending2 ) { CALL_OPERATION(false, true ) }
	else if (!ascending1 && !ascending2) { CALL_OPERATION(false, false) }

#undef CALL_OPERATION
	return result;
}

template <
        std::size_t I = 0
				, typename SerieRet
				, typename... SerieArgs
				>
typename std::enable_if_t<I == sizeof...(SerieArgs), void>
implementSetOperation(
		SerieRet&
		, SetOperations
		, std::tuple<SerieArgs...>&
		        )
				{ /* do nothing */ }


template <
        std::size_t I = 0
				, typename SerieRet
				, typename... SerieArgs
				>
typename std::enable_if_t <(I < sizeof...(SerieArgs)), void>
implementSetOperation(
		SerieRet & result
		, SetOperations operation
		, std::tuple<SerieArgs...> &args)
		{
	if constexpr (sizeof...(SerieArgs) <= 1) { return; }

	if constexpr (I == 0) {
		//there is no guarantee resulting container is empty, therefore it is end(), not begin()
		auto res_iter = std::inserter(result, result.end());
		auto const& first = std::get<I>(args);
		std::copy(first.begin(), first.end(), res_iter);

		implementSetOperation<I+1, SerieRet, SerieArgs...>(result, operation, args);
	}
	else if constexpr (I != sizeof...(SerieArgs)) {

#define CALL_OPERATION(first_cond, second_cond) \
    SerieRet result_next; \
	auto res_iter = std::inserter(result_next, result_next.begin()); \
	auto [b1, e1] = itertools::getIterators<SerieRet, first_cond>(result); \
	using Serie2 = std::tuple_element_t<I, std::tuple<SerieArgs...>>; \
	auto [b2, e2] = itertools::getIterators<Serie2, second_cond>(std::get<I>(args)); \
	IMPLEMENT_OPERATION_ON_SETS(operation, b1, e1, b2, e2, res_iter); \
	result = std::move(result_next);

		bool const ascending1 { isSortedAscending(result) };
		bool const ascending2 { isSortedAscending(std::get<I>(args)) };

		if 		(ascending1  && ascending2 ) { CALL_OPERATION(true,  true ) }
		else if (ascending1  && !ascending2) { CALL_OPERATION(true,  false) }
		else if (!ascending1 && ascending2 ) { CALL_OPERATION(false, true ) }
		else if (!ascending1 && !ascending2) { CALL_OPERATION(false, false) }

#undef CALL_OPERATION

		implementSetOperation<I+1, SerieRet, SerieArgs...>(result, operation, args);
	}
}

template <typename SerieRet, typename... SerieArgs>
void symmetric_difference (SerieRet & result, SerieArgs&& ...series) {
	SetOperations operation {SetOperations::Operation::SymmetricDifference};
	auto args = std::make_tuple(std::forward<SerieArgs>(series)...);

	if constexpr (sizeof...(SerieArgs) <= 1) {
		return;
	}
	else {
		implementSetOperation(result, operation, args);
	}
}

template <typename SerieRet, typename... SerieArgs>
void difference (SerieRet & result, SerieArgs&& ...series) {
	SetOperations operation {SetOperations::Operation::Difference};
	auto args = std::make_tuple(std::forward<SerieArgs>(series)...);

	if constexpr (sizeof...(SerieArgs) <= 1) {
		return;
	}
	else {
		implementSetOperation(result, operation, args);
	}
}

template <typename SerieRet, typename... SerieArgs>
void inner (SerieRet & result, SerieArgs&& ...series) {
	SetOperations operation {SetOperations::Operation::Intersection};
	auto args = std::make_tuple(std::forward<SerieArgs>(series)...);

	if constexpr (sizeof...(SerieArgs) <= 1) {
		return;
	}
	else {
		implementSetOperation(result, operation, args);
	}
}
template <typename SerieRet, typename... SerieArgs>
void union_ (SerieRet & result, SerieArgs&& ...series) {
	SetOperations operation {SetOperations::Operation::Union};
	auto args = std::make_tuple(std::forward<SerieArgs>(series)...);

	if constexpr (sizeof...(SerieArgs) <= 1) {
		return;
	}
	else {
		implementSetOperation(result, operation, args);
	}
}


int main () {

	std::vector<int> v1 {8, 5, 5, 2};
	std::vector<int> v2 {1, 2, 3, 4, 5, 5};
	std::vector<int> v3 {1, 2, 3, 4, 5, 5};

	std::vector<int> ret;
	auto ret_iter = std::inserter(ret, ret.end());
	std::copy(v1.begin(), v1.end(), ret_iter);
	std::cout << ret << '\n';


	std::cout << std::boolalpha;
	std::cerr << std::boolalpha;

	std::vector<int> result;
	std::set_symmetric_difference(v1.rbegin(), v1.rend(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
			);
	std::cout << "set_symmetric_difference: " << result << '\n';
	result.clear();
	symmetric_difference(result, v1, v2);
	std::cout << "set_symmetric_difference for Serie: "  << result << '\n';

	result.clear();
	std::set_symmetric_difference(v1.rbegin(), v1.rend(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
	);
	auto temp = result;
	result.clear();
	std::set_symmetric_difference(temp.begin(), temp.end(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
	);
	std::cout << "set_symmetric_difference two times: " << result << '\n';
	result.clear();
	symmetric_difference(result, v1, v2, v3);
	std::cout << "set_symmetric_difference for arbitrary Series qty: "  << result << '\n';


	result.clear();
	std::set_difference(v1.rbegin(), v1.rend(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
	);
	std::cout << "set_difference: "  << result << '\n';
	result.clear();
	difference(result, v1, v2);
	std::cout << "set_difference for Serie: "  << result << '\n';
	result.clear();
	difference(result, v1, v2, v3);
	std::cout << "set_difference for arbitrary Series qty: "  << result << '\n';

	result.clear();
	std::set_intersection(v1.rbegin(), v1.rend(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
	);
	std::cout << "set_intersection: "  << result << '\n';
	result.clear();
	inner(result, v1, v2);
	std::cout << "set_intersection for Serie: "  << result << '\n';
	result.clear();
	inner(result, v1, v2, v3);
	std::cout << "set_intersection for arbitrary Series qty: "  << result << '\n';

	result.clear();
	std::set_union(v1.rbegin(), v1.rend(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
	);
	std::cout << "set_union: "  << result << '\n';
	result.clear();
	union_(result, v1, v2);
	std::cout << "set_union for Serie: "  << result << '\n';
	result.clear();
	union_(result, v1, v2, v3);
	std::cout << "set_union for arbitrary Series qty: "  << result << '\n';

	std::map<int, std::string> m {
			{1, "one"s},
			{2, "two"s},
			{3, "three"s},
			{4, "four"s},
			{5, "five"s},
	};

	std::vector<ValueHolder> v_s {{8}, {5}, {5}, {2}};
	std::vector<ValueHolder> v_result;
	inner(v_result, v_s, m);
	std::cout << "inner join for map and vector: "  << v_result << '\n';

}
#endif


#ifdef JOIN_SQL_LIKE

#include "utils/utils.hpp"
#include "utils/joins.hpp"
#include "time_series/serie.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <tuple>

struct Key {
		int value {-1};
		operator int () {return value;}
	};
bool operator < (const Key& lhs, const Key& rhs) { return lhs.value < rhs.value; }
bool operator == (const Key& lhs, const Key& rhs) { return lhs.value == rhs.value; }
bool operator < (const Key& lhs, const int rhs) { return lhs.value < rhs; }
bool operator == (const Key& lhs, const int rhs) { return lhs.value == rhs; }
bool operator < (const int lhs, const Key& rhs) { return lhs < rhs.value; }
bool operator == (const int lhs, const Key& rhs) { return lhs == rhs.value; }
bool operator < (std::pair<const Key, int> const& lhs, std::pair<const Key, int> const& rhs) { return lhs.first.value < rhs.first.value; }
bool operator == (std::pair<const Key, int> const& lhs, std::pair<const Key, int> const& rhs) { return lhs.first.value == rhs.first.value; }

std::ostream& operator << (std::ostream& os, std::pair<Key const, int> const& p) {
	os << p.first.value << ' ' << p.second;
	return os;
}
std::ostream& operator << (std::ostream& os, std::vector<int> const& v) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& elem: v) os << elem << ' ';
	os << '}';
	return os;
}
std::ostream& operator << (std::ostream& os, std::vector<Key> const& v) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& elem: v) os << elem.value << ' ';
	os << '}';
	return os;
}
std::ostream& operator << (std::ostream& os, std::map<Key, int> const& m) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& [_, value]: m) os << value << ' ';
	os << '}';
	return os;
}


int main() {

	std::cout << std::boolalpha;

	std::map<Key, int> const m1 {
			{{1}, 3},
			{{2}, 2},
			{{3}, 3},
			{{4}, 17},
			{{5}, 25},
	};
	std::map<Key, int> const m2 {
			{{0}, 27},
			{{1}, 12},
			{{2}, 16},
			{{3}, 20},
	};

	std::map<Key, int> result;

	auto res = std::inserter(result, result.end());
	std::set_difference(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::leftExcluding(result, m1, m2);
	std::cout << result << '\n';

	result.clear();
	std::set_difference(m2.begin(), m2.end(), m1.begin(), m1.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::rightExcluding(result, m1, m2);
	std::cout << result << '\n';


	result.clear();
	std::set_union(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::outerFull(result, m1, m2);
	std::cout << result << '\n';


	result.clear();
	std::set_intersection(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::inner(result, m1, m2);
	std::cout << result << '\n';



	result.clear();
	std::set_symmetric_difference(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::outerExcluding(result, m1, m2);
	std::cout << result << '\n';



	result.clear();
	std::set_symmetric_difference(m2.begin(), m2.end(), m1.begin(), m1.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::rightOuter(result, m1, m2);
	std::cout << result << '\n';

	result.clear();
	std::set_symmetric_difference(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::leftOuter(result, m1, m2);
	std::cout << result << '\n';

	/*
  		27
   3	12
   2	16
   3	20
   17
   25


   Full Outer Join
   0	27
   3	12
   2	16
   3	20
   17	0
   25	0
   Inner Join
   3	12
   2	16
   3	20
   Outer Excluding Join
   0	27
   17	0
   25	0

   Left Outer join
   3 	12
   2	16
   3	20
   17	0
   25	0
   Left Excluding Join
   17	0
   25	0
  Right Outer join
   0	27
   3 	12
   2	16
   3	20
   Right Excluding Join
   0	27


	 */

}

#endif


#if defined(SET_OPERATIONS_BASICS)
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


std::ostream& operator << (std::ostream& os, std::vector<int> const& v) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& elem: v) os << elem << ' ';
	os << '}';
	return os;
}

int main () {
	std::vector<int> v1{1, 2, 3, 5, 5, 8};
	std::vector<int> v2{0, 1, 3, 6, 7, 8, 9};
	std::vector<int> result, result_temp, result_another_temp;
	std::cout << R"(
{   1 2 3 5 5 8 }
{ 0 1   3 6 7 8 9 }
)";

#if 0
	//inner
	result.clear();
	std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
			);
	std::cout << result << '\n';
#endif

#if 0
	//full outer
	result.clear();
	std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
			);
	std::cout << result << '\n';
#endif


#if 0
	//outer excluding
	result.clear();
	std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
			);
	std::cout << result << '\n';
	result_temp.clear();
	std::set_union(v2.begin(), v2.end(), v1.begin(), v1.end(),
			std::inserter(result_temp, result_temp.begin())
	);
	std::cout << result_temp << '\n';
	result_another_temp.clear();
	std::set_symmetric_difference (result_temp.begin(), result_temp.end(), result.begin(), result.end(),
			std::inserter(result_another_temp, result_another_temp.begin())
	);
	std::cout << result_another_temp << '\n';
#endif

#if 0
	//left excluding outer
	result.clear();
	std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
	);
	std::cout << result << '\n';
#endif


#if 0
	//right excluding outer
	result.clear();
	std::set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(),
			std::inserter(result, result.begin())
	);
	std::cout << result << '\n';
#endif

#if 0
	//right full outer
	result.clear();
	std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::inserter(result, result.begin())
			);
	std::cout << result << '\n';
	result_temp.clear();
	std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::inserter(result_temp, result_temp.begin())
	);
	std::cout << result_temp << '\n';
	result_another_temp.clear();
	std::set_difference (result.begin(), result.end(), result_temp.begin(), result_temp.end(),
			std::inserter(result_another_temp, result_another_temp.begin())
	);
	std::cout << result_another_temp << '\n';


#endif


}
#endif

#if defined (MULTI_ARG_TEMPLATE_CHECK)

#include <iostream>
#include <vector>

template<typename Container, typename = void>
struct MaybeContainer : std::false_type {};

template<typename Container>
struct MaybeContainer<Container,
					  std::void_t<
							  decltype(std::declval<Container>().begin()),
							  decltype(std::declval<Container>().end())
					  >
> : std::true_type {};

template<typename Container>
constexpr bool isContainer_v() { return MaybeContainer<Container>::value; }

template<typename Container>
using IsContainer = std::enable_if_t<isContainer_v<Container>(), bool>;

template <typename... Args>
constexpr bool areAllContainers_v () {
	bool result {true};
	return ((result = result && isContainer_v<Args>()),...);
}

template<typename... MaybeContainer>
using AreAllContainers = std::enable_if_t<areAllContainers_v<MaybeContainer...>(), bool>;

template <typename... Args, AreAllContainers<Args...> = true>
void someFunc (Args&&...) {
	std::cout << "true";
}

int main () {
	std::vector<int> v1 {1, 2};
	std::vector<int> v2 {3};
	double d {42.};

	someFunc(v1, v2);
	someFunc(v1, d);
}

#endif

#if defined(CPP20CHECK)

#include <iostream>
#include <filesystem>
#include <concepts>
#include <ranges>


template <typename T>
concept Printable = requires(T x, std::ostream &os) { os << x; };

void print(Printable auto... args) {
	(..., (std::cout << args << ' '));
}


int main() {
	std::filesystem::path f = std::filesystem::current_path();
	print(7.5, 12, "hello, world", f);
	print();

	auto const ints = {0, 1, 2, 3, 4, 5};
	auto even = [](int i) { return 0 == i % 2; };
	auto square = [](int i) { return i * i; };

	// the "pipe" syntax of composing the views:
	for (int i : ints | std::views::filter(even) | std::views::transform(square))
		std::cout << i << ' ';

	std::cout << '\n';

	// a traditional "functional" composing syntax:
	for (int i : std::views::transform(std::views::filter(ints, even), square))
		std::cout << i << ' ';

	return EXIT_SUCCESS;
}

#endif


#ifdef CONCEPTS_CIRCULAR_LINK_PROBLEM

#include <iostream>

#include "common_usage_library/types_requirements/operators.h"
#include "financial_data_structures/bid_ask.h"
#include "time_series/element.hpp"

using namespace culib;
using namespace culib::requirements;
using namespace time_series;
using namespace time_series::financial;

int main () {
	std::cout << std::boolalpha;
	BidAsk<int> ba;
	std::cout << (ba != 0) << '\n';

	Element<base::Microseconds, BidAsk<int>> e1;
	Element<base::Microseconds, BidAsk<int>> e2;
	std::cout << (e1 != 0) << '\n';
	std::cout << (e1 != e2) << '\n';
}

#endif

#ifdef INHERITANCE_SAME_NAMING_ISSUE

#include <iostream>

struct base_t {
	int value {42};
	int& get_value() & { return value; }
	int& get_base_value() & { return value; }
};

struct derived_t : base_t {
	int other_value {14};
	int& get_value() & { return other_value; }
};

int main () {
	derived_t derived;
	std::cout << derived.get_value() << ' ' << derived.get_base_value() << '\n';
}

#endif