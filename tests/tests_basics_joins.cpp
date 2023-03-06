//
// Created by Andrey Solovyev on 03/03/2023.
//

#include <gtest/gtest.h>
#include "utils/utils.hpp"
#include "utils/joins.hpp"

#include <iosfwd>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <tuple>

struct Key {
	int value {-1};
	Key (int i) : value(i) {}
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


TEST(BasicsJoins, InitValues) {
	[[maybe_unused]] auto t = std::make_tuple(m1, m2);
	[[maybe_unused]] std::string const check {R"([ , 27]
[3, 12]
[2, 16]
[3, 20]
[17,  ]
[25,  ]
)"};
//	std::cout << check << '\n';
//	std::cout << t << '\n';
}

TEST(BasicsJoins, Inner) {
	auto result_other = time_series::join::inner(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 3 2 3 }, { 12 16 20 }])"};
	ASSERT_EQ(check, ss.str());
}


TEST(BasicsJoins, OuterFull) {
	auto result_other = time_series::join::outerFull(m1, m2);
	std::stringstream ss;
	ss << result_other;
	std::string const check {R"([{ 27 3 2 3 17 25 }, { 27 12 16 20 }])"};
	ASSERT_EQ(check, ss.str());
}


TEST(BasicsJoins, OuterExcluding) {
	auto result_other = time_series::join::outerExcluding(m1, m2);
	std::stringstream ss;
	ss << result_other << '\n';
	std::string const check {R"([{ 0 17 25 }, { 27 0 0 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, LeftOuter) {
	auto result_other = time_series::join::leftOuter(m1, m2);
	std::stringstream ss;
	ss << result_other << '\n';
	std::string const check {R"([{ 3 2 3 17 25 }, { 12 16 20 0 0 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, LeftExcluding) {
	auto result_other = time_series::join::leftExcluding(m1, m2);
	std::stringstream ss;
	ss << result_other << '\n';
	std::string const check {R"([{ 17 25 }, { 0 0 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, RightOuter) {
	auto result_other = time_series::join::rightOuter(m1, m2);
	std::stringstream ss;
	ss << result_other << '\n';
	std::string const check {R"([{ 0 3 2 3 }, { 27 12 16 20 }])"};
	ASSERT_EQ(check, ss.str());
}

TEST(BasicsJoins, RightExcluding) {
	auto result_other = time_series::join::rightExcluding(m1, m2);
	std::stringstream ss;
	ss << result_other << '\n';
	std::string const check {R"([{ 0 }, { 27 }])"};
	ASSERT_EQ(check, ss.str());
}
