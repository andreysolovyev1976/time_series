//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "element.h"
#include "types_requirements/series_container.h"

#include <deque>
#include <cstddef>

#ifndef TS_SERIE_H
#define TS_SERIE_H

namespace time_series {

  //todo: requirements for iterators
  //todo: requirements for Elements, should contain Timestamp
  //todo: requirements for Container

  template <
		  typename Container = std::deque<time_series::Element<>>,
		  requirements::IsContainerOk<Container> = true
		  >
  class Serie : public Container {
  public:
	  using Container::Container;

	  template <typename Fn, typename Iter>
	  Serie& applyFunction (Fn &&fn /* should be applicable using iterators */);

	  void setMaxSize ();
	  void pushBack ();
	  void popBack ();
	  void pushFront ();
	  void popFront ();

  private:
	  std::size_t max_size;
  };


  void setMaxSize ();


}//!namespace

#endif //TS_SERIE_H
