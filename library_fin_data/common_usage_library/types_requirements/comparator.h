//
// Created by Andrey Solovyev on 16/02/2023.
//

#pragma once

#include <type_traits>
#include <functional>

#ifndef TYPE_REQUIREMENTS_COMPARATOR_H
#define TYPE_REQUIREMENTS_COMPARATOR_H

namespace culib::requirements {
  /**
 * @brief
 * check that type is a comparator for another type
 * */
  template <typename Value, typename Type, typename = void>
  struct MaybeComparator : std::false_type {};
  template <typename Value, typename Type>
  struct MaybeComparator<Value, Type,
						 std::void_t<
								 std::enable_if_t<
										 std::is_invocable_r_v<
												 bool,
												 Type,
												 std::add_const_t<std::decay_t<Value>>, std::add_const_t<std::decay_t<Value>>
										 >
								 >,
								 std::enable_if_t<
										 std::is_same_v<
												 std::invoke_result_t<Type, std::add_const_t<std::decay_t<Value>>, std::add_const_t<std::decay_t<Value>>>,
												 bool>
								 >
						 >

  > : std::true_type {};
  template <typename Value, typename Type>
  constexpr bool isComparator_v () {return MaybeComparator<Value, Type>::value;}
  template <typename Value, typename Type>
  using IsComparator = std::enable_if_t<isComparator_v<Value, Type>, bool>;

}//!namespace

#endif //TYPE_REQUIREMENTS_COMPARATOR_H
