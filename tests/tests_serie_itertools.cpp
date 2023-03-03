//
// Created by Andrey Solovyev on 23/02/2023.
//

#include <gtest/gtest.h>
#include "utils/itertools.hpp"
#include "time_series/serie_random_generator.hpp"

#include <vector>
#include <map>
#include <string>
#include <sstream>

TEST(SerieItertools, Serie_String) {
	std::size_t const ksize {3u};
	int value {42};
	auto ts = time_series::utils::generateSerie(value, ksize);

	std::string s { "abcdefghhlk" };
	std::stringstream ss;

	for (auto const& [first, second] : itertools::zip(s, ts)) {
		ss << first << ' ' << second.value << '\n';
	}
	std::string check {R"(a 42
b 43
c 44
)"};
	ASSERT_EQ(ss.str(), check);
}

TEST(SerieItertools, Serie_Serie_Serie) {
	std::size_t const ksize1 {3u}, ksize2{5u};
	int value1 {42}, value2{17};
	auto ts1 = time_series::utils::generateSerie(value1, ksize1);
	auto ts2 = time_series::utils::generateSerie(value2, ksize2);

	std::stringstream ss;

	for (auto const& [first, second, third] : itertools::zip(ts1, ts2, ts1)) {
		ss << first.value << ' ' << second.value << ' ' << third.value << '\n';
	}
	std::string check {R"(42 17 42
43 18 43
44 19 44
)"};
	ASSERT_EQ(ss.str(), check);
}

TEST(SerieItertools, Serie_Vector_Map) {
	using namespace std::string_literals;
	std::vector<int> v{ 1,2,3,4,5,6,7 };
	std::map<int, std::string> m {
			{1, "one"s},
			{2, "two"s},
			{3, "three"s},
			{4, "four"s},
			{5, "five"s},
	};

	std::size_t const ksize {3u};
	int value {42};
	auto ts = time_series::utils::generateSerie(value, ksize);

	std::stringstream ss;

	for (auto const& [first, pair, elem] : itertools::zip(v, m, ts)) {
		ss << first << ' ' << pair.first << ' ' << pair.second << ' ' << elem.value << '\n';
	}
	std::string check {R"(1 1 one 42
2 2 two 43
3 3 three 44
)"};
	ASSERT_EQ(ss.str(), check);
}

TEST(SerieItertools, Serie) {
	std::size_t const ksize {2u};
	int value {42};
	auto ts = time_series::utils::generateSerie(value, ksize);
	std::stringstream ss;

	for (auto const& [first] : itertools::zip(ts)) {
		ss << first.value << '\n';
	}
	std::string check {R"(42
43
)"};
	ASSERT_EQ(ss.str(), check);
}

TEST(SerieItertools, TwoSeries_OneEmpty) {
	std::size_t const ksize1 {3u}, ksize2{0};
	int const value1 {42}, value2{17};
	auto ts1 = time_series::utils::generateSerie(value1, ksize1);
	ASSERT_FALSE(ts1.empty());
	auto ts2 = time_series::utils::generateSerie(value2, ksize2);
	ASSERT_TRUE(ts2.empty());

	std::stringstream ss;

	for (auto const& [first, second] : itertools::zip(ts1, ts2)) {
		ss << first << ' ' << second << '\n';
	}

	std::string check;
	ASSERT_EQ(ss.str(), check);
}

TEST(SerieItertools, NonContainers) {
	[[maybe_unused]] std::size_t const ksize {2u};
	[[maybe_unused]] int const kvalue {42};
	[[maybe_unused]] auto ts = time_series::utils::generateSerie(kvalue, ksize);

	struct NotOkContainer { int value {42}; };
	[[maybe_unused]] NotOkContainer not_ok;

//	auto z = iterators::zip(ts, not_ok); //doesn't compile
	//todo: add compile time test
}

