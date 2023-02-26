//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include "time_series/serie.hpp"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

namespace time_series {

  template <
		  typename Duration
		  , typename ElemType
		  , template <typename...> typename Container
		  , typename Function
  >
  Serie<Duration, ElemType, Container> zip (
		  const Serie<Duration, ElemType, Container> &a,
		  const Serie<Duration, ElemType, Container> &b,
		  Function &&fn) {

	  std::vector<T> temp_;
	  if (a.size() != b.size())
	  {
		  cout << __FUNCTION__ << endl;
		  throw std::invalid_argument("Error in per element Vector subtraction, two vectors size are not equal");
	  } else
	  {
		  temp_.resize(a.size());
		  for ( int i = 0; i < a.size(); i++ ) temp_.at(i) = a.at(i) - b.at(i);
	  }

	  return temp_;
  }

/*
  template <
		  typename Duration
		  , typename ElemType
		  , template <typename...> typename Container
		  , typename Function
  >
  Serie<Duration, ElemType, Container> zip (
		  const Serie<Duration, ElemType, Container> &a,
		  const Serie<Duration, ElemType, Container> &b,
		  Function &&fn) {

	  std::vector<T> temp_;
	  if (a.size() != b.size())
	  {
		  cout << __FUNCTION__ << endl;
		  throw std::invalid_argument("Error in per element Vector subtraction, two vectors size are not equal");
	  } else
	  {
		  temp_.resize(a.size());
		  for ( int i = 0; i < a.size(); i++ ) temp_.at(i) = a.at(i) - b.at(i);
	  }

	  return temp_;
  }
*/

}//!namespace


#endif //FUNCTIONS_H
