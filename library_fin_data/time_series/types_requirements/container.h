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
   * Requirements to restrict collections only for usage as a serie
   * */

  template <template <typename...> typename Container, typename = void>
  struct IsContainer : std::false_type {};

  template <template <typename...> typename Container>
  struct IsContainer <Container,
					  std::void_t<
							  decltype(std::declval<Container>().begin()),
							  decltype(std::declval<Container>().end())
					  >
  > : std::true_type {};

  template <template <typename...> typename Container>
  constexpr bool IsContainer_v () {return IsContainer<Container>::value;}

  template <template <typename...> typename Container>
  using IsContainerOk = std::enable_if_t<IsContainer_v<Container>, bool>;


  template <typename Key, typename Type, typename = void>
  struct MayBeHash : std::false_type {};
  template <typename Key, typename Type>
  struct MayBeHash<Type,
				std::disjunction<
						std::is_same<typename std::hash<Key>, Type>,
						std::is_invocable_r<std::size_t, Type, std::add_const_t<std::decay_t<Key>>>
				>
  > : std::true_type {};
  template <typename Key, typename Type>
  constexpr bool IsHash_v () {return MayBeHash<Key, Type>::value;}
  template <typename Key, typename Type>
  using IsHash = std::enable_if_t<IsHash_v<Key, Type>, bool>;


  template <typename Value, typename Type, typename = void>
  struct MayBeComparator : std::false_type {};
  template <typename Value, typename Type>
  struct MayBeComparator<Value, Type,
		  std::is_invocable_r<bool, Type, std::add_const_t<std::decay_t<Value>>, std::add_const_t<std::decay_t<Value>>
		  >
  > : std::true_type {};
  template <typename Value, typename Type>
  constexpr bool IsComparator_v () {return MayBeComparator<Value, Type>::value;}
  template <typename Value, typename Type>
  using IsComparator = std::enable_if_t<IsComparator_v<Value, Type>, bool>;


  template <template <typename...> typename Container, typename Arg1, typename = void>
  struct ContainerHasSingleArg : std::false_type {};
  template <template <typename...> typename Container, typename Arg1>
  struct ContainerHasSingleArg <Container,
								Arg1,
								std::void_t<
									std::is_constructible<Container<Arg1>>
								>

  > : std::true_type {};
  template <template <typename...> typename Container, typename Arg1>
  constexpr bool ContainerHasSingleArg_v () {return ContainerHasSingleArg<Container, Arg1>::value;}

  template <template <typename...> typename Container, typename Arg1, typename Arg2, typename = void>
  struct ContainerHasTwoArgs : std::false_type {};
  template <template <typename...> typename Container, typename Arg1, typename Arg2>
  struct ContainerHasTwoArgs <Container,
							  Arg1,
							  Arg2,
							  std::void_t<
									  std::is_constructible<Container<Arg1, Arg2>>
									  >
  > : std::true_type {};

  template <template <typename...> typename Container, typename Arg1, typename Arg2>
  constexpr bool ContainerHasTwoArgs_v () {return ContainerHasTwoArgs<Container, Arg1, Arg2>::value;}


  template <template <typename...> typename Container, typename = void, typename... Args>
  struct ContainerHasArgs : std::false_type {};
  template <template <typename...> typename Container, typename... Args>
  struct ContainerHasArgs <Container, std::void_t<decltype(std::declval<Container<Args...>>())>, Args...> : std::true_type {};

  template <template <typename...> typename Container, typename... Args>
  constexpr std::size_t ContainerArgsCount () {
	  if constexpr (ContainerHasArgs<Container, Args...>::value){
		  return sizeof...(Args);
	  }
	  else {
		  return 0;
	  }
  }



#if 0
  template <typename DataStructure, typename = void>
  struct IsDataStructureAContainer : std::false_type {};

  template <typename DataStructure>
  struct IsDataStructureAContainer <DataStructure,
					  std::void_t<
							  decltype(std::declval<DataStructure>().begin()),
							  decltype(std::declval<DataStructure>().end())
					  >
  > : std::true_type {};

  template <typename DataStructure>
  constexpr bool IsDataStructureAContainer_v () {return IsDataStructureAContainer<DataStructure>::value;}
#endif


}//!namespace

#endif //TYPE_REQUIREMENTS_DATA_SERIES_CONTAINER_H
