//
// Created by Andrey Solovyev on 03/03/2023.
//

#pragma once

#include <algorithm>
#include <type_traits>
#include <cstddef>
#include <tuple>
#include <utility>

#ifndef DATA_JOINS_H
#define DATA_JOINS_H

namespace time_series {

  namespace join::details {

	namespace tupletools {

	  template <typename NewElem, typename... TupleElems>
	  decltype(auto) tuplePushBack(std::tuple<TupleElems...> &&tup, NewElem &&el) {
		  return std::tuple_cat(
				  std::forward<std::tuple<TupleElems...>>(tup),
				  std::make_tuple(std::forward<NewElem>(el)));
	  }

	  template <typename NewElem, typename... TupleElems>
	  decltype(auto) tuplePushFront(std::tuple<TupleElems...> &&tup, NewElem &&el) {
		  return std::tuple_cat(
				  std::make_tuple(std::forward<NewElem>(el)),
				  std::forward<std::tuple<TupleElems...>>(tup));
	  }

	  template <typename Tuple, std::size_t... Is>
	  decltype(auto) reverseTupleImpl (Tuple && tup, std::index_sequence<Is...>) {
		  return std::make_tuple(std::get<sizeof...(Is) - Is - 1 >(std::forward<Tuple>(tup))...);
	  }


	  template <typename Tuple>
	  decltype(auto) reverseTuple (Tuple && tup) {
		  return reverseTupleImpl(
				  std::forward<std::remove_reference_t<Tuple>>(tup),
				  std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
	  }

	  template <std::size_t I = 0, typename RetTuple, typename... Args>
	  constexpr auto
	  getSliceImpl (std::tuple<Args...> const& tup, RetTuple &&ret_tuple, std::size_t const idx) {
		  if constexpr (I == sizeof...(Args)) {
			  return std::move(ret_tuple);
		  }
		  else {
			  auto new_ret_tuple = tuplePushBack(std::move(ret_tuple), *std::next(std::get<I>(tup).begin(), idx));
			  return getSliceImpl<I+1, decltype(new_ret_tuple), Args...>(tup, std::move(new_ret_tuple), idx);
		  }
	  }

	  template <typename... Args>
	  constexpr auto getSlice (std::tuple<Args...> const& tup, std::size_t const idx) {
		  std::tuple ret;
		  return getSliceImpl(tup, std::move(ret), idx);
	  }
	}//!namespace

	namespace itertools {

	  namespace details {
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

#if 0
		template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare = std::less<>>
		_OutputIterator
		__set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
				_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp = std::less<>{})
		{
			while (__first1 != __last1)
			{
				if (__first2 == __last2)
					return _VSTD::copy(__first1, __last1, __result);

				if (__comp(*__first1, *__first2))
				{
					*__result = *__first1;
					++__result;
					++__first1;
				}
				else
				{
					if (!__comp(*__first2, *__first1))
						++__first1;
					++__first2;
				}
			}
			return __result;
		}


		template <class _Compare, class _InputIterator1, class _InputIterator2, class _OutputIterator>
		_LIBCPP_CONSTEXPR_AFTER_CXX17 _OutputIterator
		__set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
				_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp)
		{
			while (__first1 != __last1 && __first2 != __last2)
			{
				if (__comp(*__first1, *__first2))
					++__first1;
				else
				{
					if (!__comp(*__first2, *__first1))
					{
						*__result = *__first1;
						++__result;
						++__first1;
					}
					++__first2;
				}
			}
			return __result;
		}


		template <class _Compare, class _InputIterator1, class _InputIterator2, class _OutputIterator>
		_OutputIterator
		__set_union(_InputIterator1 __first1, _InputIterator1 __last1,
				_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp)
		{
			for (; __first1 != __last1; ++__result)
			{
				if (__first2 == __last2)
					return _VSTD::copy(__first1, __last1, __result);
				if (__comp(*__first2, *__first1))
				{
					*__result = *__first2;
					++__first2;
				}
				else
				{
					if (!__comp(*__first1, *__first2))
						++__first2;
					*__result = *__first1;
					++__first1;
				}
			}
			return _VSTD::copy(__first2, __last2, __result);
		}
#endif

/**
 * @brief
 * copy-paste from algorithm
 * + merge of set_intersection and set_difference
 * + default param for Comp
 * */
		template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare = std::less<>>
		constexpr _OutputIterator
		set_intersection_and_difference(_InputIterator1 __first1, _InputIterator1 __last1,
				_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, [[maybe_unused]] _Compare __comp = std::less<>{})
		{
			while (__first1 != __last1) {
				if (__first2 == __last2) {
					return std::copy(__first1, __last1, __result);
				}
				*__result = *__first1;
				++__result;
				++__first1;
				++__first2;
			}
			return __result;
		}

