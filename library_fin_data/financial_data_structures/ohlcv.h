//
// Created by Andrey Solovyev on 19/06/2022.
//

#pragma once

#include "const_values.h"
#include "time_series/value.hpp"

#include <iosfwd>
#include <string>
#include <stdexcept>
#include <initializer_list>

#ifndef FIN_OHLCV_H
#define FIN_OHLCV_H

namespace financial {

  template <typename ValueType = base::traits::ValueTypeDefault>
//  struct OHLCV final : base::traits::ValueBase<OHLCV<ValueType>> { //excluded CRTP, no need for now
  struct OHLCV final  {
	  using value_type = ValueType;

	  struct AllFields{};
	  struct Open{};
	  struct High{};
	  struct Low{};
	  struct Close{};
	  struct Volume{};

	  OHLCV () = default;
	  OHLCV (std::initializer_list<base::Value<ValueType>> values);
	  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>> = true>
	  OHLCV(const Input &input);
	  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>> = true>
	  OHLCV& operator = (const Input &input);

	  std::string toString() const;

	  base::Value<ValueType> open, high, low, close, volume {0.0};
  };

  template <typename ValueType>
  OHLCV<ValueType>::OHLCV (std::initializer_list<base::Value<ValueType>> values) {
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

  template <typename ValueType>
  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>>>
  OHLCV<ValueType>::OHLCV(const Input &input){
	  open = input;
	  high = input;
	  low = input;
	  close = input;
	  volume = input;
  }

  template <typename ValueType>
  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>>>
  OHLCV<ValueType>& OHLCV<ValueType>::operator = (const Input &input) {
	  open = input;
	  high = input;
	  low = input;
	  close = input;
	  volume = input;
	  return *this;
  }

  template <typename ValueType>
  std::string OHLCV<ValueType>::toString() const {
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

  template <typename ValueType, typename CompareBy = typename OHLCV<ValueType>::AllFields>
  bool operator == (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open == rhs.open &&
				  lhs.high == rhs.high &&
				  lhs.low == rhs.low &&
				  lhs.close == rhs.close &&
				  lhs.volume == rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open == rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high == rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low == rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close == rhs.close;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume == rhs.volume;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator == (const OHLCV<ValueType>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open == rhs &&
				  lhs.high == rhs &&
				  lhs.low == rhs &&
				  lhs.close == rhs &&
				  lhs.volume == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume == rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator == (const Other& lhs, const OHLCV<ValueType>& rhs) {
	  return rhs == lhs;
  }

  template <typename ValueType, typename CompareBy = typename OHLCV<ValueType>::AllFields>
  bool operator != (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator != (const OHLCV<ValueType>& lhs, const Other& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator != (const Other& lhs, const OHLCV<ValueType>& rhs) {
	  return !(rhs == lhs);
  }

  template <typename ValueType, typename CompareBy = typename OHLCV<ValueType>::AllFields>
  bool operator < (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open < rhs.open &&
				  lhs.high < rhs.high &&
				  lhs.low < rhs.low &&
				  lhs.close < rhs.close &&
				  lhs.volume < rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open < rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high < rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low < rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close < rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume < rhs.volume;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator < (const OHLCV<ValueType>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open < rhs &&
				  lhs.high < rhs &&
				  lhs.low < rhs &&
				  lhs.close < rhs &&
				  lhs.volume < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high > rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close < rhs;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume < rhs.volume;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator < (const Other& rhs, const OHLCV<ValueType>& lhs) {
	  return (!(lhs < rhs) && !(lhs == rhs));
  }

  template <typename ValueType, typename CompareBy = typename OHLCV<ValueType>::AllFields>
  bool operator > (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator > (const OHLCV<ValueType>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator > (const Other& lhs, const OHLCV<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }

  template <typename ValueType, typename CompareBy = typename OHLCV<ValueType>::AllFields>
  bool operator <= (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open <= rhs.open &&
				  lhs.high <= rhs.high &&
				  lhs.low <= rhs.low &&
				  lhs.close <= rhs.close &&
				  lhs.volume <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open <= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high <= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low <= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close <= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume <= rhs.volume;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator <= (const OHLCV<ValueType>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open <= rhs &&
				  lhs.high <= rhs &&
				  lhs.low <= rhs &&
				  lhs.close <= rhs &&
				  lhs.volume <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume <= rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator <= (const Other& lhs, const OHLCV<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs <= rhs.open &&
				  lhs <= rhs.high &&
				  lhs <= rhs.low &&
				  lhs <= rhs.close &&
				  lhs <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs <= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs <= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs <= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs <= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs <= rhs.volume;
	  }
	  return res;
  }

  template <typename ValueType, typename CompareBy = typename OHLCV<ValueType>::AllFields>
  bool operator >= (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open >= rhs.open &&
				  lhs.high >= rhs.high &&
				  lhs.low >= rhs.low &&
				  lhs.close >= rhs.close &&
				  lhs.volume >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open >= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high >= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low >= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close >= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume >= rhs.volume;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator >= (const OHLCV<ValueType>& lhs, const Other& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs.open >= rhs &&
				  lhs.high >= rhs &&
				  lhs.low >= rhs &&
				  lhs.close >= rhs &&
				  lhs.volume >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs.open >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs.high >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs.low >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs.close >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume >= rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename OHLCV<ValueType>::Close
  >
  bool operator >= (const Other& lhs, const OHLCV<ValueType>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::AllFields>) {
		  res = res &&
				  lhs >= rhs.open &&
				  lhs >= rhs.high &&
				  lhs >= rhs.low &&
				  lhs >= rhs.close &&
				  lhs >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Open>) {
		  res = res &&
				  lhs >= rhs.open;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::High>) {
		  res = res &&
				  lhs >= rhs.high;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Low>) {
		  res = res &&
				  lhs >= rhs.low;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Close>) {
		  res = res &&
				  lhs >= rhs.close;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename OHLCV<ValueType>::Volume>) {
		  res = res &&
				  lhs >= rhs.volume;
	  }
	  return res;
  }


  template <typename ValueType>
  OHLCV<ValueType> operator * (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open= lhs.open * rhs.open;
	  res.high= lhs.high * rhs.high;
	  res.low= lhs.low * rhs.low;
	  res.close= lhs.close * rhs.close;
	  res.volume= lhs.volume * rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator * (const OHLCV<ValueType>& lhs, const Other &rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs.open * rhs;
	  res.high = lhs.high * rhs;
	  res.low = lhs.low * rhs;
	  res.close = lhs.close * rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume * rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator * (const Other &lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs * rhs.open;
	  res.high = lhs * rhs.high;
	  res.low = lhs * rhs.low;
	  res.close = lhs * rhs.close;
	  if constexpr (VolumeToo) {
		  res.volume = lhs * rhs.volume;
	  }
	  return res;
  }

  template <typename ValueType>
  OHLCV<ValueType> operator / (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open= lhs.open / rhs.open;
	  res.high= lhs.high / rhs.high;
	  res.low= lhs.low / rhs.low;
	  res.close= lhs.close / rhs.close;
	  res.volume= lhs.volume / rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator / (const OHLCV<ValueType>& lhs, const Other &rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs.open / rhs;
	  res.high = lhs.high / rhs;
	  res.low = lhs.low / rhs;
	  res.close = lhs.close / rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume / rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator / (const Other &lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs / rhs.open;
	  res.high = lhs / rhs.high;
	  res.low = lhs / rhs.low;
	  res.close = lhs / rhs.close;
	  if constexpr (VolumeToo) {
		  res.volume = lhs / rhs.volume;
	  }
	  return res;
  }
  template <typename ValueType>
  OHLCV<ValueType> operator + (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open= lhs.open + rhs.open;
	  res.high= lhs.high + rhs.high;
	  res.low= lhs.low + rhs.low;
	  res.close= lhs.close + rhs.close;
	  res.volume= lhs.volume + rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator + (const OHLCV<ValueType>& lhs, const Other &rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs.open + rhs;
	  res.high = lhs.high + rhs;
	  res.low = lhs.low + rhs;
	  res.close = lhs.close + rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume + rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator + (const Other &lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs + rhs.open;
	  res.high = lhs + rhs.high;
	  res.low = lhs + rhs.low;
	  res.close = lhs + rhs.close;
	  if constexpr (VolumeToo) {
		  res.volume = lhs + rhs.volume;
	  }
	  return res;
  }
  template <typename ValueType>
  OHLCV<ValueType> operator - (const OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open= lhs.open - rhs.open;
	  res.high= lhs.high - rhs.high;
	  res.low= lhs.low - rhs.low;
	  res.close= lhs.close - rhs.close;
	  res.volume= lhs.volume - rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator - (const OHLCV<ValueType>& lhs, const Other &rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs.open - rhs;
	  res.high = lhs.high - rhs;
	  res.low = lhs.low - rhs;
	  res.close = lhs.close - rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume - rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType> operator - (const Other &lhs, const OHLCV<ValueType>& rhs) {
	  OHLCV<ValueType> res;
	  res.open = lhs - rhs.open;
	  res.high = lhs - rhs.high;
	  res.low = lhs - rhs.low;
	  res.close = lhs - rhs.close;
	  if constexpr (VolumeToo) {
		  res.volume = lhs - rhs.volume;
	  }
	  return res;
  }


  template <typename ValueType>
  OHLCV<ValueType>& operator += (OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  lhs.open += rhs.open;
	  lhs.high += rhs.high;
	  lhs.low += rhs.low;
	  lhs.close += rhs.close;
	  lhs.volume += rhs.volume;
	  return lhs;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType>& operator += (OHLCV<ValueType>& lhs, const Other &rhs) {
	  lhs.open += rhs;
	  lhs.high += rhs;
	  lhs.low += rhs;
	  lhs.close += rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume += rhs;
	  }
	  return lhs;
  }

  template <typename ValueType>
  OHLCV<ValueType>& operator -= (OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  lhs.open -= rhs.open;
	  lhs.high -= rhs.high;
	  lhs.low -= rhs.low;
	  lhs.close -= rhs.close;
	  lhs.volume -= rhs.volume;
	  return lhs;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType>& operator -= (OHLCV<ValueType>& lhs, const Other &rhs) {
	  lhs.open -= rhs;
	  lhs.high -= rhs;
	  lhs.low -= rhs;
	  lhs.close -= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume -= rhs;
	  }
	  return lhs;
  }

  template <typename ValueType>
  OHLCV<ValueType>& operator *= (OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  lhs.open *= rhs.open;
	  lhs.high *= rhs.high;
	  lhs.low *= rhs.low;
	  lhs.close *= rhs.close;
	  lhs.volume *= rhs.volume;
	  return lhs;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType>& operator *= (OHLCV<ValueType>& lhs, const Other &rhs) {
	  lhs.open *= rhs;
	  lhs.high *= rhs;
	  lhs.low *= rhs;
	  lhs.close *= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume *= rhs;
	  }
	  return lhs;
  }

  template <typename ValueType>
  OHLCV<ValueType>& operator /= (OHLCV<ValueType>& lhs, const OHLCV<ValueType>& rhs) {
	  lhs.open /= rhs.open;
	  lhs.high /= rhs.high;
	  lhs.low /= rhs.low;
	  lhs.close /= rhs.close;
	  lhs.volume /= rhs.volume;
	  return lhs;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  OHLCV<ValueType>& operator /= (OHLCV<ValueType>& lhs, const Other &rhs) {
	  lhs.open /= rhs;
	  lhs.high /= rhs;
	  lhs.low /= rhs;
	  lhs.close /= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume /= rhs;
	  }
	  return lhs;
  }



  template <typename ValueType>
  std::ostream& operator << (std::ostream& os, const OHLCV<ValueType>& ohlcv) {
	  return os
			  << ohlcv.open << ", "
			  << ohlcv.high << ", "
			  << ohlcv.low << ", "
			  << ohlcv.close << ", "
			  << ohlcv.volume;
  }//!operator
  template <typename ValueType>
  std::istream& operator >> (std::istream& is, OHLCV<ValueType>& ohlcv) {
	  base::Timestamp<ValueType> timestamp;
	  typename OHLCV<ValueType>::Type open, high, low, close, volume {0.0};
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
