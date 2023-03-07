//
// Created by Andrey Solovyev on 17/02/2023.
//

#pragma once

#include <type_traits>

#ifndef COMPILE_TIME_TEST_H
#define COMPILE_TIME_TEST_H

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
[[maybe_unused]]                                                                                            \
static constexpr auto name ## _compiles = detail::name ## FromArgs<R, Args...>::value;                      \
template<typename ...Args> \
[[maybe_unused]]                                                                                            \
static constexpr auto name ## _compiles_from_type = detail::name ## FromType<Args...>::value;\



template <typename Callable, typename Res = void, typename ...Args>
using CompilesOk = std::enable_if_t<
		std::disjunction_v<
				std::is_invocable<Callable, Args...>,
				std::is_invocable_r<Res, Callable, Args...>
		>
		, bool>;

template <typename Callable, typename Res = void, typename ...Args>
using DoesntCompile = std::enable_if_t<
		std::negation_v<
				std::disjunction<
						std::is_invocable<Callable, Args...>,
						std::is_invocable_r<Res, Callable, Args...>
				>
		>
		, bool>;

template <typename Callable, typename Res = void, typename ...Args>
constexpr bool Compilable ()  {
	if constexpr (std::is_same_v<bool, CompilesOk<Callable, Res, Args...>>) {
		return true;
	}
	else if constexpr (std::is_same_v<bool, DoesntCompile<Callable, Res, Args...>>) {
		return false;
	}
	else {
		throw std::invalid_argument("Unexpected template parameters");
	}
}

#endif //COMPILE_TIME_TEST_H
