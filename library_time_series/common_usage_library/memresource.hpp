//
// Created by Andrey Solovyev on 15/03/2023.
//

#pragma once

#include "common_usage_library/const_values.h"

#include <cstddef>
#include <memory_resource>
#include <array>


#ifndef MEMORY_RESOURCE_H
#define MEMORY_RESOURCE_H

namespace culib::memory {

  namespace details {
	template<std::size_t N, typename T>
	struct buf_wrapper_t {
		std::array
				<std::byte,
				 sizeof(T)*N+const_values::ALLOWANCE_FOR_CONTAINER_STATE_DATA>
				buffer;
	};
  }//!namespace

  template<std::size_t N, typename T>
  struct mem_resource_t :
		  private details::buf_wrapper_t<N, T>,
		  public std::pmr::monotonic_buffer_resource {
	  explicit mem_resource_t() noexcept
			  :monotonic_buffer_resource(
			  details::buf_wrapper_t<N, T>::buffer.data(),
			  details::buf_wrapper_t<N, T>::buffer.size(),
			  std::pmr::get_default_resource()) { }
  };

}//!namespace

#endif //MEMORY_RESOURCE_H
