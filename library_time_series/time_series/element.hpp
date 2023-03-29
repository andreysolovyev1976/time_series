//
// Created by Andrey Solovyev on 13/03/2021.
//

#pragma once

#include "time_series/const_values.h"
#include "time_series/timestamp.hpp"
#include "time_series/value.hpp"
#include "common_usage_library//types_requirements/ctor_input.h"
#include "common_usage_library/types_requirements/operators.h"

#include <iosfwd>
#include <string>
#include <type_traits>
#include <stdexcept>

#ifndef TS_ELEMENT_H
#define TS_ELEMENT_H

namespace time_series {

  //todo: add requirements on ElemType
  //todo: add requirements for Other - should be an Arithmetic or better - operator is defined?

  template <typename Duration = base::Seconds, typename ElemType = Value<value::traits::ValueTypeDefault>>
  struct Element final {
	  using elem_type = ElemType;
	  using key_type = base::Timestamp<Duration>;

	  /**
	   * @brief
	   * for map-alike collections
	   * */
	  using value_type = std::pair<base::Timestamp<Duration>, ElemType>;

	  Element() = default;
	  Element (const ElemType &e);
	  Element (ElemType &&e);
	  Element (value_type &&p);
	  Element (const value_type &p);
	  Element& operator = (const ElemType &e);
	  Element& operator = (ElemType &&e);
	  Element& operator = (value_type &&p);
	  Element& operator = (const value_type &p);

	  const value_type& operator () () const &;
	  value_type& operator()() &;
	  operator value_type () const;
	  operator elem_type () const;
	  operator key_type () const;

	  const key_type& first () const &;
	  key_type& first () &;
	  const elem_type& second () const &;
	  elem_type& second () &;

	  std::string toString () const;
	  operator std::string () const;

	  template <typename Fn, typename... Args>
	  decltype(auto) applyFunction (Fn&& fn, Args&& ...args) &;

	  template<std::size_t Index>
	  decltype(auto) get() &;

	  template<std::size_t Index>
	  decltype(auto) get() &&;

	  template<std::size_t Index>
	  decltype(auto) get() const &;

	  template<std::size_t Index>
	  decltype(auto) get() const &&;

	  base::Timestamp<Duration> timestamp;
	  ElemType value;

  private:

	  template<std::size_t Index, typename ThisType>
	  auto&& getImpl(ThisType&& t);

	  template<std::size_t Index, typename ThisType>
	  auto&& getImpl(ThisType&& t) const;
  };


  template <typename Duration, typename ElemType>
  struct ElementHasher {
	  size_t operator () (const Element<Duration, ElemType>& element) const {
		  return element.timestamp.time_point.time_since_epoch().count();
	  }
  };


  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::Element (const ElemType &e) : value (e)
  {}
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::Element (ElemType &&e) : value (std::move(e))
  {}

  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::Element (Element<Duration, ElemType>::value_type &&p)
		  : timestamp (std::move(p.first))
		  , value (std::move(p.second))
  {}
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::Element (const Element<Duration, ElemType>::value_type &p)
		  : timestamp (p.first)
		  , value (p.second)
  {}

