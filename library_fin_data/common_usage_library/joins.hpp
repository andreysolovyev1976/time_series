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

	enum class Job {
		None = 0,
		JoinInner,
		OuterFull,
		OuterExcluding,
		LeftFullOuter,
		LeftExcluding,
		RightFullOuter,
		RightExcluding,

		SetUnion,
		SetIntersection,
		SetDifference,
		SetSymmetricDifference,
	};

	template <Job join_type>
	inline
	constexpr bool is_right_to_left_v {
		join_type == Job::RightExcluding ||
		join_type == Job::RightFullOuter
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
			[[maybe_unused]] InputIterator2 first2, [[maybe_unused]] InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {

		//union
		std::copy(first1, last1, result);
		return result;
	}

	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	outerExcluding(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{}) {

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


	//todo: make a Job param template one, will prevent search through
	template <class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	constexpr OutputIterator
	makeJob (Job job,
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result
			, [[maybe_unused]] Compare comp = std::less<>{})
	{
		switch (job) {
		case Job::JoinInner : {
			return inner
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case Job::OuterFull : {
			return outerFull
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case Job::OuterExcluding : {
			return outerExcluding
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case Job::LeftFullOuter : {
			return sideFull
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case Job::LeftExcluding : {
			return sideExcluding
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case Job::RightFullOuter : {
			return sideFull
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case Job::RightExcluding : {
			return sideExcluding
					<InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					(first1, last1, first2, last2, result, comp);
		}
		case Job::SetUnion : {
			return std::set_union(first1, last1, first2, last2, result, comp);
		}
		case Job::SetIntersection : {
			return std::set_intersection(first1, last1, first2, last2, result, comp);
		}
		case Job::SetDifference : {
			return std::set_difference(first1, last1, first2, last2, result, comp);
		}
		case Job::SetSymmetricDifference : {
			return std::set_symmetric_difference(first1, last1, first2, last2, result, comp);
		}
		case Job::None: {
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
	operationOverPair(Job job, Serie1 &serie_1, Serie2 &serie_2)
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
    makeJob <decltype(b1), decltype(b2), decltype(res_iter), Result> (job, b1, e1, b2, e2, res_iter);

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
			Job join_type
			, std::tuple<SerieArgs...> &&curr_result
			, Serie &serie
			, std::index_sequence<Is...>
	) {
		(operationOverPair(join_type, std::get<Is>(curr_result), serie), ...);
	}


	template<std::size_t I = 0, typename SerieRet, typename... SerieArgs>
	auto callSetOperationImpl (Job set_op_type, SerieRet &&curr_result, std::tuple<SerieArgs...> &&args) { //todo: make it && args again
		if constexpr (I != sizeof...(SerieArgs)) {
			static_assert(std::tuple_size_v<SerieRet> == 1u, "somehow result is in multi-dimensions...");

			//filter current tuple of results with the Arg[i]
			operationOverTuple(
					set_op_type
					, std::forward<SerieRet>(curr_result)
					, std::forward<decltype(std::get<I>(args))>(std::get<I>(args))
					, std::make_index_sequence<1>{}
			);

			//continue recursion
			return callSetOperationImpl<I+1>(
					set_op_type
					, std::forward<SerieRet>(curr_result)
					, std::forward<std::tuple<SerieArgs...>>(args)
			);
		}
		else if constexpr (I == sizeof...(SerieArgs)) {
			return std::forward<SerieRet>(curr_result);
		}
	}


	template<typename... SerieArgs>
	[[nodiscard]] auto callSetOperation (Job set_op_type, std::tuple<SerieArgs...> &&args) {
		std::tuple t = std::make_tuple(std::get<0>(args));
		auto res = callSetOperationImpl (
				set_op_type
				, std::move(t)
				, std::forward<std::tuple<SerieArgs...>>(args)
		);
		static_assert(std::tuple_size_v<decltype(res)> == 1u, "somehow result is in multi-dimensions...");
		return std::move(res);
	}

	template<std::size_t I = 0, typename SerieRet, typename... SerieArgs>
	auto
	callJoinOperationImp(Job join_type, SerieRet &&curr_result, std::tuple<SerieArgs...> &&args) {
		if constexpr (I!=sizeof...(SerieArgs)) {
			//filter current tuple of results with the Arg[i]
			operationOverTuple(
					join_type
					, std::forward<decltype(curr_result)>(curr_result)
					, std::forward<decltype(std::get<I>(args))>(std::get<I>(args))
					, std::make_index_sequence<std::tuple_size_v<SerieRet>>{}
			);

			//append tuple of results with updated new element
			auto result_new = tupletools::tuplePushBack(
					std::move(curr_result),
					std::forward<decltype(std::get<I>(args))>(std::get<I>(args)));

			//continue recursion
			return callJoinOperationImp<I+1, decltype(result_new), SerieArgs...>(
					join_type
					, std::move(result_new)
					, std::forward<decltype(args)>(args)
			);
		}
		else if constexpr (I == sizeof...(SerieArgs)) {
//			//update the last tuple element with the tuple
//			operationOverPair(join_type == Job::OuterFull ? Job::OuterFull : Job::JoinInner
//					, std::get<std::tuple_size_v<SerieRet> - 1>(curr_result)
//					, std::get<0>(curr_result)
//			);

			//move is required, as serie_ret is an argument, which is not guaranteed to have RVO
			return std::forward<decltype(curr_result)>(curr_result);
		}
	}

	template<Job join_type, typename... SerieArgs>
	[[nodiscard]] auto callJoinOperation(SerieArgs&& ...series) {
		std::tuple t;
		if constexpr (sizeof...(SerieArgs) <= 1u) {
			return t;
		}
		else {
			auto args = std::make_tuple(std::forward<SerieArgs>(series)...); //todo: add lvalue_ref
			if constexpr (is_right_to_left_v<join_type>) {
				args = tupletools::reverseTuple(std::move(args));
			}
			auto union_ = callSetOperation(Job::SetUnion, std::forward<decltype(args)>(args));
			auto res = callJoinOperationImp(join_type, std::move(union_), std::forward<decltype(args)>(args));
			if constexpr (is_right_to_left_v<join_type>) {
				return tupletools::reverseTuple(std::move(res));
			} else {
				return res;
			}
		}
	}

  }//!namespace


  template<typename... SerieArgs>
  auto outerFull(SerieArgs&& ...series)
  {
	  return
			  details::callJoinOperation<details::Job::OuterFull, SerieArgs...>(std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto outerExcluding(SerieArgs&& ...series)
  {
	  return
			  details::callJoinOperation<details::Job::OuterExcluding, SerieArgs...>(std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto inner(SerieArgs&& ...series)
  {
	  return
			  details::callJoinOperation<details::Job::JoinInner, SerieArgs...>(std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto leftOuter(SerieArgs&& ...series)
  {
	  return
			  details::callJoinOperation<details::Job::LeftFullOuter, SerieArgs...>(std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto leftExcluding(SerieArgs&& ...series)
  {
	  return
			  details::callJoinOperation<details::Job::LeftExcluding, SerieArgs...>(std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto rightOuter(SerieArgs&& ...series)
  {
	  return
			  details::callJoinOperation<details::Job::RightFullOuter, SerieArgs...>(std::forward<SerieArgs>(series)...);
  }

  template<typename... SerieArgs>
  auto rightExcluding(SerieArgs&& ...series)
  {
	  return
			  details::callJoinOperation<details::Job::RightExcluding, SerieArgs...>(std::forward<SerieArgs>(series)...);
  }



}//!namespace


#endif //DATA_JOINS_H
