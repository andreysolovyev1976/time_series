//
// Created by Andrey Solovyev on 16/02/2023.
//

#pragma once

#include <type_traits>
#ifdef __cpp_concepts
#include <concepts>
#endif

#ifndef TYPE_REQUIREMENTS_OBJECT_ARGS_COUNT_H
#define TYPE_REQUIREMENTS_OBJECT_ARGS_COUNT_H

namespace culib::requirements {

#ifndef __cpp_concepts

  template <typename Object, typename... Args>
  constexpr std::size_t objectArgsCount () {
	  if constexpr (std::is_constructible_v<Object, Args...>) {
		  return sizeof...(Args);
	  }
	  else {
		  return 0;
	  }
  }

  namespace details {
	template<template<typename...> typename Object, typename Arg1, typename = void>
	struct ObjectCanHaveSingleArg : std::false_type { };
	template<template<typename...> typename Object, typename Arg1>
	struct ObjectCanHaveSingleArg<Object,
								  Arg1,
								  std::void_t<std::is_constructible<Object<Arg1>>>
	> : std::true_type {};
  }//!namespace
  template <template <typename...> typename Object, typename Arg1>
  inline constexpr bool object_can_have_single_arg_v { details::ObjectCanHaveSingleArg<Object, Arg1>::value };

  namespace details {
	template <template <typename...> typename Object, typename Arg1, typename Arg2, typename = void>
	struct objectCanHaveTwoArgs : std::false_type {};
	template <template <typename...> typename Object, typename Arg1, typename Arg2>
	struct objectCanHaveTwoArgs <Object,
								 Arg1, Arg2,
								 std::void_t<std::is_constructible<Object<Arg1, Arg2>>>
	> : std::true_type {};
  }//!namespace
  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  inline constexpr bool object_can_have_two_args_v { details::objectCanHaveTwoArgs<Object, Arg1, Arg2>::value };

#else

  template <template <typename...> typename Object, typename Arg1>
  concept ObjectCanHaveSingleArg = requires () { std::is_constructible_v<Object<Arg1>>; };

  template <template <typename...> typename Object, typename Arg1>
  inline constexpr bool object_can_have_single_arg_v {
	  ObjectCanHaveSingleArg<Object, Arg1> ? true : false };

  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  concept ObjectCanHaveTwoArgs = requires () { std::is_constructible_v<Object<Arg1, Arg2>>; };

  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  inline constexpr bool object_can_have_two_args_v {
	  ObjectCanHaveTwoArgs<Object, Arg1, Arg2> ? true : false };


#endif

}//!namespace

#endif //TYPE_REQUIREMENTS_OBJECT_ARGS_COUNT_H
