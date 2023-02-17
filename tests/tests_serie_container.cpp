//
// Created by Andrey Solovyev on 16/02/2023.
//

#include <gtest/gtest.h>
#include "time_series/serie.h"
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <list>
#include <array>


TEST(SerieContainer, Vector) {
	time_series::Serie<int, base::Seconds, std::vector> serie;
	serie.push_back({12});
	ASSERT_EQ(serie.front().value, 12.0);
}

TEST(SerieContainer, Deque) {
	time_series::Serie<int, base::Seconds, std::deque> serie;
	serie.push_back({12});
	ASSERT_EQ(serie.front().value, 12.0);
}


TEST(SerieContainer, Map) {
	time_series::Serie<int, base::Seconds, std::map> serie;
	serie.insert({base::Timestamp<base::Seconds>(), 12});
	ASSERT_EQ(serie.begin()->second, 12.0);
}

TEST(SerieContainer, UnorderedSet) {
	time_series::Serie<int, base::Seconds, std::deque> serie;
	serie.push_back({12});
	ASSERT_EQ(serie.front().value, 12.0);
}
