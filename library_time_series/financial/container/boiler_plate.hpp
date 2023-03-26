//
// Created by Andrey Solovyev on 24/03/2023.
//

#pragma once

#include "randomer.hpp"


#include <concepts>
#include <algorithm>
#include <stdexcept>
#include <memory_resource>
#include <vector>
#include <deque>
#include <map>


#ifndef PMR_CONTAINER_BOILER_PLATE_H
#define PMR_CONTAINER_BOILER_PLATE_H

namespace boiler_plate {

  enum class alloc_option_t {
	  none,
	  std,
	  pmr
  };

  template<typename T, alloc_option_t alloc_option>
  auto get_vector = [](
		  std::size_t size,
		  std::pmr::memory_resource* memory_resource = std::pmr::get_default_resource()
  ) {
	std::mt19937 rng;

	if constexpr (alloc_option == alloc_option_t::std) {
		std::vector<T> res;
		res.reserve(size);
		std::generate_n(std::inserter(res, res.end()), size, std::ref(rng));
		return res;
	}
	else if (alloc_option == alloc_option_t::pmr) {
		std::pmr::vector<T> res (memory_resource);
		res.reserve(size);
		std::generate_n(std::inserter(res, res.end()), size, std::ref(rng));
		return res;
	}
	else {
		throw std::invalid_argument("determine allocator type");
	}
  };

  template<typename T, alloc_option_t alloc_option>
  auto get_deque = [](
		  std::size_t size,
		  std::pmr::memory_resource* memory_resource = std::pmr::get_default_resource()
  ) {
	std::mt19937 rng;

	if constexpr (alloc_option == alloc_option_t::std) {
		std::deque<T> res;
		std::generate_n(std::inserter(res, res.end()), size, std::ref(rng));
		return res;
	}
	else if (alloc_option == alloc_option_t::pmr) {
		std::pmr::deque<T> res (memory_resource);
		std::generate_n(std::inserter(res, res.end()), size, std::ref(rng));
		return res;
	}
	else {
		throw std::invalid_argument("determine allocator type");
	}
  };


  template<typename Key, typename Value, alloc_option_t alloc_option>
  auto get_map = [](
		  std::size_t size,
		  std::pmr::memory_resource* memory_resource = std::pmr::get_default_resource()
  ) {

	if constexpr (alloc_option == alloc_option_t::std) {
		std::map<Key, Value> res;
		for (auto i = 0u; i != size; ++i)
			res[i] = Value{};
		return res;
	}
	else if (alloc_option == alloc_option_t::pmr) {
		std::pmr::map<Key, Value> res (memory_resource);
		for (auto i = 0u; i != size; ++i)
			res[i] = Value{};
		return res;
	}
	else {
		throw std::invalid_argument("determine allocator type");
	}
  };


  std::size_t const
		  ksmall {10'000},
		  kmed {100'000},
		  kbig {1'000'000};

  enum class ksize {
	  tiny,
	  small,
	  med,
	  big,
  };

  template <ksize size>
  auto get_size = []() {
	if constexpr (size == ksize::tiny) { return 1000u; }
	else if constexpr (size == ksize::small) { return 10'000u; }
	else if constexpr (size == ksize::med) { return 100'000u; }
	else if constexpr (size == ksize::big) { return 1'000'000u; }
  };


  template <typename T>
  auto modify_array_like = [](T& v){
	for (auto & e : v) {
		e += culib::utils::randomer.template operator()<typename T::value_type>();
	}
  };

  template <typename T>
  auto modify_map = [](T& v){
	for (auto & [key, value] : v) {
		value += culib::utils::randomer.template operator()<decltype(value)>();;
	}
  };


  namespace test_params {
	[[maybe_unused]] int const small_size_test = 3; //seconds
	[[maybe_unused]] int const mid_size_test = 6;
	[[maybe_unused]] int const big_size_test = 10;
  }

}//!namespace


#endif //PMR_CONTAINER_BOILER_PLATE_H
