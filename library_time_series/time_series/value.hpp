//
// Created by Andrey Solovyev on 19/06/2022.
//

#pragma once

#include "common_usage_library/utils.hpp"
#include "common_usage_library/floating_point_comp.hpp"
#include "common_usage_library/types_requirements/ctor_input.h"
#include "common_usage_library/types_requirements/operators.h"
#include "common_usage_library/timestamp.hpp"
#include "time_series/const_values.h"
#include "time_series/value_interface.h"

#ifndef BASE_VALUE_H
#define BASE_VALUE_H

namespace time_series {

  /**
   * @details
   * This is a wrapper, but it required from a design perspective:
   * in case there is a requirements to change an underlying type, it should be
   * easy to do.
   * Therefore all the operators are declared and defined as well.
   * */

  template <typename ValueType = value::traits::ValueTypeDefault>
//  struct Value : traits::ValueBase<Value<ValueType>> { //excluded CRTP, no need for that
  struct Value {
	  using value_type = ValueType;

	  ValueType value {};

	  Value() = default;
#ifndef __cpp_concepts
	  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
#else
	  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
	  Value(Input&& input);
#ifndef __cpp_concepts
	  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
#else
	  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
	  Value& operator = (Input&& input);

	  Value (std::string&& input);
	  Value (const std::string& input);

	  std::string toString () const;

	  operator std::string () const;
	  operator ValueType () const;

	  ValueType const& operator()() const &;
	  ValueType& operator()() &;
  };

  template <typename ValueType>
#ifndef __cpp_concepts
  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
#else
  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
  Value<ValueType>::Value(Input&& input) : value(std::forward<Input>(input)) {}

  template <typename ValueType>
#ifndef __cpp_concepts
  template <typename Input, culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
#else
  template <typename Input> requires culib::requirements::ConveribleOrConstructibleFromTo<Input, ValueType>
#endif
  Value<ValueType>& Value<ValueType>::operator = (Input&& input) {
	  value = std::forward<Input>(input);
	  return *this;
  }
  template <typename ValueType>
  Value<ValueType>::Value (std::string&& input) {
	  if constexpr (std::is_arithmetic_v<ValueType>) {
		  auto input_number = culib::utils::fromChars<ValueType>(input);
		  if (input_number.has_value()) {
			  using Input = decltype(input_number.value());
			  static_assert(culib::requirements::is_converible_or_constructible_v<Input, ValueType>);
			  value = input_number.value();
		  } else {
			  throw std::invalid_argument ("Can't construct a Value out of " + input);
		  }
	  }
	  else {
		  static_assert(culib::requirements::is_converible_or_constructible_v<std::string, ValueType>);
		  value = std::move(input);
	  }
  }
  template <typename ValueType>
  Value<ValueType>::Value (const std::string& input) {
	  if constexpr (std::is_arithmetic_v<ValueType>) {
		  auto input_number = culib::utils::fromChars<ValueType>(input);
		  if (input_number.has_value()) {
			  using Input = decltype(input_number.value());
			  static_assert(culib::requirements::is_converible_or_constructible_v<Input, ValueType>);
			  value = input_number.value();
		  } else {
			  throw std::invalid_argument ("Can't construct a Value out of " + input);
		  }
	  }
	  else {
		  static_assert(culib::requirements::is_converible_or_constructible_v<std::string, ValueType>);
		  value = std::move(input);
	  }
  }

  template <typename ValueType>
  std::string Value<ValueType>::toString () const {
	  return culib::utils::toChars(value);
  }


  template <typename ValueType>
  Value<ValueType>::operator std::string () const {
	  return toString();
  }
  template <typename ValueType>
  Value<ValueType>::operator ValueType () const {
	  return value;
  }

  template <typename ValueType>
  ValueType const& Value<ValueType>::operator()() const & {
	  return value;
  }
  template <typename ValueType>
  ValueType& Value<ValueType>::operator()() & {
	  return value;
  }


  template <typename ValueType>
  bool operator==(const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType>)
		  return culib::comp::eq(lhs.value, rhs.value);
	  else
		  return lhs.value == rhs.value;
  }
  /**
   * @brief
   * value_type is floating and other is integral\n
   * value_type is integral and other is floating\n
   * for everything else there should be operator\n
   */

