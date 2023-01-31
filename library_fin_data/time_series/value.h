//
// Created by Andrey Solovyev on 19/06/2022.
//

#pragma once

#include "const_values.h"
#include "timestamp.h"
#include "floating_point_comp.h"
#include "utils.h"
#include "value_type.h"
#include "type_requirements.h"

#ifndef BASE_VALUE_H
#define BASE_VALUE_H

namespace base {

  //todo: add requirement for Other - should be an Arithmetic or better - operator is defined?

  /**
   * @details
   * This is a wrapper, but it required from a design perspective:
   * in case there is a requirements to change an underlying type, it should be
   * easy to do.
   * Therefore all the operators are declared and defined as well.
   * */

  template <typename Duration = base::Nanoseconds, typename ValueType = traits::ValueTypeDefault>
  struct Value : public std::__1::error_code {
	  using type = ValueType;
	  ValueType value {0.0};
	  Value() = default;
	  template <typename Input,
			  requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
	  Value(Input input);
	  template <typename Input,
			  requirements::ConveribleOrConstructibleFromTo<Input, ValueType> = true>
	  Value& operator = (Input input);
	  Value (std::string input);
	  std::string toString () const;
  };

  template <typename Duration, typename ValueType>
  template <typename Input,
		  requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
  Value<Duration, ValueType>::Value(Input input) : value(input) {}

  template <typename Duration, typename ValueType>
  template <typename Input,
		  requirements::ConveribleOrConstructibleFromTo<Input, ValueType>>
  Value<Duration, ValueType>& Value<Duration, ValueType>::operator = (Input input) {
	  value = input;
	  return *this;
  }
  template <typename Duration, typename ValueType>
  Value<Duration, ValueType>::Value (std::string input) {
	  auto input_number = base::utils::fromChars(input);
	  using Input = decltype(input_number);
	  static_assert(requirements::isConveribleOrConstructible<Input, ValueType>());
	  value = input_number;
  }

  template <typename Duration, typename ValueType>
  std::string Value<Duration, ValueType>::toString () const {
	  return utils::toChars(value);
  }

  template <typename Duration, typename ValueType>
  bool operator==(const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return lhs.value == rhs.value;
  }
  template <typename Duration, typename ValueType, typename Other,
          requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator==(const Value<Duration, ValueType>& lhs, const Other& rhs) {
	  return lhs.value == rhs;
  }
  template <typename Duration, typename ValueType, typename Other,
          requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator==(const Other& lhs, const Value<Duration, ValueType>& rhs) {
	  return lhs == rhs.value;
  }
  template <typename Duration, typename ValueType>
  bool operator!=(const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return !(lhs==rhs);
  }
  template <typename Duration, typename ValueType, typename Other, 
          requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator!=(const Value<Duration, ValueType>& lhs, const Other& rhs) {
	  return !(lhs==rhs);
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator!=(const Other& lhs, const Value<Duration, ValueType>& rhs) {
	  return !(lhs==rhs);
  }
  template <typename Duration, typename ValueType>
  bool operator < (const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return lhs.value < rhs.value;
  }
  template <typename Duration, typename ValueType, typename Other, 
          requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator < (const Value<Duration, ValueType>& lhs, const Other& rhs) {
	  return lhs.value < rhs;
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator < (const Other& lhs, const Value<Duration, ValueType>& rhs) {
	  return lhs < rhs.value;
  }
  template <typename Duration, typename ValueType>
  bool operator > (const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename Duration, typename ValueType, typename Other, 
          requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator > (const Value<Duration, ValueType>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator > (const Other& lhs, const Value<Duration, ValueType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename Duration, typename ValueType>
  bool operator <= (const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename Duration, typename ValueType, typename Other, 
          requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator <= (const Value<Duration, ValueType>& lhs, const Other& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator <= (const Other& lhs, const Value<Duration, ValueType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename Duration, typename ValueType>
  bool operator >= (const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
  template <typename Duration, typename ValueType, typename Other, 
          requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator >= (const Value<Duration, ValueType>& lhs, const Other& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  bool operator >= (const Other& lhs, const Value<Duration, ValueType>& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }


  template <typename Duration, typename ValueType>
  Value<Duration, ValueType> operator*(const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value * rhs.value};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator*(const Value<Duration, ValueType>& lhs, Other &&rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value * rhs};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator*(Other &&lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs * rhs.value};
  }
  template <typename Duration, typename ValueType>
  Value<Duration, ValueType> operator / (const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value / rhs.value};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator / (const Value<Duration, ValueType>& lhs, Other &&rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value / rhs};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator / (Other &&lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs / rhs.value};
  }
  template <typename Duration, typename ValueType>
  Value<Duration, ValueType> operator + (const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value + rhs.value};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator + (const Value<Duration, ValueType>& lhs, Other &&rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value + rhs};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator + (Other &&lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs + rhs.value};
  }
  template <typename Duration, typename ValueType>
  Value<Duration, ValueType> operator-(const Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value - rhs.value};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator-(const Value<Duration, ValueType>& lhs, Other &&rhs) {
	  return Value<Duration, ValueType> {.value = lhs.value - rhs};
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType> operator-(Other &&lhs, const Value<Duration, ValueType>& rhs) {
	  return Value<Duration, ValueType> {.value = lhs - rhs.value};
  }


  template <typename Duration, typename ValueType>
  Value<Duration, ValueType>& operator += (Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  lhs.value += rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType>& operator += (Value<Duration, ValueType>& lhs, Other &&rhs) {
	  lhs.value += rhs;
	  return lhs;
  }
  template <typename Duration, typename ValueType>
  Value<Duration, ValueType>& operator -= (Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  lhs.value -= rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType>& operator -= (Value<Duration, ValueType>& lhs, Other &&rhs) {
	  lhs.value -= rhs;
	  return lhs;
  }
  template <typename Duration, typename ValueType>
  Value<Duration, ValueType>& operator *= (Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  lhs.value *= rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType>& operator *= (Value<Duration, ValueType>& lhs, Other &&rhs) {
	  lhs.value *= rhs;
	  return lhs;
  }
  template <typename Duration, typename ValueType>
  Value<Duration, ValueType>& operator /= (Value<Duration, ValueType>& lhs, const Value<Duration, ValueType>& rhs) {
	  lhs.value /= rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ValueType, typename Other,
		  requirements::BinOperatorsExist<Value<Duration, ValueType>, Other> = true>
  Value<Duration, ValueType>& operator /= (Value<Duration, ValueType>& lhs, Other &&rhs) {
	  lhs.value /= rhs;
	  return lhs;
  }

  template <typename Duration, typename ValueType>
  std::ostream& operator<<(std::ostream& os, const Value<Duration, ValueType>& single_value) {
	  return os << single_value.value;
  }//!operator

  template <typename Duration, typename ValueType>
  std::istream& operator>>(std::istream& is, Value<Duration, ValueType>& single_value) {
	  typename Value<Duration, ValueType>::Type value;
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
