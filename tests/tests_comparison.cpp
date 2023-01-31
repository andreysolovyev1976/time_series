//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"



TYPED_TEST(Compare, Equal) {
	TypeParam v1(-1.5, 1), v2(2.5, 2);
	ASSERT_TRUE(v1 == v1);
	ASSERT_TRUE(v2 == v2);
	ASSERT_TRUE(v1 == TypeParam (-1.5));
	ASSERT_TRUE(v2 == TypeParam (2.5));
}
TYPED_TEST(Compare, NotEqual) {
	TypeParam v1(-1.5, 1), v2(2.5, 2);
	ASSERT_TRUE(v2 != v1);
	ASSERT_TRUE(v1 != v2);
	ASSERT_TRUE(v1 != 42);
	ASSERT_TRUE(v2 != -42);
	ASSERT_TRUE(v1 != TypeParam (42.42));
	ASSERT_TRUE(v2 != TypeParam (-42.42));
}
TYPED_TEST(Compare, Less) {
	TypeParam v1(-1.5, 1), v2(2.5, 2);
	ASSERT_TRUE(v1 < v2);
	ASSERT_TRUE(!(v2 < v1));
	ASSERT_TRUE(v1 < 42);
	ASSERT_TRUE(-42 < v2);
	ASSERT_TRUE(v1 < TypeParam (42.42));
	ASSERT_TRUE(TypeParam (-42.42) < v2);
}
TYPED_TEST(Compare, Greater) {
	TypeParam v1(-1.5, 1), v2(2.5, 2);
	ASSERT_TRUE(v2 > v1);
	ASSERT_TRUE(!(v1 > v2));
	ASSERT_TRUE(!(v1 > 42));
	ASSERT_TRUE(v2 > -42);
	ASSERT_TRUE(!(v1 > TypeParam (42.42)));
	ASSERT_TRUE(v2 > TypeParam (-42.42));
}
TYPED_TEST(Compare, LessOrEqual) {
	TypeParam v1(-1.5, 1), v2(2.5, 2);
	ASSERT_TRUE(v1 <= v1);
	ASSERT_TRUE(v1 <= v2);
	ASSERT_TRUE(v1 <= 42);
	ASSERT_TRUE(v1 <= TypeParam (-1.5));
	ASSERT_TRUE(!(v1 <= -42));
	ASSERT_TRUE(TypeParam (2.5) <= v2);
	ASSERT_TRUE(TypeParam (-42.42) <= v2);
	ASSERT_TRUE(!(v2 <= -42));
}
TYPED_TEST(Compare, GreaterOrEqual) {
	TypeParam v1(-1.5, 1), v2(2.5, 2);
	ASSERT_TRUE(v2 >= v2);
	ASSERT_TRUE(v2 >= v1);
	ASSERT_TRUE(42 >= v1 );
	ASSERT_TRUE(v1 >= TypeParam (-1.5));
	ASSERT_TRUE(!(v1 >= 42));
	ASSERT_TRUE(TypeParam (2.5) >= v2);
	ASSERT_TRUE(v2 >= TypeParam (-42.42));
	ASSERT_TRUE(!(v2 >= 42));
}
