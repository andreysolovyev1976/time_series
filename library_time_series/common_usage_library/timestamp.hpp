//
// Created by Andrey Solovyev on 02/11/2021.
//

#pragma once

#include "date/date.h"

#include "types_requirements/time.h"
#include "types_requirements/numerics.h"

#include <string>
#include <ctime>
#include <chrono>
#include <ratio>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <iosfwd>

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

namespace culib::time {


  using SysClock = std::chrono::system_clock;
  using UtcClock = std::chrono::utc_clock;
  using HiResClock = std::chrono::high_resolution_clock;
  using DefaultClock = SysClock;


#ifndef __cpp_concepts
  template<
		  typename ClockType = DefaultClock,
          typename Duration = typename ClockType::duration,
		  requirements::IsClock<ClockType> = true,
		  requirements::IsDuration<Duration> = true>
#else
  template<
		  requirements::IsClock ClockType = DefaultClock,
		  requirements::IsDuration Duration = typename ClockType::duration
  >
#endif
  using Timepoint = std::chrono::time_point<ClockType, Duration>;

#ifndef __cpp_concepts
  template<
		  typename Duration,
          typename ClockType = DefaultClock,
		  requirements::IsDuration<Duration> = true,
		  requirements::IsClock<ClockType> = true>
#else
  template<
		  requirements::IsDuration Duration,
		  requirements::IsClock ClockType = DefaultClock>
#endif
  struct Timestamp final {
	  using clock_type = ClockType;
	  using duration_type = Duration;

	  // todo: check floor is ok
	  Timestamp() noexcept : time_point(std::chrono::floor<Duration>(ClockType::now()))
	  {}

	  auto getNumeric () const noexcept { return time_point.time_since_epoch().count(); }

	  std::string toString () const {
		  std::string output;
		  output.reserve(24);
		  output += date::format("%F", time_point);
		  output += ' ';
		  output += date::format("%T", time_point);
		  return output;
	  }

	  Timepoint<ClockType, Duration> time_point;
  };

#ifndef __cpp_concepts
  template<typename Duration, requirements::IsDuration<Duration> = true>
#else
  template<requirements::IsDuration Duration>
#endif
  struct TimestampHasher {
	  std::size_t operator () (const Timestamp<Duration>& t) const {
		  return t.getNumeric();
	  }
  };


  using Nanoseconds = std::chrono::nanoseconds;
  using Microseconds = std::chrono::microseconds;
  using Milliseconds = std::chrono::milliseconds;
  using Seconds = std::chrono::seconds;
  using ThirtySeconds = std::chrono::duration<long, std::ratio<30>>;
  using Minutes = std::chrono::minutes;
  using FiveMinutes = std::chrono::duration<long, std::ratio<300>>;


  /*
typedef duration<long long,         nano> nanoseconds;
typedef duration<long long,        micro> microseconds;
typedef duration<long long,        milli> milliseconds;
typedef duration<long long              > seconds;
typedef duration<     long, ratio<  60> > minutes;
typedef duration<     long, ratio<3600> > hours;
*/


  template <typename Duration>
  constexpr bool IsMicroseconds (Duration) {
	  return std::is_same_v<Duration, Microseconds>;
  }
  template <typename Duration>
  constexpr bool IsMilliseconds (Duration) {
	  return std::is_same_v<Duration, Milliseconds>;
  }
  template <typename Duration>
  constexpr bool IsSeconds (Duration) {
	  return std::is_same_v<Duration, Seconds>;
  }
  template <typename Duration>
  constexpr bool IsMinutes (Duration) {
	  return std::is_same_v<Duration, Minutes>;
  }


#ifndef __cpp_concepts
  template<typename Duration, requirements::IsDuration<Duration> = true>
#else
  template<requirements::IsDuration Duration>
#endif
  static constexpr auto ZeroDuration {Duration::zero()};


#ifndef __cpp_concepts
  template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
  template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
  bool operator==(const Timestamp<Duration1>& lhs, const Timestamp<Duration2>& rhs) {
	  return lhs.time_point == rhs.time_point;
  }

#ifndef __cpp_concepts
  template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
  template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
  bool operator!=(const Timestamp<Duration1>& lhs, const Timestamp<Duration2>& rhs){
	  return not (lhs==rhs);
  }

#ifndef __cpp_concepts
  template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
  template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
  bool operator < (const Timestamp<Duration1>& lhs, const Timestamp<Duration2>& rhs){
	  return lhs.time_point < rhs.time_point;
  }

#ifndef __cpp_concepts
  template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
  template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
  bool operator > (const Timestamp<Duration1>& lhs, const Timestamp<Duration2>& rhs){
	  return not (lhs < rhs) && not (lhs == rhs);
  }

#ifndef __cpp_concepts
  template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
  template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
  bool operator<=(const Timestamp<Duration1>& lhs, const Timestamp<Duration2>& rhs){
	  return not (lhs > rhs);
  }

#ifndef __cpp_concepts
  template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
  template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
  bool operator>=(const Timestamp<Duration1>& lhs, const Timestamp<Duration2>& rhs) {
	  return not (lhs < rhs);
  }

