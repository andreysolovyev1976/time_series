//
// Created by Andrey Solovyev on 23/02/2023.
//

#include <gtest/gtest.h>
#include "utils/itertools.hpp"
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <type_traits>

TEST(BasicsItertools, ZipIter_Vector_String) {
	std::vector<int> v{ 1,2,3,4,5 };
	std::string s { "abcdefghhlk" };

	std::stringstream ss;

	for (auto const& [first, second] : itertools::zip(s, v)) {
		ss << first << ' ' << second << '\n';
	}
	std::string check {R"(a 1
b 2
c 3
d 4
e 5
)"};
	ASSERT_EQ(ss.str(), check);
}
TEST(BasicsItertools, ZipIter_Vector_Map) {
	using namespace std::string_literals;
	std::vector<int> v{ 1,2,3,4,5 };
	std::map<int, std::string> m {
		{1, "one"s},
		{2, "two"s},
		{3, "three"s},
		{4, "four"s},
		{5, "five"s},
	};

	std::stringstream ss;

	for (auto const& [first, pair] : itertools::zip(v, m)) {
		ss << first << ' ' << pair.first << ' ' << pair.second << '\n';
	}
	std::string check {R"(1 1 one
2 2 two
3 3 three
4 4 four
5 5 five
)"};
	ASSERT_EQ(ss.str(), check);
}
TEST(BasicsItertools, ZipIter_Vector_Map_String) {
	using namespace std::string_literals;
	std::vector<int> v{ 1,2,3,4,5 };
	std::map<int, std::string> m {
			{1, "one"s},
			{2, "two"s},
			{3, "three"s},
			{4, "four"s},
			{5, "five"s},
	};
	std::string s { "abcdefghhlk" };

	std::stringstream ss;

	for (auto const& [first, pair, ch] : itertools::zip(v, m, s)) {
		ss << first << ' ' << pair.first << ' ' << pair.second << ' ' << ch << '\n';
	}
	std::string check {R"(1 1 one a
2 2 two b
3 3 three c
4 4 four d
5 5 five e
)"};
	ASSERT_EQ(ss.str(), check);
}
TEST(BasicsItertools, ZipIter_OneContainer) {
	std::vector<int> v{ 1,2,3,4,5 };
	std::stringstream ss;

	for (auto const& [first] : itertools::zip(v)) {
		ss << first << '\n';
	}
	std::string check {R"(1
2
3
4
5
)"};
	ASSERT_EQ(ss.str(), check);
}
TEST(BasicsItertools, ZipIter_TwoContainers_OneEmpty) {
	std::vector<int> v{ 1,2,3,4,5 };
	std::string s;
	ASSERT_TRUE(s.empty());

	std::stringstream ss;

	for (auto const& [first, ch] : itertools::zip(v, s)) {
		ss << first << ' ' << ch << '\n';
	}

	std::string check;
	ASSERT_EQ(ss.str(), check);
}
TEST(BasicsItertools, ZipIter_NonContainers) {

	struct NotOkContainer { int value {42}; };
	[[maybe_unused]] NotOkContainer not_ok;
	[[maybe_unused]] std::vector<int> v{ 1,2,3,4,5 };

//	auto z = iterators::zip(arr, not_ok); //doesn't compile
	//todo: add compile time test
}

TEST(BasicsItertools, GetIterators_XValue) {
	std::vector<int> vec {5, 2};
	auto p = itertools::getIterators<decltype(vec), true>(vec);
	bool constexpr is_same = std::is_same_v<decltype(vec.begin()), decltype(p.begin_)>;
	ASSERT_TRUE(is_same);
}
TEST(BasicsItertools, GetIterators_LValue) {
	std::vector<int> const vec_const {5, 2};
	auto p = itertools::getIterators<decltype(vec_const), true>(vec_const);
	bool constexpr is_same = std::is_same_v<decltype(vec_const.begin()), decltype(p.begin_)>;
	ASSERT_TRUE(is_same);
}
TEST(BasicsItertools, GetIterators_XValueRef) {
	std::vector<int> vec {5, 2};
	std::vector<int> & vec_lref (vec);
	auto p = itertools::getIterators<decltype(vec_lref), true>(vec_lref);
	bool constexpr is_same = std::is_same_v<decltype(vec_lref.begin()), decltype(p.begin_)>;
	ASSERT_TRUE(is_same);
}
TEST(BasicsItertools, GetIterators_XValueRef_Const) {
	std::vector<int> vec {5, 2};
	std::vector<int> const& vec_const_ref (vec);
	auto p = itertools::getIterators<decltype(vec_const_ref), true>(vec_const_ref);
	bool constexpr is_same = std::is_same_v<decltype(vec_const_ref.begin()), decltype(p.begin_)>;
	ASSERT_TRUE(is_same);
}
TEST(BasicsItertools, GetIterators_RValue) {
	std::vector<int> && vec_rref {5, 2};
	auto p = itertools::getIterators<decltype(vec_rref), true>(vec_rref);
	bool constexpr is_same = std::is_same_v<decltype(vec_rref.begin()), decltype(p.begin_)>;
	ASSERT_TRUE(is_same);
}
