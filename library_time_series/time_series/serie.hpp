//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "time_series/element.hpp"
#include "common_usage_library/types_requirements/container.h"
#include "common_usage_library/types_requirements/object_args_count.h"
#include "common_usage_library/types_requirements/numerics.h"

#include <list>
#include <cstddef>
#include <stdexcept>

#ifdef __cpp_concepts
#include <concepts>
#endif


#ifndef TS_SERIE_H
#define TS_SERIE_H

namespace time_series {

  namespace details {
	/**
	 * @brief
	 * Empty Object Optimization
	 * as the only result that is required from a factory
	 * is a correct type of a Container, that is to be used as a parent for a Serie.
	*/
	template<typename Container>
	struct ContainerTypeHolder {
		using container_type = Container;
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
		using namespace culib::requirements;
		using Elem = Element<Duration, ElemType>;
		using Timestamp = culib::time::Timestamp<Duration>;

		/**
		 * @brief
		 * It is required to separate the conditions, as instantiation happens before
		 * evaluation and therefore the second condition is being attempted to instantiate
		 * with the wrong arguments, resulting in a compile error
		*/
		if constexpr (object_can_have_single_arg_v<Container, Elem>) {
			if constexpr (is_container_v<Container<Elem, Args...>>) {
				return ContainerTypeHolder<Container<Elem, Args...>>{};
			}
		}
		else if constexpr (object_can_have_two_args_v<Container, Timestamp, ElemType>) {
			if constexpr (is_container_v<Container<Timestamp, ElemType, Args...>>) {
				return ContainerTypeHolder<Container<Timestamp, ElemType, Args...>>{};
			}
		}
		else {
			throw std::invalid_argument("Unexpected parameters while trying to construct a Serie");
		}
	}
  }//!namespace

  template <
		  typename Duration = culib::time::Seconds
		  , typename ElemType = Value<value::traits::ValueTypeDefault>
		  , template <typename...> typename Container = std::list
		  , typename ...Args
  >
  struct Serie : public
					   decltype(details::getContainer<Duration, ElemType, Container, Args...>())::container_type {
	  using container_type = typename decltype(details::getContainer<Duration, ElemType, Container, Args...>())::container_type;
	  using container_type::container_type; //ctors inheritance

	  using duration_type = Duration;
	  using elem_type = ElemType;

	  struct serie_tag {}; //tag to identify a container as a TimeSerie

	  template <typename Fn, typename Iter>
	  Serie& applyFunction (Fn &&fn, Iter b = container_type::begin(), Iter e = container_type::end());
  };

}//!namespace

#endif //TS_SERIE_H