#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator==(const Value<ValueType>& lhs, const Other& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return culib::comp::eq(lhs.value, rhs);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return culib::comp::eq(lhs.value, rhs);
	  else
		  return lhs.value == rhs;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator==(const Other& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return culib::comp::eq(lhs, rhs.value);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return culib::comp::eq(lhs, rhs.value);
	  else
		  return lhs == rhs.value;
  }
  template <typename ValueType>
  bool operator!=(const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType>)
		  return culib::comp::ne(lhs.value, rhs.value);
	  else
		  return !(lhs.value == rhs.value);
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator!=(const Value<ValueType>& lhs, const Other& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return culib::comp::ne(lhs.value, rhs);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return culib::comp::ne(lhs.value, rhs);
	  else
		  return !(lhs.value==rhs);
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator!=(const Other& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return culib::comp::ne(lhs, rhs.value);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return culib::comp::ne(lhs, rhs.value);
	  else
		  return !(lhs == rhs.value);
  }
  template <typename ValueType>
  bool operator < (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType>)
		  return culib::comp::lt(lhs.value, rhs.value);
	  else
		  return lhs.value < rhs.value;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator < (const Value<ValueType>& lhs, const Other& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return culib::comp::lt(lhs.value, rhs);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return culib::comp::lt(lhs.value, rhs);
	  else
		  return lhs.value < rhs;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator < (const Other& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return culib::comp::lt(lhs, rhs.value);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return culib::comp::lt(lhs, rhs.value);
	  else
		  return lhs < rhs.value;
  }
  template <typename ValueType>
  bool operator > (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator > (const Value<ValueType>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator > (const Other& lhs, const Value<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename ValueType>
  bool operator <= (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator <= (const Value<ValueType>& lhs, const Other& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator <= (const Other& lhs, const Value<ValueType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename ValueType>
  bool operator >= (const Value<ValueType>& lhs, const Value<ValueType>& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator >= (const Value<ValueType>& lhs, const Other& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  bool operator >= (const Other& lhs, const Value<ValueType>& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }


  template <typename ValueType>
  Value<ValueType> operator*(const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value * rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator*(const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value * std::forward<Other>(rhs);
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator*(Other &&lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = std::forward<Other>(lhs) * rhs.value;
	  return res;
  }
  template <typename ValueType>
  Value<ValueType> operator / (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value / rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator / (const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value / std::forward<Other>(rhs);
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator / (Other &&lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = std::forward<Other>(lhs) / rhs.value;
	  return res;
  }
  template <typename ValueType>
  Value<ValueType> operator + (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value + rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator + (const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value + std::forward<Other>(rhs);
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator + (Other &&lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = std::forward<Other>(lhs) + rhs.value;
	  return res;
  }
  template <typename ValueType>
  Value<ValueType> operator - (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value - rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator - (const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value - std::forward<Other>(rhs);
	  return res;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType> operator - (Other &&lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = std::forward<Other>(lhs) - rhs.value;
	  return res;
  }


  template <typename ValueType>
  Value<ValueType>& operator += (Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  lhs.value += rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType>& operator += (Value<ValueType>& lhs, Other &&rhs) {
	  lhs.value += std::forward<Other>(rhs);
	  return lhs;
  }
  template <typename ValueType>
  Value<ValueType>& operator -= (Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  lhs.value -= rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType>& operator -= (Value<ValueType>& lhs, Other &&rhs) {
	  lhs.value -= std::forward<Other>(rhs);
	  return lhs;
  }
  template <typename ValueType>
  Value<ValueType>& operator *= (Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  lhs.value *= rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType>& operator *= (Value<ValueType>& lhs, Other &&rhs) {
	  lhs.value *= std::forward<Other>(rhs);
	  return lhs;
  }
  template <typename ValueType>
  Value<ValueType>& operator /= (Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  lhs.value /= rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename ValueType, typename Other,
		  culib::requirements::NotSame<Value<ValueType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ValueType, Other> = true>
#else
  template <typename ValueType, typename Other>
  requires
  culib::requirements::NotSame<Value<ValueType>, Other> &&
		  culib::requirements::BinOperatorsExist<ValueType, Other>
#endif
  Value<ValueType>& operator /= (Value<ValueType>& lhs, Other &&rhs) {
	  lhs.value /= std::forward<Other>(rhs);
	  return lhs;
  }

  template <typename ValueType>
  std::ostream& operator<<(std::ostream& os, const Value<ValueType>& single_value) {
	  return os << single_value.value;
  }//!operator

  template <typename ValueType>
  std::istream& operator>>(std::istream& is, Value<ValueType>& single_value) {
	  ValueType value;
	  if (is) {
		  is >> value;
		  if (is) {
			  single_value.value = value;
		  }
		  else {
			  is.setstate(std::ios_base::failbit);
		  }
	  }
	  return is;
  }//!operator

}//!namespace


#endif //BASE_VALUE_H
