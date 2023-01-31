//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include "value_type.h"

#include <type_traits>
#include <functional>

#ifndef TYPE_REQUIREMENTS_H
#define TYPE_REQUIREMENTS_H

namespace requirements {

  template <typename Number>
  using IsFloatinPoint = std::enable_if_t<std::is_floating_point_v<Number>, bool>;
  template <typename Number>
  using IsNotFloatinPoint = std::enable_if_t<not std::is_floating_point_v<Number>, bool>;

  template <typename Number>
  using IsArithmetic = std::enable_if_t<std::is_arithmetic_v<Number>, bool>;

  template <typename Number>
  using IsNotArithmetic = std::enable_if_t<not std::is_arithmetic_v<Number>, bool>;


  template <typename Number>
  using MayBecomeValue = std::enable_if_t<
		  std::disjunction_v<
				  std::is_convertible<Number, base::traits::ValueType>,
				  std::is_constructible<base::traits::ValueType, Number>
		  >
		  , bool>;


  template <typename Function, typename Type>
  using Unary = std::enable_if_t<std::is_invocable_r<Type, Function, Type>::value, bool>;


  /**
   * @details
   * Operators check is based on a SFINAE technic, is copied from this source
   * https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
   *
   */
  template<class X, class Y, class Op>
  struct op_valid_impl
  {
	  template<class U, class L, class R>
	  static auto test(int) -> decltype(std::declval<U>()(std::declval<L>(), std::declval<R>()),
			  void(), std::true_type());

	  template<class U, class L, class R>
	  static auto test(...) -> std::false_type;

	  using type = decltype(test<Op, X, Y>(0));
  };

  template<class X, class Y, class Op> using op_valid = typename op_valid_impl<X, Y, Op>::type;

  namespace notstd {

	struct left_shift {
		template <class L, class R>
		constexpr auto operator()(L&& l, R&& r) const
		noexcept(noexcept(std::forward<L>(l) << std::forward<R>(r)))
		-> decltype(std::forward<L>(l) << std::forward<R>(r))
		{
			return std::forward<L>(l) << std::forward<R>(r);
		}
	};

	struct right_shift {
		template <class L, class R>
		constexpr auto operator()(L&& l, R&& r) const
		noexcept(noexcept(std::forward<L>(l) >> std::forward<R>(r)))
		-> decltype(std::forward<L>(l) >> std::forward<R>(r))
		{
			return std::forward<L>(l) >> std::forward<R>(r);
		}
	};
  }

  template<class X, class Y> using has_equality = op_valid<X, Y, std::equal_to<>>;
  template<class X, class Y> using has_inequality = op_valid<X, Y, std::not_equal_to<>>;
  template<class X, class Y> using has_less_than = op_valid<X, Y, std::less<>>;
  template<class X, class Y> using has_less_equal = op_valid<X, Y, std::less_equal<>>;
  template<class X, class Y> using has_greater_than = op_valid<X, Y, std::greater<>>;
  template<class X, class Y> using has_greater_equal = op_valid<X, Y, std::greater_equal<>>;
  template<class X, class Y> using has_bit_xor = op_valid<X, Y, std::bit_xor<>>;
  template<class X, class Y> using has_bit_or = op_valid<X, Y, std::bit_or<>>;
  template<class X, class Y> using has_left_shift = op_valid<X, Y, notstd::left_shift>;
  template<class X, class Y> using has_right_shift = op_valid<X, Y, notstd::right_shift>;

  template <class X, class Y>using has_plus = op_valid<X, Y, std::plus<>>;
  template <class X, class Y>using has_minus = op_valid<X, Y, std::minus<>>;
  template <class X, class Y>using has_multiply = op_valid<X, Y, std::multiplies<>>;
  template <class X, class Y>using has_divides = op_valid<X, Y, std::divides<>>;
  template <class X, class Y>using has_modulus = op_valid<X, Y, std::modulus<>>;

  template <class X, class Y>using has_logical_and = op_valid<X, Y, std::logical_and<>>;
  template <class X, class Y>using has_logical_or = op_valid<X, Y, std::logical_or<>>;
  /**
   * @details
   * this is the end of copied fragment
   */


  template <typename L, typename R>
  using Comparable = std::enable_if_t<
		  std::conjunction_v<
				  has_equality<L, R>,
				  has_equality<R, L>,
				  has_less_than<L, R>,
				  has_less_than<R, L>
		  >
		  , bool>;

  template <typename L, typename R>
  using ArithmeticOperationsDefined = std::enable_if_t<
		  std::conjunction_v<
				  has_plus<L,R>,
				  has_plus<R,L>,
				  has_minus<L,R>,
				  has_minus<R,L>,
				  has_divides<L,R>,
				  has_divides<R,L>,
				  has_multiply<L,R>,
				  has_multiply<R,L>
		  >
		  , bool>;

  template <typename L, typename R,
		  typename = Comparable<L,R>,
		  typename = ArithmeticOperationsDefined<L, R>>
  using IsSuitable = bool;


}//!namespace
#endif //TYPE_REQUIREMENTS_H
