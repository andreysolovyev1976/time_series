//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <type_traits>
#include <memory>
#include <utility>

#ifndef ITERTOOLS_H
#define ITERTOOLS_H

template <typename Iterator>
class Sequence {
public:
	Sequence(Iterator begin, Iterator end)
			: begin_(begin)
			, end_(end)
	{
	}
	auto begin() const { return begin_; }
	auto end() const { return end_; }

private:
	Iterator begin_;
	Iterator end_;
};

template <typename Iter>
using select_access_type_for = typename Iter::reference;

template <typename Iter1, typename Iter2>
class zip_iterator
{
public:

	using value_type = std::pair<
			select_access_type_for<Iter1>,
			select_access_type_for<Iter2>
	>;

	zip_iterator() = delete;

	zip_iterator(Iter1 iter_1_begin, Iter2 iter_2_begin)
			: m_iter_1_begin{ iter_1_begin }
			, m_iter_2_begin{ iter_2_begin }
	{
	}

	auto operator++() -> zip_iterator&
	{
		++m_iter_1_begin;
		++m_iter_2_begin;
		return *this;
	}

	auto operator++(int) -> zip_iterator
	{
		auto tmp = *this;
		++* this;
		return tmp;
	}

	auto operator!=(zip_iterator const& other) const
	{
		return !(*this == other);
	}

	auto operator==(zip_iterator const& other) const
	{
		return
				m_iter_1_begin == other.m_iter_1_begin ||
						m_iter_2_begin == other.m_iter_2_begin;
	}

	auto operator*() -> value_type
	{
		return value_type{ *m_iter_1_begin, *m_iter_2_begin };
	}

private:
	Iter1 m_iter_1_begin;
	Iter2 m_iter_2_begin;
};

template <typename T>
using select_iterator_for = std::conditional_t<
		std::is_const_v<std::remove_reference_t<T>>,
		typename std::decay_t<T>::const_iterator,
		typename std::decay_t<T>::iterator>;

template <typename T, typename U>
class zipper
{
public:

	using Iter1 = select_iterator_for<T>;
	using Iter2 = select_iterator_for<U>;

	using zip_type = zip_iterator<Iter1, Iter2>;

	template <typename V, typename W>
	zipper(V&& a, W&& b)
			: m_a{ a }
			, m_b{ b }
	{
	}

	auto begin() -> zip_type
	{
		return zip_type{ std::begin(m_a), std::begin(m_b) };
	}
	auto end() -> zip_type
	{
		return zip_type{ std::end(m_a), std::end(m_b) };
	}
private:
	T m_a;
	U m_b;
};

template <typename T, typename U>
auto zip(T&& t, U&& u)
{
	auto res = zipper<T, U>{ std::forward<T>(t), std::forward<U>(u) };
	return Sequence<decltype(res.begin())>(res.begin(),res.end());
}



#endif //ITERTOOLS_H