		template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare = std::less<>>
		constexpr _OutputIterator
		set_append_with_zeros(_InputIterator1 __first1, _InputIterator1 __last1,
				_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, [[maybe_unused]] _Compare __comp = std::less<>{})
		{

			while (__first1 != __last1)
			{
				if (__first2 == __last2) {
					while (__first1 != __last1) {
						*__result = *__first1;
						++__result;
						++__first1;
					}
					return __result;
				}

				if (__comp(*__first1, *__first2)) {
//					*__result = zero_value;
					++__result;
					++__first1;
				}
				else if (*__first1 == *__first2) {
					*__result = *__first1;
					++__result;
					++__first1;
				}
				else
				{
					if (!__comp(*__first2, *__first1)) {
						++__first1;
					}
					++__first2;
				}
			}
			return __result;
		}


	  }//!namespace

/**
 * @brief
 * There are two ideas behind this func:
 * 1) avoid having a Serie&& as a param, as if submitted by value then may be lost;
 * 2) using constexpr for runtime func is necessary as it allows to return different types;
 * 3) ascending is a runtime parameter, that can't be used to determine a return type;
 *
 * UI
 * */
	  template<typename Serie, bool ascending>
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

	namespace boiler_plate {

	  struct SetOperations {
		  enum class Operation {
			  None = 0,
			  Intersection,
			  Difference,
			  SymmetricDifference,
			  Union,
			  IntersectionAndDifference,
			  FillInWithZerosHelper,
		  };
		  operator int() { return static_cast<int>(op); }

		  Operation op;

		  enum class Order {
			  None = 0,
			  LeftToRight,
			  RightToLeft,
		  };
	  };

#define IMPLEMENT_OPERATION_ON_SETS(c, ...) \
switch(c) \
{ case 1: std::set_intersection          (__VA_ARGS__); break; \
  case 2: std::set_difference            (__VA_ARGS__); break; \
  case 3: std::set_symmetric_difference  (__VA_ARGS__); break; \
  case 4: std::set_union				 (__VA_ARGS__); break; \
  case 5: itertools::details::set_intersection_and_difference (__VA_ARGS__); break;  }


	  template<typename Serie>
	  bool isSortedAscending(Serie const& v)
	  {
		  return *v.begin()<*std::prev(v.end());
	  }

	  template<typename Serie1, typename Serie2>
	  void
	  operationPairwise(SetOperations operation, Serie1 &&serie_1, Serie2 &&serie_2)
	  {
		  //there is no guarantee resulting container is empty, therefore it is end(), not begin()
		  using Result = std::remove_reference_t<Serie1>;
		  Result result;
		  auto res_iter = std::inserter(result, result.end());

		  bool const ascending1{isSortedAscending(serie_1)};
		  bool const ascending2{isSortedAscending(serie_2)};

#define CALL_OPERATION(first_cond, second_cond) \
    auto [b1, e1] = itertools::getIterators<Serie1, first_cond>(serie_1); \
    auto [b2, e2] = itertools::getIterators<Serie2, second_cond>(serie_2); \
    IMPLEMENT_OPERATION_ON_SETS(operation, b1, e1, b2, e2, res_iter);

		  if (ascending1 && ascending2) { CALL_OPERATION(true, true) }
		  else if (ascending1 && !ascending2) { CALL_OPERATION(true, false) }
		  else if (!ascending1 && ascending2) { CALL_OPERATION(false, true) }
		  else if (!ascending1 && !ascending2) { CALL_OPERATION(false, false) }

#undef CALL_OPERATION
		  serie_1 = std::move(result);
	  }

	  template <typename Serie, typename... SerieArgs, std::size_t ...Is>
	  void
	  implementOperation (
			  SetOperations operation
			  , std::tuple<SerieArgs...> &curr_result
			  , Serie &serie
			  , std::index_sequence<Is...>
	  ) {
		  (operationPairwise(
				  operation
				  , std::get<Is>(curr_result)
				  , std::forward<Serie>(serie)), ...);
	  }


