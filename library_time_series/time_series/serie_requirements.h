//
// Created by Andrey Solovyev on 06/04/2023.
//

#pragma once

#include <type_traits>
#ifdef __cpp_concepts
#include <concepts>
#endif


#ifndef TS_REQUIREMENTS_H
#define TS_REQUIREMENTS_H


namespace time_series {


  namespace requirements {

#ifndef __cpp_concepts

	namespace details {
	  template<typename S, typename = void>
	  struct MaybeSerie : std::false_type {};

	  template<typename S>
	  struct MaybeSerie<S, std::void_t<decltype(S::serie_tag)> > : std::true_type {};

	  template<typename S, typename = void>
	  struct MaybeSerieSynthetic : std::false_type {};

	  template<typename S>
	  struct MaybeSerieSynthetic<S, std::void_t<decltype(S::serie_synthetic_tag)> > : std::true_type {};

	}


	template<typename S>
	inline constexpr bool is_serie_v { details::MaybeSerie<S>::value };

	template<typename S>
	using IsSerie = std::enable_if_t<is_serie_v<S>, bool>;


	template<typename S>
	inline constexpr bool is_serie_synthetic_v { details::MaybeSerieSynthetic<S>::value };

	template<typename S>
	using IsSerieSynthetic = std::enable_if_t<is_serie_synthetic_v<S>, bool>;


#else

	template <typename S>
	  concept IsSerie = requires () {S::serie_tag;};

	  template <typename S>
	  concept IsSerieSynthetic = requires () {S::serie_synthetic_tag;};

	  template<typename S>
	  inline constexpr bool is_serie_v { IsSerie<S> ? true : false };

	  template<typename S>
	  inline constexpr bool is_serie_synthetic_v { IsSerieSynthetic<S> ? true : false };

#endif
  }//!namespace

}//!namespace

#endif //TS_REQUIREMENTS_H
