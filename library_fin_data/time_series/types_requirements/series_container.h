//
// Created by Andrey Solovyev on 16/02/2023.
//

#pragma once

#include <type_traits>

#ifndef TYPE_REQUIREMENTS_DATA_SERIES_CONTAINER_H
#define TYPE_REQUIREMENTS_DATA_SERIES_CONTAINER_H

namespace requirements {
  /**
   * @details
   * Requirements to restrict collections only for usage as a serie
   * */

  template <typename Container, typename = void>
  struct IsContainer : std::false_type {};

  template <typename Container>
  struct IsContainer <Container,
					  std::void_t<
							  decltype(std::declval<Container>().begin()),
							  decltype(std::declval<Container>().end())
					  >
  > : std::true_type {};

  template <typename Container>
  constexpr bool IsContainer_v () {return IsContainer<Container>::value;}

  template <typename Container>
  using IsContainerOk = std::enable_if_t<IsContainer_v<Container>, bool>;

}//!namespace

#endif //TYPE_REQUIREMENTS_DATA_SERIES_CONTAINER_H
