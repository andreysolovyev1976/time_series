//
// Created by Andrey Solovyev on 15/03/2023.
//

#pragma once

#include <memory_resource>
#include <array>
#include <cstdio>
#include <cstddef>

#ifndef PMR_CONTAINER_CUSTOM_MEMEORY_RESOURCE_H
#define PMR_CONTAINER_CUSTOM_MEMEORY_RESOURCE_H

namespace utils::mem_usage {

  template<typename T, std::size_t N>
  using buffer_t = std::array<std::byte, N * sizeof(T) + 32>;

#if 0
  template<typename T, std::size_t N>
  struct stackheap_memory_resource_t : public std::pmr::memory_resource {
  public:
	  explicit stackheap_memory_resource_t(std::pmr::memory_resource* parent =
	  std::pmr::get_default_resource())
			  : parent_(parent) {}
	  ~stackheap_memory_resource_t() {}
	  std::pmr::memory_resource* parent() const { return parent_; }

  protected:
	  void* do_allocate(std::size_t bytes, std::size_t alignment) override {
		  if (bytes > max_size_) {
			  p_curr_alloc_ = parent_->allocate(bytes, alignment);
			  return p_curr_alloc_;
		  }
#ifdef UTILS_MEM_OUTPUT_ALLOCATION_PTR
		  std::printf("allocate: %p\n", p_curr_alloc_);
#endif
		  return p_curr_alloc_;
	  }
	  void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override
	  {
		  if (p != std::addressof(buffer_)) {
			  parent_->deallocate(p, bytes, alignment);
		  }
		  else {
			  /// do nothing, as there is no point to call delete for a stack memory chunk;
		  }
#ifdef UTILS_MEM_OUTPUT_ALLOCATION_PTR
		  std::printf("deallocate: %p\n", p);
#endif
	  }
	  bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
		  return *this==other;
	  }

  private:
	  std::pmr::memory_resource* parent_;
	  std::size_t const max_size_ {N * sizeof(T)};
	  alignas(T) std::array<std::byte, N * sizeof(T)> buffer_ ;
	  void* p_curr_alloc_{std::addressof(buffer_)};
  };

#endif

}//!namespace

#endif //PMR_CONTAINER_CUSTOM_MEMEORY_RESOURCE_H
