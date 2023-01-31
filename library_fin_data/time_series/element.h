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


  template <typename ElemType, typename Duration>
  struct Element final {

	  Element() = default;
	  Element (const ElemType &e);
	  Element (ElemType &&e);

	  std::string toString () const;

	  base::Timestamp<Duration> timestamp;
	  ElemType value;

	  template <typename Fn, requirements::Unary<Fn, ElemType> = true>
	  Element& applyFunction (Fn &&fn);
  };

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration>::Element (const ElemType &e) : value (e)
  {}
  template <typename ElemType, typename Duration>
  Element<ElemType, Duration>::Element (ElemType &&e) : value (std::move(e))
  {}

  template <typename ElemType, typename Duration>
  std::string Element<ElemType, Duration>::toString () const {
	  using namespace std::string_literals;
	  std::string res;
	  res.reserve(const_values::EXPECTED_ELEMENT_LENGTH);
	  res.append(timestamp.toString());
	  res.append(" "s);
	  res.append(value.toString());
	  return res;
  }

  template <typename ElemType, typename Duration>
  template <typename Fn, requirements::Unary<Fn, ElemType>>
  Element<ElemType, Duration>& Element<ElemType, Duration>::applyFunction (Fn &&fn) {
	  std::invoke(fn, value);
	  return *this;
  }


  template <typename ElemType, typename Duration>
  bool operator==(const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return lhs.timestamp == rhs.timestamp && lhs.value == rhs.value;
  }

  template <typename ElemType, typename Duration, typename Other>
  bool operator == (const Element<ElemType, Duration>& lhs, const Other& rhs) {
	  return lhs.value == rhs;
  }

  template <typename ElemType, typename Duration>
  bool operator!=(const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return !(operator == <ElemType, Duration> (lhs,rhs));
  }

  template <typename ElemType, typename Duration>
  bool operator < (const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return lhs.timestamp != rhs.timestamp ? lhs.timestamp < rhs.timestamp : lhs.value < rhs.value;
  }

  template <typename ElemType, typename Duration, typename Other>
  bool operator < (const Element<ElemType, Duration>& lhs, const Other& rhs) {
	  return lhs.value < rhs;
  }

  template <typename ElemType, typename Duration>
  bool operator > (const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return (!(operator == <ElemType, Duration> (rhs, lhs)) && !(operator< <ElemType, Duration> (lhs, rhs)));
  }

  template <typename ElemType, typename Duration, typename Other>
  bool operator > (const Element<ElemType, Duration>& lhs, const Other& rhs) {
	  return (!(operator == <ElemType, Duration> (rhs, lhs)) && !(operator < <ElemType, Duration> (lhs, rhs)));
  }

  template <typename ElemType, typename Duration>
  bool operator <= (const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return !(operator > <ElemType, Duration> (lhs, rhs));
  }

  template <typename ElemType, typename Duration, typename Other>
  bool operator <= (const Element<ElemType, Duration>& lhs, const Other& rhs) {
	  return !(operator > <ElemType, Duration> (lhs, rhs));
  }

  template <typename ElemType, typename Duration>
  bool operator >= (const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs){
	  return !(operator < <ElemType, Duration> (lhs, rhs));
  }

  template <typename ElemType, typename Duration, typename Other>
  bool operator >= (const Element<ElemType, Duration>& lhs, const Other& rhs){
	  return !(operator < <ElemType, Duration> (lhs, rhs));
  }

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration> operator*(const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = {0},
			  .value = lhs.value * rhs.value };
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration> operator*(const Element<ElemType, Duration>& lhs, Other rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value * rhs.value };
  }

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration> operator / (const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = {0},
			  .value = lhs.value / rhs.value };
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration> operator/(const Element<ElemType, Duration>& lhs, Other rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value / rhs };
  }

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration> operator + (const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = {0},
			  .value = lhs.value + rhs.value };
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration> operator + (const Element<ElemType, Duration>& lhs, Other rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value + rhs };
  }
  template <typename ElemType, typename Duration>
  Element<ElemType, Duration> operator-(const Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = {0},
			  .value = lhs.value - rhs.value };
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration> operator - (const Element<ElemType, Duration>& lhs, Other rhs) {
	  return Element<ElemType, Duration> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value - rhs.value };
  }

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration>& operator += (Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value += rhs.value;
	  return lhs;
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration>& operator += (Element<ElemType, Duration>& lhs, Other rhs) {
	  lhs.value += rhs;
	  return lhs;
  }

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration>& operator -= (Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value -= rhs.value;
	  return lhs;
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration>& operator -= (Element<ElemType, Duration>& lhs, Other rhs) {
	  lhs.value -= rhs;
	  return lhs;
  }

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration>& operator *= (Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value *= rhs.value;
	  return lhs;
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration>& operator *= (Element<ElemType, Duration>& lhs, Other rhs) {
	  lhs.value *= rhs;
	  return lhs;
  }

  template <typename ElemType, typename Duration>
  Element<ElemType, Duration>& operator /= (Element<ElemType, Duration>& lhs, const Element<ElemType, Duration>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value /= rhs.value;
	  return lhs;
  }

  template <typename ElemType, typename Duration, typename Other>
  Element<ElemType, Duration>& operator /= (Element<ElemType, Duration>& lhs, Other rhs) {
	  lhs.value /= rhs;
	  return lhs;
  }

  template <typename ElemType, typename Duration>
  std::ostream& operator<<(std::ostream& os, const Element<ElemType, Duration>& element) {
	  return os << element.timestamp << ' ' << element.value;
  }//!operator

  template <typename ElemType, typename Duration>
  std::istream& operator>>(std::istream& is, Element<ElemType, Duration>& element) {
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
