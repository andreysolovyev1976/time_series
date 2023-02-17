//
// Created by Andrey Solovyev on 16/02/2023.
//

#include <gtest/gtest.h>

#include "time_series/types_requirements/ctor_input.h"
#include "time_series/types_requirements/container.h"
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <string>
#include <variant>


template <typename T>
struct S {
	T value {42};
	operator double () {return static_cast<double>(value);}
};

TEST(BasicsTypesRequirements, CtorInputOk) {
	bool ok = requirements::isConveribleOrConstructible<double, int>();
	ASSERT_TRUE(ok);
	ok = requirements::isConveribleOrConstructible<int, double>();
	ASSERT_TRUE(ok);
	ok = requirements::isConveribleOrConstructible<int, bool>();
	ASSERT_TRUE(ok);
	ok = requirements::isConveribleOrConstructible<S<int>, double>();
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
	ASSERT_TRUE(requirements::IsContainer_v<std::vector>());
	ASSERT_TRUE(requirements::IsContainer_v<std::map>());
	ASSERT_TRUE(requirements::IsContainer_v<std::deque>());
	ASSERT_TRUE(requirements::IsContainer_v<std::set>());
//	ASSERT_TRUE(requirements::IsContainer_v<std::string>());
}

TEST(BasicsTypesRequirements, ContainerNotOk) {
//	ASSERT_FALSE(requirements::IsContainer_v<double>());
//	ASSERT_FALSE(requirements::IsContainer_v<int>());
	ASSERT_FALSE(requirements::IsContainer_v<S>());
	ASSERT_FALSE(requirements::IsContainer_v<std::variant>());
	ASSERT_FALSE(true);
}

