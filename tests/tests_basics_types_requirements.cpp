//
// Created by Andrey Solovyev on 16/02/2023.
//

#include <gtest/gtest.h>

#include "time_series/types_requirements/ctor_input.h"
#include "time_series/types_requirements/series_container.h"
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <string>
#include <variant>


struct S {
	int value {42};
	operator double () {return static_cast<double>(value);}
};

TEST(BasicsTypesRequirements, CtorInputOk) {
	bool ok = requirements::isConveribleOrConstructible<double, int>();
	ASSERT_TRUE(ok);
	ok = requirements::isConveribleOrConstructible<int, double>();
	ASSERT_TRUE(ok);
	ok = requirements::isConveribleOrConstructible<int, bool>();
	ASSERT_TRUE(ok);
	ok = requirements::isConveribleOrConstructible<S, double>();
	ASSERT_TRUE(ok);
}

TEST(BasicsTypesRequirements, CtorInputNotOk) {
//	bool ok = requirements::isConveribleOrConstructible<std::string, int>();
//	ok = requirements::isConveribleOrConstructible<double, S>();
//	ok = requirements::isConveribleOrConstructible<bool, std::string>();
	ASSERT_FALSE(true);
}


TEST(BasicsTypesRequirements, ContainerOk) {
	std::map<int, int> m;
	ASSERT_TRUE(requirements::IsContainer_v<std::vector<int>>());
	ASSERT_TRUE(requirements::IsContainer_v<decltype(m)>());
	// compile-time fail
//	ASSERT_TRUE(requirements::IsContainer_v<std::map<int, int>>());
	ASSERT_TRUE(requirements::IsContainer_v<std::deque<int>>());
	ASSERT_TRUE(requirements::IsContainer_v<std::set<int>>());
	ASSERT_TRUE(requirements::IsContainer_v<std::string>());
}

TEST(BasicsTypesRequirements, ContainerNotOk) {
	ASSERT_FALSE(requirements::IsContainer_v<double>());
	ASSERT_FALSE(requirements::IsContainer_v<int>());
	ASSERT_FALSE(requirements::IsContainer_v<S>());
	std::variant<int, double> v;
	ASSERT_FALSE(requirements::IsContainer_v<decltype(v)>());
}

