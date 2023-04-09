//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include "types_requirements/container.h"
#include "types_requirements/iterators.h"

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
	constexpr auto weakComparisonImpl(std::tuple<T1...> const& t1, std::tuple<T2...> const& t2,
			std::index_sequence<I...>)
	{
		bool result{false};
		((result = result || std::get<I>(t1)==std::get<I>(t2)), ...);
		return result;
	}
	template<typename... T1, typename... T2>
	constexpr auto weakComparison(std::tuple<T1...> const& t1, std::tuple<T2...> const& t2)
	{
		static_assert(sizeof...(T1)==sizeof...(T2));
		return weakComparisonImpl(t1, t2, std::make_index_sequence<sizeof...(T1)>{});
	}
  }

  namespace details {

#ifndef __cpp_concepts
	template<typename... Iterators>
#else
	template<culib::requirements::AreAllIterators ...Iterators>
#endif
	class ZipIterator {
	private:

		template<typename SomeIter>
		using ValueTypeFor = typename SomeIter::value_type;

		template<typename SomeIter>
		using ReferenceTypeFor = typename SomeIter::reference;

		template<typename SomeIter>
		using DifferenceTypeFor = typename SomeIter::difference_type;

		template<typename SomeIter>
		using IteratorCategoryFor = typename std::iterator_traits<SomeIter>::iterator_category;

		/**
   * @details
   * It is taken from O'Dwyer blog's post, see there some explanation.\n\n
   * https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/ \n\n
   * But basically, this is requried to have a proper \n
   * operator -> () \n
   * thing for this iterator.\n\n
   * */
		template <typename Reference>
		struct arrowProxy {
			Reference R;
			Reference *operator->() { return &R; } // non const
		};

	public:
		using iterator_type = ZipIterator;
		using iterator_category = std::common_type_t<IteratorCategoryFor<Iterators>...>;
		using value_type = typename std::tuple<ValueTypeFor<Iterators>...>;
		using reference = typename std::tuple<ReferenceTypeFor<Iterators>...>;
		using pointer = arrowProxy<reference>;
		using difference_type = std::common_type_t<DifferenceTypeFor<Iterators>...>;

		ZipIterator() = delete;
		ZipIterator(ZipIterator const&) = default;
		ZipIterator(ZipIterator &&) = default;
		ZipIterator& operator = (ZipIterator const&) = default;
		ZipIterator& operator = (ZipIterator &&) = default;

#ifndef __cpp_concepts
		template<culib::requirements::AreAllIterators<Iterators...> = true>
#endif
		explicit
		ZipIterator(Iterators&&... iters) :iterators (std::make_tuple(std::forward<Iterators>(iters)...))
		{}
		ZipIterator& operator++() {
			std::apply([](Iterators&... iter){ ((++iter), ...); }, iterators);
			return *this;
		}
		ZipIterator operator++(int) {
			ZipIterator tmp(*this);
			operator++();
			return tmp;
		}
		ZipIterator& operator+=(int n) {
			if constexpr (culib::requirements::areAllRandomAccess_v<IteratorCategoryFor<Iterators>...>()) {
				std::apply([n](Iterators&... iter){ ((iter += n), ...); }, iterators);
			}
			else  {
				for (int i = 0; i<n; ++i) ++(*this);
			}
			return *this;
		}

#ifndef __cpp_concepts
		template<
			  typename DummyArg = iterator_category,
			  culib::requirements::BiDirectionalOrRandomAccess<DummyArg> = true>
#endif
		ZipIterator& operator--()
#ifdef __cpp_concepts
		requires culib::requirements::BiDirectionalOrRandomAccess<iterator_category>
#endif
		{
			std::apply([](Iterators&... iter){ ((--iter), ...); }, iterators);
			return *this;
		}

#ifndef __cpp_concepts
		template<
			  typename DummyArg = iterator_category,
			  culib::requirements::BiDirectionalOrRandomAccess<DummyArg> = true>
#endif
		ZipIterator operator--(int)
#ifdef __cpp_concepts
		requires culib::requirements::BiDirectionalOrRandomAccess<iterator_category>
#endif
		{
			ZipIterator tmp(*this);
			operator--();
			return tmp;
		}


#ifndef __cpp_concepts
		template<
			  typename DummyArg = iterator_category,
			  culib::requirements::BiDirectionalOrRandomAccess<DummyArg> = true>
#endif
		ZipIterator& operator-=(int n)
#ifdef __cpp_concepts
		requires culib::requirements::BiDirectionalOrRandomAccess<iterator_category>
#endif
		{
			if constexpr (culib::requirements::areAllRandomAccess_v<IteratorCategoryFor<Iterators>...>()) {
				std::apply([n](Iterators&... iter){ ((iter -= n), ...); }, iterators);
			}
			else {
				for (int i = 0; i<n; ++i) --(*this);
			}
			return *this;
		}

		bool equals(ZipIterator const& other) const {
			return base::utils::weakComparison(this->iterators, other.iterators);
		}
		reference operator*() { return makeRefs(); }
		reference operator*() const { return makeRefs(); }
		//it is supposed to survive just a drill-down
		pointer operator->() { return pointer{makeRefs()}; }

		/**
		 * @details
		 * Having this for being able to decompose\n
		 * this type itself using structural\n
		 * bindings.\n
		 * See the tests for the details - \n
		 * the one, where zipping is for iterators.\n
		 *
		 * */
		template<std::size_t Index>
		decltype(auto) get() &  { return std::get<Index>(iterators).operator*(); }
		template<std::size_t Index>
		decltype(auto) get() && { return std::get<Index>(iterators).operator*(); }
		template<std::size_t Index>
		decltype(auto) get() const &  { return std::get<Index>(iterators).operator*(); }
		template<std::size_t Index>
		decltype(auto) get() const && { return std::get<Index>(iterators).operator*(); }

	private:

		std::tuple<Iterators...> iterators;

		template <std::size_t... I>
		auto makeRefsImpl (std::index_sequence<I...>) {
			return reference ({ std::get<I>(iterators).operator*()... });
		}
		auto makeRefs () {
			return makeRefsImpl (std::make_index_sequence<sizeof...(Iterators)>{});
		}
	};

	template<typename... Iterators>
	static inline bool operator==(ZipIterator<Iterators...> lhs, ZipIterator<Iterators...> rhs) {
		return lhs.equals(rhs);
	}

	template<typename... Iterators>
	static inline bool operator!=(ZipIterator<Iterators...> lhs, ZipIterator<Iterators...> rhs) {
		return !lhs.equals(rhs);
	}

	template<typename... Iterators>
	static inline ZipIterator<Iterators...> operator+(ZipIterator<Iterators...> it, int n) {
		it += n;
		return it;
	}

	template<typename... Iterators>
	static inline ZipIterator<Iterators...> operator-(ZipIterator<Iterators...> it, int n) {
		it -= n;
		return it;
	}

#ifndef __cpp_concepts
	template<typename... Containers>
#else
	template<culib::requirements::AreAllContainers... Containers>
#endif
	class Zipper {
	private:

		/**
		 * @details
		 * This seemingly innocent iterator type selection serves\n
		 * as a guard - no Type without an iterator can pass this.\n
		 * \n
		 * */
		template<typename Input>
		using IteratorTypeSelect = std::conditional_t<
				std::is_const_v<std::remove_reference_t<Input>>,
				typename std::remove_reference_t<Input>::const_iterator,
				typename std::remove_reference_t<Input>::iterator>;

	public:
		using zip_type = ZipIterator<IteratorTypeSelect<Containers>...>;

		Zipper() = delete;

#ifndef __cpp_concepts
		template<culib::requirements::AreAllContainers<Containers...> = true>
#endif
		explicit
		Zipper(Containers&& ... inputs)
				:begin_(std::forward<Containers>(inputs).begin()...),
				 end_(std::forward<Containers>(inputs).end()...) { }

		zip_type begin() const { return begin_; }
		zip_type& begin() { return begin_; }
		zip_type end() const { return end_; }
		zip_type& end() { return end_; }
	private:
		zip_type begin_, end_;
	};

  }//!details

  /**
   * @details
   * This is UI for zip_iterator\n
   * \n
   * */

  /**
   * @details
   * Considering all the options, there should be at least a pair\n
   * of iterators to provide a valid range.\n\n
   * So all of the inputs should satisfy the requirements to have\n
   * both .begin() and end(), therefore each argument is, or  \n
   * should be, a container.\n
   *
   * */

