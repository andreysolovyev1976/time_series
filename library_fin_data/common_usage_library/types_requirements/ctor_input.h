//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>

#ifndef TYPE_REQUIREMENTS_CTORS_H
#define TYPE_REQUIREMENTS_CTORS_H

namespace culib::requirements {

  template <typename Input, typename Result, typename = void>
  struct MaybeConveribleOrConstructibleFromTo : std::false_type {};

  template <typename Input, typename Result>
  struct MaybeConveribleOrConstructibleFromTo <
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

  template <typename Input, typename Result>
  constexpr bool isConveribleOrConstructible () {
	  return MaybeConveribleOrConstructibleFromTo<Input, Result>::value;
  }

  template <typename Input, typename Result>
  using ConveribleOrConstructibleFromTo = std::enable_if_t<isConveribleOrConstructible<Input, Result>(), bool>;

}//!namespace
#endif //TYPE_REQUIREMENTS_CTORS_H
