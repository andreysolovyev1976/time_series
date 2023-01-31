//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "element.h"

#ifndef TS_SERIE_H
#define TS_SERIE_H

namespace time_series {

  template <typename Elem, template <typename = Elem> typename Container>
  class Serie {
  public:
	  Serie () = default;
	  explicit Serie (std::vector<Elem> &&values);
	  explicit Serie (const std::vector<Elem> &values);
	  explicit Serie (std::initializer_list<Elem> values);

	  template <typename Fn>
	  Serie& applyFunction (Fn &&fn);
  private:
	  Container<Elem> data;
  };

}//!namespace

#endif //TS_SERIE_H
