//
// Created by Andrey Solovyev on 03/03/2023.
//

#pragma once

#include "utils/itertools.hpp"

#include <algorithm>
#include <type_traits>
#include <cstddef>


#ifndef DATA_JOINS_H
#define DATA_JOINS_H

namespace base::join {

  struct SetOperations {
	  enum class Operation {
		  None = 0,
		  Intersection,
		  Difference,
		  SymmetricDifference,
		  Union,
	  };
	  operator int () { return static_cast<int>(op); }

	  Operation op;
  };

#define IMPLEMENT_OPERATION_ON_SETS(c, ...) \
switch(c) \
{ case 1: std::set_intersection			(__VA_ARGS__); break; \
  case 2: std::set_difference			(__VA_ARGS__); break; \
  case 3: std::set_symmetric_difference	(__VA_ARGS__); break; \
  case 4: std::set_union				(__VA_ARGS__); break;  }


  template <typename Serie>
  bool isSortedAscending(Serie const& v) {
	  return *v.begin() < *std::prev(v.end());
  }

  template <
		  typename SerieRet
		  , typename... SerieArgs
		  , std::enable_if_t<sizeof...(SerieArgs)==2u, void>
  >
  void
  implementSetOperation(
		  SerieRet & result
		  , SetOperations operation
		  , std::tuple<SerieArgs...> &args) {
	  //there is no guarantee resulting container is empty, therefore it is end(), not begin()
	  auto ret_iter = std::inserter(result, result.end());

	  bool const ascending1 { isSortedAscending(std::get<0>(args)) };
	  bool const ascending2 { isSortedAscending(std::get<1>(args)) };
	  using Serie1 = std::remove_reference_t<decltype(std::get<0>(args))>;
	  using Serie2 = std::remove_reference_t<decltype(std::get<1>(args))>;

#define CALL_OPERATION(first_cond, second_cond) \
	auto [b1, e1] = itertools::getIterators<Serie1, first_cond>(std::get<0>(args)); \
	auto [b2, e2] = itertools::getIterators<Serie2, second_cond>(std::get<1>(args)); \
	IMPLEMENT_OPERATION_ON_SETS(operation, b1, e1, b2, e2, ret_iter);

	  if 		(ascending1  && ascending2 ) { CALL_OPERATION(true,  true ) }
	  else if (ascending1  && !ascending2) { CALL_OPERATION(true,  false) }
	  else if (!ascending1 && ascending2 ) { CALL_OPERATION(false, true ) }
	  else if (!ascending1 && !ascending2) { CALL_OPERATION(false, false) }

#undef CALL_OPERATION
	  return result;
  }

  template <
		  std::size_t I = 0
		  , typename SerieRet
		  , typename... SerieArgs
  >
  typename std::enable_if_t<I == sizeof...(SerieArgs), void>
  implementSetOperation(
		  SerieRet&
		  , SetOperations
		  , std::tuple<SerieArgs...>&
  )
  { /* do nothing */ }


  template <
		  std::size_t I = 0
		  , typename SerieRet
		  , typename... SerieArgs
  >
  typename std::enable_if_t <(I < sizeof...(SerieArgs)), void>
  implementSetOperation(
		  SerieRet & result
		  , SetOperations operation
		  , std::tuple<SerieArgs...> &args)
  {
	  if constexpr (sizeof...(SerieArgs) <= 1) { return; }

	  if constexpr (I == 0) {
		  //there is no guarantee resulting container is empty, therefore it is end(), not begin()
		  auto res_iter = std::inserter(result, result.end());
		  auto const& first = std::get<I>(args);
		  std::copy(first.begin(), first.end(), res_iter);

		  implementSetOperation<I+1, SerieRet, SerieArgs...>(result, operation, args);
	  }
	  else if constexpr (I != sizeof...(SerieArgs)) {

#define CALL_OPERATION(first_cond, second_cond) \
    SerieRet result_next; \
	auto res_iter = std::inserter(result_next, result_next.begin()); \
	auto [b1, e1] = itertools::getIterators<SerieRet, first_cond>(result); \
	using Serie2 = std::tuple_element_t<I, std::tuple<SerieArgs...>>; \
	auto [b2, e2] = itertools::getIterators<Serie2, second_cond>(std::get<I>(args)); \
	IMPLEMENT_OPERATION_ON_SETS(operation, b1, e1, b2, e2, res_iter); \
	result = std::move(result_next);

		  bool const ascending1 { isSortedAscending(result) };
		  bool const ascending2 { isSortedAscending(std::get<I>(args)) };

		  if 		(ascending1  && ascending2 ) { CALL_OPERATION(true,  true ) }
		  else if (ascending1  && !ascending2) { CALL_OPERATION(true,  false) }
		  else if (!ascending1 && ascending2 ) { CALL_OPERATION(false, true ) }
		  else if (!ascending1 && !ascending2) { CALL_OPERATION(false, false) }

#undef CALL_OPERATION

		  implementSetOperation<I+1, SerieRet, SerieArgs...>(result, operation, args);
	  }
  }

  template <typename SerieRet, typename... SerieArgs>
  void callOperation (SerieRet & result, SetOperations operation, SerieArgs&& ...series){
	  auto args = std::make_tuple(std::forward<SerieArgs>(series)...);
	  if constexpr (sizeof...(SerieArgs) <= 1) {
		  return;
	  }
	  else {
		  implementSetOperation(result, operation, args);
	  }
  }

  template <typename SerieRet, typename... SerieArgs>
  void symmetric_difference (SerieRet & result, SerieArgs&& ...series) {
	  SetOperations operation {SetOperations::Operation::SymmetricDifference};
	  callOperation<std::forward<SerieRet>, std::forward<SerieArgs>...>(result, operation, series...);
  }

  template <typename SerieRet, typename... SerieArgs>
  void difference (SerieRet & result, SerieArgs&& ...series) {
	  SetOperations operation {SetOperations::Operation::Difference};
	  callOperation<std::forward<SerieRet>, std::forward<SerieArgs>...>(result, operation, series...);
  }

  template <typename SerieRet, typename... SerieArgs>
  void inner (SerieRet & result, SerieArgs&& ...series) {
	  SetOperations operation {SetOperations::Operation::Intersection};
	  callOperation<std::forward<SerieRet>, std::forward<SerieArgs>...>(result, operation, series...);
  }

  template <typename SerieRet, typename... SerieArgs>
  void union_ (SerieRet & result, SerieArgs&& ...series) {
	  SetOperations operation {SetOperations::Operation::Union};
	  callOperation<std::forward<SerieRet>, std::forward<SerieArgs>...>(result, operation, series...);
  }
}//!namespace


#endif //DATA_JOINS_H
