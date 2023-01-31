//
// Created by Andrey Solovyev on 19/06/2022.
//

#pragma once

#include "const_values.h"
#include "time_series//value.h"

#include <iosfwd>
#include <string>
#include <stdexcept>
#include <initializer_list>

#ifndef FIN_OHLCV_H
#define FIN_OHLCV_H

namespace financial {

  //todo: add requirement for Other - should be an Arithmetic or better - operator is defined?

  template <typename Duration>
  struct OHLCV final {

	  struct AllFields{};
	  struct Open{};
	  struct High{};
	  struct Low{};
	  struct Close{};
	  struct Volume{};

	  OHLCV () = default;
	  OHLCV (std::initializer_list<base::Value<Duration>> values);

	  std::string toString() const;

	  base::Value<Duration> open, high, low, close, volume {0.0};
  };

  template <typename Duration>
  OHLCV<Duration>::OHLCV (std::initializer_list<base::Value<Duration>> values) {
	  using namespace std::string_literals;
	  if (values.size() != 5u) {
		  throw std::invalid_argument (
				  "OHLCV ctor doesn't get correct arg count. Received "s +
				  std::to_string(values.size()) +
				  " instead of 5"s);
	  }
	  open = *(values.begin());
	  high = *(values.begin() + 1);
	  low = *(values.begin() + 2);
	  close = *(values.begin() + 3);
	  volume = *(values.begin() + 4);
  }

  template <typename Duration>
  std::string OHLCV<Duration>::toString() const {
	  std::string msg;
	  msg.reserve(const_values::EXPECTED_OHLCV_LENGTH);
	  msg.append(open.toString());
	  msg.append(", ");
	  msg.append(high.toString());
	  msg.append(", ");
	  msg.append(low.toString());
	  msg.append(", ");
	  msg.append(close.toString());
	  msg.append(", ");
	  msg.append(volume.toString());
	  return msg;
  }

