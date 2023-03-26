//
// Created by Andrey Solovyev on 15/03/2023.
//

#pragma once

#include "common_usage_library/const_values.h"
#include <memory_resource>
#include <array>

#ifndef PMR_CONTAINER_CUSTOM_MEMORY_RESOURCE_H
#define PMR_CONTAINER_CUSTOM_MEMORY_RESOURCE_H

namespace culib::mem_usage {

  namespace details {
	template<typename T, std::size_t N>
	struct buf_wrapper_t {
		std::array
				<std::byte,
				 sizeof(T) * N + const_values::ALLOWANCE_FOR_CONTAINER_STATE_DATA>
				buffer;
	};
  }//!namespace

  template<typename T, std::size_t N>
  struct mem_resource_t :
		  private details::buf_wrapper_t <T, N>,
		  public std::pmr::monotonic_buffer_resource {
	  explicit mem_resource_t () noexcept
			  : monotonic_buffer_resource (
			  details::buf_wrapper_t<T, N>::buffer.data(),
			  details::buf_wrapper_t<T, N>::buffer.size(),
			  std::pmr::get_default_resource())
	  {}
  };


}//!namespace

#endif //PMR_CONTAINER_CUSTOM_MEMORY_RESOURCE_H
