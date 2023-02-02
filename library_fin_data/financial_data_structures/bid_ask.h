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

  //todo: add requirement for Other - should be an Arithmetic or better - operator is defined?

  template <typename Duration = base::Nanoseconds>
  struct BidAsk final {

	  struct AllFields{};
	  struct Bid{};
	  struct Ask{};
	  struct Middle{};
	  struct Price{};
	  struct Volume{};

	  BidAsk () = default;
	  BidAsk (std::initializer_list<base::Value<Duration>> values);

	  std::string toString() const;

	  base::Value<Duration> bid, ask, middle, price, volume {0.0};
  };

  template <typename Duration>
  BidAsk<Duration>::BidAsk (std::initializer_list<base::Value<Duration>> values) {
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

  template <typename Duration>
  std::string BidAsk<Duration>::toString() const {
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
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator == (const Other& lhs, const BidAsk<Duration>& rhs) {
	  return (rhs == lhs);
  }
  template <typename Duration, typename CompareBy = typename BidAsk<Duration>::AllFields>
  bool operator == (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid == rhs.bid &&
				  lhs.ask == rhs.ask &&
				  lhs.middle == rhs.middle &&
				  lhs.price == rhs.price &&
				  lhs.volume == rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid == rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask == rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle == rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price == rhs.price;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume == rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator == (const BidAsk<Duration>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid == rhs &&
				  lhs.ask == rhs &&
				  lhs.middle == rhs &&
				  lhs.price == rhs &&
				  lhs.volume == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price == rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume == rhs;
	  }
	  return res;
  }

  template <typename Duration, typename CompareBy = typename BidAsk<Duration>::AllFields>
  bool operator != (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator != (const BidAsk<Duration>& lhs, const Other& rhs) {
	  return !(lhs == rhs);
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator != (const Other& lhs, const BidAsk<Duration>& rhs) {
	  return !(rhs == lhs);
  }

  template <typename Duration, typename CompareBy = typename BidAsk<Duration>::AllFields>
  bool operator < (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid < rhs.bid &&
				  lhs.ask < rhs.ask &&
				  lhs.middle < rhs.middle &&
				  lhs.price < rhs.price &&
				  lhs.volume < rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid < rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask < rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle < rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price < rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume < rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator < (const BidAsk<Duration>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid < rhs &&
				  lhs.ask < rhs &&
				  lhs.middle < rhs &&
				  lhs.price < rhs &&
				  lhs.volume < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask > rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle < rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price < rhs;

	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume < rhs;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator < (const Other& lhs, const BidAsk<Duration>& rhs) {
	  return (!(rhs < lhs) && !(rhs == lhs));
  }

  template <typename Duration, typename CompareBy = typename BidAsk<Duration>::AllFields>
  bool operator > (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator > (const BidAsk<Duration>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator > (const Other& lhs, const BidAsk<Duration>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }

  template <typename Duration, typename CompareBy = typename BidAsk<Duration>::AllFields>
  bool operator <= (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid <= rhs.bid &&
				  lhs.ask <= rhs.ask &&
				  lhs.middle <= rhs.middle &&
				  lhs.price <= rhs.price &&
				  lhs.volume <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid <= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask <= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle <= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price <= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume <= rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator <= (const BidAsk<Duration>& lhs, const Other& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid <= rhs &&
				  lhs.ask <= rhs &&
				  lhs.middle <= rhs &&
				  lhs.price <= rhs &&
				  lhs.volume <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price <= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume <= rhs;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator <= (const Other& lhs, const BidAsk<Duration>& rhs) {
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs <= rhs.bid &&
				  lhs <= rhs.ask &&
				  lhs <= rhs.middle &&
				  lhs <= rhs.price &&
				  lhs <= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs <= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs <= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs <= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs <= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs <= rhs.volume;
	  }
	  return res;
  }

  template <typename Duration, typename CompareBy = typename BidAsk<Duration>::AllFields>
  bool operator >= (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid >= rhs.bid &&
				  lhs.ask >= rhs.ask &&
				  lhs.middle >= rhs.middle &&
				  lhs.price >= rhs.price &&
				  lhs.volume >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid >= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask >= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle >= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price >= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume >= rhs.volume;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator >= (const BidAsk<Duration>& lhs, const Other& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs.bid >= rhs &&
				  lhs.ask >= rhs &&
				  lhs.middle >= rhs &&
				  lhs.price >= rhs &&
				  lhs.volume >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs.bid >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs.ask >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs.middle >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs.price >= rhs;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs.volume < rhs;
	  }
	  return res;
  }
  template <
		  typename Duration,
		  typename Other,
		  requirements::NotSame<base::Value<Duration>, Other> = true,
		  requirements::ComparisonOperationsDefined<base::Value<Duration>, Other> = true,
		  typename CompareBy = typename BidAsk<Duration>::Price
  >
  bool operator >= (const Other& lhs, const BidAsk<Duration>& rhs){
	  bool res {true};
	  if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::AllFields>) {
		  res = res &&
				  lhs >= rhs.bid &&
				  lhs >= rhs.ask &&
				  lhs >= rhs.middle &&
				  lhs >= rhs.price &&
				  lhs >= rhs.volume;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Bid>) {
		  res = res &&
				  lhs >= rhs.bid;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Ask>) {
		  res = res &&
				  lhs >= rhs.ask;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Middle>) {
		  res = res &&
				  lhs >= rhs.middle;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Price>) {
		  res = res &&
				  lhs >= rhs.price;
	  }
	  else if constexpr (std::is_same_v<CompareBy, typename BidAsk<Duration>::Volume>) {
		  res = res &&
				  lhs >= rhs.volume;
	  }
	  return res;
  }

  //todo: concept on Other?
  template <typename Duration>
  BidAsk<Duration> operator * (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  return BidAsk<Duration> {
			  .bid = lhs.bid * rhs.bid,
			  .ask = lhs.ask * rhs.ask,
			  .middle = lhs.middle * rhs.middle,
			  .price = lhs.price * rhs.price,
			  .volume = lhs.volume * rhs.volume
	  };
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration> operator * (const BidAsk<Duration>& lhs, Other rhs) {
	  BidAsk<Duration> res;
	  res.bid = lhs.bid * rhs;
	  res.ask = lhs.ask * rhs;
	  res.middle = lhs.middle * rhs;
	  res.price = lhs.price * rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume * rhs;
	  }
	  return res;
  }

  template <typename Duration>
  BidAsk<Duration> operator / (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  return BidAsk<Duration> {
			  .bid = lhs.bid / rhs.bid,
			  .ask = lhs.ask / rhs.ask,
			  .middle = lhs.middle / rhs.middle,
			  .price = lhs.price / rhs.price,
			  .volume = lhs.volume / rhs.volume
	  };
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration> operator / (const BidAsk<Duration>& lhs, Other rhs) {
	  BidAsk<Duration> res;
	  res.bid = lhs.bid / rhs;
	  res.ask = lhs.ask / rhs;
	  res.middle = lhs.middle / rhs;
	  res.price = lhs.price / rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume / rhs;
	  }
	  return res;
  }
  template <typename Duration>
  BidAsk<Duration> operator + (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  return BidAsk<Duration> {
			  .bid = lhs.bid + rhs.bid,
			  .ask = lhs.ask + rhs.ask,
			  .middle = lhs.middle + rhs.middle,
			  .price = lhs.price + rhs.price,
			  .volume = lhs.volume + rhs.volume
	  };
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration> operator + (const BidAsk<Duration>& lhs, Other rhs) {
	  BidAsk<Duration> res;
	  res.bid = lhs.bid + rhs;
	  res.ask = lhs.ask + rhs;
	  res.middle = lhs.middle + rhs;
	  res.price = lhs.price + rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume + rhs;
	  }
	  return res;
  }
  template <typename Duration>
  BidAsk<Duration> operator - (const BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  return BidAsk<Duration> {
			  .bid = lhs.bid - rhs.bid,
			  .ask = lhs.ask - rhs.ask,
			  .middle = lhs.middle - rhs.middle,
			  .price = lhs.price - rhs.price,
			  .volume = lhs.volume - rhs.volume
	  };
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration> operator - (const BidAsk<Duration>& lhs, Other rhs) {
	  BidAsk<Duration> res;
	  res.bid = lhs.bid - rhs;
	  res.ask = lhs.ask - rhs;
	  res.middle = lhs.middle - rhs;
	  res.price = lhs.price - rhs;
	  if constexpr (VolumeToo) {
		  res.volume = lhs.volume - rhs;
	  }
	  return res;
  }



  template <typename Duration>
  BidAsk<Duration>& operator += (BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  lhs.bid += rhs.bid;
	  lhs.ask += rhs.ask;
	  lhs.middle += rhs.middle;
	  lhs.price += rhs.price;
	  lhs.volume += rhs.volume;
	  return lhs;
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration>& operator += (BidAsk<Duration>& lhs, Other rhs) {
	  lhs.bid += rhs;
	  lhs.ask += rhs;
	  lhs.middle += rhs;
	  lhs.price += rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume += rhs;
	  }
	  return lhs;
  }

  template <typename Duration>
  BidAsk<Duration>& operator -= (BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  lhs.bid -= rhs.bid;
	  lhs.ask -= rhs.ask;
	  lhs.middle -= rhs.middle;
	  lhs.price -= rhs.price;
	  lhs.volume -= rhs.volume;
	  return lhs;
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration>& operator -= (BidAsk<Duration>& lhs, Other rhs) {
	  lhs.bid -= rhs;
	  lhs.ask -= rhs;
	  lhs.middle -= rhs;
	  lhs.price -= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume -= rhs;
	  }
	  return lhs;
  }

  template <typename Duration>
  BidAsk<Duration>& operator *= (BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  lhs.bid *= rhs.bid;
	  lhs.ask *= rhs.ask;
	  lhs.middle *= rhs.middle;
	  lhs.price *= rhs.price;
	  lhs.volume *= rhs.volume;
	  return lhs;
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration>& operator *= (BidAsk<Duration>& lhs, Other rhs) {
	  lhs.bid *= rhs;
	  lhs.ask *= rhs;
	  lhs.middle *= rhs;
	  lhs.price *= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume *= rhs;
	  }
	  return lhs;
  }

  template <typename Duration>
  BidAsk<Duration>& operator /= (BidAsk<Duration>& lhs, const BidAsk<Duration>& rhs) {
	  lhs.bid /= rhs.bid;
	  lhs.ask /= rhs.ask;
	  lhs.middle /= rhs.middle;
	  lhs.price /= rhs.price;
	  lhs.volume /= rhs.volume;
	  return lhs;
  }
  template <typename Duration, typename Other, bool VolumeToo = false>
  BidAsk<Duration>& operator /= (BidAsk<Duration>& lhs, Other rhs) {
	  lhs.bid /= rhs;
	  lhs.ask /= rhs;
	  lhs.middle /= rhs;
	  lhs.price /= rhs;
	  if constexpr (VolumeToo) {
		  lhs.volume /= rhs;
	  }
	  return lhs;
  }


  template <typename Duration>
  std::ostream& operator << (std::ostream& os, const BidAsk<Duration>& ohlcv) {
	  return os
			  << ohlcv.bid << ", "
			  << ohlcv.ask << ", "
			  << ohlcv.middle << ", "
			  << ohlcv.price << ", "
			  << ohlcv.volume;
  }//!operator
  template <typename Duration>
  std::istream& operator >> (std::istream& is, BidAsk<Duration>& ohlcv) {
	  typename BidAsk<Duration>::Type open, high, low, close, volume {0.0};
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
