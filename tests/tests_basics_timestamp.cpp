//
// Created by Andrey Solovyev on 04/11/2021.
//

#include <gtest/gtest.h>
#include "time_series/timestamp.h"
#include <sstream>
#include <thread>

using namespace base;
using namespace std::chrono_literals;

TEST(BasicsTimestamp, Timer) {

	auto stop = std::chrono::system_clock::now() + 7s;
	auto time_left = std::chrono::duration_cast<Seconds>(stop - std::chrono::system_clock::now()).count();

	while (time_left > 0) {
		std::this_thread::sleep_for(3s);
		time_left = std::chrono::duration_cast<Seconds>(stop - std::chrono::system_clock::now()).count();
		std::cerr << time_left << '\n';
	}
}

TEST(BasicsTimestamp, ComparisonOk) {
}
TEST(BasicsTimestamp, ComparisonNotOk) {
}
TEST(BasicsTimestamp, Hasher) {
}
TEST(BasicsTimestamp, Add) {
	Timestamp<Seconds> t1;
	std::this_thread::sleep_for(2s);
	Timestamp<Seconds> t2;
	std::cerr << (t2 + t1) << '\n';
}
TEST(BasicsTimestamp, Subtract) {
	Timestamp<Seconds> t1;
	std::this_thread::sleep_for(2s);
	Timestamp<Seconds> t2;
	std::cerr << (t2 - t1) << '\n';
}
TEST(BasicsTimestamp, Multiply) {
}
TEST(BasicsTimestamp, Divide) {
}



TEST(BasicsTimestamp, Print) {

	Timestamp<Seconds> t1;
	std::this_thread::sleep_for(2s);
	Timestamp<Seconds> t2;

	std::stringstream ss;
	ss << t1 << ' ' << t2;
	ASSERT_EQ(ss.str(), "");

}
