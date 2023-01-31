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
   * in case there is a requirements to change an underlying type, it should
   * easy to do.
   * Therefore all the operators are declared and defined as well.
   * */

  template <typename Duration>
  struct Value {
	  using Type = traits::ValueType;
	  traits::ValueType value {0.0};
	  Value() = default;
	  template <typename Input, requirements::MayBecomeValue<Input> = true>
	  Value(Input input);
	  template <typename Input, requirements::MayBecomeValue<Input> = true>
	  Value& operator = (Input input);

	  std::string toString () const;
  };

  template <typename Duration>
  template <typename Input, requirements::MayBecomeValue<Input>>
  Value<Duration>::Value(Input input) : value(input) {}

  template <typename Duration>
  template <typename Input, requirements::MayBecomeValue<Input>>
  Value<Duration>& Value<Duration>::operator = (Input input) {
	  value = input;
	  return *this;
  }

  template <typename Duration>
  std::string Value<Duration>::toString () const {
	  return utils::toChars(value);
  }

  template <typename Duration>
  bool operator==(const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return lhs.value()==rhs.value();
  }
  template <typename Duration, typename Other, requirements::IsSuitable<Value<Duration>, Other> = true>
  bool operator==(const Value<Duration>& lhs, const Other& rhs) {
	  return lhs.value()==rhs;
  }
  template <typename Duration>
  bool operator!=(const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return !(lhs==rhs);
  }
  template <typename Duration, typename Other>
  bool operator!=(const Value<Duration>& lhs, const Other& rhs) {
	  return !(lhs==rhs);
  }
  template <typename Duration>
  bool operator < (const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return lhs.value () < rhs.value();
  }
  template <typename Duration, typename Other>
  bool operator < (const Value<Duration>& lhs, const Other& rhs) {
	  return lhs.value () < rhs;
  }
  template <typename Duration>
  bool operator > (const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename Duration, typename Other>
  bool operator > (const Value<Duration>& lhs, const Other& rhs) {
	  return (!(rhs == lhs) && !(lhs < rhs));
  }
  template <typename Duration>
  bool operator <= (const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return !(lhs > rhs);
  }
  template <typename Duration, typename Other>
  bool operator <= (const Value<Duration>& lhs, const Other& rhs) {
	  return !(lhs > rhs);
  }
  template <typename Duration>
  bool operator >= (const Value<Duration>& lhs, const Value<Duration>& rhs){
	  return !(lhs < rhs);
  }
  template <typename Duration, typename Other>
  bool operator >= (const Value<Duration>& lhs, const Other& rhs){
	  return !(lhs < rhs);
  }


  template <typename Duration>
  Value<Duration> operator*(const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return Value<Duration> {.value = lhs.value * rhs.value};
  }
  template <typename Duration>
  Value<Duration> operator / (const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return Value<Duration> {.value = lhs.value / rhs.value};
  }
  template <typename Duration>
  Value<Duration> operator + (const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return Value<Duration> {.value = lhs.value + rhs.value};
  }
  template <typename Duration>
  Value<Duration> operator-(const Value<Duration>& lhs, const Value<Duration>& rhs) {
	  return Value<Duration> {.value = lhs.value - rhs.value};
  }


  template <typename Duration>
  Value<Duration>& operator += (Value<Duration>& lhs, const Value<Duration>& rhs) {
  lhs.value += rhs.value;
  return lhs;
}
template <typename Duration>
Value<Duration>& operator -= (Value<Duration>& lhs, const Value<Duration>& rhs) {
	lhs.value -= rhs.value;
	return lhs;
}
template <typename Duration>
Value<Duration>& operator *= (Value<Duration>& lhs, const Value<Duration>& rhs) {
	lhs.value *= rhs.value;
	return lhs;
}
template <typename Duration>
Value<Duration>& operator /= (Value<Duration>& lhs, const Value<Duration>& rhs) {
	lhs.value /= rhs.value;
	return lhs;
}

template <typename Duration>
std::ostream& operator<<(std::ostream& os, const Value<Duration>& single_value) {
	return os << single_value.value;
}//!operator

template <typename Duration>
std::istream& operator>>(std::istream& is, Value<Duration>& single_value) {
	typename Value<Duration>::Type value;
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
