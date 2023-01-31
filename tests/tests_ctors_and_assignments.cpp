//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"
#include <utility>

template <typename T>
using IsOk = std::enable_if_t<
		std::conjunction_v<
				std::is_copy_assignable<T>,
				std::is_copy_constructible<T>
		>
		, bool>;

template <typename T>
using IsNotOk = std::enable_if_t<
		not std::conjunction_v<
				std::is_copy_assignable<T>,
				std::is_copy_constructible<T>
		>
		, bool>;

template <typename T, IsOk<T> = true>
constexpr bool typeIsOk_v () {return true;}
template <typename T, IsNotOk<T> = true>
constexpr bool typeIsOk_v () {return false;}


TYPED_TEST(Constructible, OkForVector) {
	ASSERT_TRUE(typeIsOk_v<TypeParam>());
}
TYPED_TEST(Ctors, Ints) {
	ASSERT_NO_THROW(TypeParam (42));
	ASSERT_NO_THROW(TypeParam (42u));
	ASSERT_NO_THROW(TypeParam (42l));
	ASSERT_NO_THROW(TypeParam (42ll));
	ASSERT_NO_THROW(TypeParam (42ul));
	ASSERT_NO_THROW(TypeParam (42ull));
//	ASSERT_NO_THROW(TypeParam (types::BigInt(42)));
//	ASSERT_NO_THROW(TypeParam (types::BigUInt(42)));
	ASSERT_NO_THROW(TypeParam (-1));
	ASSERT_NO_THROW(TypeParam (0));
	ASSERT_NO_THROW(TypeParam (1));
	ASSERT_NO_THROW(TypeParam (+1));
	ASSERT_NO_THROW(TypeParam (1+1));
	ASSERT_NO_THROW(TypeParam ( [](){return 42;}() ));
	ASSERT_NO_THROW(TypeParam (3'000'000'000));
	ASSERT_NO_THROW(TypeParam (3'000'000'000'000'000'000));
	int a {42};
	ASSERT_NO_THROW(TypeParam i (a));
}
TYPED_TEST(Ctors, Floats) {
	ASSERT_NO_THROW(TypeParam d1 (42.0, 2));
	ASSERT_NO_THROW(TypeParam d2 (42.));
//	ASSERT_NO_THROW(TypeParam F (types::Float (42.)));
	ASSERT_NO_THROW(TypeParam (-1.42));
	ASSERT_NO_THROW(TypeParam (0.0));
	ASSERT_NO_THROW(TypeParam (0.0005, 3));
	ASSERT_NO_THROW(TypeParam (0.0005, 4));
	ASSERT_NO_THROW(TypeParam (0.0005, 5));
	ASSERT_NO_THROW(TypeParam (1.42));
	ASSERT_NO_THROW(TypeParam (+1.42));
	ASSERT_NO_THROW(TypeParam (1.42+1.42));
	ASSERT_NO_THROW(TypeParam ( [](){return 42.42;}() ));
	double a {42.42};
	ASSERT_NO_THROW(TypeParam i (a));
}
TYPED_TEST(Ctors, Strings) {
	using namespace std::literals;
	ASSERT_NO_THROW(TypeParam s3 ("42.5"s));
	ASSERT_NO_THROW(TypeParam ("-1.42"s));
	ASSERT_NO_THROW(TypeParam ("0.0"s));
	ASSERT_NO_THROW(TypeParam ("1.42"s));
	ASSERT_NO_THROW(TypeParam ("+1.42"s));
}
TYPED_TEST(Ctors, CopyCtor) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 (i1));
	TypeParam i2 (i1);
	ASSERT_EQ(i1, i2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2 (d1));
	TypeParam d2 (d1);
	ASSERT_EQ(d1, d2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 (s1));
	TypeParam s2 (s1);
	ASSERT_EQ(s1, s2);
}
TYPED_TEST(Ctors, MoveCtor) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 (std::move(i1)));
	TypeParam i2 (std::move(i1));
	ASSERT_EQ(i2, TypeParam (42));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2 (std::move(d1)));
	TypeParam d2 (std::move(d1));
	ASSERT_EQ(d2, TypeParam (42.42, 5));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 (std::move(s1)));
	TypeParam s2 (std::move(s1));
	ASSERT_EQ(s2, TypeParam ("42"s));
}
TYPED_TEST(Ctors, Throws) {
	using namespace std::literals;
	ASSERT_ANY_THROW(TypeParam s4 (42.42, -1));
	ASSERT_ANY_THROW(TypeParam s4 (42.42, 51));

	struct S {};
	[[maybe_unused]] S s;
//	ASSERT_ANY_THROW(isConstructable<TypeParam, S>(s));

	ASSERT_ANY_THROW(TypeParam s4 ("42,5"s));
	ASSERT_ANY_THROW(TypeParam s1 ("fuck"s));
}

TYPED_TEST(Assignments, Copy) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 = i1);
	TypeParam i2 = i1;
	ASSERT_EQ(i1, i2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2 = d1);
	TypeParam d2 = d1;
	ASSERT_EQ(d1, d2);
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 = s1);
	TypeParam s2 = s1;
	ASSERT_EQ(s1, s2);
}
TYPED_TEST(Assignments, Move) {
	using namespace std::literals;
	TypeParam i1 (42);
	TypeParam d1 (42.42, 5);
	TypeParam s1 ("42"s);

	ASSERT_NO_THROW([[maybe_unused]] TypeParam i2 = std::move(i1));
	TypeParam i2 = std::move(i1);
	ASSERT_EQ(i2, TypeParam (42));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam d2  = std::move(d1));
	TypeParam d2 = std::move(d1);
	ASSERT_EQ(d2, TypeParam (42.42, 5));
	ASSERT_NO_THROW([[maybe_unused]] TypeParam s2 = std::move(s1));
	TypeParam s2 = std::move(s1);
	ASSERT_EQ(s2, TypeParam ("42"s));
}

