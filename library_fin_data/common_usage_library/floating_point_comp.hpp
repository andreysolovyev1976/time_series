//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include "types_requirements/numerics.h"
#include "const_values.h"

#include <mutex>

#ifndef BASE_VALUE_COMP_H
#define BASE_VALUE_COMP_H

namespace culib::comp {
/**
 * @details
 * overloads the operators to compare double and floats (floating_point)
 * with a required precision, that can be changed runtime.\n
 * Implementation provides strong protection for keeping just one instance of a
 * kValue that is used in comparison - see all deleted ctors.
 */
  template<typename T, requirements::IsFloatinPoint<T> = true>
  struct CompareWithPrecision final {
  public:
	  CompareWithPrecision() = delete;
	  CompareWithPrecision(CompareWithPrecision const&) = delete;
	  CompareWithPrecision& operator=(CompareWithPrecision const&) = delete;
	  CompareWithPrecision(CompareWithPrecision&&) = delete;
	  CompareWithPrecision& operator=(CompareWithPrecision&&) = delete;

	  static
	  CompareWithPrecision& getStaticInstance(T precision)
	  {
		  static CompareWithPrecision cmp(precision);
		  return cmp;
	  }
	  void setEpsilon(T e) {
		  std::lock_guard<std::mutex> lg(mtx);
		  epsilon = e;
	  }
  private:
	  explicit CompareWithPrecision(T precision)
			  :epsilon(precision)
	  {}
	  std::mutex mtx;

  public:
	  T epsilon;
  };

/**
 * @details
 * a beauty of this lambda is that it is a reasonable usage of IILE -\n
 * Immediately Invoked Lambda Expression,\n
 * for returning a ref to static,
 * as well as decltype(auto) that prevents loosing a lvalue reference
 * from a return statement, that would be really unpleasant considering
 * all deleted ctors - see that above.
 */
  static
  decltype(auto) floating_comp =
		  []() -> CompareWithPrecision<double>& {
			return CompareWithPrecision<double>::getStaticInstance(culib::const_values::EPSILON_BY_DEFAULT);
		  }();

/**
 * @details
 * operators for basic types can't be overloaded
 * */


  template<typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  bool eq(T a, U b) noexcept
  {
	  return (((a - b) < floating_comp.epsilon) &&
			  ((b - a) < floating_comp.epsilon));
  }
  template<typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  bool eq(U b, T a) noexcept
  {
	  return eq(a, b);
  }
  template<typename T,
		  requirements::IsFloatinPoint<T> = true>
  bool eq(T a, T b) noexcept
  {
	  return (((a - b) < floating_comp.epsilon) &&
			  ((b - a) < floating_comp.epsilon));
  }


  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool ne (T a, U b) noexcept
  {
	  return not eq(a, b);
  }
  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool ne (U b, T a) noexcept
  {
	  return not eq(a, b);
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  static bool ne (T a, T b) noexcept
  {
	  return not eq(a, b);
  }

  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool lt (T a, U b) noexcept
  {
	  return a < b - floating_comp.epsilon;
  }
  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool lt (U b, T a) noexcept
  {
	  return b < a - floating_comp.epsilon;;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  static bool lt (T a, T b) noexcept
  {
	  return a < b - floating_comp.epsilon;
  }


  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool gt (T a, U b) noexcept
  {
	  return a > b + floating_comp.epsilon;
  }
  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool gt (U b, T a) noexcept
  {
	  return b > a + floating_comp.epsilon;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  static bool gt (T a, T b) noexcept
  {
	  return a > b + floating_comp.epsilon;
  }


  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool ge (T a, U b) noexcept
  {
	  return a > b - floating_comp.epsilon;
  }
  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool ge (U b, T a) noexcept
  {
	  return b > a - floating_comp.epsilon;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  static bool ge (T a, T b) noexcept
  {
	  return a > b - floating_comp.epsilon;
  }


  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool le (T a, U b) noexcept
  {
	  return a < b + floating_comp.epsilon;
  }
  template <typename T, typename U,
		  requirements::IsFloatinPoint<T> = true,
		  requirements::IsIntegral<U> = true>
  static bool le (U b, T a) noexcept
  {
	  return b < a + floating_comp.epsilon;
  }
  template <typename T,
		  requirements::IsFloatinPoint<T> = true>
  static bool le (T a, T b) noexcept
  {
	  return a < b + floating_comp.epsilon;
  }
}//!namespace

#endif //BASE_VALUE_COMP_H
