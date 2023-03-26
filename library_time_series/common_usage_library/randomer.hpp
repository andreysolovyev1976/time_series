//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include <chrono>
#include <random>
#include <stdexcept>
#include <type_traits>
#ifdef __cpp_concepts
#include <concepts>
#endif
#include <limits>
#include <mutex>


#ifndef RANDOMER_H
#define RANDOMER_H

namespace culib::utils {

  class Randomer {
  private:
	  template <typename T>
	  using IsArithmetic = std::enable_if_t<std::is_arithmetic_v<T>, bool>;
	  template <typename T>
	  using IsIntegral = std::enable_if_t<std::is_integral_v<T>, bool>;
	  template <typename T>
	  using IsFloating = std::enable_if_t<std::is_floating_point_v<T>, bool>;

	  using UniformedInt = std::uniform_int_distribution<>;
	  using UniformedReal = std::uniform_real_distribution<>;

  public:
	  Randomer (Randomer const&) = delete;
	  Randomer (Randomer &&) = delete;
	  Randomer& operator = (Randomer const&) = delete;
	  Randomer& operator = (Randomer &&) = delete;

	  static
	  Randomer& getInstance () {
		  static Randomer r;
		  return r;
	  }

#ifdef __cpp_concepts
	  template <typename T>
	  requires std::integral<T> || std::floating_point<T>
#else
	  template <typename T, IsArithmetic<T> = true>
#endif
	  T operator()(
			  T lower_bound = std::numeric_limits<T>::min(),
			  T upper_bound = std::numeric_limits<T>::max()) {
		  std::lock_guard<std::mutex> lg (mtx);
#ifdef __cpp_concepts
		  if constexpr (std::integral<T>) {
			  return UniformedInt{lower_bound, upper_bound}(mtre);
		  } else if constexpr (std::floating_point<T>) {
			  return UniformedReal{lower_bound, upper_bound}(mtre);
		  }
#else
		if constexpr (std::is_same_v<bool, IsIntegral<T>>) {
            return UniformedInt{lower_bound, upper_bound}(mtre);
        } else if constexpr (std::is_same_v<bool, IsFloating<T>>) {
            return UniformedReal{lower_bound, upper_bound}(mtre);
        }
#endif
		  else {
			  throw std::invalid_argument("Unexpected argument for Randomer::operator()");
		  }
	  }


  private:
	  std::mt19937 mtre;
	  std::mutex mtx;

	  Randomer() { mtre.seed(setSeed()); } //!ctor

	  int setSeed() {
		  //		microseconds ms = duration_cast<microseconds>(system_clock::now().time_since_epoch());
		  std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
		  double rndBase = s.count() / 13. * 3.141592653589793;
		  int seed = (rndBase - int(rndBase)) * 1e7;
		  return (seed < 0) ? -seed : seed;
	  } //!func

  }; //!class

  /**
   * @brief
   * IILE\n
   * */
  static
  decltype(auto) randomer = []() -> Randomer& {
	return Randomer::getInstance();
  }();

} // namespace culib::utils

#endif //RANDOMER_H
