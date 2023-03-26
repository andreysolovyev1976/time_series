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
//	testing::GTEST_FLAG(filter) = "BasicsTypesRequirements.Concepts*";
	return RUN_ALL_TESTS();
}

//todo:
// test overflow
// test double < int , int < double, double < string, string < double
// test change of epsilon
// test arithmetics and comparison with other field, than default
// timestamp works poorly with high_res clock
// test pmr:: containers
// test
// 	operator value_type () const;
// 	operator elem_type () const;
// 	operator key_type () const;

//todo:
// decorate methods with ref decorations
// operator + is to be made in terms of operator +=
// operator += should be made inside of a class
// series - make joins for the series
// duration cast
// joins, make joins possible if any of arg is a tuple of args
// construct series from several series using one index
// add own container using polymorphic allocator (if small then stack else heap)
// add own unique_ptr / shrared_ptr on polymorphic allocator
// Fenvick tree is required instead of accumulate for own container
// offload to videocard
