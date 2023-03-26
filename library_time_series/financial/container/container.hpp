//
// Created by Andrey Solovyev on 26/03/2023.
//

#pragma once

#include "common_usage_library/memuse.hpp"
#include "time_series/element.hpp"

#include <deque>

#ifndef FIN_DATA_CONTAINER_H
#define FIN_DATA_CONTAINER_H

namespace time_series::financial {


  template <
		  std::size_t N,
		  typename Duration = base::Seconds,
		  typename ElemType = Value<value::traits::ValueTypeDefault>,
		  typename Datapoint = Element<Duration, ElemType>>
  class Container final
		  : private culib::mem_usage::mem_resource_t<N, Datapoint>,
			public std::pmr::deque<Datapoint>
  {
	  explicit Container ()
			  : std::pmr::deque<Datapoint> (&this-> template mem_resource_t <N, Datapoint>)
	  {}


#if 0

#if __cplusplus >= 201103L
	  deque() = default;
#else
	  deque() { }
#endif

	  /**
	   *  @brief  Creates a %deque with no elements.
	   *  @param  __a  An allocator object.
	   */
	  explicit
	  deque(const allocator_type& __a)
			  : _Base(__a, 0) { }

#if __cplusplus >= 201103L
	  /**
	   *  @brief  Creates a %deque with default constructed elements.
	   *  @param  __n  The number of elements to initially create.
	   *  @param  __a  An allocator.
	   *
	   *  This constructor fills the %deque with @a n default
	   *  constructed elements.
	   */
	  explicit
	  deque(size_type __n, const allocator_type& __a = allocator_type())
	  : _Base(__a, _S_check_init_len(__n, __a))
	  { _M_default_initialize(); }

	  /**
	   *  @brief  Creates a %deque with copies of an exemplar element.
	   *  @param  __n  The number of elements to initially create.
	   *  @param  __value  An element to copy.
	   *  @param  __a  An allocator.
	   *
	   *  This constructor fills the %deque with @a __n copies of @a __value.
	   */
	  deque(size_type __n, const value_type& __value,
	  const allocator_type& __a = allocator_type())
	  : _Base(__a, _S_check_init_len(__n, __a))
	  { _M_fill_initialize(__value); }
#else
	  /**
       *  @brief  Creates a %deque with copies of an exemplar element.
       *  @param  __n  The number of elements to initially create.
       *  @param  __value  An element to copy.
       *  @param  __a  An allocator.
       *
       *  This constructor fills the %deque with @a __n copies of @a __value.
       */
      explicit
      deque(size_type __n, const value_type& __value = value_type(),
	    const allocator_type& __a = allocator_type())
      : _Base(__a, _S_check_init_len(__n, __a))
      { _M_fill_initialize(__value); }
#endif

	  /**
	   *  @brief  %Deque copy constructor.
	   *  @param  __x  A %deque of identical element and allocator types.
	   *
	   *  The newly-created %deque uses a copy of the allocator object used
	   *  by @a __x (unless the allocator traits dictate a different object).
	   */
	  deque(const deque& __x)
			  : _Base(_Alloc_traits::_S_select_on_copy(__x._M_get_Tp_allocator()),
			  __x.size())
	  { std::__uninitialized_copy_a(__x.begin(), __x.end(),
				  this->_M_impl._M_start,
				  _M_get_Tp_allocator()); }

#if __cplusplus >= 201103L
	  /**
	   *  @brief  %Deque move constructor.
	   *
	   *  The newly-created %deque contains the exact contents of the
	   *  moved instance.
	   *  The contents of the moved instance are a valid, but unspecified
	   *  %deque.
	   */
	  deque(deque&&) = default;

	  /// Copy constructor with alternative allocator
	  deque(const deque& __x, const allocator_type& __a)
			  : _Base(__a, __x.size())
	  { std::__uninitialized_copy_a(__x.begin(), __x.end(),
				  this->_M_impl._M_start,
				  _M_get_Tp_allocator()); }

	  /// Move constructor with alternative allocator
	  deque(deque&& __x, const allocator_type& __a)
	  : deque(std::move(__x), __a, typename _Alloc_traits::is_always_equal{})
	  { }

  private:
	  deque(deque&& __x, const allocator_type& __a, true_type)
	  : _Base(std::move(__x), __a)
	  { }

	  deque(deque&& __x, const allocator_type& __a, false_type)
	  : _Base(std::move(__x), __a, __x.size())
	  {
		  if (__x.get_allocator() != __a && !__x.empty())
		  {
			  std::__uninitialized_move_a(__x.begin(), __x.end(),
					  this->_M_impl._M_start,
					  _M_get_Tp_allocator());
			  __x.clear();
		  }
	  }

  public:
	  /**
	   *  @brief  Builds a %deque from an initializer list.
	   *  @param  __l  An initializer_list.
	   *  @param  __a  An allocator object.
	   *
	   *  Create a %deque consisting of copies of the elements in the
	   *  initializer_list @a __l.
	   *
	   *  This will call the element type's copy constructor N times
	   *  (where N is __l.size()) and do no memory reallocation.
	   */
	  deque(initializer_list<value_type> __l,
	  const allocator_type& __a = allocator_type())
	  : _Base(__a)
			  {
					  _M_range_initialize(__l.begin(), __l.end(),
							  random_access_iterator_tag());
			  }
#endif

	  /**
	   *  @brief  Builds a %deque from a range.
	   *  @param  __first  An input iterator.
	   *  @param  __last  An input iterator.
	   *  @param  __a  An allocator object.
	   *
	   *  Create a %deque consisting of copies of the elements from [__first,
	   *  __last).
	   *
	   *  If the iterators are forward, bidirectional, or random-access, then
	   *  this will call the elements' copy constructor N times (where N is
	   *  distance(__first,__last)) and do no memory reallocation.  But if only
	   *  input iterators are used, then this will do at most 2N calls to the
	   *  copy constructor, and logN memory reallocations.
	   */
#if __cplusplus >= 201103L
	  template<typename _InputIterator,
			  typename = std::_RequireInputIter<_InputIterator>>
	  deque(_InputIterator __first, _InputIterator __last,
	  const allocator_type& __a = allocator_type())
	  : _Base(__a)
			  {
					  _M_range_initialize(__first, __last,
							  std::__iterator_category(__first));
			  }
#else
	  template<typename _InputIterator>
	deque(_InputIterator __first, _InputIterator __last,
	      const allocator_type& __a = allocator_type())
	: _Base(__a)
	{
	  // Check whether it's an integral type.  If so, it's not an iterator.
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
	  _M_initialize_dispatch(__first, __last, _Integral());
	}
#endif




#endif
  };

}//!namespace

#endif //FIN_DATA_CONTAINER_H