	  template<std::size_t I = 0, typename SerieRet, typename... SerieArgs>
	  auto
	  implementSetOperation(SetOperations operation, SerieRet&& curr_result, std::tuple<SerieArgs...>&& args)
	  {
		  if constexpr (sizeof...(SerieArgs)<=1u) { return; }

		  if constexpr (I==0) {
			  auto result_new = tupletools::tuplePushBack(
					  std::forward<SerieRet>(curr_result),
					  std::forward<decltype(std::get<I>(args))>(std::get<I>(args)));
			  return implementSetOperation<I+1, decltype(result_new), SerieArgs...>(
					  operation
					  , std::move(result_new)
					  , std::forward<std::tuple<SerieArgs...>>(args)
			  );
		  }
		  else if constexpr (I!=sizeof...(SerieArgs)) {

			  //filter current tuple of results with the Arg[i]
			  implementOperation(
					  operation
					  , curr_result
					  , std::get<I>(args)
					  , std::make_index_sequence<std::tuple_size_v<SerieRet>>{}
			  );

			  //get new elem of tuple of results
			  using Serie = std::remove_reference_t<decltype(std::get<I>(args))>;
			  Serie goes_to_tuple = std::move(std::get<I>(args));

			  //filter new element with existing tuple of results
			  SetOperations operation_appending;
			  operation_appending.op = SetOperations::Operation::Intersection;
			  operationPairwise(
					  operation_appending
					  , goes_to_tuple
					  , std::get<0>(curr_result)
			  );

			  //append tuple of results with updated new element
			  auto result_new = tupletools::tuplePushBack(
					  std::move(curr_result),
					  std::move(goes_to_tuple));

			  //continue recursion
			  using NewSerieRet = decltype(result_new);
			  return implementSetOperation<I+1, NewSerieRet, SerieArgs...>(
					  operation
					  , std::move(result_new)
					  , std::forward<std::tuple<SerieArgs...>>(args)
			  );
		  }
		  else if constexpr (I == sizeof...(SerieArgs)) {
			  //move is required, as serie_ret is an argument, which is not guaranteed to have RVO
			  return std::move(curr_result);
		  }
	  }

	  template<typename... SerieArgs>
	  auto callOperation(
			  boiler_plate::SetOperations operation
			  , boiler_plate::SetOperations::Order order
			  , SerieArgs&& ...series
	  )
	  {
		  std::tuple t;
		  if constexpr (sizeof...(SerieArgs) == 0u) {
			  return t;
		  }
		  else if constexpr (sizeof...(SerieArgs) == 1u) {
			  return tupletools::tuplePushBack(t, std::forward<SerieArgs>(series)...);
		  }
		  else {
			  auto args = std::make_tuple(std::forward<SerieArgs>(series)...);
			  if (order==boiler_plate::SetOperations::Order::RightToLeft) {
				  args = tupletools::reverseTuple(std::move(args));
				  auto _ = implementSetOperation(operation, std::move(t), std::move(args));
				  return tupletools::reverseTuple(std::move(_));
			  }
			  return implementSetOperation(operation, std::move(t), std::move(args));
		  }
	  }
	}//!namespace

  }//!namespace

  namespace join {

	template<typename... SerieArgs>
	auto outerFull(SerieArgs&& ...series)
	{
		using namespace details::boiler_plate;
		auto const order = SetOperations::Order::LeftToRight;
		SetOperations operation{SetOperations::Operation::Union};
		return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs>
	auto outerExcluding(SerieArgs&& ...series)
	{
		using namespace details::boiler_plate;
		auto const order = SetOperations::Order::LeftToRight;
		SetOperations operation{SetOperations::Operation::SymmetricDifference};
		return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs>
	auto inner(SerieArgs&& ...series)
	{
		using namespace details::boiler_plate;
		auto const order = SetOperations::Order::LeftToRight;
		SetOperations operation{SetOperations::Operation::Intersection};
		return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs>
	auto leftOuter(SerieArgs&& ...series)
	{
		using namespace details::boiler_plate;
		auto const order = SetOperations::Order::LeftToRight;
		SetOperations operation{SetOperations::Operation::IntersectionAndDifference};
		return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs>
	auto leftExcluding(SerieArgs&& ...series)
	{
		using namespace details::boiler_plate;
		auto const order = SetOperations::Order::LeftToRight;
		SetOperations operation{SetOperations::Operation::Difference};
		return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs>
	auto rightOuter(SerieArgs&& ...series)
	{
		using namespace details::boiler_plate;
		auto const order = SetOperations::Order::RightToLeft;
		SetOperations operation{SetOperations::Operation::IntersectionAndDifference};
		return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs>
	auto rightExcluding(SerieArgs&& ...series)
	{
		using namespace details::boiler_plate;
		auto const order = SetOperations::Order::RightToLeft;
		SetOperations operation{SetOperations::Operation::Difference};
		return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	}

  }//!namespace

}//!namespace


#endif //DATA_JOINS_H
