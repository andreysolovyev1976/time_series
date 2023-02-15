//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "element.h"
#include <vector>

#ifndef TS_SERIE_H
#define TS_SERIE_H

namespace time_series {

  //todo: requirements for iterators
  //todo: requirements for Elements, should contain Timestamp
  //todo: requirements for Container

  template <typename Elem, template <typename = Elem> typename Container = std::vector>
//  template <typename Container = std::vector<Element<>>>

  class Serie {
  public:
	  using Iter = typename Container<Elem>::iterator;
	  using CIter = typename Container<Elem>::const_iterator;
	  using RIter = typename Container<Elem>::reverse_iterator;
	  using CRIter = typename Container<Elem>::const_reverse_iterator;

	  Serie () = default;
	  explicit Serie (Container<Elem> &&values);
	  explicit Serie (const Container<Elem> &values);
	  explicit Serie (std::initializer_list<Elem> values);

	  template <typename Fn, typename Iter>
	  Serie& applyFunction (Fn &&fn /* should be applicable using iterators */);

	  template <typename Iter>
	  Iter begin();
	  void end();
	  void begin() const;
	  void end() const;
  private:
	  Container<Elem> data;
  };

}//!namespace

#endif //TS_SERIE_H
