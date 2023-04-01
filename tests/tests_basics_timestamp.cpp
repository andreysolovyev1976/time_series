//
// Created by Andrey Solovyev on 04/11/2021.
//

#include <gtest/gtest.h>
#include "common_usage_library/timestamp.hpp"

#include <unordered_map>
#include <sstream>
#include <thread>


using namespace std::chrono_literals;
using Duration = culib::time::Microseconds;
using TS = culib::time::Timestamp<Duration>;

TEST(BasicsTimestamp, CtorsFloorIsOk) {}

TEST(BasicsTimestamp, AdditionDurationAssign) {
	TS t1;
	auto ts_copy = t1.time_point.time_since_epoch().count();
	t1 += 1us;
	auto new_ts = t1.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_copy + 1, new_ts);
}
TEST(BasicsTimestamp, AdditionNumberAssign) {
	TS t1;
	auto ts_copy = t1.time_point.time_since_epoch().count();
	t1 += 1;
	auto new_ts = t1.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_copy + 1, new_ts);
}

TEST(BasicsTimestamp, AdditionDurationNewValue) {
	TS t1;
	auto t2 = t1 + 1us;

	auto ts_1 = t1.time_point.time_since_epoch().count();
	auto ts_2 = t2.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_1 + 1, ts_2);
}
TEST(BasicsTimestamp, AdditionNumberNewValue) {
	TS t1;
	auto t2 = t1 + 1;

	auto ts_1 = t1.time_point.time_since_epoch().count();
	auto ts_2 = t2.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_1 + 1, ts_2);
}

TEST(BasicsTimestamp, SubtractionDurationAssign) {
	TS t1;
	auto ts_copy = t1.time_point.time_since_epoch().count();
	t1 -= 1us;
	auto new_ts = t1.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_copy - 1, new_ts);
}
TEST(BasicsTimestamp, SubtractionNumberAssign) {
	TS t1;
	auto ts_copy = t1.time_point.time_since_epoch().count();
	t1 -= 1;
	auto new_ts = t1.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_copy - 1, new_ts);
}

TEST(BasicsTimestamp, SubtractionDurationNewValue) {
	TS t1;
	auto t2 = t1 - 1us;

	auto ts_1 = t1.time_point.time_since_epoch().count();
	auto ts_2 = t2.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_1 - 1, ts_2);
}
TEST(BasicsTimestamp, SubtractionNumberNewValue) {
	TS t1;
	auto t2 = t1 - 1;

	auto ts_1 = t1.time_point.time_since_epoch().count();
	auto ts_2 = t2.time_point.time_since_epoch().count();
	ASSERT_EQ(ts_1 - 1, ts_2);
}

TEST(BasicsTimestamp, Comparison) {
	auto first = TS{};
	auto first_copy = first;
	auto second = first + 1s;

	ASSERT_EQ(first, first_copy);
	ASSERT_NE(first, second);
	ASSERT_LT(first, second);
	ASSERT_LE(first, second);
	ASSERT_LE(first, first_copy);
	ASSERT_GT(second, first);
	ASSERT_GE(second, first);
	ASSERT_GE(first_copy, first);
}
TEST(BasicsTimestamp, Hasher) {
	std::unordered_map<TS, int, culib::time::TimestampHasher<Duration>> um;
	auto first = TS{};
	std::this_thread::sleep_for(10ms);
	auto second = TS{};
	um[first] = 1;
	um[second] = 2;

	ASSERT_EQ(um[first], 1);
	ASSERT_EQ(um[second], 2);
}



TEST(BasicsTimestamp, Timer) {
	using namespace culib::time;
	auto const stop = Timestamp<Milliseconds>{} + 100ms;
	auto time_left = (stop - Timestamp<Milliseconds>{}).count();

	while (time_left > 0) {
		std::this_thread::sleep_for(1ms);
		time_left = (stop - Timestamp<Milliseconds>{}).count();
	}

	ASSERT_TRUE(time_left <= 0);
}
