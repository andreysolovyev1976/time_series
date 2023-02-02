//
// Created by Andrey Solovyev on 13/03/2021.
//

#pragma once

#include "const_values.h"
#include "timestamp.h"
#include "value.h"
#include "type_requirements.h"

#include <iosfwd>
#include <string>
#include <type_traits>

#ifndef TS_ELEMENT_H
#define TS_ELEMENT_H

namespace time_series {

  //todo: add requirements on ElemType
  //todo: add requirements for Other - should be an Arithmetic or better - operator is defined?
  //todo: add requirements for Function - should be callable over ElemType


  template <typename Duration, typename ElemType = base::Value<Duration>>
  struct Element final {

	  Element() = default;
	  explicit Element (const ElemType &e);
	  explicit Element (ElemType &&e);

	  std::string toString () const;

	  base::Timestamp<Duration> timestamp;
	  ElemType value;

	  template <typename Fn, requirements::Unary<Fn, ElemType> = true>
	  Element& applyFunction (Fn &&fn);
  };

  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::Element (const ElemType &e) : value (e)
  {}
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::Element (ElemType &&e) : value (std::move(e))
  {}

  template <typename Duration, typename ElemType>
  std::string Element<Duration, ElemType>::toString () const {
	  using namespace std::string_literals;
	  std::string res;
	  res.reserve(const_values::EXPECTED_ELEMENT_LENGTH);
	  res.append(timestamp.toString());
	  res.append(" "s);
	  res.append(value.toString());
	  return res;
  }

  template <typename Duration, typename ElemType>
  template <typename Fn, requirements::Unary<Fn, ElemType>>
  Element<Duration, ElemType>& Element<Duration, ElemType>::applyFunction (Fn &&fn) {
	  std::invoke(fn, value);
	  return *this;
  }


  template <typename Duration, typename ElemType>
  bool operator == (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs.timestamp == rhs.timestamp && lhs.value == rhs.value;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator == (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return lhs.value==rhs;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator == (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs==rhs.value;
  }

  template <typename Duration, typename ElemType>
  bool operator != (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return !(lhs == rhs);
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator != (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return !(lhs==rhs);
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator != (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return !(rhs==lhs);
  }

  template <typename Duration, typename ElemType>
  bool operator < (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs.timestamp != rhs.timestamp ? lhs.timestamp < rhs.timestamp : lhs.value < rhs.value;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator < (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return lhs.value < rhs;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator < (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs < rhs.value;
  }

  template <typename Duration, typename ElemType>
  bool operator > (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return (!(operator == <ElemType, Duration> (rhs, lhs)) && !(operator< <ElemType, Duration> (lhs, rhs)));
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator > (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return (!(operator == <ElemType, Duration> (rhs, lhs)) && !(operator < <ElemType, Duration> (lhs, rhs)));
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator > (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }

  template <typename Duration, typename ElemType>
  bool operator <= (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator <= (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator <= (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return ((rhs == lhs) || (lhs < rhs));
  }

  template <typename Duration, typename ElemType>
  bool operator >= (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator >= (const Element<Duration, ElemType>& lhs, const Other& rhs){
	  return ((rhs == lhs) || (lhs > rhs));
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  bool operator >= (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return ((rhs == lhs) || (lhs > rhs));
  }



  template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator * (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = {0},
			  .value = lhs.value * rhs.value };
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator * (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value * rhs};
  }
  template <typename Duration, typename ElemType, typename Other,
		  requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator * (Other &&lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs * rhs.value };
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator / (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = {0},
			  .value = lhs.value / rhs.value };
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator / (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value / rhs };
  }
  template <typename Duration, typename ElemType, typename Other,
		  requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator / (Other &&lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs / rhs.value };
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator + (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = {0},
			  .value = lhs.value + rhs.value };
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator + (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value + rhs };
  }
  template <typename Duration, typename ElemType, typename Other,
		  requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator + (Other &&rhs, const Element<Duration, ElemType>& lhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs + rhs.value };
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator - (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = {0},
			  .value = lhs.value - rhs.value };
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator - (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value - rhs};
  }
  template <typename Duration, typename ElemType, typename Other,
		  requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType> operator - (Other &&rhs, const Element<Duration, ElemType>& lhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs - rhs.value };
  }

  
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator += (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value += rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType>& operator += (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  lhs.value += rhs;
	  return lhs;
  }

  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator -= (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value -= rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType>& operator -= (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  lhs.value -= rhs;
	  return lhs;
  }
  
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator *= (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value *= rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType>& operator *= (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  lhs.value *= rhs;
	  return lhs;
  }
  
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator /= (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value /= rhs.value;
	  return lhs;
  }
  template <typename Duration, typename ElemType, typename Other, 
          requirements::BinOperatorsExist<Element<Duration, ElemType>, Other> = true>
  Element<Duration, ElemType>& operator /= (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  lhs.value /= rhs;
	  return lhs;
  }

  
  
  template <typename Duration, typename ElemType>
  std::ostream& operator << (std::ostream& os, const Element<Duration, ElemType>& element) {
	  return os << element.timestamp << ' ' << element.value;
  }//!operator
  template <typename Duration, typename ElemType>
  std::istream& operator >> (std::istream& is, Element<Duration, ElemType>& element) {
	  base::Timestamp<Duration> timestamp;
	  ElemType elem_type_value;
	  if (is) {
		  is >> timestamp;
		  if (is && !is.eof()) {
			  is >> elem_type_value;
		  }

		  if (is && !is.eof()) {
			  element.timestamp = timestamp;
			  element.value = std::move(elem_type_value);
		  }
		  else {
			  is.setstate(std::ios_base::failbit);
		  }
	  }
	  return is;
  }//!operator

}//!namespace
#endif //TS_ELEMENT_H
