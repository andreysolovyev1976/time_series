//
// Created by Andrey Solovyev on 03/03/2023.
//


/*
	27
 3	12
 2	16
 3	20
 17
 25


 Full Outer Join
 0	27
 3	12
 2	16
 3	20
 17	0
 25	0
 Inner Join
 3	12
 2	16
 3	20
 Outer Excluding Join
 0	27
 17	0
 25	0

 Left Outer join
 3 	12
 2	16
 3	20
 17	0
 25	0
 Left Excluding Join
 17	0
 25	0
Right Outer join
 0	27
 3 	12
 2	16
 3	20
 Right Excluding Join
 0	27

 */


#include <gtest/gtest.h>
#include "common_usage_library/joins.hpp"

#include <iosfwd>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <tuple>

struct Key {
	int value {0};
	Key (int i) : value(i) {}
	Key () = default;
	operator int () {return value;}
};

bool operator < (const Key& lhs, const Key& rhs) { return lhs.value < rhs.value; }
bool operator == (const Key& lhs, const Key& rhs) { return lhs.value == rhs.value; }
bool operator < (const Key& lhs, const int rhs) { return lhs.value < rhs; }
bool operator == (const Key& lhs, const int rhs) { return lhs.value == rhs; }
bool operator < (const int lhs, const Key& rhs) { return lhs < rhs.value; }
bool operator == (const int lhs, const Key& rhs) { return lhs == rhs.value; }
bool operator < (std::pair<const Key, int> const& lhs, std::pair<const Key, int> const& rhs) { return lhs.first.value < rhs.first.value; }
bool operator == (std::pair<const Key, int> const& lhs, std::pair<const Key, int> const& rhs) { return lhs.first.value == rhs.first.value; }
bool operator < (std::pair<const Key, int> const& lhs, const int rhs) { return lhs.first.value < rhs; }
bool operator == (std::pair<const Key, int> const& lhs, const int rhs) { return lhs.first.value == rhs; }
bool operator < (const int lhs, std::pair<const Key, int> const& rhs) { return lhs < rhs.first.value; }
bool operator == (const int lhs, std::pair<const Key, int> const& rhs) { return lhs == rhs.first.value; }

std::ostream& operator << (std::ostream& os, std::pair<Key const, int> const& p) {
	os << p.second;
	return os;
}
std::ostream& operator << (std::ostream& os, std::vector<int> const& v) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& elem: v) os << elem << ' ';
	os << '}';
	return os;
}
std::ostream& operator << (std::ostream& os, std::list<int> const& l) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& elem: l) os << elem << ' ';
	os << '}';
	return os;
}
std::ostream& operator << (std::ostream& os, std::vector<Key> const& v) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& elem: v) os << elem.value << ' ';
	os << '}';
	return os;
}
std::ostream& operator << (std::ostream& os, std::map<Key, int> const& m) {
	using namespace std::string_literals;
	os << "{ "s;
	for (auto const& [_, value]: m) os << value << ' ';
	os << '}';
	return os;
}

// https://en.cppreference.com/w/cpp/utility/apply
template<typename... Ts>
std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& theTuple){
	std::apply
			(
					[&os](Ts const&... tupleArgs)
					{
					  os << '[';
					  std::size_t n{0};
					  ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
					  os << ']';
					}, theTuple
			);
	return os;
}

std::map<Key, int> const m1 {
		{{1}, 3},
		{{2}, 2},
		{{3}, 3},
		{{4}, 17},
		{{5}, 25},
};
std::map<Key, int> const m2 {
		{{0}, 27},
		{{1}, 12},
		{{2}, 16},
		{{3}, 20},
};

std::map<Key, int> result;
auto res = std::inserter(result, result.end());


//todo: test pointer, refs and values
//todo: test it moves while joining
//todo: test reverse tuple for move semantics
//todo: test actual tuples - several values
//todo: test complicate inputs
//todo: add containers limitations



TEST(BasicsJoins, InnerTwoContainers) {
	auto result_other = culib::join::inner(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 3 2 3 }, { 12 16 20 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, OuterFullTwoContainers) {
	auto result_other = culib::join::outerFull(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 3 2 3 17 25 }, { 27 12 16 20 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, OuterExcludingTwoContainers) {
	auto result_other = culib::join::outerExcluding(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 17 25 }, { 27 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, LeftOuterTwoContainers) {
	auto result_other = culib::join::leftOuter(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 3 2 3 17 25 }, { 12 16 20 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, LeftExcludingTwoContainers) {
	auto result_other = culib::join::leftExcluding(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 17 25 }, { }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, RightOuterTwoContainers) {
	auto result_other = culib::join::rightOuter(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 3 2 3 }, { 27 12 16 20 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, RightExcludingTwoContainers) {
	auto result_other = culib::join::rightExcluding(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ }, { 27 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, InnerMultipleContainers) {
	std::vector<int>
			v1 {1, 2, 3},
			v2 {2, 3, 4, 17};

	std::list<int> l {2, 5, 17};

	auto result_other = culib::join::inner(v1, v2, l);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 2 }, { 2 17 }, { 2 17 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, OuterFullMultipleContainers) {
	std::vector<int>
			v1 {1, 2, 3},
			v2 {2, 3, 4, 17};

	std::list<int> l {2, 5, 17};

	auto result_other = culib::join::outerFull(v1, v2, l);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 1 2 3 }, { 2 3 4 17 }, { 2 5 17 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, OuterExcludingMultipleContainers) {
	std::vector<int>
			v1 {1, 2, 3},
			v2 {2, 3, 4, 17};

	std::list<int> l {2, 5, 17};

	auto result_other = culib::join::outerExcluding(v1, v2, l);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 1 3 }, { 4 }, { 5 }])"};
	ASSERT_EQ(check, ss.str());
}
