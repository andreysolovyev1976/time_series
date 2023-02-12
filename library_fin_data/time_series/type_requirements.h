//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

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
  using IsIntegral = std::enable_if_t<std::is_integral_v<Number>, bool>;

  template <typename Number>
  using IsNotIntegral = std::enable_if_t<not std::is_integral_v<Number>, bool>;


  template <typename Input, typename Result>
  using ConveribleOrConstructibleFromTo = std::enable_if_t<
		  std::disjunction_v<
				  std::is_convertible<Input, Result>,
				  std::is_constructible<Result, Input>
		  >
		  , bool>;

  template <typename Input, typename Result, ConveribleOrConstructibleFromTo<Input, Result> = true>
  constexpr bool isConveribleOrConstructible () {return true;}


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
  using ComparisonOperationsDefined = std::enable_if_t<
		  std::conjunction_v<
				  has_equality<std::decay_t<L>, std::decay_t<R>>
				  , has_equality<std::decay_t<R>, std::decay_t<L>>
				  , has_less_than<std::decay_t<L>, std::decay_t<R>>
				  , has_less_than<std::decay_t<R>, std::decay_t<L>>
		  >
		  , bool>;

  template <typename L, typename R>
  using ArithmeticOperationsDefined = std::enable_if_t<
		  std::conjunction_v<
				  has_plus<std::decay_t<L>, std::decay_t<R>>
				  , has_plus<std::decay_t<R>, std::decay_t<L>>
				  , has_minus<std::decay_t<L>, std::decay_t<R>>
				  , has_minus<std::decay_t<R>, std::decay_t<L>>
				  , has_divides<std::decay_t<L>, std::decay_t<R>>
				  , has_divides<std::decay_t<R>, std::decay_t<L>>
				  , has_multiply<std::decay_t<L>, std::decay_t<R>>
				  , has_multiply<std::decay_t<R>, std::decay_t<L>>
		  >
		  , bool>;

  template <typename L, typename R
		  , ComparisonOperationsDefined<L, R> = true
		  , ArithmeticOperationsDefined<L, R> = true
		  >
  using BinOperatorsExist = bool;




  template <typename L, typename R>
  using NotSame = std::enable_if_t<not std::is_same_v<std::decay_t<L>, std::decay_t<R>>, bool>;




  template <typename DataStructure, BinOperatorsExist<DataStructure, double> = true>
  using CanBeElemType = bool;


  template <typename Fn, typename Elem>
  using ModifiesElemInPlace = std::enable_if_t<
          std::is_invocable_v<Fn, std::add_lvalue_reference<std::decay_t<Elem>>>, bool>;
  template <typename Fn, typename Elem>
  using CreatesNewElem = std::enable_if_t<std::is_invocable_r_v<Elem, Fn, Elem>, bool>;


}//!namespace
#endif //TYPE_REQUIREMENTS_H