  template <typename Duration, typename CompareBy = typename OHLCV<Duration>::AllFields>
  bool operator==(const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open == rhs.open &&
				  lhs.high == rhs.high &&
				  lhs.low == rhs.low &&
				  lhs.close == rhs.close &&
				  lhs.volume == rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open == rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high == rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low == rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close == rhs.close;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume == rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator == (const OHLCV<Duration>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open == rhs &&
				  lhs.high == rhs &&
				  lhs.low == rhs &&
				  lhs.close == rhs &&
				  lhs.volume == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume == rhs;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator == (const Other& lhs, const OHLCV<Duration>& rhs) {
	  return rhs == lhs;
  }

  template <typename Duration, typename CompareBy = typename OHLCV<Duration>::AllFields>
  bool operator != (const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator != (const OHLCV<Duration>& lhs, const Other& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator != (const Other& lhs, const OHLCV<Duration>& rhs) {
	  return !(rhs == lhs);
  }

  template <typename Duration, typename CompareBy = typename OHLCV<Duration>::AllFields>
  bool operator < (const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open < rhs.open &&
				  lhs.high < rhs.high &&
				  lhs.low < rhs.low &&
				  lhs.close < rhs.close &&
				  lhs.volume < rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open < rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high < rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low < rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close < rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume < rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator < (const OHLCV<Duration>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open < rhs &&
				  lhs.high < rhs &&
				  lhs.low < rhs &&
				  lhs.close < rhs &&
				  lhs.volume < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high > rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close < rhs;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume < rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator < (const Other& rhs, const OHLCV<Duration>& lhs) {
	  return (!(lhs < rhs) && !(lhs == rhs));
  }

  template <typename Duration, typename CompareBy = typename OHLCV<Duration>::AllFields>
  bool operator > (const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator > (const OHLCV<Duration>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator > (const Other& lhs, const OHLCV<Duration>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }

  template <typename Duration, typename CompareBy = typename OHLCV<Duration>::AllFields>
  bool operator <= (const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open <= rhs.open &&
				  lhs.high <= rhs.high &&
				  lhs.low <= rhs.low &&
				  lhs.close <= rhs.close &&
				  lhs.volume <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open <= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high <= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low <= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close <= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume <= rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator <= (const OHLCV<Duration>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open <= rhs &&
				  lhs.high <= rhs &&
				  lhs.low <= rhs &&
				  lhs.close <= rhs &&
				  lhs.volume <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume <= rhs;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator <= (const Other& lhs, const OHLCV<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs <= rhs.open &&
				  lhs <= rhs.high &&
				  lhs <= rhs.low &&
				  lhs <= rhs.close &&
				  lhs <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs <= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs <= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs <= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs <= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs <= rhs.volume;
	  }
	  return res;
  }

  template <typename Duration, typename CompareBy = typename OHLCV<Duration>::AllFields>
  bool operator >= (const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open >= rhs.open &&
				  lhs.high >= rhs.high &&
				  lhs.low >= rhs.low &&
				  lhs.close >= rhs.close &&
				  lhs.volume >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open >= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high >= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low >= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close >= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume >= rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator >= (const OHLCV<Duration>& lhs, const Other& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs.open >= rhs &&
				  lhs.high >= rhs &&
				  lhs.low >= rhs &&
				  lhs.close >= rhs &&
				  lhs.volume >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs.open >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs.high >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs.low >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs.close >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs.volume >= rhs;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::Comparable<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename OHLCV<Duration>::Close
  >
  bool operator >= (const Other& lhs, const OHLCV<Duration>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::AllFields>) {
		  res = res &&
				  lhs >= rhs.open &&
				  lhs >= rhs.high &&
				  lhs >= rhs.low &&
				  lhs >= rhs.close &&
				  lhs >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Open>) {
		  res = res &&
				  lhs >= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::High>) {
		  res = res &&
				  lhs >= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Low>) {
		  res = res &&
				  lhs >= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Close>) {
		  res = res &&
				  lhs >= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<Duration>::Volume>) {
		  res = res &&
				  lhs >= rhs.volume;
	  }
	  return res;
  }


  template <typename Duration>
  OHLCV<Duration> operator*(const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  return OHLCV<Duration> {
			  .open= lhs.open * rhs.open,
			  .high= lhs.high * rhs.high,
			  .low= lhs.low * rhs.low,
			  .close= lhs.close * rhs.close,
			  .volume= lhs.volume * rhs.volume
	  };
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration> operator*(const OHLCV<Duration>& lhs, Other rhs) {
	  OHLCV<Duration> res;
	  res.open = lhs.open * rhs;
	  res.high = lhs.high * rhs;
	  res.low = lhs.low * rhs;
	  res.close = lhs.close * rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume * rhs;
	  }
	  return res;
  }

  template <typename Duration>
  OHLCV<Duration> operator / (const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  return OHLCV<Duration> {
			  .open= lhs.open / rhs.open,
			  .high= lhs.high / rhs.high,
			  .low= lhs.low / rhs.low,
			  .close= lhs.close / rhs.close,
			  .volume= lhs.volume / rhs.volume
	  };
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration> operator/(const OHLCV<Duration>& lhs, Other rhs) {
	  OHLCV<Duration> res;
	  res.open = lhs.open / rhs;
	  res.high = lhs.high / rhs;
	  res.low = lhs.low / rhs;
	  res.close = lhs.close / rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume / rhs;
	  }
	  return res;
  }
  template <typename Duration>
  OHLCV<Duration> operator + (const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  return OHLCV<Duration> {
			  .open= lhs.open + rhs.open,
			  .high= lhs.high + rhs.high,
			  .low= lhs.low + rhs.low,
			  .close= lhs.close + rhs.close,
			  .volume= lhs.volume + rhs.volume
	  };
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration> operator + (const OHLCV<Duration>& lhs, Other rhs) {
	  OHLCV<Duration> res;
	  res.open = lhs.open + rhs;
	  res.high = lhs.high + rhs;
	  res.low = lhs.low + rhs;
	  res.close = lhs.close + rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume + rhs;
	  }
	  return res;
  }
  template <typename Duration>
  OHLCV<Duration> operator-(const OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  return OHLCV<Duration> {
			  .open= lhs.open - rhs.open,
			  .high= lhs.high - rhs.high,
			  .low= lhs.low - rhs.low,
			  .close= lhs.close - rhs.close,
			  .volume= lhs.volume - rhs.volume
	  };
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration> operator - (const OHLCV<Duration>& lhs, Other rhs) {
	  OHLCV<Duration> res;
	  res.open = lhs.open - rhs;
	  res.high = lhs.high - rhs;
	  res.low = lhs.low - rhs;
	  res.close = lhs.close - rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume - rhs;
	  }
	  return res;
  }


  template <typename Duration>
  OHLCV<Duration>& operator += (OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  lhs.open += rhs.open;
	  lhs.high += rhs.high;
	  lhs.low += rhs.low;
	  lhs.close += rhs.close;
	  lhs.volume += rhs.volume;
	  return lhs;
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration>& operator += (OHLCV<Duration>& lhs, Other rhs) {
	  lhs.open += rhs;
	  lhs.high += rhs;
	  lhs.low += rhs;
	  lhs.close += rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume += rhs;
	  }
	  return lhs;
  }

  template <typename Duration>
  OHLCV<Duration>& operator -= (OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  lhs.open -= rhs.open;
	  lhs.high -= rhs.high;
	  lhs.low -= rhs.low;
	  lhs.close -= rhs.close;
	  lhs.volume -= rhs.volume;
	  return lhs;
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration>& operator -= (OHLCV<Duration>& lhs, Other rhs) {
	  lhs.open -= rhs;
	  lhs.high -= rhs;
	  lhs.low -= rhs;
	  lhs.close -= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume -= rhs;
	  }
	  return lhs;
  }

  template <typename Duration>
  OHLCV<Duration>& operator *= (OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  lhs.open *= rhs.open;
	  lhs.high *= rhs.high;
	  lhs.low *= rhs.low;
	  lhs.close *= rhs.close;
	  lhs.volume *= rhs.volume;
	  return lhs;
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration>& operator *= (OHLCV<Duration>& lhs, Other rhs) {
	  lhs.open *= rhs;
	  lhs.high *= rhs;
	  lhs.low *= rhs;
	  lhs.close *= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume *= rhs;
	  }
	  return lhs;
  }

  template <typename Duration>
  OHLCV<Duration>& operator /= (OHLCV<Duration>& lhs, const OHLCV<Duration>& rhs) {
	  lhs.open /= rhs.open;
	  lhs.high /= rhs.high;
	  lhs.low /= rhs.low;
	  lhs.close /= rhs.close;
	  lhs.volume /= rhs.volume;
	  return lhs;
  }
//todo: concept on Other?
  template <typename Duration, typename Other, bool VolumeToo = false>
  OHLCV<Duration>& operator /= (OHLCV<Duration>& lhs, Other rhs) {
	  lhs.open /= rhs;
	  lhs.high /= rhs;
	  lhs.low /= rhs;
	  lhs.close /= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume /= rhs;
	  }
	  return lhs;
  }



  template <typename Duration>
  std::ostream& operator<<(std::ostream& os, const OHLCV<Duration>& ohlcv) {
	  return os
			  << ohlcv.open << ", "
			  << ohlcv.high << ", "
			  << ohlcv.low << ", "
			  << ohlcv.close << ", "
			  << ohlcv.volume;
  }//!operator
  template <typename Duration>
  std::istream& operator>>(std::istream& is, OHLCV<Duration>& ohlcv) {
	  base::Timestamp<Duration> timestamp;
	  typename OHLCV<Duration>::Type open, high, low, close, volume {0.0};
	  bool ohlc_read_ok {false}, volume_read_ok {false};
	  if (is && !is.eof()) {
		  is >> open >> high >> low >> close;
		  if (is && !is.eof()) {
			  ohlc_read_ok = true;
			  is >> volume;
		  }
		  if (is && !is.eof()) {
			  volume_read_ok = true;
		  }

		  if (ohlc_read_ok) {
			  ohlcv.open = open;
			  ohlcv.high = high;
			  ohlcv.low = low;
			  ohlcv.close = close;
			  if (volume_read_ok) ohlcv.volume = volume;
		  }
		  else {
			  is.setstate(std::ios_base::failbit);
		  }
	  }
	  return is;
  }//!operator


}//!namespace


#endif //FIN_OHLCV_H
