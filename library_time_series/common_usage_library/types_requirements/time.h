//
// Created by Andrey Solovyev on 01/04/2023.
//

#pragma once

#include <chrono>
#include <type_traits>
#ifdef __cpp_concepts
#include <concepts>
#endif

#ifndef TIME_H
#define TIME_H

namespace culib::requirements {

#ifndef __cpp_concepts

  template <typename Duration, typename = void>
  struct MaybeDuration : std::false_type{};

  template <typename Duration>
  struct MaybeDuration <Duration,
						std::void_t<
								decltype( std::chrono::duration_values<Duration>::zero() ),
								decltype( std::chrono::duration_values<Duration>::min() ),
								decltype( std::chrono::duration_values<Duration>::max() ),
								decltype(std::chrono::duration_cast<std::chrono::minutes>(std::declval<Duration>()))
						>
  > : std::true_type{};


  template <typename Duration>
  using IsDuration = std::enable_if_t<MaybeDuration<Duration>::value, bool>;

  template <typename Duration>
  using IsNotDuration = std::enable_if_t<!MaybeDuration<Duration>::value, bool>;

  template <typename Duration, IsDuration<Duration> = true>
  constexpr bool isDuration_v () { return true; }

  template <typename Duration, IsNotDuration<Duration> = true>
  constexpr bool isDuration_v () { return false; }


  template <typename SomeType>
  using CanBeDuration  = std::enable_if_t<
		  std::disjunction_v<
		  	MaybeDuration<SomeType>,
		  	std::is_arithmetic_v<SomeType>
		  >,
		  bool>;


  /**
   * @details
   * form here\n
   * https://en.cppreference.com/w/cpp/chrono/is_clock \n
   * */
  template<typename T, typename = void>
  struct MaybeClock : std::false_type {};

  template<class T>
  struct MaybeClock <T, std::void_t<
		  typename T::rep,
		  typename T::period,
		  typename T::duration,
		  typename T::time_point,
		  decltype(T::is_steady),
		  decltype(T::now())>
  > : std::true_type {};

  template <typename Clock>
  using IsClock = std::enable_if_t<MaybeClock<Clock>::value, bool>;

  template <typename Clock>
  using IsNotClock = std::enable_if_t<!MaybeClock<Clock>::value, bool>;

  template <typename Clock, IsClock<Clock> = true>
  bool constexpr isClock_v () { return true; }

  template <typename Clock, IsNotClock<Clock> = true>
  bool constexpr isClock_v () { return false; }


#else

  template<class T>
  concept IsClock = requires {
	  typename T::rep;
	  typename T::period;
	  typename T::duration;
	  typename T::time_point;
	  T::is_steady;
	  T::now();
  };

  template <typename Clock>
  concept IsNotClock = !IsClock<Clock>;

  template <IsClock Clock>
  bool constexpr isClock_v () { return true; }

  template <IsNotClock Clock>
  bool constexpr isClock_v () { return false; }


  template <typename Duration>
  concept IsDuration = requires() {
	  std::chrono::duration_values<Duration>::zero();
	  std::chrono::duration_values<Duration>::min();
	  std::chrono::duration_values<Duration>::max();
	  std::chrono::duration_cast<std::chrono::minutes>(std::declval<Duration>());
  };

  template <typename Duration>
  concept IsNotDuration = !IsDuration<Duration>;

  template <IsDuration Duration>
  constexpr bool isDuration_v () { return true; }

  template <IsNotDuration Duration>
  constexpr bool isDuration_v () { return false; }


#endif


}//!namespace



#endif //TIME_H
