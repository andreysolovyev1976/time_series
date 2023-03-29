//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "time_series/value.hpp"
#include "financial/data_structures/const_values.h"

#include <iosfwd>
#include <vector>
#include <initializer_list>

#ifndef FIN_BID_ASK_H
#define FIN_BID_ASK_H

namespace time_series::financial {

  template <typename ValueType = value::traits::ValueTypeDefault>
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
	  BidAsk (std::initializer_list<Value<ValueType>> values);
#ifndef __cpp_concepts
	  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
#else
	  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
	  BidAsk(const Input &input);
#ifndef __cpp_concepts
	  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
#else
	  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
	  BidAsk& operator = (const Input &input);

	  std::string toString() const;

	  template<std::size_t Index>
	  decltype(auto) get() &;

	  template<std::size_t Index>
	  decltype(auto) get() &&;

	  template<std::size_t Index>
	  decltype(auto) get() const &;

	  template<std::size_t Index>
	  decltype(auto) get() const &&;

	  Value<ValueType> bid, ask, middle, price, volume {};

  private:
	  template<std::size_t Index, typename ThisType>
	  auto&& getImpl(ThisType&& t);

	  template<std::size_t Index, typename ThisType>
	  auto&& getImpl(ThisType&& t) const;
  };

  template <typename ValueType>
  BidAsk<ValueType>::BidAsk (std::initializer_list<Value<ValueType>> values) {
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
#ifndef __cpp_concepts
  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
#else
  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
  BidAsk<ValueType>::BidAsk(const Input &input){
	  bid = input;
	  ask = input;
	  middle = input;
	  price = input;
	  volume = input;
  }

  template <typename ValueType>
#ifndef __cpp_concepts
  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
#else
  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
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


  template <typename ValueType>
  template<std::size_t Index>
  decltype(auto) BidAsk<ValueType>::get() &  { return getImpl<Index>(*this); }

  template <typename ValueType>
  template<std::size_t Index>
  decltype(auto) BidAsk<ValueType>::get() && { return getImpl<Index>(*this); }

  template <typename ValueType>
  template<std::size_t Index>
  decltype(auto) BidAsk<ValueType>::get() const &  { return getImpl<Index>(*this); }

  template <typename ValueType>
  template<std::size_t Index>
  decltype(auto) BidAsk<ValueType>::get() const && { return getImpl<Index>(*this); }


  template <typename ValueType>
  template<std::size_t Index, typename ThisType>
  auto&& BidAsk<ValueType>::getImpl(ThisType&& t) {
	  static_assert(Index < 5u, "Index out of bounds for BidAsk");
	  if constexpr (Index == 0) return std::forward<ThisType>(t).bid;
	  if constexpr (Index == 1) return std::forward<ThisType>(t).ask;
	  if constexpr (Index == 2) return std::forward<ThisType>(t).middle;
	  if constexpr (Index == 3) return std::forward<ThisType>(t).price;
	  if constexpr (Index == 4) return std::forward<ThisType>(t).volume;
  }
  template <typename ValueType>
  template<std::size_t Index, typename ThisType>
  auto&& BidAsk<ValueType>::getImpl(ThisType&& t) const {
	  static_assert(Index < 5u, "Index out of bounds for BidAsk");
	  if constexpr (Index == 0) return std::forward<ThisType>(t).bid;
	  if constexpr (Index == 1) return std::forward<ThisType>(t).ask;
	  if constexpr (Index == 2) return std::forward<ThisType>(t).middle;
	  if constexpr (Index == 3) return std::forward<ThisType>(t).price;
	  if constexpr (Index == 4) return std::forward<ThisType>(t).volume;
  }


#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif
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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

  bool operator != (const BidAsk<ValueType>& lhs, const Other& rhs) {
	  return !(lhs == rhs);
  }
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

  bool operator < (const Other& lhs, const BidAsk<ValueType>& rhs) {
	  return (!(rhs < lhs) && !(rhs == lhs));
  }

  template <typename ValueType, typename CompareBy = typename BidAsk<ValueType>::AllFields>
  bool operator > (const BidAsk<ValueType>& lhs, const BidAsk<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

  bool operator > (const BidAsk<ValueType>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other> = true,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
#else
  template <
		  typename ValueType,
		  typename Other,
		  typename CompareBy = typename BidAsk<ValueType>::Price
  >
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::ComparisonOperationsDefined<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif
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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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
#ifndef __cpp_concepts
  template <
		  typename ValueType,
		  typename Other,
		  bool VolumeToo = false,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other> = true
  >
#else
  template <typename ValueType,
		  typename Other,
		  bool VolumeToo = false>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<Value<ValueType>, Other>
#endif

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


/**
 * @details
 * Template specialization for std:: namespace \n
 * to make a structural binding available\n\n
 *
 * */

namespace std {
  template <typename ValueType>
  struct tuple_size<time_series::financial::BidAsk<ValueType>> : integral_constant<std::size_t, 5u> {};

  template<std::size_t Index, typename ValueType>
  struct tuple_element<Index, time_series::financial::BidAsk<ValueType>>
		  : tuple_element<Index, tuple<
				  typename time_series::financial::BidAsk<ValueType>::value_type,
				  typename time_series::financial::BidAsk<ValueType>::value_type,
				  typename time_series::financial::BidAsk<ValueType>::value_type,
				  typename time_series::financial::BidAsk<ValueType>::value_type,
				  typename time_series::financial::BidAsk<ValueType>::value_type>
		  >{};
}//!namespace

#if 0
//todo: can be used here

  template<std::size_t Index, typename... Iterators>
struct tuple_element<Index, itertools::ZipIterator<Iterators...>> {
	using type = decltype(std::get<Index>(std::declval<itertools::ZipIterator<Iterators...>>().operator*() ));
};

#endif

#endif //FIN_BID_ASK_H
