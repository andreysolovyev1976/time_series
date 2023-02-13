//
// Created by Andrey Solovyev on 14/02/2023.
//

#pragma once

#include <type_traits>

#ifndef ARG_TYPES_TRAITS_H
#define ARG_TYPES_TRAITS_H

namespace arg_traits {
  template <typename Arg>
  constexpr bool isLValueRefMutable () {
	  return std::is_lvalue_reference_v<Arg>;
  }

  template <typename Arg>
  constexpr bool isLValueRefConst () {
	  return std::conjunction_v<
			  std::is_lvalue_reference<Arg>,
			  std::is_const<std::remove_reference_t<Arg>>
	  >;
  }
  template <typename Arg>
  constexpr bool isRValueRef () {
	  return std::conjunction_v<
			  std::is_rvalue_reference<Arg>,
			  std::is_const<std::remove_reference_t<Arg>>
	  >;
  }
  template <typename Arg>
  constexpr bool isObjectConst () {
	  return std::is_const_v<Arg>;
  }
  template <typename Arg>
  constexpr bool isObjectMutable () {
	  return std::is_same_v<Arg, std::decay_t<Arg>>;
  }

}//!namespace


#endif //ARG_TYPES_TRAITS_H