  template<typename Duration>
  std::ostream& operator<<(std::ostream& os, const Timestamp<Duration>& timestamp){
	  os << timestamp.toString();
	  return os;
  }

  template<typename Duration>
  Timestamp<Duration> fromString(std::string input, const std::string& input_fmt){
	  Timestamp<Duration> timestamp;
	  std::stringstream ss(std::move(input));
	  date::from_stream(ss, input_fmt.c_str(), timestamp.time_point);
	  return timestamp;
  }


#ifndef __cpp_concepts
  template<typename Duration, typename Number,
		  requirements::IsDuration<Duration> = true,
		  requirements::CanBeDuration<Number> = true>
#else
  template<typename Duration, typename Number>
  requires requirements::IsDuration<Duration> &&
		  (requirements::IsArithmetic<Number> || requirements::IsDuration<Duration>)
#endif
  Timestamp<Duration>& operator -= (Timestamp<Duration>& lhs, Number rhs) {
	  if constexpr (requirements::IsArithmetic<Number>) {
		  lhs.time_point -= Duration {rhs};
	  }
	  else if constexpr (requirements::IsDuration<Number>) {
		  lhs.time_point -= rhs;
	  }
	  return lhs;
  }
#ifndef __cpp_concepts
  template<typename Duration, typename Number,
		  requirements::IsDuration<Duration> = true,
		  requirements::CanBeDuration<Number> = true>
#else
  template<typename Duration, typename Number>
  requires requirements::IsDuration<Duration> &&
		  (requirements::IsArithmetic<Number> || requirements::IsDuration<Duration>)
#endif
  Timestamp<Duration> operator - (Timestamp<Duration> const& lhs, Number rhs){
	  Timestamp<Duration> result{lhs};
	  result -= rhs;
	  return result;
  }



#ifndef __cpp_concepts
  template<typename Duration, typename Number,
		  requirements::IsDuration<Duration> = true,
		  requirements::CanBeDuration<Number> = true>
#else
  template<typename Duration, typename Number>
  requires requirements::IsDuration<Duration> &&
		  (requirements::IsArithmetic<Number> || requirements::IsDuration<Duration>)
#endif
  Timestamp<Duration>& operator += (Timestamp<Duration>& lhs, Number rhs) {
	  if constexpr (requirements::IsArithmetic<Number>) {
		  lhs.time_point += Duration {rhs};
	  }
	  else if constexpr (requirements::IsDuration<Number>) {
		  lhs.time_point += rhs;
	  }
	  return lhs;
  }

#ifndef __cpp_concepts
  template<typename Duration, typename Number,
		  requirements::IsDuration<Duration> = true,
		  requirements::CanBeDuration<Number> = true>
#else
  template<typename Duration, typename Number>
  requires requirements::IsDuration<Duration> &&
		  (requirements::IsArithmetic<Number> || requirements::IsDuration<Duration>)
#endif
  Timestamp<Duration> operator + (Timestamp<Duration> const& lhs, Number rhs){
	  Timestamp<Duration> result{lhs};
	  result += rhs;
	  return result;
  }



#ifndef __cpp_concepts
  template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
  template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
  auto operator-(const Timestamp<Duration1>& lhs, const Timestamp<Duration2>& rhs) {
	  return lhs.time_point - rhs.time_point;
  }

}//!namespace

#endif //TIMESTAMP_H
