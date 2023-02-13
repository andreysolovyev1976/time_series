//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include <charconv>
#include <string>
#include <array>
#include <stdexcept>
#include <system_error>

#ifndef BASE_UTILS_H
#define BASE_UTILS_H

namespace base {

  namespace utils {

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


  }//!namespace
}//!namespace
#endif //BASE_UTILS_H
