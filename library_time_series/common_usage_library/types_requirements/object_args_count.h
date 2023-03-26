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

  template <template <typename...> typename Object, typename Arg1, typename = void>
  struct ObjectCanHaveSingleArg : std::false_type {};
  template <template <typename...> typename Object, typename Arg1>
  struct ObjectCanHaveSingleArg <Object,
								 Arg1,
								 std::void_t<std::is_constructible<Object<Arg1>>>
  > : std::true_type {};
  template <template <typename...> typename Object, typename Arg1>
  constexpr bool objectCanHaveSingleArg_v () {return ObjectCanHaveSingleArg<Object, Arg1>::value;}

  template <template <typename...> typename Object, typename Arg1, typename Arg2, typename = void>
  struct objectCanHaveTwoArgs : std::false_type {};
  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  struct objectCanHaveTwoArgs <Object,
							   Arg1, Arg2,
							   std::void_t<std::is_constructible<Object<Arg1, Arg2>>>
  > : std::true_type {};
  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  constexpr bool objectCanHaveTwoArgs_v () {return objectCanHaveTwoArgs<Object, Arg1, Arg2>::value;}

#else

  template <template <typename...> typename Object, typename Arg1>
  concept ObjectCanHaveSingleArg = requires () { std::is_constructible_v<Object<Arg1>>; };
  template <template <typename...> typename Object, typename Arg1>
  concept NotObjectCanHaveSingleArg = !ObjectCanHaveSingleArg<Object, Arg1> ;

  template <template <typename...> typename Object, typename Arg1>
  requires ObjectCanHaveSingleArg<Object, Arg1>
  constexpr bool objectCanHaveSingleArg_v () {return true;}

  template <template <typename...> typename Object, typename Arg1>
  requires NotObjectCanHaveSingleArg<Object, Arg1>
  constexpr bool objectCanHaveSingleArg_v () {return false;}

  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  concept ObjectCanHaveTwoArgs = requires () { std::is_constructible_v<Object<Arg1, Arg2>>; };
  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  concept NotObjectCanHaveTwoArgs = !ObjectCanHaveTwoArgs<Object, Arg1, Arg2>;

  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  requires ObjectCanHaveTwoArgs<Object, Arg1, Arg2>
  constexpr bool objectCanHaveTwoArgs_v () {return true;}

  template <template <typename...> typename Object, typename Arg1, typename Arg2>
  requires NotObjectCanHaveTwoArgs<Object, Arg1, Arg2>
  constexpr bool objectCanHaveTwoArgs_v () {return false;}

#endif

}//!namespace

#endif //TYPE_REQUIREMENTS_OBJECT_ARGS_COUNT_H
