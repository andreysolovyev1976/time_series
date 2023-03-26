//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include "types_requirements/container.h"

#include <utility>
#include <tuple>
#include <iterator>
#include <type_traits>

#ifndef ITERTOOLS_H
#define ITERTOOLS_H

namespace culib::itertools {

  /**
 * @brief
 * Common element, can be used anywhere
 * */
  namespace base::utils {
	template<typename... T1, typename... T2, std::size_t... I>
	constexpr auto weakComparisonImpl(std::tuple<T1...> const& t1, std::tuple<T2...> const& t2, std::index_sequence<I...>) {
		bool result {false};
		((result = result || std::get<I>(t1) == std::get<I>(t2)) ,...);
		return result;
	}
	template<typename... T1, typename... T2>
	constexpr auto weakComparison(std::tuple<T1...> const& t1, std::tuple<T2...> const& t2) {
		static_assert(sizeof...(T1) == sizeof...(T2));
		return weakComparisonImpl(t1, t2, std::make_index_sequence<sizeof...(T1)>{});
	}
  }

  namespace details {
	/**
   * @brief
   * Zip Iterator
   * */
	template<typename... Iter>
	class ZipIterator {
	private:
		template<typename SomeIter>
		using AccessTypeFor = typename SomeIter::reference;

	public:
		using iterator_type = ZipIterator;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename std::tuple<AccessTypeFor<Iter>...>;
		using difference_type = int;
		using pointer = value_type *;
		using reference = value_type &;


		ZipIterator() = delete;

		explicit
		ZipIterator(Iter... iter)
				:iterators(std::make_tuple(iter...)) { }

		ZipIterator& operator++()
		{
			std::apply([](Iter& ... iter) { ((++iter), ...); }, iterators);
			return *this;
		}

		bool operator==(ZipIterator const& other) const
		{
			return base::utils::weakComparison(this->iterators, other.iterators);
		}
		bool operator!=(ZipIterator const& other) const { return !(*this==other); }
		value_type operator*() { return makeRefs(); }

	private:
		std::tuple<Iter...> iterators;

		template<std::size_t... I>
		auto makeRefsImpl(std::index_sequence<I...>)
		{
			return std::tuple<AccessTypeFor<Iter>...>({std::get<I>(iterators).operator*()...});
		}
		auto makeRefs()
		{
			return makeRefsImpl(std::make_index_sequence<sizeof...(Iter)>{});
		}
	};

	/**
   * @brief
   * Zipper, using ZipIterators
   * */
	template<typename... Container>
	class Zipper {
	private:

		/**
		 * @brief
		 * This seemingly innocent iterator type selection serves
		 * as a guard - no Type without an iterator can pass this.
		 * */
		template<typename Type>
		using IteratorTypeSelect = std::conditional_t<
				std::is_const_v<std::remove_reference_t<Type>>,
				typename std::remove_reference_t<Type>::const_iterator,
				typename std::remove_reference_t<Type>::iterator>;

	public:
		using zip_type = ZipIterator<IteratorTypeSelect<Container>...>;

		Zipper() = delete;

		explicit
		Zipper(Container&& ... containers)
				:begin_(containers.begin()...), end_(containers.end()...) { }

		zip_type begin() const { return begin_; }
		zip_type end() const { return end_; }
	private:
		zip_type begin_, end_;
	};
  }//!details

  /**
 * @brief
 * UI
 * */
#ifndef __cpp_concepts
  template<typename... Containers, requirements::AreAllContainers<Containers...> = true>
#else
  template<requirements::AreAllContainers... Containers>
#endif
  auto zip(Containers&&... containers) {
	  return details::Zipper<Containers...> (std::forward<Containers>(containers)...);
  }


  namespace details {
		//todo Add Iterators requirements
	/**
   * @brief
   * pair of iterators
   * */
	template<typename Iter>
	struct Range {
		Iter begin_, end_;
		Iter begin() const { return begin_; }
		Iter end() const { return end_; }
	};

	/**
   * @brief
   * Detect Type of Iterator
   * */
	template<typename Serie, bool ascending>
	struct DetectIter {
		using Container = std::remove_reference_t<Serie>;
		bool static constexpr r_value{std::is_rvalue_reference_v<Container>};
		bool static constexpr constant{std::is_const_v<Container>};

		using type =
				std::conditional_t<
						ascending,
						std::conditional_t< //ascending
								r_value,
								decltype(std::make_move_iterator(
										std::declval<Container>().begin())), //ascending && rvalue
								std::conditional_t< //ascending && !rvalue
										constant,
										typename Container::const_iterator, //ascending && !rvalue && const
										typename Container::iterator //ascending && !rvalue && !const
								>
						>,
						std::conditional_t< // !ascending
								r_value,
								decltype(std::make_move_iterator(
										std::declval<Container>().rbegin())), // !ascending && rvalue
								std::conditional_t<
										constant,
										typename Container::const_reverse_iterator, // !ascending && !rvalue && const
										typename Container::reverse_iterator // !ascending && !rvalue && !const
								>
						>
				>;
	};
  }//!namespace

#ifndef __cpp_concepts
  template<typename Container, requirements::IsContainer<Container> = true>
#else
  template<requirements::IsContainer Container>
#endif
  bool isSortedAscending(Container const& v) {
	  return *v.begin() < *std::prev(v.end());
  }

  /**
 * @brief
 * There are two ideas behind this func:\n
 * 1) avoid having a Serie&& as a param, as if submitted by value then may be lost;\n
 * 2) using constexpr for runtime func is necessary as it allows to return different types;\n
 * 3) ascending is a runtime parameter, that can't be used to determine a return type;\n
 *\n
 * UI\n
 * */
#ifndef __cpp_concepts
  template<typename Serie, bool ascending, requirements::IsContainer<Serie> = true>
#else
  template<requirements::IsContainer Serie, bool ascending>
#endif
  details::Range<typename details::DetectIter<Serie, ascending>::type>
  getIterators(Serie& serie)
  {
	  using Container = std::remove_reference_t<Serie>;
	  bool constexpr r_value{std::is_rvalue_reference_v<Container>};
	  bool constexpr constant{std::is_const_v<Container>};

	  if constexpr (constant && ascending) return {.begin_ = serie.cbegin(), .end_ = serie.cend()};
	  else if constexpr (constant && !ascending) return {.begin_ = serie.crbegin(), .end_ = serie.crend()};
	  else if constexpr (!constant && ascending && !r_value) return {.begin_ = serie.begin(), .end_ = serie.end()};
	  else if constexpr (!constant && !ascending && !r_value) return {.begin_ = serie.rbegin(), .end_ = serie.rend()};
	  else if constexpr (!constant && ascending && r_value)
		  return {
				  .begin_ = std::make_move_iterator(serie.begin()),
				  .end_ = std::make_move_iterator(serie.end())
		  };
	  else if constexpr (!constant && !ascending && r_value)
		  return {
				  .begin_ = std::make_move_iterator(serie.rbegin()),
				  .end_ = std::make_move_iterator(serie.rend())};

	  else {
		  return {};  //to shut "func doesn't return" warning
	  }
  }

}//!namespace
#endif //ITERTOOLS_H
