//
// Created by Andrey Solovyev on 24/01/2023.
//

#include <gtest/gtest.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
	testing::InitGoogleTest(&argc, argv);
	testing::GTEST_FLAG(color) = "yes";
//	testing::GTEST_FLAG(filter) = "Ctors*:Assignments*:Constructible*:Compare*:Arithmetics*";
//	testing::GTEST_FLAG(filter) = "ElementFnApplication*";
//	testing::GTEST_FLAG(filter) = "ElementFnApplication/1.*";
	testing::GTEST_FLAG(filter) = "BasicsTypesRequirements*";
	return RUN_ALL_TESTS();
}

//todo:
// test overflow
// test double < int , int < double, double < string, string < double
// test change of epsilon
// test arithmetics and comparison with other field, than default
