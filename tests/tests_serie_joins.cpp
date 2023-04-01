//
// Created by Andrey Solovyev on 01/03/2023.
//

#include <gtest/gtest.h>
#include "common_usage_library/itertools.hpp"
#include "time_series/serie_join.hpp"
#include "time_series/serie_random_generator.hpp"

#include <vector>
#include <deque>
#include <map>
#include <string>
#include <sstream>

/*
		  typename Duration = culib::time::Microseconds
		  , typename ElemType = time_series::Value<int>
		  , template <typename...> typename Container = std::vector

 */
TEST(SerieJoins, InnerOk) {
	std::size_t const ksize {5u};
	int value {42};
	auto ts1 = time_series::utils::generateSerie<culib::time::Microseconds, time_series::Value<int>, std::deque>(value, ksize);
	auto ts2 = ts1;
	ts2.pop_front();
	ts2.push_back({50});
#if 0
	auto inner_join = time_series::join::inner(ts1, ts2);


	for (auto const& [first, second] : itertools::zip(s, ts)) {
		ss << first << ' ' << second.value << '\n';
	}
	std::string check {R"(a 42
b 42
c 42
)"};
	ASSERT_EQ(ss.str(), check);
#endif
}

TEST(SerieJoins, InnerEmptyResult) {
}

TEST(SerieJoins, InnerOneContainer) {
}

TEST(SerieJoins, InnerTwoContainers_OneEmpty) {
}
