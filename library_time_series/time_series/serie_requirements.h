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


namespace time_series::requirements {

#ifndef __cpp_concepts

  namespace details {
	template<typename S, typename = void>
	struct MaybeSerieRegular : std::false_type {};

	template<typename S>
	struct MaybeSerieRegular<S, std::void_t<typename S::serie_regular_tag> > : std::true_type {};

	template<typename S, typename = void>
	struct MaybeSerieSynthetic : std::false_type {};

	template<typename S>
	struct MaybeSerieSynthetic<S, std::void_t<typename S::serie_synthetic_tag> > : std::true_type {};

  }

  template<typename S>
  inline constexpr bool is_serie_regular_v { details::MaybeSerieRegular<S>::value };

  template<typename S>
  using IsSerieRegular = std::enable_if_t<is_serie_regular_v<S>, bool>;

  template<typename S>
  inline constexpr bool is_serie_synthetic_v { details::MaybeSerieSynthetic<S>::value };

  template<typename S>
  using IsSerieSynthetic = std::enable_if_t<is_serie_synthetic_v<S>, bool>;

  template<typename S>
  inline constexpr bool is_serie_v { is_serie_regular_v<S> || is_serie_synthetic_v<S> };

  template<typename S>
  using IsSerie = std::enable_if_t<is_serie_v<S>, bool>;


  namespace details {
	template<typename ElemType, typename = void>
	struct MaybeCollisionAllowed : std::false_type {};

	template<typename ElemType>
	struct MaybeCollisionAllowed<ElemType, std::void_t<
			  std::is_invocable<decltype(&ElemType::collideWith), ElemType&, ElemType>
	>> : std::true_type {};
  }//!namespace

  template<typename ElemType>
  inline constexpr bool is_collision_allowed_v { details::MaybeCollisionAllowed<ElemType>::value };

  template<typename ElemType>
  using IsCollisionAllowed = std::enable_if_t<is_collision_allowed_v<ElemType>, bool>;

  template<typename ElemType>
  using IsNotCollisionAllowed = std::enable_if_t<!is_collision_allowed_v<ElemType>, bool>;


#else

  template <typename S>
	concept IsSerieRegular = requires () { typename S::serie_regular_tag; };

	template <typename S>
	concept IsSerieSynthetic = requires () { typename S::serie_synthetic_tag;};

	template <typename S>
	concept IsSerie = IsSerieRegular<S> || IsSerieSynthetic<S>;

	template<typename S>
	inline constexpr bool is_serie_regular_v { IsSerieRegular<S> ? true : false };

	template<typename S>
	inline constexpr bool is_serie_synthetic_v { IsSerieSynthetic<S> ? true : false };

	template<typename S>
	inline constexpr bool is_serie_v { IsSerie<S> ? true : false };


	template<typename ElemType>
	concept IsCollisionAllowed = requires (ElemType e, ElemType other) { e.collideWith(other); };

	template<typename ElemType>
	inline constexpr bool is_collision_allowed_v { IsCollisionAllowed<ElemType> ? true : false };


#endif

}//!namespace

#endif //TS_REQUIREMENTS_H
