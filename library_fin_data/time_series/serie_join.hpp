//
// Created by Andrey Solovyev on 01/03/2023.
//

#pragma once

#include "common_usage_library/itertools.hpp"
#include "common_usage_library/joins.hpp"

#include <algorithm>
#include <type_traits>
#include <cstddef>

#ifndef FIN_DATA_SERIE_JOIN_H
#define FIN_DATA_SERIE_JOIN_H

namespace time_series {

  namespace join {


	//todo: add serie requirementes

#if 0
	template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare = std::less<>>
		_OutputIterator
		__set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
				_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp = std::less<>{})
		{
			while (__first1 != __last1)
			{
				if (__first2 == __last2)
					return _VSTD::copy(__first1, __last1, __result);

				if (__comp(*__first1, *__first2))
				{
					*__result = *__first1;
					++__result;
					++__first1;
				}
				else
				{
					if (!__comp(*__first2, *__first1))
						++__first1;
					++__first2;
				}
			}
			return __result;
		}

		template <class _Compare, class _InputIterator1, class _InputIterator2, class _OutputIterator>
		_OutputIterator
		__set_union(_InputIterator1 __first1, _InputIterator1 __last1,
				_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp)
		{
			for (; __first1 != __last1; ++__result)
			{
				if (__first2 == __last2)
					return _VSTD::copy(__first1, __last1, __result);
				if (__comp(*__first2, *__first1))
				{
					*__result = *__first2;
					++__first2;
				}
				else
				{
					if (!__comp(*__first1, *__first2))
						++__first2;
					*__result = *__first1;
					++__first1;
				}
			}
			return _VSTD::copy(__first2, __last2, __result);
		}

/**
 * @brief
 * copy-paste from algorithm.h\n
 * + merge of set_intersection and set_difference\n
 * + default param for Comp\n
 *
 * */

	template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare = std::less<>>
	constexpr _OutputIterator
	__set_inner(_InputIterator1 __first1, _InputIterator1 __last1,
			_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, [[maybe_unused]] _Compare __comp = std::less{})
	{
		while (__first1 != __last1 && __first2 != __last2)
		{
			if (__comp(*__first1, *__first2))
				++__first1;
			else
			{
				if (!__comp(*__first2, *__first1))
				{
					*__result = *__first1;
					++__result;
					++__first1;
				}
				++__first2;
			}
		}
		return __result;
	}


	template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare = std::less<>>
	constexpr _OutputIterator
	set_intersection_and_difference(_InputIterator1 __first1, _InputIterator1 __last1,
			_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, [[maybe_unused]] _Compare __comp = std::less<>{})
	{
		while (__first1 != __last1) {
			if (__first2 == __last2) {
				return std::copy(__first1, __last1, __result);
			}
			*__result = *__first1;
			++__result;
			++__first1;
			++__first2;
		}
		return __result;
	}

	template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare = std::less<>>
	constexpr _OutputIterator
	set_append_with_zeros(_InputIterator1 __first1, _InputIterator1 __last1,
			_InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, [[maybe_unused]] _Compare __comp = std::less<>{})
	{
		using ZeroType = typename std::iterator_traits<decltype(__first1)>::value_type;
		ZeroType const zero_value {};

		while (__first2 != __last2) {

			if (__first1 == __last1) {
				while (__first2 != __last2) {
					*__result = zero_value;
					++__result;
					++__first2;
				}
				return __result;
			}


			if (*__first2 < *__first1) {
				*__result = zero_value;
				++__result;
				++__first2;
			}
			else if (*__first2 == *__first1) {
				*__result = *__first2;
				++__first2;
				++__first1;
			}
			else if (*__first2 > *__first1) {
				++__first1;
			}
		}
		return __result;
	}
#endif
/**
 * @brief
 * A Serie must be
 * a) not empty
 * b) sorted
 * otherwise you'll have your UB
 *
 * */


  }//!namespace
}//!namespace
#endif //FIN_DATA_SERIE_JOIN_H
