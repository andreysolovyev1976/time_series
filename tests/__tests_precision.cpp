//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"
#include <string>
#include <sstream>


TYPED_TEST(Precision, Print) {
	TypeParam v1 (-1.0000000001, 10), v2(2.500000000012, 13), v3(-2.5, 4), v4(-2.126, 2);

	using namespace std::literals;
	std::stringstream output;

	output << v1;
	ASSERT_EQ (output.str(), "-1.0000000001"s);
	output.str(std::string());

	output << v2;
	ASSERT_EQ (output.str(), "2.5000000000120"s);
	output.str(std::string());

	output << v3;
	ASSERT_EQ (output.str(), "-2.5000"s);
	output.str(std::string());

	output << v4;
	ASSERT_EQ (output.str(), "-2.13"s);
	output.str(std::string());
}
