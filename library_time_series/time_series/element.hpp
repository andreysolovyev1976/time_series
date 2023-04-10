//
// Created by Andrey Solovyev on 13/03/2021.
//

#pragma once

#include "time_series/element_requirements.h"
#include "time_series/const_values.h"
#include "time_series/value.hpp"
#include "common_usage_library/timestamp.hpp"
#include "common_usage_library//types_requirements/ctor_input.h"
#include "common_usage_library/types_requirements/operators.h"

#include <iosfwd>
#include <string>
#include <type_traits>
#include <stdexcept>

#ifndef TS_ELEMENT_H
#define TS_ELEMENT_H

namespace time_series {
  
#ifndef __cpp_concepts
  template <typename Duration = culib::time::Seconds
		  , typename ElemType = Value<value::traits::ValueTypeDefault>
				  >
#else
  template<typename Duration = culib::time::Seconds,
		  typename ElemType = Value<value::traits::ValueTypeDefault> >
#endif
  struct Element final {
	  using mapped_type = ElemType;
	  using key_type = culib::time::Timestamp<Duration>;

	  /**
	   * @brief
	   * for map-alike collections
	   * */
	  using value_type = std::pair<key_type, mapped_type>;

	  Element() = default;
	  Element(Element const&) = default;
	  Element(Element &&) = default;
	  Element& operator = (Element const&) = default;
	  Element& operator = (Element &&) = default;

	  Element (const ElemType & e);
	  Element (ElemType &&e);
	  Element (const value_type &p);
	  Element (value_type &&p);
	  Element (culib::time::Timestamp<Duration> ts, ElemType && value);
	  Element (culib::time::Timestamp<Duration> ts, ElemType const& value);

	  Element& operator = (const ElemType &e);
	  Element& operator = (ElemType &&e);
	  Element& operator = (value_type &&p);
	  Element& operator = (const value_type &p);

	  const value_type& operator () () const;
	  value_type& operator()();
	  operator value_type () const;
	  operator mapped_type () const;
	  operator key_type () const;

	  const key_type& first () const;
	  key_type& first ();
	  const mapped_type& second () const;
	  mapped_type& second ();

	  std::string toString () const;

#ifndef __cpp_concepts
	  template <typename... ProtectionPack,
			  typename DummyArg = mapped_type,
			  time_series::requirements::HasMethod_ContainsZero<DummyArg> = true>
	  bool containsZero () const;
#else
	  bool containsZero () const
	  requires time_series::requirements::HasMethod_ContainsZero<mapped_type>;
#endif


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

