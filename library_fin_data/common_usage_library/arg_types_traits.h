//
// Created by Andrey Solovyev on 14/02/2023.
//

#pragma once

#include <type_traits>

#ifndef ARG_TYPES_TRAITS_H
#define ARG_TYPES_TRAITS_H

namespace culib::arg_traits {

  template <typename Arg>
  constexpr bool isLValueRefMutable (Arg&& arg) {
	  return std::is_lvalue_reference_v<decltype(std::forward<Arg>(arg))>;
  }

  template <typename Arg>
  constexpr bool isLValueRefConst (Arg&& arg) {
	  return std::conjunction_v<
			  std::is_lvalue_reference<decltype(std::forward<Arg>(arg))>,
			  std::is_const<std::remove_reference_t<decltype(std::forward<Arg>(arg))>>
	  >;
  }
  template <typename Arg>
  constexpr bool isRValueRef (Arg&& arg) {
	  return std::conjunction_v<
			  std::is_rvalue_reference<decltype(std::forward<Arg>(arg))>,
			  std::is_const<std::remove_reference_t<decltype(std::forward<Arg>(arg))>>
	  >;
  }
  template <typename Arg>
  constexpr bool isObjectConst (Arg&& arg) {
	  return std::is_const_v<decltype(std::forward<Arg>(arg))>;
  }
  template <typename Arg>
  constexpr bool isObjectMutable (Arg&& arg) {
	  return std::is_same_v<decltype(std::forward<Arg>(arg)), std::decay_t<decltype(std::forward<Arg>(arg))>>;
  }

}//!namespace


#endif //ARG_TYPES_TRAITS_H
