//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>

#ifndef TYPE_REQUIREMENTS_NUMERICS_H
#define TYPE_REQUIREMENTS_NUMERICS_H


namespace culib::requirements {

#ifndef __cpp_concepts

  template <typename Number>
  using IsFloatinPoint = std::enable_if_t<std::is_floating_point_v<Number>, bool>;
  template <typename Number>
  using IsNotFloatinPoint = std::enable_if_t<not std::is_floating_point_v<Number>, bool>;

  template <typename Number>
  using IsIntegral = std::enable_if_t<std::is_integral_v<Number>, bool>;

  template <typename Number>
  using IsNotIntegral = std::enable_if_t<not std::is_integral_v<Number>, bool>;

#endif

}//!namespace
#endif //TYPE_REQUIREMENTS_NUMERICS_H

