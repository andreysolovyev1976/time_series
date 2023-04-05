//
// Created by Andrey Solovyev on 15/03/2023.
//

#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <new>

#ifndef MEMORY_USAGE_H
#define MEMORY_USAGE_H

namespace culib::memory {

//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Memory usage: redefinition of operator new to track standard allocations
//
//----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//
// This code snippet was significantly changed by Andrey Solovyev,
// although basic idea is still visible there
//
//----------------------------------------------------------------------------

  namespace details {
	struct heap_allocation_stats_t {
	public:
		heap_allocation_stats_t(heap_allocation_stats_t const&) = delete;
		heap_allocation_stats_t(heap_allocation_stats_t&&) = delete;
		heap_allocation_stats_t& operator=(heap_allocation_stats_t const&) = delete;
		heap_allocation_stats_t& operator=(heap_allocation_stats_t&&) = delete;

		static
		auto get_instance() -> heap_allocation_stats_t&
		{
			static heap_allocation_stats_t m;
			return m;
		}

		inline auto dump_to(std::ostream& os) { os << memory << " : " << alloc << '\n'; }

	private:
		heap_allocation_stats_t() { }

	public:
		std::size_t memory{0};
		std::size_t alloc{0};
	};
  }//!namespace

  static
  decltype(auto) heap_stats = []() -> details::heap_allocation_stats_t& {
	return details::heap_allocation_stats_t::get_instance();
  }();

  decltype(auto) operator<<(std::ostream& os, details::heap_allocation_stats_t const&) {
	  heap_stats.dump_to(os);
	  return os;
  }

}//!namespace


#ifdef UTILS_MEM_OPERATORS_OVERLOADED

/**
 * @brief
 * basically taken from here\n
 * https://en.cppreference.com/w/cpp/memory/new/operator_delete \n
 * */

void* operator new(size_t s) noexcept(false) {
	culib::mem_usage::heap_stats.memory += s;
	++culib::mem_usage::heap_stats.alloc;

	if (s == 0) ++s; // avoid std::malloc(0) which may return nullptr on success
	if (void *ptr = std::malloc(s); ptr) return ptr;

	throw std::bad_alloc{};
}

void operator delete(void* p) noexcept {
	--culib::mem_usage::heap_stats.alloc;
	std::free(p);
}

void operator delete(void* p, std::size_t s) noexcept {
	culib::mem_usage::heap_stats.memory -= s;
	--utils::mem_usage::heap_stats.alloc;
	std::free(p);
}
#endif

#endif //MEMORY_USAGE_H