#ifndef __cpp_concepts
  template<typename... Containers, culib::requirements::AreAllContainers<Containers...> = true>
#else
  template<culib::requirements::AreAllContainers... Containers>
#endif
  auto zip(Containers&& ... containers)
  {
	  return details::Zipper<Containers...>(std::forward<Containers>(containers)...);
  }
  /**
   * @details
   * Just a ZipIterator will be provided from here.\n
   *
   * */

#ifndef __cpp_concepts
  template<typename... Iterators, culib::requirements::AreAllIterators<Iterators...> = true>
#else
  template<culib::requirements::AreAllIterators... Iterators>
#endif
  auto zip(Iterators&& ... iterators)
  {
	  return details::ZipIterator<Iterators...>(std::forward<Iterators>(iterators)...);
  }
}//!namespace

/**
 * @details
 * Defining structural bindings\n
 * for zip_iterator itself.\n
 *
 * */

namespace std {
  template<typename... Iterators>
  struct tuple_size<culib::itertools::details::ZipIterator<Iterators...>> :
		  std::integral_constant<std::size_t, sizeof...(Iterators)> {};
  template<std::size_t Index, typename... Iterators>
  struct tuple_element<Index, culib::itertools::details::ZipIterator<Iterators...>> {
	  using type = decltype(std::get<Index>(std::declval<culib::itertools::details::ZipIterator<Iterators...>>().operator*() ));
  };
}//!namespace


namespace culib::itertools {
  namespace details {

	/**
   * @brief
   * pair of iterators
   * */
#ifndef __cpp_concepts
	template<typename Iter, culib::requirements::IsIterator<Iter> = true>
#else
	template<culib::requirements::Iterator Iter>
#endif
	struct Range {
		Iter begin_, end_;
		Iter begin() const { return begin_; }
		Iter end() const { return end_; }
	};

	/**
   * @brief
   * Detect Type of Iterator
   * */
#ifndef __cpp_concepts
	template<typename Serie, bool ascending, requirements::IsContainer<Serie> = true>
#else
	template<requirements::IsContainer Serie, bool ascending>
#endif
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

#ifndef __cpp_concepts
  template<typename Iterator, requirements::IsIterator<Iterator> = true>
#else
  template<requirements::Iterator Iterator>
#endif
  bool isSortedAscending(Iterator first, Iterator second) {
	  return *first < *second;
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
