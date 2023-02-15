//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>

#ifndef TYPE_REQUIREMENTS_CTORS_H
#define TYPE_REQUIREMENTS_CTORS_H

namespace requirements {

  template <typename Input, typename Result>
  using ConveribleOrConstructibleFromTo = std::enable_if_t<
		  std::disjunction_v<
				  std::is_convertible<Input, Result>,
				  std::is_constructible<Result, Input>
		  >
		  , bool>;

  template <typename Input, typename Result, ConveribleOrConstructibleFromTo<Input, Result> = true>
  constexpr bool isConveribleOrConstructible () {return true;}


}//!namespace
#endif //TYPE_REQUIREMENTS_CTORS_H
