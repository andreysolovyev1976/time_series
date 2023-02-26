//
// Created by Andrey Solovyev on 23/02/2023.
//

#include <gtest/gtest.h>
#include "utils/itertools.hpp"
#include "time_series/serie.hpp"

#include <sstream>

TEST(BasicsItertools, VectorAndVector) {
	std::vector<int> arr{ 1,2,3,4,5 };
	std::string s { "abcdefghhlk" };

	std::stringstream ss;

	for (auto const& elem : iterators::zip(s, arr)) {
		ss << elem.first << ' ' << elem.second << '\n';
	}
	std::string check {R"(a 1
b 2
c 3
d 4
e 5
)"};
	ASSERT_EQ(ss.str(), check);
}

TEST(BasicsItertools, VectorAndMap) {
	using namespace std::string_literals;
	std::vector<int> arr{ 1,2,3,4,5 };
	std::map<int, std::string> m {
		{1, "one"s},
		{2, "two"s},
		{3, "three"s},
		{4, "four"s},
		{5, "five"s},
	};

	std::stringstream ss;

	for (auto const& elem : iterators::zip(arr, m)) {
		ss << elem.first << ' ' << elem.second.first << ' ' << elem.second.second << '\n';
	}
	std::string check {R"(1 1 one
2 2 two
3 3 three
4 4 four
5 5 five
)"};
	ASSERT_EQ(ss.str(), check);
}

TEST(BasicsItertools, TimeSeries) {
	std::vector<int> arr{ 1,2,3,4,5 };
	std::string s { "abcdefghhlk" };

	std::stringstream ss;

	for (auto const& elem : iterators::zip(arr, s)) {
		ss << elem.second << ' ' << elem.first << '\n';
	}
	std::string check {R"(a 1
b 2
c 3
d 4
e 5
)"};

	ASSERT_EQ(ss.str(), check);
}
