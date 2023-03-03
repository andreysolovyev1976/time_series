//
// Created by Andrey Solovyev on 16/02/2023.
//

#pragma once

#include <type_traits>

#ifndef TYPE_REQUIREMENTS_SERIES_H
#define TYPE_REQUIREMENTS_SERIES_H

namespace requirements {

  template<typename Serie, typename = void>
  struct MaybeSerie : std::false_type {};

  template<typename Serie>
  struct MaybeSerie<Serie, std::void_t<decltype(Serie::serie_tag)> > : std::true_type {};

  template<typename Serie>
  constexpr bool isSerie_v() { return MaybeSerie<Serie>::value; }

  template<typename Serie>
  using IsSerie = std::enable_if_t<isSerie_v<Serie>(), bool>;

}//!namespace

#endif //TYPE_REQUIREMENTS_SERIES_H