	  culib::time::Timestamp<Duration> timestamp;
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
  Element<Duration, ElemType>::Element (culib::time::Timestamp<Duration> ts, ElemType &&value)
		  : timestamp (ts)
		  , value (std::move(value))
  {}

template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::Element (culib::time::Timestamp<Duration> ts, ElemType const& value)
		  : timestamp (ts)
		  , value (value)
  {}


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
  const typename Element<Duration, ElemType>::value_type& Element<Duration, ElemType>::operator () () const {
	  return {timestamp, value}; //todo: check what is a subject for reference - a pair itself or two referencies of the respective fields
  }

template <typename Duration, typename ElemType>
  typename Element<Duration, ElemType>::value_type& Element<Duration, ElemType>::operator()() {
	  return {timestamp, value}; //todo: check what is a subject for reference - a pair itself or two referencies of the respective fields
  }

template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::operator typename Element<Duration, ElemType>::value_type () const {
	  return {timestamp, value};
  }

template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::operator typename Element<Duration, ElemType>::mapped_type () const {
	  return value;
  }

template <typename Duration, typename ElemType>
  Element<Duration, ElemType>::operator Element<Duration, ElemType>::key_type () const {
	  return timestamp;
  }

template <typename Duration, typename ElemType>
  const typename Element<Duration, ElemType>::key_type& Element<Duration, ElemType>::first () const {
	  return timestamp;
  }

template <typename Duration, typename ElemType>
  typename Element<Duration, ElemType>::key_type& Element<Duration, ElemType>::first () {
	  return timestamp;
  }

template <typename Duration, typename ElemType>
  const typename Element<Duration, ElemType>::mapped_type& Element<Duration, ElemType>::second () const {
	  return value;
  }

template <typename Duration, typename ElemType>
  typename Element<Duration, ElemType>::mapped_type& Element<Duration, ElemType>::second () {
	  return value;
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
#ifndef __cpp_concepts
  template <typename... ProtectionPack,
		  typename DummyArg,
		  time_series::requirements::HasMethod_ContainsZero<DummyArg> R>
		    bool Element<Duration, ElemType>::containsZero () const
			{
	  static_assert(sizeof...(ProtectionPack)==0u, "Do not specify template arguments for Element.containsZero()!");
	  		return value.containsZero();
	  }
#else
  bool Element<Duration, ElemType>::containsZero () const
  requires time_series::requirements::HasMethod_ContainsZero<typename Element<Duration, ElemType>::mapped_type>
  {
	  return value.containsZero();
  }
#endif

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

  /**
   * -----------------------------------------------------\n
   * Operators\n
   * -----------------------------------------------------\n
   * */


template <typename Duration, typename ElemType>
  bool operator == (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  return lhs.timestamp == rhs.timestamp && lhs.value == rhs.value;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator == (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return lhs.value==rhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
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
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator != (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return !(lhs==rhs);
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
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
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator < (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return lhs.value < rhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
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
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator > (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return (!(lhs == rhs) && !(lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
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
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator <= (const Element<Duration, ElemType>& lhs, const Other& rhs) {
	  return ((lhs == rhs) || (lhs < rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
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
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator >= (const Element<Duration, ElemType>& lhs, const Other& rhs){
	  return ((lhs == rhs) || (lhs > rhs));
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  bool operator >= (const Other& lhs, const Element<Duration, ElemType>& rhs) {
	  return ((lhs == rhs) || (lhs > rhs));
  }



template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator * (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value * rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator * (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value * rhs;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator * (Other &&lhs, const Element<Duration, ElemType>& rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = rhs.timestamp;
	  res.value = lhs * rhs.value;
	  return res;
  }
template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator / (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  bool divider_is_zero {false};
	  if constexpr (time_series::requirements::has_method_contains_zero_v<ElemType>) {
		  divider_is_zero = rhs.containsZero();
	  } else if constexpr (time_series::requirements::is_somparable_to_zero_v<ElemType>) {
		  divider_is_zero = rhs.value == 0;
	  }
	  if (divider_is_zero) { throw std::invalid_argument("Trying division by zero");}

	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value / rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator / (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  bool divider_is_zero {false};
	  if constexpr (time_series::requirements::has_method_contains_zero_v<Other>) {
		  divider_is_zero = rhs.containsZero();
	  } else if constexpr (time_series::requirements::is_somparable_to_zero_v<Other>) {
		  divider_is_zero = rhs == 0;
	  }
	  if (divider_is_zero) { throw std::invalid_argument("Trying division by zero");}

	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value / rhs;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator / (Other &&lhs, const Element<Duration, ElemType>& rhs) {
	  bool divider_is_zero {false};
	  if constexpr (time_series::requirements::has_method_contains_zero_v<ElemType>) {
		  divider_is_zero = rhs.containsZero();
	  } else if constexpr (time_series::requirements::is_somparable_to_zero_v<ElemType>) {
		  divider_is_zero = rhs.value == 0;
	  }
	  if (divider_is_zero) { throw std::invalid_argument("Trying division by zero");}


	  Element<Duration, ElemType> res;
	  res.timestamp = rhs.timestamp;
	  res.value = lhs / rhs.value;
	  return res;
  }
template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator + (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value + rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator + (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value + rhs;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator + (Other &&lhs, const Element<Duration, ElemType>& rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = rhs.timestamp;
	  res.value = lhs + rhs.value;
	  return res;
  }
template <typename Duration, typename ElemType>
  Element<Duration, ElemType> operator - (const Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value - rhs.value;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator - (const Element<Duration, ElemType>& lhs, Other &&rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = lhs.timestamp;
	  res.value = lhs.value - rhs;
	  return res;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType> operator - (Other &&lhs, const Element<Duration, ElemType>& rhs) {
	  Element<Duration, ElemType> res;
	  res.timestamp = rhs.timestamp;
	  res.value = lhs - rhs.value;
	  return res;
  }


template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator += (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.value += rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType>& operator += (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  lhs.value += rhs;
	  return lhs;
  }

template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator -= (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.value -= rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType>& operator -= (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  lhs.value -= rhs;
	  return lhs;
  }

template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator *= (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  lhs.value *= rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType>& operator *= (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  lhs.value *= rhs;
	  return lhs;
  }

template <typename Duration, typename ElemType>
  Element<Duration, ElemType>& operator /= (Element<Duration, ElemType>& lhs, const Element<Duration, ElemType>& rhs) {
	  bool divider_is_zero {false};
	  if constexpr (time_series::requirements::has_method_contains_zero_v<ElemType>) {
		  divider_is_zero = rhs.containsZero();
	  } else if constexpr (time_series::requirements::is_somparable_to_zero_v<ElemType>) {
		  divider_is_zero = rhs.value == 0;
	  }
	  if (divider_is_zero) { throw std::invalid_argument("Trying division by zero");}

	  lhs.value /= rhs.value;
	  return lhs;
  }
#ifndef __cpp_concepts
  template <typename Duration, typename ElemType, typename Other,
		  culib::requirements::NotSame<Element<Duration, ElemType>, Other> = true,
		  culib::requirements::BinOperatorsExist<ElemType, Other> = true>
#else
  template <typename Duration, typename ElemType, typename Other>
  requires culib::requirements::NotSame<Element<Duration, ElemType>, Other> &&
  culib::requirements::BinOperatorsExist<ElemType, Other>
#endif
  Element<Duration, ElemType>& operator /= (Element<Duration, ElemType>& lhs, Other &&rhs) {
	  bool divider_is_zero {false};
	  if constexpr (time_series::requirements::has_method_contains_zero_v<Other>) {
		  divider_is_zero = rhs.containsZero();
	  } else if constexpr (time_series::requirements::is_somparable_to_zero_v<Other>) {
		  divider_is_zero = rhs == 0;
	  }
	  if (divider_is_zero) { throw std::invalid_argument("Trying division by zero");}

	  lhs.value /= rhs;
	  return lhs;
  }


template <typename Duration, typename ElemType>
  std::ostream& operator << (std::ostream& os, const Element<Duration, ElemType>& element) {
	  return os << element.timestamp << ' ' << element.value;
  }//!operator
  template <typename Duration, typename ElemType>
  std::istream& operator >> (std::istream& is, Element<Duration, ElemType>& element) {
	  culib::time::Timestamp<Duration> timestamp;
	  ElemType elem_type_value;
	  if (is) {

		  is >> timestamp >> elem_type_value;

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
 * to make a structured binding available\n\n
 *
 * */

namespace std {
template <typename Duration, typename ElemType>
  struct tuple_size<time_series::Element<Duration, ElemType>> : integral_constant<std::size_t, 2u> {};

#ifndef __cpp_concepts
  template<std::size_t Index, typename Duration, typename ElemType>
#else
  template<std::size_t Index, typename Duration, typename ElemType>
#endif
  struct tuple_element<Index, time_series::Element<Duration, ElemType>>
		  : tuple_element<Index, tuple<
				  typename time_series::Element<Duration, ElemType>::key_type,
				  typename time_series::Element<Duration, ElemType>::mapped_type>
		  >{};

}//!namespace



#endif //TS_ELEMENT_H
