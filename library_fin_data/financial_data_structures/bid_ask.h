//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "time_series/value.h"

#include <iosfwd>
#include <vector>
#include <initializer_list>

#ifndef FIN_BID_ASK_H
#define FIN_BID_ASK_H

namespace financial {

  template <typename ValueType = base::traits::ValueTypeDefault>
//  struct BidAsk final : base::traits::ValueBase<BidAsk<ValueType>> { //excluded CRTP, no need for now
  struct BidAsk final {
	  using value_type = ValueType;

	  struct AllFields{};
	  struct Bid{};
	  struct Ask{};
	  struct Middle{};
	  struct Price{};
	  struct Volume{};

	  BidAsk () = default;
	  BidAsk (std::initializer_list<base::Value<ValueType>> values);
	  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>> = true>
	  BidAsk(const Input &input);
	  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>> = true>
	  BidAsk& operator = (const Input &input);

	  std::string toString() const;

	  base::Value<ValueType> bid, ask, middle, price, volume {0.0};
  };

  template <typename ValueType>
  BidAsk<ValueType>::BidAsk (std::initializer_list<base::Value<ValueType>> values) {
	  using namespace std::string_literals;
	  if (values.size() != 5u) {
		  throw std::invalid_argument (
				  "BidAsk ctor doesn't get correct arg count. Received "s +
						  std::to_string(values.size()) +
						  " instead of 5"s);
	  }
	  bid = *(values.begin());
	  ask = *(values.begin() + 1);
	  middle = *(values.begin() + 2);
	  price = *(values.begin() + 3);
	  volume = *(values.begin() + 4);
  }

  template <typename ValueType>
  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>>>
  BidAsk<ValueType>::BidAsk(const Input &input){
	  bid = input;
	  ask = input;
	  middle = input;
	  price = input;
	  volume = input;
  }

  template <typename ValueType>
  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, base::Value<ValueType>>>
  BidAsk<ValueType>& BidAsk<ValueType>::operator = (const Input &input) {
	  bid = input;
	  ask = input;
	  middle = input;
	  price = input;
	  volume = input;
	  return *this;
  }


  template <typename ValueType>
  std::string BidAsk< ValueType>::toString() const {
	  std::string msg;
	  msg.reserve(const_values::EXPECTED_BidAsk_LENGTH);
	  msg.append(bid.toString());
	  msg.append(", ");
	  msg.append(ask.toString());
	  msg.append(", ");
	  msg.append(middle.toString());
	  msg.append(", ");
	  msg.append(price.toString());
	  msg.append(", ");
	  msg.append(volume.toString());
	  return msg;
  }


  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator == (const Other& lhs, const BidAsk<ValueType>& rhs) {
	  return (rhs == lhs);
  }
  template <typename ValueType, typename CompareBy = typename BidAsk<ValueType>::AllFields>
  bool operator == (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid == rhs.bid &&
				  lhs.ask == rhs.ask &&
				  lhs.middle == rhs.middle &&
				  lhs.price == rhs.price &&
				  lhs.volume == rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid == rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask == rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle == rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price == rhs.price;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
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
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator == (const BidAsk<ValueType>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid == rhs &&
				  lhs.ask == rhs &&
				  lhs.middle == rhs &&
				  lhs.price == rhs &&
				  lhs.volume == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume == rhs;
	  }
	  return res;
  }

