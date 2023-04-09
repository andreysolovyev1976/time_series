//
// Created by Andrey Solovyev on 09/04/2023.
//

#pragma once

#include <type_traits>
#ifdef __cpp_concepts
#include <concepts>
#endif


#ifndef FIN_DATA_ELEMENT_REQUIREMENTS_H
#define FIN_DATA_ELEMENT_REQUIREMENTS_H

namespace time_series::requirements {

#ifndef __cpp_concepts

  namespace details {

	template<typename V, typename = void>
	struct MaybeHasMethod_ContainsZero : std::false_type {};

	template<typename V>
	struct MaybeHasMethod_ContainsZero<V,
//			std::is_invocable<decltype(&V::containsZero), V&>
			std::void_t<decltype(std::declval<V>().containsZero())>
			> : std::true_type {};

	template<typename V, typename = void>
	struct MaybeComparableToZero : std::false_type {};

	template<typename V>
	struct MaybeComparableToZero<V,
//			std::is_invocable<decltype(&V::containsZero), V&>
			std::void_t<decltype(std::declval<V>() == 0)>
	> : std::true_type {};

  }//!namespace

  template<typename V>
  inline constexpr bool has_method_contains_zero_v { details::MaybeHasMethod_ContainsZero<V>::value };

  template<typename V>
  using HasMethod_ContainsZero = std::enable_if_t<has_method_contains_zero_v<V>, bool>;

  template<typename V>
  inline constexpr bool is_somparable_to_zero_v { details::MaybeComparableToZero<V>::value };

  template<typename V>
  using IsComparableToZero = std::enable_if_t<is_somparable_to_zero_v<V>, bool>;


#else

  template <typename V>
	concept HasMethod_ContainsZero = requires (V v) { v.containsZero(); };

  template<typename V>
  inline constexpr bool has_method_contains_zero_v { HasMethod_ContainsZero<V> };

    template <typename V>
	concept IsComparableToZero = requires (V v) { v == 0; };

  template<typename V>
  inline constexpr bool is_somparable_to_zero_v { IsComparableToZero<V> };

#endif


}//!namespace


#endif //FIN_DATA_ELEMENT_REQUIREMENTS_H
