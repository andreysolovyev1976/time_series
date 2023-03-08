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

	enum class SetOperation {
		None = 0,
		Inner,
		OuterFull,
		OuterExcluding,
		LeftFullOuter,
		LeftExcluding,
		RightFullOuter,
		RightExcluding,
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

//		//intersection
//		ResultContainer temp1;
//		auto temp1_iter = std::inserter(temp1, temp1.begin());
//		std::set_intersection(first1, last1, first2, last2, temp1_iter);
//
//		//union
//		ResultContainer temp2;
//		auto temp2_iter = std::inserter(temp2, temp2.begin());
//		std::set_union(first1, last1, first2, last2, temp2_iter);


		ResultContainer temp2;
		auto temp2_iter = std::inserter(temp2, temp2.begin());
		std::set_symmetric_difference(first1, last1, first2, last2, temp2_iter);

		return
				std::set_difference (temp2.begin(), temp2.end(), first2, last2, result);
	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	sideFull(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {

		//union
		ResultContainer temp2;
		auto temp2_iter = std::inserter(temp2, temp2.begin());
		std::set_union(first1, last1, first2, last2, temp2_iter);

		//difference between the two
		ResultContainer temp1;
		auto temp1_iter = std::inserter(temp1, temp1.begin());
		std::set_difference(first2, last2, first1, last1, temp1_iter);

		return std::set_difference (temp2.begin(), temp2.end(), temp1.begin(), temp1.end(), result);

	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	sideExcluding(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {
		return std::set_difference(first1, last1, first2, last2, result);
	}


	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	makeJoin (SetOperation operation,
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{})
	{
		switch (operation) {
		case SetOperation::Inner : {
			return inner
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case SetOperation::OuterFull : {
			return outerFull
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case SetOperation::OuterExcluding : {
			return outerExcluding
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case SetOperation::LeftFullOuter : {
			return sideFull
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case SetOperation::LeftExcluding : {
			return sideExcluding
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case SetOperation::RightFullOuter : {
			return sideFull
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case SetOperation::RightExcluding : {
			return sideExcluding
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case SetOperation::None: {
			/* do nothing */
			//todo: add exception?
		}
		default: {
			//todo: add exception?
			//having it here to suppress the warning
			return result;
		}
		}
	}

	template<typename Serie1, typename Serie2>
	void
	operationOverPair(SetOperation operation, Serie1 &&serie_1, Serie2 &&serie_2)
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
    makeJoin <decltype(b1), decltype(b2), decltype(res_iter), Result> (operation, b1, e1, b2, e2, res_iter);

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
			SetOperation operation
			, std::tuple<SerieArgs...> &curr_result
			, Serie &serie
			, std::index_sequence<Is...>
	) {
		(operationOverPair(
				operation, std::get<Is>(curr_result), std::forward<Serie>(serie)), ...);
	}


	template<std::size_t I = 0, typename SerieRet, typename... SerieArgs>
	auto
	callOperationImp(SetOperation operation, SerieRet&& curr_result, std::tuple<SerieArgs...>&& args)
	{
		if constexpr (I==0) {
			auto result_new = tupletools::tuplePushBack(
					std::forward<SerieRet>(curr_result),
					std::forward<decltype(std::get<I>(args))>(std::get<I>(args)));
			return callOperationImp<I+1, decltype(result_new), SerieArgs...>(
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
			SetOperation operation_appending;
			operation_appending = SetOperation::Inner; //todo: seems like it doesn't work here
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
			return callOperationImp<I+1, NewSerieRet, SerieArgs...>(
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
	auto callOperation(SetOperation operation, SerieArgs&& ...series) {
		std::tuple t;
		if constexpr (sizeof...(SerieArgs) == 0u) {
			return t;
		}
		else if constexpr (sizeof...(SerieArgs) == 1u) {
			return tupletools::tuplePushBack(t, std::forward<SerieArgs>(series)...);
		}
		else {
			auto args = std::make_tuple(std::forward<SerieArgs>(series)...);
			if (operation == SetOperation::RightExcluding || operation == SetOperation::RightFullOuter) {
				args = tupletools::reverseTuple(std::move(args));
				auto res = callOperationImp(operation, std::move(t), std::move(args));
				return tupletools::reverseTuple(std::move(res));
			}

			return callOperationImp(operation, std::move(t), std::move(args));
		}
	}

  }//!namespace


  template<typename... SerieArgs>
  auto outerFull(SerieArgs&& ...series)
  {
	  details::SetOperation operation {details::SetOperation::OuterFull};
	  return details::callOperation<SerieArgs...>(operation, std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto outerExcluding(SerieArgs&& ...series)
  {
	  details::SetOperation operation {details::SetOperation::OuterExcluding};
	  return details::callOperation<SerieArgs...>(operation, std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto inner(SerieArgs&& ...series)
  {
	  details::SetOperation operation {details::SetOperation::Inner};
	  return details::callOperation<SerieArgs...>(operation, std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto leftOuter(SerieArgs&& ...series)
  {
	  details::SetOperation operation {details::SetOperation::LeftFullOuter};
	  return details::callOperation<SerieArgs...>(operation, std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto leftExcluding(SerieArgs&& ...series)
  {
	  details::SetOperation operation {details::SetOperation::LeftExcluding};
	  return details::callOperation<SerieArgs...>(operation, std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto rightOuter(SerieArgs&& ...series)
  {
	  details::SetOperation operation {details::SetOperation::RightFullOuter};
	  return details::callOperation<SerieArgs...>(operation, std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto rightExcluding(SerieArgs&& ...series)
  {
	  details::SetOperation operation {details::SetOperation::RightExcluding};
	  return details::callOperation<SerieArgs...>(operation, std::forward<SerieArgs>(series)...);
  }

}//!namespace


#endif //DATA_JOINS_H
