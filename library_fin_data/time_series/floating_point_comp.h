//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include "type_requirements.h"
#include "const_values.h"

#ifndef BASE_VALUE_COMP_H
#define BASE_VALUE_COMP_H

namespace base {

/**
 * @details
 * overloads the operators to compare double and floats (floating_point)
 * with a required precision, that can be changed runtime.
 * Implementation provides strong protection for keeping just one instance of a
 * kValue that is used in comparison - see all deleted ctors.
 */
  template <typename T, requirements::IsFloatinPoint<T> = true>
  struct CompareWithPrecision final {
  public:
	  CompareWithPrecision() = delete;
	  CompareWithPrecision(CompareWithPrecision const&) = delete;
	  CompareWithPrecision& operator=(CompareWithPrecision const&) = delete;
	  CompareWithPrecision(CompareWithPrecision &&) = delete;
	  CompareWithPrecision& operator=(CompareWithPrecision &&) = delete;

	  static
	  CompareWithPrecision& getStaticInstance (T precision) {
		  static CompareWithPrecision cmp (precision);
		  return cmp;
	  }
	  void setEpsilon (T e) {
		  epsilon = e;
	  }
  private:
	  explicit CompareWithPrecision(T precision) :epsilon(precision) {}
  public:
	  T epsilon;
  };

/**
 * @details
 * a beauty of this lambda is that it is a reasonable usage of IILE -
 * Immediately Invoked Lambda Expression, as well as
 * decltype(auto) that prevents loosing a lvalue reference
 * from a return statement.
 */
  decltype(auto) floating_comp =
		  []() -> CompareWithPrecision<double>& {
			return CompareWithPrecision<double>::getStaticInstance(const_values::EPSILON_BY_DEFAULT);
		  }();

  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true, requirements::IsArithmetic<U> = true>
  bool operator == (T a, U b){
	  return ((a-b) < floating_comp.epsilon) && ((b-a) < floating_comp.epsilon);
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  bool operator == (T a, T b){
	  return ((a-b) < floating_comp.epsilon) && ((b-a) < floating_comp.epsilon);
  }

  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true, requirements::IsArithmetic<U> = true>
  bool operator != (T a, U b){
	  return (not (a == b));
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  bool operator != (T a, T b){
	  return (not (a == b));
  }

  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true, requirements::IsArithmetic<U> = true>
  bool operator < (T a, U b) {
	  return a<b - floating_comp.epsilon;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  bool operator < (T a, T b) {
	  return a<b - floating_comp.epsilon;
  }
  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true, requirements::IsArithmetic<U> = true>
  bool operator > (T a, U b) {
	  return a>b + floating_comp.epsilon;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  bool operator > (T a, T b) {
	  return a>b + floating_comp.epsilon;
  }

  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true, requirements::IsArithmetic<U> = true>
  bool operator >= (T a, U b) {
	  return a>b - floating_comp.epsilon;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  bool operator >= (T a, T b) {
	  return a>b - floating_comp.epsilon;
  }
  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true, requirements::IsArithmetic<U> = true>
  bool operator <= (T a, U b) {
	  return a<b + floating_comp.epsilon;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  bool operator <= (T a, T b) {
	  return a<b + floating_comp.epsilon;
  }

}//!namespace
#endif //BASE_VALUE_COMP_H
