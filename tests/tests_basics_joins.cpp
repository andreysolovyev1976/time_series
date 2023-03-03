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
	os << p.first.value << ' ' << p.second;
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
	result.clear();
	std::set_intersection(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::inner(result, m1, m2);
	std::cout << result << '\n';
	std::string const check {R"( 27
3 12
2 16
3 20
17
25
)"};

}


TEST(BasicsJoins, Inner) {
	result.clear();
	std::set_intersection(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::inner(result, m1, m2);
	std::cout << result << '\n';
	std::string const check {R"(3 12
2 16
3 20
)"};

}

TEST(BasicsJoins, OuterFull) {
	result.clear();
	std::set_union(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::outerFull(result, m1, m2);
	std::cout << result << '\n';
	std::string const check {R"(3 12
2 16
3 20
17 0
25 0
)"};

}

TEST(BasicsJoins, OuterExcluding) {
	result.clear();
	std::set_symmetric_difference(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::outerExcluding(result, m1, m2);
	std::cout << result << '\n';
	std::string const check {R"(0 27
17 0
25 0
)"};

}


TEST(BasicsJoins, LeftOuter) {
	result.clear();
	std::set_symmetric_difference(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::leftOuter(result, m1, m2);
	std::cout << result << '\n';

	std::string const check {R"(3 12
2 16
3 20
17 0
25 0
)"};

}

TEST(BasicsJoins, LeftExcluding) {
	std::set_difference(m1.begin(), m1.end(), m2.begin(), m2.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::leftExcluding(result, m1, m2);
	std::cout << result << '\n';

	std::string const check {R"(17 0
25 0
)"};

}

TEST(BasicsJoins, RightOuter) {
	result.clear();
	std::set_symmetric_difference(m2.begin(), m2.end(), m1.begin(), m1.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::rightOuter(result, m1, m2);
	std::cout << result << '\n';
	std::string const check {R"(0 27
3 12
2 16
3 20
)"};
}

TEST(BasicsJoins, RightExcluding) {
	result.clear();
	std::set_difference(m2.begin(), m2.end(), m1.begin(), m1.end(), res);
	std::cout << result << '\n';
	result.clear();
	base::join::rightExcluding(result, m1, m2);
	std::cout << result << '\n';
	std::string const check {R"(0 27
)"};
}
