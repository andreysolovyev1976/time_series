//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "element.h"
#include <vector>

#ifndef TS_SERIE_H
#define TS_SERIE_H

namespace time_series {

  template <
          typename Duration = base::Seconds,
		  typename Elem = base::Value<Duration>,
		  template <typename = Elem> typename Container = std::vector
				  >
  class Serie {
  public:
	  Serie () = default;
	  explicit Serie (std::vector<Elem> &&values);
	  explicit Serie (const std::vector<Elem> &values);
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
