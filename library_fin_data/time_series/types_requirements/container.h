//
// Created by Andrey Solovyev on 16/02/2023.
//

#pragma once

#include <type_traits>
#include <functional>


#ifndef TYPE_REQUIREMENTS_DATA_SERIES_CONTAINER_H
#define TYPE_REQUIREMENTS_DATA_SERIES_CONTAINER_H

namespace requirements {
  /**
   * @details
   * Requirements to restrict containers only to be used as a serie
   * */

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

/**
 * @brief
 * check that type is a hash for another type
 * */
  template<typename Key, typename Type, typename HashResult, typename = void>
  struct MaybeUserDefinedHash : std::false_type { };
  template<typename Key, typename Type, typename HashResult>
  struct MaybeUserDefinedHash<Key, Type, HashResult,
							  std::void_t<
									  std::enable_if_t<std::negation_v<std::is_same<HashResult, bool>>>,
									  std::enable_if_t<std::is_invocable_r_v<std::size_t, Type, std::add_const_t<std::decay_t<Key>>>>,
									  std::enable_if_t<std::is_copy_constructible_v<Type>>,
									  std::enable_if_t<std::is_move_constructible_v<Type>>
  >
  > : std::true_type {};
  template<typename Key, typename Type, typename = void>
  struct MaybeBuiltInHash : std::false_type { };
  template<typename Key, typename Type>
  struct MaybeBuiltInHash<Key, Type,
						  std::void_t<
								  std::enable_if_t<std::is_same_v<typename std::hash<Key>, Type>>
						  >
  > : std::true_type {};
  template <typename Key, typename Type, typename HashResult = std::size_t>
  constexpr bool isHash_v () {
	  return
			  MaybeUserDefinedHash<Key, Type, HashResult>::value ||
			  MaybeBuiltInHash<Key, Type>::value;
  }
  template <typename Key, typename Type, typename HashResult = std::size_t>
  using IsHash = std::enable_if_t<isHash_v<Key, Type, HashResult>, bool>;

  /**
 * @brief
 * check that type is a comparator for another type
 * */
  template <typename Value, typename Type, typename = void>
  struct MaybeComparator : std::false_type {};
  template <typename Value, typename Type>
  struct MaybeComparator<Value, Type,
						 std::void_t<
								 std::enable_if_t<
										 std::is_invocable_r_v<
												 bool,
												 Type,
												 std::add_const_t<std::decay_t<Value>>, std::add_const_t<std::decay_t<Value>>
										 >
								 >,
								 std::enable_if_t<
										 std::is_same_v<
												 std::invoke_result_t<Type, std::add_const_t<std::decay_t<Value>>, std::add_const_t<std::decay_t<Value>>>,
												 bool>
								 >
						 >

  > : std::true_type {};
  template <typename Value, typename Type>
  constexpr bool isComparator_v () {return MaybeComparator<Value, Type>::value;}
  template <typename Value, typename Type>
  using IsComparator = std::enable_if_t<isComparator_v<Value, Type>, bool>;



  template <typename Container, typename... Args>
  constexpr std::size_t containerArgsCount () {
	  if constexpr (std::is_constructible_v<Container, Args...>) {
		  return sizeof...(Args);
	  }
	  else {
		  return 0;
	  }
  }

  template <template <typename...> typename Container, typename Arg1, typename = void>
  struct ObjectCanHaveSingleArg : std::false_type {};
  template <template <typename...> typename Container, typename Arg1>
  struct ObjectCanHaveSingleArg <Container,
								 Arg1,
								 std::void_t<std::is_constructible<Container<Arg1>>>
  > : std::true_type {};
  template <template <typename...> typename Container, typename Arg1>
  constexpr bool objectCanHaveSingleArg_v () {return ObjectCanHaveSingleArg<Container, Arg1>::value;}

  template <template <typename...> typename Container, typename Arg1, typename Arg2, typename = void>
  struct objectCanHaveTwoArgs : std::false_type {};
  template <template <typename...> typename Container, typename Arg1, typename Arg2>
  struct objectCanHaveTwoArgs <Container,
							   Arg1, Arg2,
							   std::void_t<std::is_constructible<Container<Arg1, Arg2>>>
  > : std::true_type {};
  template <template <typename...> typename Container, typename Arg1, typename Arg2>
  constexpr bool objectCanHaveTwoArgs_v () {return objectCanHaveTwoArgs<Container, Arg1, Arg2>::value;}


}//!namespace

#endif //TYPE_REQUIREMENTS_DATA_SERIES_CONTAINER_H
