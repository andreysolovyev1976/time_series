//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include <charconv>
#include <string>
#include <array>
#include <stdexcept>
#include <system_error>

#include <tuple>


#ifndef BASE_UTILS_H
#define BASE_UTILS_H

namespace base::utils {

	template<typename Number, typename = std::enable_if_t<std::is_arithmetic_v<Number>>>
	std::string toChars(Number input) {
#ifdef __APPLE__
		return std::to_string(input);
#else

		std::array<char, 32> str;
		if (auto [ptr, ec] = std::to_chars(str.data(), str.data()+str.size(), input);
				ec == std::errc()) {
			return std::string(str.data(), ptr);
		}
		else {
			std::invalid_argument(std::make_error_code(ec).message());
		}
#endif
	}

	inline
	auto fromChars(const std::string &str) {
		double local_result;
#ifdef __APPLE__
		local_result = std::stod(str);
#else

		auto [ptr, ec]{std::from_chars(str_int.data(), str_int.data() + str_int.size(), local_result)};
		if (ec == std::errc::invalid_argument) {
			throw std::invalid_argument("Attempt to convert not a number; ");
		} else if (ec == std::errc::result_out_of_range) {
			throw std::invalid_argument("Out of bound for an int64_t; ");
		}
#endif
		return local_result;
	}


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


}//!namespace
#endif //BASE_UTILS_H
