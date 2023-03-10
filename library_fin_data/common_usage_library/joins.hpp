//
// Created by Andrey Solovyev on 03/03/2023.
//

#pragma once

#include "types_requirements/container.h"
#include "itertools.hpp"
#include "tupletools.hpp"

#include <algorithm>
#include <type_traits>
#include <cstddef>
#include <tuple>
#include <utility>

#ifndef DATA_JOINS_H
#define DATA_JOINS_H

namespace culib {
  namespace join {
	namespace details {

	  enum class Job {
		  None = 0,
		  JoinInner,
		  JoinOuterFull,
		  JoinOuterExcluding,
		  JoinLeftFullOuter,
		  JoinLeftExcluding,
		  JoinRightFullOuter,
		  JoinRightExcluding,

		  SetUnion,
		  SetIntersection,
		  SetDifference,
		  SetSymmetricDifference,
	  };

	  template <Job join_type>
	  inline
	  constexpr bool requires_reverse {
			  join_type == Job::JoinRightExcluding ||
			  join_type == Job::JoinRightFullOuter
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
	  sideFull(InputIterator1 first1, InputIterator1 last1,
			  InputIterator2 first2, InputIterator2 last2,
			  OutputIterator result
			  , [[maybe_unused]] Compare comp = std::less<>{}) {
		  //union
		  ResultContainer temp2;
		  auto temp2_iter = std::inserter(temp2, temp2.begin());
		  std::set_union(first1, last1, first2, last2, temp2_iter);

		  return std::set_intersection(temp2.begin(), temp2.end(), first1, last1, result);
	  }


	  template <Job job, class InputIterator1, class InputIterator2, class OutputIterator, class ResultContainer, class Compare = std::less<>>
	  constexpr OutputIterator
	  makeJob (
			  InputIterator1 first1, InputIterator1 last1,
			  InputIterator2 first2, InputIterator2 last2,
			  OutputIterator result
			  , [[maybe_unused]] Compare comp = std::less<>{})
	  {
		  if constexpr (job == Job::JoinInner) {
			  return std::set_intersection(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::JoinOuterFull) {
			  return std::copy(first1, last1, result);
		  }
		  else if constexpr (job == Job::JoinOuterExcluding) {
			  return std::set_symmetric_difference(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::JoinLeftFullOuter) {
			  return sideFull
					  <InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					  (first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::JoinLeftExcluding) {
			  return std::set_difference(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::JoinRightFullOuter) {
			  return sideFull
					  <InputIterator1, InputIterator2, OutputIterator, ResultContainer, Compare>
					  (first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::JoinRightExcluding) {
			  return std::set_difference(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::SetUnion) {
			  return std::set_union(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::SetIntersection) {
			  return std::set_intersection(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::SetDifference) {
			  return std::set_difference(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::SetSymmetricDifference) {
			  return std::set_symmetric_difference(first1, last1, first2, last2, result, comp);
		  }
		  else if constexpr (job == Job::None) {
			  /* do nothing */
			  //todo: add exception?
		  }
	  }

	  template<Job job, typename Serie1, typename Serie2>
	  void
	  jobBoilerPlateImpl(Serie1 &serie_1, Serie2 &serie_2)
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
    makeJob <job, decltype(b1), decltype(b2), decltype(res_iter), Result> (b1, e1, b2, e2, res_iter);

		  if (ascending1 && ascending2) { CALL_OPERATION(true, true) }
		  else if (ascending1 && !ascending2) { CALL_OPERATION(true, false) }
		  else if (!ascending1 && ascending2) { CALL_OPERATION(false, true) }
		  else if (!ascending1 && !ascending2) { CALL_OPERATION(false, false) }

#undef CALL_OPERATION
		  serie_1 = std::move(result);
	  }

	  template <Job job, typename Serie, typename... SerieArgs, std::size_t ...Is>
	  void
	  jobBoilerPlate (std::tuple<SerieArgs...> &&curr_result, Serie &serie, std::index_sequence<Is...>) {
		  (jobBoilerPlateImpl<job>(std::get<Is>(curr_result), serie), ...);
	  }

	  template<Job set_op_type, std::size_t I = 0, typename SerieRet, typename... SerieArgs>
	  auto
	  callSetOperationImpl (SerieRet &&curr_result, std::tuple<SerieArgs...> &&args) { //todo: make it && args again
		  if constexpr (I != sizeof...(SerieArgs)) {
			  static_assert(std::tuple_size_v<SerieRet> == 1u, "Somehow you've managed to achieve a result is in multi-dimensional space...");

			  //filter current tuple of results with the Arg[i]
			  jobBoilerPlate<set_op_type>(
					  std::forward<SerieRet>(curr_result), std::forward<decltype(std::get<I>(args))>(std::get<I>(args)),
					  std::make_index_sequence<1>{}
			  );

			  //continue recursion
			  return callSetOperationImpl<set_op_type, I+1>(
					  std::forward<SerieRet>(curr_result)
					  , std::forward<std::tuple<SerieArgs...>>(args)
			  );
		  }
		  else if constexpr (I == sizeof...(SerieArgs)) {
			  return std::forward<SerieRet>(curr_result);
		  }
	  }

	  template<Job set_op_type, typename... SerieArgs>
	  [[nodiscard]] auto callSetOperation (std::tuple<SerieArgs...> &&args) {
		  if constexpr (sizeof...(SerieArgs) <= 1u) {
			  return std::tuple();
		  }
		  else {
			  std::tuple t = std::make_tuple(std::get<0>(args));
			  auto res = callSetOperationImpl<set_op_type>(
					  std::move(t), std::forward<std::tuple<SerieArgs...>>(args)
			  );
			  static_assert(std::tuple_size_v<decltype(res)> == 1u, "somehow result is in multi-dimensions...");
			  return std::move(res);
		  }
	  }

	  template<Job join_type, std::size_t I = 0, typename SerieRet, typename... SerieArgs>
	  auto
	  callJoinOperationImpl(SerieRet &&curr_result, std::tuple<SerieArgs...> &&args) {
		  if constexpr (I!=sizeof...(SerieArgs)) {
			  //filter current tuple of results with the Arg[i]
			  jobBoilerPlate<join_type>(
					  std::forward<decltype(curr_result)>(curr_result),
					  std::forward<decltype(std::get<I>(args))>(std::get<I>(args)),
					  std::make_index_sequence<std::tuple_size_v<SerieRet>>{}
			  );

			  //append tuple of results with updated new element
			  auto result_new = tupletools::pushBack(
					  std::move(curr_result),
					  std::forward<decltype(std::get<I>(args))>(std::get<I>(args)));

			  //continue recursion
			  return callJoinOperationImpl<join_type, I+1, decltype(result_new), SerieArgs...>(
					  std::move(result_new), std::forward<decltype(args)>(args)
			  );
		  }
		  else if constexpr (I == sizeof...(SerieArgs)) {
			  if constexpr (join_type == Job::JoinOuterExcluding) {
				  return std::forward<decltype(curr_result)>(curr_result);
			  }
			  else {
				  static_assert(
						  std::tuple_size_v<std::remove_reference_t<decltype(curr_result)>>>1u,
						  "unexpected size of results tuple while processing a join");
				  constexpr std::size_t last_elem_idx =
						  std::tuple_size_v<std::remove_reference_t<decltype(curr_result)>>-1;
				  using last_elem_type = decltype(std::get<last_elem_idx>(
						  curr_result)); //todo: check if remove_ref is required

				  constexpr std::size_t prev_elem_idx = last_elem_idx-1;
				  using prev_elem_type = decltype(std::get<prev_elem_idx>(
						  curr_result)); //todo: check if remove_ref is required

				  //align the last part of a tuple of results along with previously received
				  jobBoilerPlateImpl
						  <join_type==Job::JoinOuterFull ? Job::JoinOuterFull : Job::JoinInner>(
						  std::forward<last_elem_type>(std::get<last_elem_idx>(curr_result)),
						  std::forward<prev_elem_type>(std::get<prev_elem_idx>(curr_result))
				  );

				  //move is required, as serie_ret is an argument, which is not guaranteed to have RVO
				  return std::forward<decltype(curr_result)>(curr_result);
			  }
		  }
	  }

	  template<Job join_type, typename... SerieArgs>
	  [[nodiscard]] auto callJoinOperation(SerieArgs&& ...series) {
		  if constexpr (sizeof...(SerieArgs) <= 1u) {
			  return std::tuple();
		  }
		  else {
			  auto args = std::make_tuple(std::forward<SerieArgs>(series)...); //todo: add lvalue_ref
			  if constexpr (requires_reverse<join_type>) {
				  args = tupletools::reverse(std::move(args));
			  }

			  if constexpr (join_type == Job::JoinOuterExcluding) { //requires special attention
				  auto init_value = callSetOperation<Job::SetUnion>(std::forward<decltype(args)>(args));
				  auto tmp = callJoinOperationImpl<join_type>(std::forward<decltype(args)>(args), std::move(init_value));
				  auto res = tupletools::popBack(std::move(tmp));
				  if constexpr (requires_reverse<join_type>) return res;
				  else return tupletools::reverse(std::move(res));
			  }
			  else {
				  auto init_value = std::make_tuple(std::get<0>(args)); //todo: freakin' copy, should think how to avoid
				  auto tmp = callJoinOperationImpl<join_type>(std::move(init_value), std::forward<decltype(args)>(args));
				  auto res = tupletools::popFront(std::move(tmp));
				  if constexpr (requires_reverse<join_type>) return tupletools::reverse(std::move(res));
				  else return res;
			  }
		  }
	  }

	}//!namespace

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto outerFull(SerieArgs&& ...series)
	{
		return
				details::callJoinOperation
						<details::Job::JoinOuterFull, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto outerExcluding(SerieArgs&& ...series)
	{
		return
				details::callJoinOperation
						<details::Job::JoinOuterExcluding, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto inner(SerieArgs&& ...series)
	{
		return
				details::callJoinOperation
						<details::Job::JoinInner, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto leftOuter(SerieArgs&& ...series)
	{
		return
				details::callJoinOperation
						<details::Job::JoinLeftFullOuter, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto leftExcluding(SerieArgs&& ...series)
	{
		return
				details::callJoinOperation
						<details::Job::JoinLeftExcluding, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto rightOuter(SerieArgs&& ...series)
	{
		return
				details::callJoinOperation
						<details::Job::JoinRightFullOuter, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto rightExcluding(SerieArgs&& ...series)
	{
		return
				details::callJoinOperation
						<details::Job::JoinRightExcluding, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto setIntersection(SerieArgs&& ...series)
	{
		return
				details::callSetOperation
						<details::Job::SetIntersection, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto setUnion(SerieArgs&& ...series)
	{
		return
				details::callSetOperation
						<details::Job::SetUnion, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto setDifference(SerieArgs&& ...series)
	{
		return
				details::callSetOperation
						<details::Job::SetDifference, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

	template<typename... SerieArgs, requirements::AreAllContainers<SerieArgs...> = true>
	auto setSymmetricDifference(SerieArgs&& ...series)
	{
		return
				details::callSetOperation
						<details::Job::SetSymmetricDifference, SerieArgs...>
						(std::forward<SerieArgs>(series)...);
	}

  }//!namespace
}//!namespace


#endif //DATA_JOINS_H
