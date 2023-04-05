//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "types_requirements/numerics.h"

#include <charconv>
#include <string>
#include <array>
#include <stdexcept>
#include <system_error>
#include <optional>

#ifndef BASE_UTILS_H
#define BASE_UTILS_H

namespace culib::utils {

#ifndef __cpp_concepts
  template<typename Number, requirements::IsArithmetic<Number> = true>
#else
  template<requirements::IsArithmetic Number>
#endif
	std::string toChars(Number input) {
		std::array<char, 32> str;
		if (auto [ptr, ec] = std::to_chars(str.data(), str.data()+str.size(), input);
				ec == std::errc()) {
			return std::string(str.data(), ptr);
		}
		else {
			std::invalid_argument(std::make_error_code(ec).message());
		}
	}



#ifndef __cpp_concepts
  template<typename Number, requirements::IsArithmetic<Number> = true>
#else
  template<requirements::IsArithmetic Number>
#endif
  std::optional<Number> fromChars(std::string_view str) {
	  Number local_result;
	  auto const last = str.data() + str.size();

	  auto [ptr, ec]{std::from_chars(str.data(), last, local_result)};

	  if (ec == std::errc::invalid_argument
	  || ec == std::errc::result_out_of_range
	  || ptr != last
		) {
		  return std::nullopt;
	  }
	  return {local_result};
//	  throw std::invalid_argument("Attempt to convert not a number; ");
//	  throw std::invalid_argument("Out of bound; ");
  }


}//!namespace
#endif //BASE_UTILS_H
