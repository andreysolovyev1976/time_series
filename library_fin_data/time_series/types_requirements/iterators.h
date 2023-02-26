//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>
#include <iterator>

#ifndef TYPE_REQUIREMENTS_CTORS_H
#define TYPE_REQUIREMENTS_CTORS_H

namespace requirements {

  template <typename Type>
  using IteratorTypeSelect = std::conditional_t<
		  std::is_const_v<std::remove_reference_t<Type>>,
		  typename std::decay_t<Type>::const_iterator,
		  typename std::decay_t<Type>::iterator>;


  template <typename Iter>
  using require_input_iterator = std::enable_if_t<std::is_base_of_v<
		  std::input_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;


}//!namespace
#endif //TYPE_REQUIREMENTS_CTORS_H
