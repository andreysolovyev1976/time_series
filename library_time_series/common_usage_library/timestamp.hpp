//
// Created by Andrey Solovyev on 02/11/2021.
//

#pragma once

#include "date/date.h"

#include "types_requirements/time.h"

#include <string>
#include <ctime>
#include <chrono>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <stdexcept>

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

namespace culib::time {

  namespace details {
#ifndef __cpp_concepts
	template<typename Duration1, typename Duration2,
		  requirements::IsDuration<Duration1> = true,
		  requirements::IsDuration<Duration2> = true>
#else
	template<requirements::IsDuration Duration1, requirements::IsDuration Duration2>
#endif
	constexpr auto shorter() noexcept
	{
		using R1 = typename Duration1::period;
		using R2 = typename Duration2::period;
		if constexpr (!(requirements::is_ratio_v<R1> && requirements::is_ratio_v<R2>)) {
			throw std::invalid_argument("can't compare durations");
		}
		return std::ratio_less_v<R1, R2>;
	}

	template<typename Duration1, typename Duration2>
	inline bool constexpr is_shorter_v { shorter<Duration1, Duration2>() };

#ifndef __cpp_concepts
	template<typename Duration1, typename Duration2>
	using ConversionAllowed = std::enable_if_t<is_shorter_v<Duration1, Duration2>, bool>;
#else
	template<typename Duration1, typename Duration2>
	concept ConversionAllowed = is_shorter_v<Duration1, Duration2>;

#endif

  }//!namespace


  using SysClock = std::chrono::system_clock;
  using HiResClock = std::chrono::high_resolution_clock;

#if defined(__cplusplus) && (__cplusplus>201703L)
  using UtcClock = std::chrono::utc_clock;
  using TaiClock = std::chrono::tai_clock;
  using GpsClock = std::chrono::gps_clock;
#endif
  using DefaultClock = HiResClock;


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
	  Timepoint<ClockType, Duration> time_point;

	  using clock_type = ClockType;
	  using duration_type = Duration;
	  using rep_type = typename Timepoint<ClockType, Duration>::rep;

	  // todo: check floor is ok
	  Timestamp() noexcept : time_point(std::chrono::floor<Duration>(ClockType::now()))
	  {}

	  std::string toString () const {
		  std::string output;
		  output.reserve(24);
		  output += date::format("%F", time_point);
		  output += ' ';
		  output += date::format("%T", time_point);
		  return output;
	  }

	  operator rep_type () const noexcept { return time_point.time_since_epoch().count(); }

	  auto getNumeric () const noexcept { return time_point.time_since_epoch().count(); }



#ifndef __cpp_concepts
	  template<typename DurationTo, details::ConversionAllowed<Duration, DurationTo> = true>
#else
	  template<requirements::IsDuration DurationTo>
	  requires details::ConversionAllowed<Duration, DurationTo>
#endif
	  operator Timestamp<DurationTo> () noexcept {
		  Timestamp<DurationTo> result;
		  result.time_point = std::chrono::time_point_cast<DurationTo>(time_point);
		  return result;
	  }


#ifndef __cpp_concepts
	  template<typename DurationTo, details::ConversionAllowed<Duration, DurationTo> = true>
#else
	  template<requirements::IsDuration DurationTo>
	  requires details::ConversionAllowed<Duration, DurationTo>
#endif
	decltype(auto) castTo () const noexcept {
		  Timestamp<DurationTo> result;
		  result.time_point = std::chrono::time_point_cast<DurationTo>(time_point);
		  return result;
	  }
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
  using FiveSeconds = std::chrono::duration<long, std::ratio<5>>;
  using TenSeconds = std::chrono::duration<long, std::ratio<10>>;
  using ThirtySeconds = std::chrono::duration<long, std::ratio<30>>;
  using Minutes = std::chrono::minutes;
  using TwoMinutes = std::chrono::duration<long, std::ratio<120>>;
  using ThreeMinutes = std::chrono::duration<long, std::ratio<180>>;
  using FiveMinutes = std::chrono::duration<long, std::ratio<300>>;
  using TenMinutes = std::chrono::duration<long, std::ratio<600>>;
  using FifteenMinutes = std::chrono::duration<long, std::ratio<900>>;
  using ThirtyMinutes = std::chrono::duration<long, std::ratio<1'800>>;
  using Hours = std::chrono::duration<long, std::ratio<3'600>>;
  using TwoHours = std::chrono::duration<long, std::ratio<7'200>>;
  using ThreeHours = std::chrono::duration<long, std::ratio<10'800>>;
  using SixHours = std::chrono::duration<long, std::ratio<21'600>>;
  using EightHours = std::chrono::duration<long, std::ratio<28'800>>;
  using TwelveHours = std::chrono::duration<long, std::ratio<43'200>>;
  using Days = std::chrono::duration<long, std::ratio<86'400>>;
  using TwoDays = std::chrono::duration<long, std::ratio<172'800>>;
  using ThreeDays = std::chrono::duration<long, std::ratio<259'200>>;
  using Weeks = std::chrono::duration<long, std::ratio<604'800>>;
  using Months = std::chrono::duration<long, std::ratio<2'628'000>>;
  using Quarters = std::chrono::duration<long, std::ratio<7'884'000>>;
  using Year = std::chrono::duration<long, std::ratio<31'536'000>>;
//todo: buisness days
//todo: weeks
//todo: month


#if 0
//a hint from std::chrono::
typedef duration<long long,         nano> nanoseconds;
typedef duration<long long,        micro> microseconds;
typedef duration<long long,        milli> milliseconds;
typedef duration<long long              > seconds;
typedef duration<     long, ratio<  60> > minutes;
typedef duration<     long, ratio<3600> > hours;
#endif

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
	  if constexpr (std::is_arithmetic_v<Number>) {
		  lhs.time_point -= Duration {rhs};
	  }
	  else if constexpr (requirements::is_duration_v<Number>) {
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
	  if constexpr (std::is_arithmetic_v<Number>) {
		  lhs.time_point += Duration {rhs};
	  }
	  else if constexpr (requirements::is_duration_v<Number>) {
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