  template <typename Duration, typename ElemType>
  const typename Element<Duration, ElemType>::value_type& Element<Duration, ElemType>::operator () () const & {
	  return {timestamp, value}; //todo: check what is a subject for reference - a pair itself or two referencies of the respective fields
  }
  template <typename Duration, typename ElemType>
  typename Element<Duration, ElemType>::value_type& Element<Duration, ElemType>::operator()() & {
	  return {timestamp, value}; //todo: check what is a subject for reference - a pair itself or two referencies of the respective fields
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::operator typename Element<Duration, ElemType>::value_type () const {
	  return {timestamp, value};
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::operator typename Element<Duration, ElemType>::elem_type () const {
	  return value;
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::operator Element<Duration, ElemType>::key_type () const {
	  return timestamp;
  }

  template <typename Duration, typename ElemType>
  const typename Element<Duration, ElemType>::key_type& Element<Duration, ElemType>::first () const & {
	  return timestamp;
  }
  template <typename Duration, typename ElemType>
  typename Element<Duration, ElemType>::key_type& Element<Duration, ElemType>::first () & {
	  return timestamp;
  }
  template <typename Duration, typename ElemType>
  const typename Element<Duration, ElemType>::elem_type& Element<Duration, ElemType>::second () const & {
	  return value;
  }
  template <typename Duration, typename ElemType>
  typename Element<Duration, ElemType>::elem_type& Element<Duration, ElemType>::second () & {
	  return value;
  }

  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& Element<Duration, ElemType>::operator = (const ElemType &e) {
	  value = e;
	  return *this;
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& Element<Duration, ElemType>::operator = (ElemType &&e)  {
	  value = std::move(e);
	  return *this;
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& Element<Duration, ElemType>::operator = (value_type &&p) {
	  timestamp = std::move(p.first);
	  value = std::move(p.second);
	  return *this;
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& Element<Duration, ElemType>::operator = (const value_type &p) {
	  timestamp = p.first;
	  value = p.second;
	  return *this;
  }

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
  Element<Duration, ElemType>::operator std::string () const {
	  return toString();
  }


  template <typename Duration, typename ElemType>
  template <typename Fn, typename... Args>
  decltype(auto) Element<Duration, ElemType>::applyFunction (Fn&& fn, Args&& ...args) & {
	  if constexpr (std::is_same_v<std::invoke_result_t<Fn, ElemType&, Args...>, void>) {
		  std::invoke(std::forward<Fn>(fn), value, std::forward<Args...>(args...));
		  return *this;
	  }
	  else if constexpr (std::is_same_v<std::invoke_result_t<Fn, ElemType, Args...>, ElemType>) {
		  return Element<Duration, ElemType>(std::invoke(std::forward<Fn>(fn), value, std::forward<Args...>(args...)));
	  } else {
		  static_assert(sizeof(Fn) == 0, "Unexpected callable while applying over element");
	  }
  }

  template <typename Duration, typename ElemType>
  template<std::size_t Index>
  decltype(auto) Element<Duration, ElemType>::get() &  { return getImpl<Index>(*this); }

  template <typename Duration, typename ElemType>
  template<std::size_t Index>
  decltype(auto) Element<Duration, ElemType>::get() && { return getImpl<Index>(*this); }

  template <typename Duration, typename ElemType>
  template<std::size_t Index>
  decltype(auto) Element<Duration, ElemType>::get() const &  { return getImpl<Index>(*this); }

  template <typename Duration, typename ElemType>
  template<std::size_t Index>
  decltype(auto) Element<Duration, ElemType>::get() const && { return getImpl<Index>(*this); }


  template <typename Duration, typename ElemType>
  template<std::size_t Index, typename ThisType>
  auto&& Element<Duration, ElemType>::getImpl(ThisType&& t) {
	  static_assert(Index < 2u, "Index out of bounds for Element");
	  if constexpr (Index == 0) return std::forward<ThisType>(t).timestamp;
	  if constexpr (Index == 1) return std::forward<ThisType>(t).value;
  }

  template <typename Duration, typename ElemType>
  template<std::size_t Index, typename ThisType>
  auto&& Element<Duration, ElemType>::getImpl(ThisType&& t) const {
	  static_assert(Index < 2u, "Index out of bounds for Element");
	  if constexpr (Index == 0) return std::forward<ThisType>(t).timestamp;
	  if constexpr (Index == 1) return std::forward<ThisType>(t).value;
  }

  template <typename Duration, typename ElemType>
  bool operator == (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs.timestamp == rhs.timestamp && lhs.value == rhs.value;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator == (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return lhs.value==rhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator == (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs==rhs.value;
  }

  template <typename Duration, typename ElemType>
  bool operator != (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return !(lhs == rhs);
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator != (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return !(lhs==rhs);
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator != (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return !(rhs==lhs);
  }

  template <typename Duration, typename ElemType>
  bool operator < (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs.timestamp != rhs.timestamp ? lhs.timestamp < rhs.timestamp : lhs.value < rhs.value;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator < (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return lhs.value < rhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator < (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs < rhs.value;
  }

  template <typename Duration, typename ElemType>
  bool operator > (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return (!(lhs == rhs) && !(lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator > (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return (!(lhs == rhs) && !(lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator > (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return (!(lhs == rhs) && !(lhs < rhs));
  }

  template <typename Duration, typename ElemType>
  bool operator <= (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return ((lhs == rhs) || (lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator <= (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return ((lhs == rhs) || (lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator <= (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return ((lhs == rhs) || (lhs < rhs));
  }

  template <typename Duration, typename ElemType>
  bool operator >= (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs){
	  return ((lhs == rhs) || (lhs > rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator >= (const Element<Duration, ElemType>& lhs, const Other& rhs){
	  return ((lhs == rhs) || (lhs > rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator >= (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return ((lhs == rhs) || (lhs > rhs));
  }



  template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator * (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = {0},
			  .value = lhs.value * rhs.value };
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator * (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value * rhs};
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
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
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator / (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value / rhs };
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
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
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator + (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value + rhs };
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator + (Other &&rhs, const Element<Duration, ElemType>& lhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs + rhs.data };
  }
  template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator - (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = {0},
			  .value = lhs.value - rhs.value };
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator - (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs.value - rhs};
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator - (Other &&rhs, const Element<Duration, ElemType>& lhs) {
	  return Element<Duration, ElemType> {
			  .timestamp = lhs.timestamp,
			  .value = lhs - rhs.data };
  }


  template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator += (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.timestamp = {0};
	  lhs.value += rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
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
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
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
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
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
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
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

/**
 * @details
 * Template specialization for std:: namespace \n
 * to make a structural binding available\n\n
 *
 * */

namespace std {
  template <typename Duration, typename ElemType>
  struct tuple_size<time_series::Element<Duration, ElemType>> : integral_constant<std::size_t, 2u> {};

  template<std::size_t Index, typename Duration, typename ElemType>
  struct tuple_element<Index, time_series::Element<Duration, ElemType>>
		  : tuple_element<Index, tuple<
				  typename time_series::Element<Duration, ElemType>::key_type,
				  typename time_series::Element<Duration, ElemType>::elem_type>
		  >{};

}//!namespace

#if 0
//todo: can be used here

  template<std::size_t Index, typename... Iterators>
struct tuple_element<Index, itertools::ZipIterator<Iterators...>> {
	using type = decltype(std::get<Index>(std::declval<itertools::ZipIterator<Iterators...>>().operator*() ));
};

#endif


#endif //TS_ELEMENT_H
