//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>

#ifdef __cpp_concepts
#include <concepts>
#endif

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

#else

  /**
   * @brief
   * This concepts also exist in std::\n
   * but were redefined for sake of keeping\n
   * a code style
   * */

  template <typename Number>
  concept IsFloatinPoint = requires () {std::is_floating_point_v<Number>;};

  template <typename Number>
  concept IsNotFloatinPoint = requires () {not IsFloatinPoint<Number>;};

  template <typename Number>
  concept IsIntegral = requires () {std::is_integral_v<Number>;};

  template <typename Number>
  concept IsNotIntegral = requires () {not IsIntegral<Number>;};


#endif

}//!namespace
#endif //TYPE_REQUIREMENTS_NUMERICS_H

