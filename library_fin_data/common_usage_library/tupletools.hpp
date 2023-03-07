//
// Created by Andrey Solovyev on 07/03/2023.
//

#pragma once

#include <tuple>
#include <type_traits>

#ifndef TUPLETOOLS_H
#define TUPLETOOLS_H


namespace culib::tupletools {

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
		  return ret_tuple;
	  }
	  else {
		  auto new_ret_tuple = tuplePushBack(std::move(ret_tuple), *std::next(std::get<I>(tup).begin(), idx));
		  return getSliceImpl<I+1, decltype(new_ret_tuple), Args...>(tup, std::move(new_ret_tuple), idx);
	  }
  }

  template <typename... Args>
  constexpr auto getSlice (std::tuple<Args...> const& tup, std::size_t const idx) {
	  std::tuple ret;
	  return getSliceImpl(tup, ret, idx);
  }
}//!namespace


#endif //TUPLETOOLS_H
