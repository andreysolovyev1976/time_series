//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>
#ifdef __cpp_concepts
#include <concepts>
#endif

#ifndef TYPE_REQUIREMENTS_CTORS_H
#define TYPE_REQUIREMENTS_CTORS_H

namespace culib::requirements {


#ifndef __cpp_concepts
  namespace details {
	template<typename Input, typename Result, typename = void>
	struct MaybeConveribleOrConstructibleFromTo : std::false_type { };

	template<typename Input, typename Result>
	struct MaybeConveribleOrConstructibleFromTo<
			Input, Result,
			std::void_t<
					std::enable_if_t<
							std::disjunction_v<
									std::is_convertible<Input, Result>,
									std::is_constructible<Result, Input>
							>
					>
			>
	> : std::true_type {};
  }//!namespace
  template <typename Input, typename Result>
  inline constexpr bool is_converible_or_constructible_v {
	  details::MaybeConveribleOrConstructibleFromTo<Input, Result>::value };

  template <typename Input, typename Result>
  using ConveribleOrConstructibleFromTo = std::enable_if_t<is_converible_or_constructible_v<Input, Result>, bool>;

#else

  template <typename Input, typename Result>
  concept ConveribleOrConstructibleFromTo =
			  std::is_convertible_v<Input, Result> ||
			  std::is_constructible_v<Result, Input>;

  template <typename Input, typename Result>
  inline constexpr bool is_converible_or_constructible_v  {
	  ConveribleOrConstructibleFromTo<Input, Result> ? true : false };


#endif
}//!namespace
#endif //TYPE_REQUIREMENTS_CTORS_H
