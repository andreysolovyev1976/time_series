//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#include <type_traits>
#include <functional>
#ifdef __cpp_concepts
#include <concepts>
#endif

#ifndef TYPE_REQUIREMENTS_OPERATORS_H
#define TYPE_REQUIREMENTS_OPERATORS_H

namespace culib::requirements {
  /**
   * @details
   * Operators check is based on a SFINAE technic, is copied from this source\n
   * https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
   *
   */
#ifndef __cpp_concepts
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

#else

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

  template<class X, class Y> concept has_equality = requires (X x, Y y) {std::equal_to<>{}(x, y);};
  template<class X, class Y> concept has_inequality = requires (X x, Y y) {std::not_equal_to<>{}(x, y);};
  template<class X, class Y> concept has_less_than = requires (X x, Y y) {std::less<>{}(x, y);};
  template<class X, class Y> concept has_less_equal = requires (X x, Y y) {std::less_equal<>{}(x, y);};
  template<class X, class Y> concept has_greater_than = requires (X x, Y y) {std::greater<>{}(x, y);};
  template<class X, class Y> concept has_greater_equal = requires (X x, Y y) {std::greater_equal<>{}(x, y);};
  template<class X, class Y> concept has_bit_xor = requires (X x, Y y) {std::bit_xor<>{}(x, y);};
  template<class X, class Y> concept has_bit_or = requires (X x, Y y) {std::bit_or<>{}(x, y);};
  template<class X, class Y> concept has_left_shift = requires (X x, Y y) {notstd::left_shift{}(x, y);};
  template<class X, class Y> concept has_right_shift = requires (X x, Y y) {notstd::right_shift{}(x, y);};

  template <class X, class Y> concept has_plus = requires (X x, Y y) {std::plus<>{}(x, y);};
  template <class X, class Y> concept has_minus = requires (X x, Y y) {std::minus<>{}(x, y);};
  template <class X, class Y> concept has_multiply = requires (X x, Y y) {std::multiplies<>{}(x, y);};
  template <class X, class Y> concept has_divides = requires (X x, Y y) {std::divides<>{}(x, y);};
  template <class X, class Y> concept has_modulus = requires (X x, Y y) {std::modulus<>{}(x, y);};

  template <class X, class Y> concept has_logical_and = requires (X x, Y y) {std::logical_and<>{}(x, y);};
  template <class X, class Y> concept has_logical_or = requires (X x, Y y) {std::logical_or<>{}(x, y);};
  /**
   * @details
   * this is the end of copied fragment
   */


  template <typename L, typename R>
  concept ComparisonOperationsDefined = requires () {
	  requires has_equality<std::decay_t<L>, std::decay_t<R>>;
	  requires has_equality<std::decay_t<R>, std::decay_t<L>>;
	  requires has_less_than<std::decay_t<L>, std::decay_t<R>>;
	  requires has_less_than<std::decay_t<R>, std::decay_t<L>>;
  };

  template <typename L, typename R>
  concept ArithmeticOperationsDefined = requires () {
			  requires has_plus<std::decay_t<L>, std::decay_t<R>>;
			  requires has_plus<std::decay_t<R>, std::decay_t<L>>;
			  requires has_minus<std::decay_t<L>, std::decay_t<R>>;
			  requires has_minus<std::decay_t<R>, std::decay_t<L>>;
			  requires has_divides<std::decay_t<L>, std::decay_t<R>>;
			  requires has_divides<std::decay_t<R>, std::decay_t<L>>;
			  requires has_multiply<std::decay_t<L>, std::decay_t<R>>;
			  requires has_multiply<std::decay_t<R>, std::decay_t<L>>;
  };

  template <typename L, typename R>
  concept BinOperatorsExist = requires () {
	  requires ComparisonOperationsDefined<L, R>;
	  requires ArithmeticOperationsDefined<L, R>;
  };


  template <typename L, typename R>
  concept NotSame = not std::is_same_v<std::decay_t<L>, std::decay_t<R>>;

#endif

}//!namespace
#endif //TYPE_REQUIREMENTS_OPERATORS_H
