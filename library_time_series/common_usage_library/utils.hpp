//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include "types_requirements/numerics.h"

#include <cstddef>
#include <charconv>
#include <string>
#include <array>
#include <vector>
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



  inline
  auto splitStringBy(std::string const& line, char delim, std::size_t expected_lines_count) {
	  std::string_view str(line);
	  std::vector<std::string_view> result;
	  result.reserve(expected_lines_count);
	  std::size_t found_delim{0};
	  while (true) {
		  found_delim = str.find(delim);
		  auto substr = str.substr(0, found_delim);
		  if (not substr.empty()) result.push_back(substr);
		  if (found_delim==str.npos) break;
		  else str.remove_prefix(found_delim+1);
	  }
	  return result;
  }

  inline
  auto splitStringViewBy(std::string_view s, char sep) {
	  std::vector<std::string_view> result;
	  while (!s.empty()) {
		  size_t pos = s.find(sep);
		  result.push_back(s.substr(0, pos));
		  s.remove_prefix(pos!=s.npos ? pos+1 : s.size());
	  }
	  return result;
  }


}//!namespace
#endif //BASE_UTILS_H
