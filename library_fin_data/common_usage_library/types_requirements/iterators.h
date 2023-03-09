//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>
#include <iterator>

#ifndef TYPE_REQUIREMENTS_CTORS_H
#define TYPE_REQUIREMENTS_CTORS_H

namespace culib::requirements {


  template <typename Iter>
  using Iterator = std::enable_if_t<std::conjunction_v<
		  typename Iter::value_type,
		  typename Iter::difference_type,
		  typename Iter::reference,
		  typename Iter::pointer
  >, bool>;


  template <typename Iter>
  using InputIterator = std::enable_if_t<std::is_base_of_v<
		  std::input_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;


}//!namespace
#endif //TYPE_REQUIREMENTS_CTORS_H
