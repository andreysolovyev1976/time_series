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
	testing::GTEST_FLAG(filter) = "EndToEnd.*";
	return RUN_ALL_TESTS();
}

//todo:
// test overflow
// test double < int , int < double, double < string, string < double
// test change of epsilon
// test arithmetics and comparison with other field, than default
// test pmr:: containers
// test
// 	operator value_type () const;
// 	operator elem_type () const;
// 	operator key_type () const;
// test structured bindings for Element, OHLCV, BidAsk for rvalue- lvalue refs
// test operator >> for timestamp
// test Element for user defined classes
// DONE timestamp works poorly with high_res clock
// DONE fix timestamp
// DONE test structured bindings access for Element, OHLCV, BidAsk




//todo:
// use flat_map as a container, that combines vectors of Open High Low Close
// decorate methods with ref decorations
// operator + is to be made in terms of operator +=
// operator += should be made inside of a class
// noexcept everywhere it is possible
// series - make joins for the series
// joins, make joins possible if any of arg is a tuple of args
// joins, make them for pair of iterators
// joins, make them for range
// construct series from several series using one index
// add own container using polymorphic allocator (if small then stack else heap), here first is stack then it required to create another monotonoc buffer fro heap, as chosen list (due to multithreading) requires to have cash-friendly environment. SO there are required two memeory resources for one container
// add own unique_ptr / shrared_ptr on polymorphic allocator
// Fenvick tree is required instead of accumulate for own container
// offload to videocard
// get pre-compiled headers by introducing CMakeLists.txt in the library_time_series folder
// is it a floor or a ceiling for a timepoint?
// DONE check that const version of getImpl is required for structural bindings
// DONE duration cast

