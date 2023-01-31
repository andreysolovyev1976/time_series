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

  }//!namespace
}//!namespace
#endif //BASE_UTILS_H
