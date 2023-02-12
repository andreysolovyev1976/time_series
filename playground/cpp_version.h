//
// Created by Andrey Solovyev on 12/02/2023.
//

#pragma once

#include <string>
#include <type_traits>
#include <string_view>
#include <stdexcept>

#ifndef CPP_VERSION_H
#define CPP_VERSION_H

namespace cpp_standard {

  enum class version {
	  unknown,
	  no_cpp, //LOL
	  cpp_17,
	  cpp_20,
	  cpp_23,
	  cpp_future,
  };

  namespace requirments {
	template <typename T>
	using CalledStr = std::enable_if_t<std::is_same_v<T, std::string_view>, bool>;
	template <typename T>
	using CalledEnum = std::enable_if_t<std::is_same_v<T, version>, bool>;
	template <typename T>
	using WrongCall = std::enable_if_t<
			std::conjunction_v<
					std::negation<std::is_same<T, std::string_view>>,
					std::negation<std::is_same<T, version>>
			>
			, bool>;


  }//!namespace

  template <typename T, requirments::WrongCall<T> = true>
  constexpr auto get_version() {
	  throw std::runtime_error (
			  "\nfunc name:\n" +
					  std::string(__PRETTY_FUNCTION__) +
					  "\nreason:\n" +
					  "should be called with either \"std::string_view\" or enum class \"cpp_standard::version\" as a template parameter.\n");
  }

  template <typename T, requirments::CalledEnum<T> = true>
  constexpr version get_version()
  {
#if defined(__cplusplus)
#if __cplusplus==201703L
	  return version::cpp_17;
#elif __cplusplus==202002L
	  return version::cpp_20;
#elif __cplusplus == 202207L
	  	return version::cpp_23;
#elif __cplusplus > 202207L
	  	return version::cpp_future;
#endif
	  return version::unknown;
#endif
	  return version::no_cpp;
  }

  template <typename T, requirments::CalledStr<T> = true>
  constexpr std::string_view get_version() {
	  using namespace std::string_view_literals;
	  constexpr version v = get_version<version>();
	  switch (v) {
	  case (version::unknown): { return "unknown"sv; }
	  case (version::cpp_17): { return "C++17"sv; }
	  case (version::cpp_20): { return "C++20"sv; }
	  case (version::cpp_23): { return "C++23"sv; }
	  case (version::cpp_future): { return "some future C++"sv; }
	  default: { return "no C++ defined"sv; }
	  }
  }

}//!namespace

#endif //CPP_VERSION_H
