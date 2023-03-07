//
// Created by Andrey Solovyev on 16/02/2023.
//

#pragma once

#include <type_traits>
#include <functional>

#ifndef TYPE_REQUIREMENTS_HASH_H
#define TYPE_REQUIREMENTS_HASH_H

namespace culib::requirements {
/**
 * @brief
 * check that type is a hash for another type
 * */
  template<typename Key, typename Type, typename HashResult, typename = void>
  struct MaybeUserDefinedHash : std::false_type { };
  template<typename Key, typename Type, typename HashResult>
  struct MaybeUserDefinedHash<Key, Type, HashResult,
							  std::void_t<
									  std::enable_if_t<std::negation_v<std::is_same<HashResult, bool>>>,
									  std::enable_if_t<std::is_invocable_r_v<std::size_t, Type, std::add_const_t<std::decay_t<Key>>>>,
									  std::enable_if_t<std::is_copy_constructible_v<Type>>,
									  std::enable_if_t<std::is_move_constructible_v<Type>>
  >
  > : std::true_type {};
  template<typename Key, typename Type, typename = void>
  struct MaybeBuiltInHash : std::false_type { };
  template<typename Key, typename Type>
  struct MaybeBuiltInHash<Key, Type,
						  std::void_t<
								  std::enable_if_t<std::is_same_v<typename std::hash<Key>, Type>>
						  >
  > : std::true_type {};
  template <typename Key, typename Type, typename HashResult = std::size_t>
  constexpr bool isHash_v () {
	  return
			  MaybeUserDefinedHash<Key, Type, HashResult>::value ||
			  MaybeBuiltInHash<Key, Type>::value;
  }
  template <typename Key, typename Type, typename HashResult = std::size_t>
  using IsHash = std::enable_if_t<isHash_v<Key, Type, HashResult>, bool>;

}//!namespace

#endif //TYPE_REQUIREMENTS_HASH_H
