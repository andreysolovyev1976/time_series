//
// Created by Andrey Solovyev on 19/06/2022.
//

#pragma once

#include "const_values.h"
#include "timestamp.h"
#include "floating_point_comp.h"
#include "utils/utils.h"
#include "value_traits.h"
#include "types_requirements/ctor_input.h"
#include "types_requirements/operators.h"

#ifndef BASE_VALUE_H
#define BASE_VALUE_H

namespace base {
  
  /**
   * @details
   * This is a wrapper, but it required from a design perspective:
   * in case there is a requirements to change an underlying type, it should be
   * easy to do.
   * Therefore all the operators are declared and defined as well.
   * */

  template <typename ValueType = traits::ValueTypeDefault>
//  struct Value : traits::ValueBase<Value<ValueType>> { //excluded CRTP, no need for that
	struct Value {
	  using value_type = ValueType;

	  ValueType value {};

	  Value() = default;
	  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
	  Value(Input&& input);
	  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
	  Value& operator = (Input&& input);
	  Value (std::string&& input);
	  Value (const std::string& input);

	  std::string toString () const;
  };

  template <typename ValueType>
  template <typename Input,
		  requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
  Value<ValueType>::Value(Input&& input) : value(std::forward<Input>(input)) {}

  template <typename ValueType>
  template <typename Input, requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
  Value<ValueType>& Value<ValueType>::operator = (Input&& input) {
	  value = std::forward<Input>(input);
	  return *this;
  }
  template <typename ValueType>
  Value<ValueType>::Value (std::string&& input) {
	  auto input_number = base::utils::fromChars(std::move(input));
	  using Input = decltype(input_number);
	  static_assert(requirements::isConveribleOrConstructible<Input, ValueType>());
	  value = input_number;
  }
  template <typename ValueType>
  Value<ValueType>::Value (const std::string& input) {
	  auto input_number = base::utils::fromChars(input);
	  using Input = decltype(input_number);
	  static_assert(requirements::isConveribleOrConstructible<Input, ValueType>());
	  value = input_number;
  }

  template <typename ValueType>
  std::string Value<ValueType>::toString () const {
	  return utils::toChars(value);
  }


  template <typename ValueType>
  bool operator==(const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType>)
	  	return comp::eq(lhs.value, rhs.value);
	  else 
		  return lhs.value == rhs.value;
  }
  /**
   *
   * value_type is floating and other is floating
   * value_type is floating and other is integral
   * value_type is integral and other is floating
   * for everyting else there should be operator
   */

  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator==(const Value<ValueType>& lhs, const Other& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return comp::eq(lhs.value, rhs);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return comp::eq(lhs.value, rhs);
	  else
		  return lhs.value == rhs;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator==(const Other& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return comp::eq(lhs, rhs.value);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return comp::eq(lhs, rhs.value);
	  else
		  return lhs == rhs.value;
  }
  template <typename ValueType>
  bool operator!=(const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType>)
		  return comp::ne(lhs.value, rhs.value);
	  else
		  return !(lhs.value == rhs.value);
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator!=(const Value<ValueType>& lhs, const Other& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return comp::ne(lhs.value, rhs);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return comp::ne(lhs.value, rhs);
	  else
		  return !(lhs.value==rhs);
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator!=(const Other& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return comp::ne(lhs, rhs.value);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return comp::ne(lhs, rhs.value);
	  else
		  return !(lhs == rhs.value);
  }
  template <typename ValueType>
  bool operator < (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType>)
		  return comp::lt(lhs.value, rhs.value);
	  else
	  return lhs.value < rhs.value;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator < (const Value<ValueType>& lhs, const Other& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return comp::lt(lhs.value, rhs);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return comp::lt(lhs.value, rhs);
	  else
		  return lhs.value < rhs;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator < (const Other& lhs, const Value<ValueType>& rhs) {
	  if constexpr (std::is_floating_point_v<ValueType> && std::is_integral_v<Other>)
		  return comp::lt(lhs, rhs.value);
	  else if constexpr (std::is_integral_v<ValueType> && std::is_floating_point_v<Other>)
		  return comp::lt(lhs, rhs.value);
	  else
		  return lhs < rhs.value;
  }
  template <typename ValueType>
  bool operator > (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator > (const Value<ValueType>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator > (const Other& lhs, const Value<ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename ValueType>
  bool operator <= (const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator <= (const Value<ValueType>& lhs, const Other& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator <= (const Other& lhs, const Value<ValueType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename ValueType>
  bool operator >= (const Value<ValueType>& lhs, const Value<ValueType>& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator >= (const Value<ValueType>& lhs, const Other& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  bool operator >= (const Other& lhs, const Value<ValueType>& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }


  template <typename ValueType>
  Value<ValueType> operator*(const Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value * rhs.value;
	  return res;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  Value<ValueType> operator*(const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value * std::forward<Other>(rhs);
	  return res;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
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
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  Value<ValueType> operator / (const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value / std::forward<Other>(rhs);
	  return res;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
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
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  Value<ValueType> operator + (const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value + std::forward<Other>(rhs);
	  return res;
  }
  template <typename ValueType, typename Other,
		  typename = std::enable_if_t<not std::is_same_v<std::decay_t<Value<ValueType>>, std::decay_t<Other>>>,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
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
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  Value<ValueType> operator - (const Value<ValueType>& lhs, Other &&rhs) {
	  Value<ValueType> res;
	  res.value = lhs.value - std::forward<Other>(rhs);
	  return res;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
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
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  Value<ValueType>& operator += (Value<ValueType>& lhs, Other &&rhs) {
	  lhs.value += std::forward<Other>(rhs);
	  return lhs;
  }
  template <typename ValueType>
  Value<ValueType>& operator -= (Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  lhs.value -= rhs.value;
	  return lhs;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  Value<ValueType>& operator -= (Value<ValueType>& lhs, Other &&rhs) {
	  lhs.value -= std::forward<Other>(rhs);
	  return lhs;
  }
  template <typename ValueType>
  Value<ValueType>& operator *= (Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  lhs.value *= rhs.value;
	  return lhs;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
  Value<ValueType>& operator *= (Value<ValueType>& lhs, Other &&rhs) {
	  lhs.value *= std::forward<Other>(rhs);
	  return lhs;
  }
  template <typename ValueType>
  Value<ValueType>& operator /= (Value<ValueType>& lhs, const Value<ValueType>& rhs) {
	  lhs.value /= rhs.value;
	  return lhs;
  }
  template <typename ValueType, typename Other,
		  requirements::NotSame<Value<ValueType>, Other> = true,
		  requirements::BinOperatorsExist<ValueType, Other> = true>
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
	  typename Value<ValueType>::Type value;
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
