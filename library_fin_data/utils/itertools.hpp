//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <type_traits>
#include <memory>
#include <utility>

#ifndef ITERTOOLS_H
#define ITERTOOLS_H

namespace iterators {

  namespace requirements {

	template<typename Iter>
	using AccessTypeFor = typename Iter::reference;

	template<typename Type>
	using IteratorTypeSelect = std::conditional_t<
			std::is_const_v<std::remove_reference_t<Type>>,
			typename std::decay_t<Type>::const_iterator,
			typename std::decay_t<Type>::iterator>;


	template<typename Container, typename = void>
	struct MaybeContainer : std::false_type {};

	template<typename Container>
	struct MaybeContainer<Container,
						  std::void_t<
								  decltype(std::declval<Container>().begin()),
								  decltype(std::declval<Container>().end())
						  >
	> : std::true_type {};

	template<typename Container>
	constexpr bool is_container_v { MaybeContainer<Container>::value };

	template<typename Container>
	using IsContainer = std::enable_if_t<is_container_v<Container>, bool>;

  }//!namespace

  template<typename Iterator>
  class Range {
  public:
	  Range(Iterator begin, Iterator end) : begin_(begin) , end_(end) {}
	  auto begin() const { return begin_; }
	  auto end() const { return end_; }
  private:
	  Iterator begin_;
	  Iterator end_;
  };

  template<typename Iter1, typename Iter2>
  class ZipIterator_ {
  public:
	  using value_type = typename std::pair<
			  requirements::AccessTypeFor<Iter1>,
			  requirements::AccessTypeFor<Iter2>
	  >;

	  ZipIterator_() = delete;

	  ZipIterator_(Iter1 iter_1, Iter2 iter_2)
			  : iter_1{iter_1}
			  , iter_2{iter_2}
	  {}

	  ZipIterator_& operator++() {
		  ++iter_1;
		  ++iter_2;
		  return *this;
	  }
#if 0
	  ZipIterator_ operator++(int) {
		  auto tmp = *this;
		  ++*this;
		  return tmp;
	  }
#endif
	  bool operator==(ZipIterator_ const& other) const { return iter_1==other.iter_1 || iter_2==other.iter_2; }
	  bool operator!=(ZipIterator_ const& other) const { return !(*this == other); }
	  auto operator*() -> value_type { return value_type{*iter_1, *iter_2}; }
  private:
	  Iter1 iter_1;
	  Iter2 iter_2;
  };


  template<typename Iter1, typename Iter2>
  class ZipIterator {
  public:
	  using value_type = typename std::pair<
			  requirements::AccessTypeFor<Iter1>,
			  requirements::AccessTypeFor<Iter2>
	  >;

	  ZipIterator() = delete;

	  ZipIterator(Iter1 iter_1, Iter2 iter_2)
			  : iter_1{iter_1}
			  , iter_2{iter_2}
			  {}

	  ZipIterator& operator++() {
		  ++iter_1;
		  ++iter_2;
		  return *this;
	  }
#if 0
	  ZipIterator operator++(int) {
		  auto tmp = *this;
		  ++*this;
		  return tmp;
	  }
#endif
	  bool operator==(ZipIterator const& other) const { return iter_1==other.iter_1 || iter_2==other.iter_2; }
	  bool operator!=(ZipIterator const& other) const { return !(*this == other); }
	  auto operator*() -> value_type { return value_type{*iter_1, *iter_2}; }
  private:
	  Iter1 iter_1;
	  Iter2 iter_2;
  };

  template<
		  typename Container1
		  , typename Container2
		  , requirements::IsContainer<Container1> = true
		  , requirements::IsContainer<Container2> = true
		  >
  class Zipper {
  public:
	  using Iter1 = requirements::IteratorTypeSelect<Container1>;
	  using Iter2 = requirements::IteratorTypeSelect<Container2>;
	  using zip_type = ZipIterator<Iter1, Iter2>;

	  Zipper() = delete;

	  explicit
	  Zipper(Container1&& a, Container2&& b)
			  : begin_1 (a.begin())
			  , end_1 (a.end())
			  , begin_2 (b.begin())
			  , end_2 (b.end())
			  {}

	  zip_type begin(){ return zip_type {begin_1, begin_2}; }
	  zip_type end() { return zip_type {end_1, end_2}; }
  private:
	  Iter1 begin_1, end_1;
	  Iter2 begin_2, end_2;
  };

  template<typename T, typename U>
  auto zip(T&& t, U&& u) {
	  Zipper<T, U> zipper (std::forward<T>(t), std::forward<U>(u));
	  return Range<typename Zipper<T, U>::zip_type>(zipper.begin(), zipper.end());
  }
}//!namespace
#endif //ITERTOOLS_H
