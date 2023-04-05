//
// Created by Andrey Solovyev on 16/02/2023.
//

#pragma once

#include <type_traits>
#include <functional>
#ifdef __cpp_concepts
#include <concepts>
#endif


#ifndef TYPE_REQUIREMENTS_COMPARATOR_H
#define TYPE_REQUIREMENTS_COMPARATOR_H

namespace culib::requirements {
  /**
 * @brief
 * check that type is a comparator for another type
 * */

#ifndef __cpp_concepts
  namespace details {
	template<typename Value, typename Type, typename = void>
	struct MaybeComparator : std::false_type { };
	template<typename Value, typename Type>
	struct MaybeComparator<Value, Type,
						   std::void_t<
								   std::enable_if_t<
										   std::is_invocable_r_v<
												   bool,
												   Type,
												   std::add_const_t<std::decay_t<Value>>,
												   std::add_const_t<std::decay_t<Value>>
										   >
								   >,
								   std::enable_if_t<
										   std::is_same_v<
												   std::invoke_result_t<Type,
																		std::add_const_t<std::decay_t<Value>>,
																		std::add_const_t<std::decay_t<Value>>>,
												   bool>
								   >
						   >
	> : std::true_type {};
  }//!namespace
  template <typename Value, typename Type>
  inline constexpr bool is_comparator_v { details::MaybeComparator<Value, Type>::value };

  template <typename Value, typename Type>
  using IsComparator = std::enable_if_t<is_comparator_v<Value, Type>, bool>;
#else

  template <typename Value, typename Type>
  concept IsComparator = requires () {
	  requires std::is_invocable_r_v<
			  bool,
			  Type,
			  std::add_const_t<std::decay_t<Value>>, std::add_const_t<std::decay_t<Value>>
	  >;
	  requires
	  std::is_same_v<
			  std::invoke_result_t<Type, std::add_const_t<std::decay_t<Value>>, std::add_const_t<std::decay_t<Value>>>,
			  bool>;
  };


  template <typename Value, typename Type>
  inline constexpr bool is_comparator_v { IsComparator<Value, Type> ? true : false };



#endif


}//!namespace

#endif //TYPE_REQUIREMENTS_COMPARATOR_H
