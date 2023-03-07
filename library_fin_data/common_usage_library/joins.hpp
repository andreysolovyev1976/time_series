//
// Created by Andrey Solovyev on 03/03/2023.
//

#pragma once

#include "itertools.hpp"
#include "tupletools.hpp"

#include <algorithm>
#include <type_traits>
#include <cstddef>
#include <tuple>
#include <utility>

#ifndef DATA_JOINS_H
#define DATA_JOINS_H

namespace culib::join {

  namespace details {

	  struct Set {
		  enum class Operation {
			  None = 0,
			  Inner,
			  FullOuter,
			  ExcludingOuter,
			  LeftFullOuter,
			  LeftExcluding,
			  RightFullOuter,
			  RightExcluding,
		  };
		  operator int() { return static_cast<int>(_); }
		  Operation _;
	  };
	  struct Operation {
		  enum class Order {
			  None = 0,
			  LeftToRight,
			  RightToLeft,
		  };
		  Order _;
	  };

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

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	inner(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {
		return std::set_intersection(first1, last1, first2, last2, result);
	}


	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	outerFull(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {
		return std::set_union(first1, last1, first2, last2, result);
	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	outerExcluding(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {

		//intersection
		ResultContainer temp1;
		auto temp1_iter = std::inserter(temp1, temp1.begin());
		std::set_intersection(first1, last1, first2, last2, temp1_iter);

		//union
		ResultContainer temp2;
		auto temp2_iter = std::inserter(temp2, temp2.begin());
		std::set_union(first1, last1, first2, last2, temp2_iter);

		return
		std::set_symmetric_difference (temp2.begin(), temp2.end(), temp1.begin(), temp1.end(),result);
	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	leftFull(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {

	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	leftExcluding(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {
		return std::set_difference(first1, last1, first2, last2, result);
	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	rightFull(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {
		//union
		ResultContainer temp2;
		auto temp2_iter = std::inserter(temp2, temp2.begin());
		std::set_union(first1, last1, first2, last2, temp2_iter);

		//intersection
		ResultContainer temp1;
		auto temp1_iter = std::inserter(temp1, temp1.begin());
		std::set_intersection(first1, last1, first2, last2, temp1_iter);


	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	rightExcluding(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {
		return std::set_difference(first2, last2, first1, last1, result);
	}

#define IMPLEMENT_OPERATION_ON_SETS(c, ...) \
switch(c) \
{ case 1: std::set_intersection          (__VA_ARGS__); break; \
  case 2: std::set_difference            (__VA_ARGS__); break; \
  case 3: std::set_symmetric_difference  (__VA_ARGS__); break; \
  case 4: std::set_union				 (__VA_ARGS__); break; \
  case 5: set_intersection_and_difference (__VA_ARGS__); break; }


	  template<typename Serie1, typename Serie2>
	  void
	  operationOverPair(Set operation, Serie1 &&serie_1, Serie2 &&serie_2)
	  {
		  //there is no guarantee resulting container is empty, therefore it is end(), not begin()
		  using Result = std::remove_reference_t<Serie1>;
		  Result result;
		  auto res_iter = std::inserter(result, result.end());

		  bool const ascending1{itertools::isSortedAscending(serie_1)};
		  bool const ascending2{itertools::isSortedAscending(serie_2)};

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
	  operationOverTuple (
			  Set operation
			  , std::tuple<SerieArgs...> &curr_result
			  , Serie &serie
			  , std::index_sequence<Is...>
	  ) {
		  (operationOverPair(
				  operation, std::get<Is>(curr_result), std::forward<Serie>(serie)), ...);
	  }


	  template<std::size_t I = 0, typename SerieRet, typename... SerieArgs>
	  auto
	  implementSetOperation(Set operation, SerieRet&& curr_result, std::tuple<SerieArgs...>&& args)
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
			  operationOverTuple(
					  operation
					  , curr_result
					  , std::get<I>(args)
					  , std::make_index_sequence<std::tuple_size_v<SerieRet>>{}
			  );

			  //get new elem of tuple of results
			  using Serie = std::remove_reference_t<decltype(std::get<I>(args))>;
			  Serie appending_tuple_with = std::move(std::get<I>(args));

			  //filter new element with existing tuple of results
			  Set operation_appending;
			  operation_appending._ = Set::Operation::Intersection;
			  operationOverPair(
					  operation_appending
					  , appending_tuple_with
					  , std::get<0>(curr_result)
			  );

			  //append tuple of results with updated new element
			  auto result_new = tupletools::tuplePushBack(
					  std::move(curr_result),
					  std::move(appending_tuple_with));

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
			  return curr_result;
		  }
	  }

	  template<typename... SerieArgs>
	  auto callOperation(
			  Set operation
			  , Operation order
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
			  if (order._ == Operation::Order::RightToLeft) {
				  args = tupletools::reverseTuple(std::move(args));
				  auto res = implementSetOperation(operation, std::move(t), std::move(args));
				  return tupletools::reverseTuple(std::move(res));
			  }
			  return implementSetOperation(operation, std::move(t), std::move(args));
		  }
	  }

  }//!namespace


	  template<typename... SerieArgs>
	  auto outerFull(SerieArgs&& ...series)
	  {
		  using namespace details;
		  Operation order {Operation::Order::LeftToRight};
		  Set operation {Set::Operation::Union};
		  return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	  }

	  template<typename... SerieArgs>
	  auto outerExcluding(SerieArgs&& ...series)
	  {
		  using namespace details;
		  Operation order {Operation::Order::LeftToRight};
		  Set operation {Set::Operation::SymmetricDifference};
		  return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	  }

	  template<typename... SerieArgs>
	  auto inner(SerieArgs&& ...series)
	  {
		  using namespace details;
		  Operation order {Operation::Order::LeftToRight};
		  Set operation {Set::Operation::Intersection};
		  return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	  }

	  template<typename... SerieArgs>
	  auto leftOuter(SerieArgs&& ...series)
	  {
		  using namespace details;
		  Operation order {Operation::Order::LeftToRight};
		  Set operation {Set::Operation::IntersectionAndDifference};
		  return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	  }

	  template<typename... SerieArgs>
	  auto leftExcluding(SerieArgs&& ...series)
	  {
		  using namespace details;
		  Operation order {Operation::Order::LeftToRight};
		  Set operation {Set::Operation::Difference};
		  return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	  }

	  template<typename... SerieArgs>
	  auto rightOuter(SerieArgs&& ...series)
	  {
		  using namespace details;
		  Operation order {Operation::Order::RightToLeft};
		  Set operation {Set::Operation::IntersectionAndDifference};
		  return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	  }

	  template<typename... SerieArgs>
	  auto rightExcluding(SerieArgs&& ...series)
	  {
		  using namespace details;
		  Operation order {Operation::Order::RightToLeft};
		  Set operation {Set::Operation::Difference};
		  return callOperation<SerieArgs...>(operation, order, std::forward<SerieArgs>(series)...);
	  }

  }//!namespace


#endif //DATA_JOINS_H
