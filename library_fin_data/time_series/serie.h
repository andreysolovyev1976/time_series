//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "element.h"
#include "types_requirements/container.h"
#include "types_requirements/numerics.h"

#include <deque>
#include <cstddef>
#include <stdexcept>

#ifndef TS_SERIE_H
#define TS_SERIE_H

namespace time_series {

  //todo: requirements for iterators
  //todo: requirements for Elements, should contain Timestamp
  //todo: requirements for Container

namespace details {

  template<typename C>
  struct TypeHolder {
	  using type = C;
  };

  template<
		  typename Duration,
		  typename ElemType,
		  template<typename...> typename Container
  >
  constexpr auto getContainer()
  {
	  using namespace requirements;
#if 0
	  if  constexpr (requirements::ContainerArgsCount<Container, time_series::Element<Duration, ElemType>>() == 1u) {
		  return TypeHolder<Container<time_series::Element<Duration, ElemType>>>{};
	  }
	  else if constexpr (requirements::ContainerArgsCount<Container, base::Timestamp<Duration>, ElemType>() == 2u) {
		  return TypeHolder<Container<base::Timestamp<Duration>, ElemType>>{};
	  }
	  else {
		  throw std::invalid_argument("Unexpected parameters");
	  }
#endif
#if 1
	  if constexpr (
//			  IsContainer_v<Container>() &&
			  ContainerHasSingleArg_v<Container, time_series::Element<Duration, ElemType>>()) {
		  return TypeHolder<Container<time_series::Element<Duration, ElemType>>>{};
	  }
	  else if constexpr (
//			  IsContainer_v<Container>() &&
			  ContainerHasTwoArgs_v<Container, base::Timestamp<Duration>, ElemType>()) {
		  return TypeHolder<Container<base::Timestamp<Duration>, ElemType>>{};
	  }
	  else {
		  throw std::invalid_argument("Unexpected parameters");
	  }
#endif

#if 0
	  if 	  constexpr (std::is_constructible_v<Container<time_series::Element<Duration, ElemType>>>) {
		  return TypeHolder<Container<time_series::Element<Duration, ElemType>>>{};
	  }
	  else if constexpr (std::is_constructible_v<Container<time_series::Element<Duration, ElemType>, time_series::ElementHasher<Duration, ElemType>>>) {
		  return TypeHolder<Container<time_series::Element<Duration, ElemType>, time_series::ElementHasher<Duration, ElemType>>>{};
	  }
	  else if constexpr (std::is_constructible_v<Container<time_series::Element<Duration, ElemType>, Number>>) {
		  return TypeHolder<Container<time_series::Element<Duration, ElemType>, Number>>{};
	  }
	  else if constexpr (std::is_constructible_v<Container<Duration, ElemType>>) {
		  return TypeHolder<Container<time_series::Element<Duration, ElemType>>>{};
	  }
	  else if constexpr (std::is_constructible_v<Container<Duration, ElemType, base::TimestampHasher<Duration>>>) {
		  return TypeHolder<Container<Duration, ElemType, base::TimestampHasher<Duration>>>{};
	  }
	  else {
		  throw std::invalid_argument("Unexpected parameters");
	  }
#endif
  }

}//!namespace

  template <
		  typename ElemType = base::Value<base::traits::ValueTypeDefault>
		  , typename Duration = base::Seconds
		  , template <typename...> typename Container = std::vector
  >
struct Serie final : public decltype(details::getContainer<Duration, ElemType, Container>())::type {
	using container_type = typename decltype(details::getContainer<Duration, ElemType, Container>())::type;
	using container_type::container_type;

	  template <typename Fn, typename Iter>
	  Serie& applyFunction (Fn &&fn /* should be applicable using iterators */);
  };



}//!namespace

#endif //TS_SERIE_H