  template <typename ValueType, typename CompareBy = typename BidAsk<ValueType>::AllFields>
  bool operator != (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator != (const BidAsk<ValueType>& lhs, const Other& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator != (const Other& lhs, const BidAsk<ValueType>& rhs) {
	  return !(rhs == lhs);
  }

  template <typename ValueType, typename CompareBy = typename BidAsk<ValueType>::AllFields>
  bool operator < (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid < rhs.bid &&
				  lhs.ask < rhs.ask &&
				  lhs.middle < rhs.middle &&
				  lhs.price < rhs.price &&
				  lhs.volume < rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid < rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask < rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle < rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price < rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
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
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator < (const BidAsk<ValueType>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid < rhs &&
				  lhs.ask < rhs &&
				  lhs.middle < rhs &&
				  lhs.price < rhs &&
				  lhs.volume < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask > rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price < rhs;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume < rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator < (const Other& lhs, const BidAsk<ValueType>& rhs) {
	  return (!(rhs < lhs) && !(rhs == lhs));
  }

  template <typename ValueType, typename CompareBy = typename BidAsk<ValueType>::AllFields>
  bool operator > (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator > (const BidAsk<ValueType>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator > (const Other& lhs, const BidAsk<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }

  template <typename ValueType, typename CompareBy = typename BidAsk<ValueType>::AllFields>
  bool operator <= (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid <= rhs.bid &&
				  lhs.ask <= rhs.ask &&
				  lhs.middle <= rhs.middle &&
				  lhs.price <= rhs.price &&
				  lhs.volume <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid <= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask <= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle <= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price <= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
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
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator <= (const BidAsk<ValueType>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid <= rhs &&
				  lhs.ask <= rhs &&
				  lhs.middle <= rhs &&
				  lhs.price <= rhs &&
				  lhs.volume <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
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
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator <= (const Other& lhs, const BidAsk<ValueType>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs <= rhs.bid &&
				  lhs <= rhs.ask &&
				  lhs <= rhs.middle &&
				  lhs <= rhs.price &&
				  lhs <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs <= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs <= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs <= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs <= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
		  res = res &&
				  lhs <= rhs.volume;
	  }
	  return res;
  }

  template <typename ValueType, typename CompareBy = typename BidAsk<ValueType>::AllFields>
  bool operator >= (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid >= rhs.bid &&
				  lhs.ask >= rhs.ask &&
				  lhs.middle >= rhs.middle &&
				  lhs.price >= rhs.price &&
				  lhs.volume >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid >= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask >= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle >= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price >= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
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
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator >= (const BidAsk<ValueType>& lhs, const Other& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs.bid >= rhs &&
				  lhs.ask >= rhs &&
				  lhs.middle >= rhs &&
				  lhs.price >= rhs &&
				  lhs.volume >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs.bid >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs.ask >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs.middle >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs.price >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
		  res = res &&
				  lhs.volume < rhs;
	  }
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  bool operator >= (const Other& lhs, const BidAsk<ValueType>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::AllFields>) {
		  res = res &&
				  lhs >= rhs.bid &&
				  lhs >= rhs.ask &&
				  lhs >= rhs.middle &&
				  lhs >= rhs.price &&
				  lhs >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Bid>) {
		  res = res &&
				  lhs >= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Ask>) {
		  res = res &&
				  lhs >= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Middle>) {
		  res = res &&
				  lhs >= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Price>) {
		  res = res &&
				  lhs >= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<ValueType>::Volume>) {
		  res = res &&
				  lhs >= rhs.volume;
	  }
	  return res;
  }


  template <typename ValueType>
  BidAsk<ValueType> operator * (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid * rhs.bid;
	  res.ask = lhs.ask * rhs.ask;
	  res.middle = lhs.middle * rhs.middle;
	  res.price = lhs.price * rhs.price;
	  res.volume = lhs.volume * rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  BidAsk<ValueType> operator * (const BidAsk<ValueType>& lhs, const Other &rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid * rhs;
	  res.ask = lhs.ask * rhs;
	  res.middle = lhs.middle * rhs;
	  res.price = lhs.price * rhs;
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
  BidAsk<ValueType> operator * (const Other &lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs * rhs.bid;
	  res.ask = lhs * rhs.ask;
	  res.middle = lhs * rhs.middle;
	  res.price = lhs * rhs.price;
	  if constexpr (VolumeToo) {
		  res.volume = lhs * rhs.volume;
	  }
	  return res;
  }

  template <typename ValueType>
  BidAsk<ValueType> operator / (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid / rhs.bid;
	  res.ask = lhs.ask / rhs.ask;
	  res.middle = lhs.middle / rhs.middle;
	  res.price = lhs.price / rhs.price;
	  res.volume = lhs.volume / rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  BidAsk<ValueType> operator / (const BidAsk<ValueType>& lhs, const Other &rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid / rhs;
	  res.ask = lhs.ask / rhs;
	  res.middle = lhs.middle / rhs;
	  res.price = lhs.price / rhs;
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
  BidAsk<ValueType> operator / (const Other &lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs / rhs.bid;
	  res.ask = lhs / rhs.ask;
	  res.middle = lhs / rhs.middle;
	  res.price = lhs / rhs.price;
	  if constexpr (VolumeToo) {
		  res.volume = lhs / rhs.volume;
	  }
	  return res;
  }

  template <typename ValueType>
  BidAsk<ValueType> operator + (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid + rhs.bid;
	  res.ask = lhs.ask + rhs.ask;
	  res.middle = lhs.middle + rhs.middle;
	  res.price = lhs.price + rhs.price;
	  res.volume = lhs.volume + rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  BidAsk<ValueType> operator + (const BidAsk<ValueType>& lhs, const Other &rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid + rhs;
	  res.ask = lhs.ask + rhs;
	  res.middle = lhs.middle + rhs;
	  res.price = lhs.price + rhs;
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
  BidAsk<ValueType> operator + (const Other &lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs + rhs.bid;
	  res.ask = lhs + rhs.ask;
	  res.middle = lhs + rhs.middle;
	  res.price = lhs + rhs.price;
	  if constexpr (VolumeToo) {
		  res.volume = lhs + rhs.volume;
	  }
	  return res;
  }

  template <typename ValueType>
  BidAsk<ValueType> operator - (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid - rhs.bid;
	  res.ask = lhs.ask - rhs.ask;
	  res.middle = lhs.middle - rhs.middle;
	  res.price = lhs.price - rhs.price;
	  res.volume = lhs.volume - rhs.volume;
	  return res;
  }
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  requirements::NotSame<base::Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<base::Value<ValueType>, Other> = true
  >
  BidAsk<ValueType> operator - (const BidAsk<ValueType>& lhs, const Other &rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs.bid - rhs;
	  res.ask = lhs.ask - rhs;
	  res.middle = lhs.middle - rhs;
	  res.price = lhs.price - rhs;
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
  BidAsk<ValueType> operator - (const Other &lhs, const BidAsk<ValueType>& rhs) {
	  BidAsk<ValueType> res;
	  res.bid = lhs - rhs.bid;
	  res.ask = lhs - rhs.ask;
	  res.middle = lhs - rhs.middle;
	  res.price = lhs - rhs.price;
	  if constexpr (VolumeToo) {
		  res.volume = lhs - rhs.volume;
	  }
	  return res;
  }


  template <typename ValueType>
  BidAsk<ValueType>& operator += (BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  lhs.bid += rhs.bid;
	  lhs.ask += rhs.ask;
	  lhs.middle += rhs.middle;
	  lhs.price += rhs.price;
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
  BidAsk<ValueType>& operator += (BidAsk<ValueType>& lhs, const Other &rhs) {
	  lhs.bid += rhs;
	  lhs.ask += rhs;
	  lhs.middle += rhs;
	  lhs.price += rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume += rhs;
	  }
	  return lhs;
  }

  template <typename ValueType>
  BidAsk<ValueType>& operator -= (BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  lhs.bid -= rhs.bid;
	  lhs.ask -= rhs.ask;
	  lhs.middle -= rhs.middle;
	  lhs.price -= rhs.price;
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
  BidAsk<ValueType>& operator -= (BidAsk<ValueType>& lhs, const Other &rhs) {
	  lhs.bid -= rhs;
	  lhs.ask -= rhs;
	  lhs.middle -= rhs;
	  lhs.price -= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume -= rhs;
	  }
	  return lhs;
  }

  template <typename ValueType>
  BidAsk<ValueType>& operator *= (BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  lhs.bid *= rhs.bid;
	  lhs.ask *= rhs.ask;
	  lhs.middle *= rhs.middle;
	  lhs.price *= rhs.price;
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
  BidAsk<ValueType>& operator *= (BidAsk<ValueType>& lhs, const Other &rhs) {
	  lhs.bid *= rhs;
	  lhs.ask *= rhs;
	  lhs.middle *= rhs;
	  lhs.price *= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume *= rhs;
	  }
	  return lhs;
  }

  template <typename ValueType>
  BidAsk<ValueType>& operator /= (BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  lhs.bid /= rhs.bid;
	  lhs.ask /= rhs.ask;
	  lhs.middle /= rhs.middle;
	  lhs.price /= rhs.price;
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
  BidAsk<ValueType>& operator /= (BidAsk<ValueType>& lhs, const Other &rhs) {
	  lhs.bid /= rhs;
	  lhs.ask /= rhs;
	  lhs.middle /= rhs;
	  lhs.price /= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume /= rhs;
	  }
	  return lhs;
  }


  template <typename ValueType>
  std::ostream& operator << (std::ostream& os, const BidAsk<ValueType>& ohlcv) {
	  return os
			  << ohlcv.bid << ", "
			  << ohlcv.ask << ", "
			  << ohlcv.middle << ", "
			  << ohlcv.price << ", "
			  << ohlcv.volume;
  }//!operator
  template <typename ValueType>
  std::istream& operator >> (std::istream& is, BidAsk<ValueType>& ohlcv) {
	  typename BidAsk<ValueType>::Type open, high, low, close, volume {0.0};
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
			  ohlcv.bid = open;
			  ohlcv.ask = high;
			  ohlcv.middle = low;
			  ohlcv.price = close;
			  if (volume_read_ok) ohlcv.volume = volume;
		  }
		  else {
			  is.setstate(std::ios_base::failbit);
		  }
	  }
	  return is;
  }//!operator

}//!namespace


#endif //FIN_BID_ASK_H
