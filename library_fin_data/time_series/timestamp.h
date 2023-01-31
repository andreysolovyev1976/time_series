//
// Created by Andrey Solovyev on 02/11/2021.
//

#pragma once

#include "date/date.h"

#include <string>
#include <ctime>
#include <chrono>
#include <ratio>
#include <cstdint>
#include <iosfwd>

#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H

namespace base {

    //todo: add is_convertible to duration

	using Clock = std::chrono::system_clock;
	using HiResClock = std::chrono::high_resolution_clock;

	constexpr auto zero() {return std::chrono::nanoseconds::zero();}
  	using Nanoseconds = std::chrono::nanoseconds;
	using Microseconds = std::chrono::microseconds;
	using Milliseconds = std::chrono::milliseconds;
	using Seconds = std::chrono::seconds;
	using Minutes = std::chrono::minutes;
	using FiveMinutes = std::chrono::duration<long, std::ratio<300>>;
	using ThirtySeconds = std::chrono::duration<long, std::ratio<30>>;

	/*
typedef duration<long long,         nano> nanoseconds;
typedef duration<long long,        micro> microseconds;
typedef duration<long long,        milli> milliseconds;
typedef duration<long long              > seconds;
typedef duration<     long, ratio<  60> > minutes;
typedef duration<     long, ratio<3600> > hours;
 */

	//todo: add SFINAE / concept to check Duration is a time constructable


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


	template<typename Duration>
	using TimePoint = std::chrono::time_point<Clock, Duration>;

	template<typename Duration>
	struct Timestamp {
		// todo: check floor is ok
		Timestamp() : time_point(std::chrono::floor<Duration>(Clock::now()))
		{}

		TimePoint<Duration> time_point;

		std::string toString () const {
			std::string output;
			output.reserve(24);
			output += date::format("%F", time_point);
			output += ' ';
			output += date::format("%T", time_point);
			return output;
		}
		void setToZero() noexcept {
			time_point(Clock::from_time_t(0));
		}
		void setToNow() noexcept {
			time_point(std::chrono::floor<Duration>(Clock::now()));
		}
	};

	template <typename Duration>
	struct TimestampHasher {
		size_t operator () (const Timestamp<Duration>& t) const {
			return t.time_point.time_since_epoch().count();
		}
	};

	template<typename Duration>
	bool operator==(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs) {
		return lhs.time_point == rhs.time_point;
	}
	template<typename Duration>
	bool operator!=(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
		return not (rhs==lhs);
	}
	template<typename Duration>
	bool operator < (const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
		return lhs.time_point < rhs.time_point;
	}
	template<typename Duration>
	bool operator >(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
		return not (rhs < lhs);
	}
	template<typename Duration>
	bool operator<=(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
		return not (rhs > lhs);
	}
	template<typename Duration>
	bool operator>=(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs) {
		return not (lhs<rhs);
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

	template<typename Duration>
	Timestamp<Duration> operator - (const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
		Timestamp<Duration> result;
		result.time_point -= std::chrono::duration_cast<Duration>(lhs.time_point - rhs.time_point);
		return result;
	}

	//todo: unfinished
	template<typename Duration>
	Timestamp<Duration> operator + ([[maybe_unused]] const Timestamp<Duration>& lhs, [[maybe_unused]] const Timestamp<Duration>& rhs){
		Timestamp<Duration> result;
//		result.time_point += std::chrono::duration_cast<Duration>(lhs.time_point + rhs.time_point);
		return result;
	}

}//!namespace

#endif //BASE_TIMESTAMP_H
