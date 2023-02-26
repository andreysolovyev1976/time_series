//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include <random>
#include <chrono>
#include <type_traits>
#include <stdexcept>

#ifndef RANDOMER_H
#define RANDOMER_H

namespace base::utils {

  class Randomer {

	  template <typename T>
	  using IsArithmetic = std::enable_if_t<std::is_arithmetic_v<T>, bool>;
	  template <typename T>
	  using IsIntegral = std::enable_if_t<std::is_integral_v<T>, bool>;
	  template <typename T>
	  using IsFloating = std::enable_if_t<std::is_floating_point_v<T>, bool>;

  public:
	  using UniformedInt = std::uniform_int_distribution<>;
	  using UniformedReal = std::uniform_real_distribution<>;

//	Randomer() : mtre{std::random_device{}()} {}//!ctor
	  Randomer() { mtre.seed(setSeed()); } //!ctor

	  template <typename T, IsArithmetic<T> = true>
	  T operator () (T lower_bound, T upper_bound ) {
		  if constexpr (std::is_same_v<bool, IsIntegral<T>>) {
			  return UniformedInt{lower_bound, upper_bound}(mtre);
		  }
		  else if constexpr (std::is_same_v<bool, IsFloating<T>>) {
			  return UniformedReal{lower_bound, upper_bound}(mtre);
		  }
		  else {
			  throw std::invalid_argument("Unexpected argument for Randomer::operator()");
		  }
	  }

	  int setSeed(){
//		microseconds ms = duration_cast<microseconds>(system_clock::now().time_since_epoch());
		  std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
		  double rndBase = s.count() / 13. * 3.141592653589793;
		  int seed = (rndBase - int(rndBase)) * 1e7;
		  if (seed < 0) seed = -seed;
		  return seed;
	  }//!func

  private:

	  std::mt19937 mtre;

  };//!class

  static inline
  Randomer randomer;
}//!namespace


#endif //RANDOMER_H
