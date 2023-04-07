//
// Created by Andrey Solovyev on 05/04/2023.
//

#include <gtest/gtest.h>


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <list>
#include <vector>
#include <string>
#include <string_view>
#include <iterator>
#include <filesystem>

#include "culib.h"
#include "tslib.h"
#include "finlib.h"

#include "tests_end_to_end_sv_file_reader.hpp"

using namespace culib::utils;
using namespace culib::time;
using namespace time_series;
using namespace time_series::financial;
using namespace time_series::requirements;
using namespace csv_reader;

namespace const_values {
  static const std::size_t EXPECTED_LINES_COUNT {330'000u};
}

template <typename ReturnType>
ReturnType readFromFile (std::filesystem::path path, char const sep) {
	using ohlcv_t = OHLCV<double>;
	using duration_t = Minutes;
	using elem_t = Element<duration_t, ohlcv_t>;

	auto raw_data = readFile(path);
	auto lines_of_data = splitStringBy(raw_data, '\n', ::const_values::EXPECTED_LINES_COUNT);

	ReturnType WTI;

	if (lines_of_data.empty()) return WTI;
	auto line_iter = lines_of_data.begin();

	if (headersArePresented(line_iter)) {
		if constexpr (is_serie_synthetic_v<ReturnType>) {
			WTI.setField(&ReturnType::headers, makeHeaders(line_iter, sep));
		}
		line_iter = std::next(line_iter);
	}

	for (auto itb = line_iter, ite = lines_of_data.end(); itb!=ite; ++itb) {
		auto [ts, values] = parseData<duration_t, double>(itb, sep);
		if constexpr (is_serie_synthetic_v<ReturnType>) {
			WTI.emplaceBack(ts, std::move(values));
		}
		else if constexpr (is_serie_regular_v<ReturnType>) {
			  elem_t elem;
			  elem.timestamp = ts;
			  elem.value.open = values[0];
			  elem.value.high = values[1];
			  elem.value.low = values[2];
			  elem.value.close = values[3];
			  elem.value.volume = values[4];
			  WTI.push_back(std::move(elem));
		}
	}

	return WTI;
}

void printSample () {
	std::string data_sample{
R"(Date,Open,High,Low,Close,Volume
9/23/2015 0:00,46.54,46.6,46.5,46.53,89
9/23/2015 0:01,46.53,46.54,46.51,46.52,14
9/23/2015 0:02,46.51,46.51,46.48,46.48,30
9/23/2015 0:03,46.48,46.48,46.46,46.47,37
9/23/2015 0:04,46.47,46.47,46.46,46.46,20
)"};
	std::cout << "original data sample:\n" << data_sample;
}

TEST(EndToEnd, SingleThread_Read_CSV_Synthetic) {

//	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016.csv"};
	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016 short.csv"};

	using ValueType = double;
	char const comma_sep {','};
	using SyntheticOHLCV = time_series::SerieSynthethic<Minutes, ValueType, 5u>;

	auto WTI = readFromFile<SyntheticOHLCV>(test_file, comma_sep);
	WTI.setField (&SyntheticOHLCV::name, "CL OHLCV minute data for Sep 2016");

	auto const& [ts, open, high, low, close, volume] = WTI.begin();
	ASSERT_EQ (open, 46.54);
	ASSERT_EQ (high, 46.6);
	ASSERT_EQ (low, 46.5);
	ASSERT_EQ (close, 46.53);
	ASSERT_EQ (volume, 89);

	auto column_close = WTI.getColumn ("Close");
	auto column_four = WTI.getColumn (3);

	ASSERT_TRUE (column_close.size() == 100u);
	ASSERT_TRUE (column_four.size() == 100u);
	ASSERT_EQ (column_close, column_four);
}

TEST(EndToEnd, SingleThread_Read_CSV_Natural) {

//	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016.csv"};
	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016 short.csv"};

	char const comma_sep {','};
	using NaturalOHLCV = Serie<Minutes, OHLCV<double>>;

	auto WTI = readFromFile<NaturalOHLCV>(test_file, comma_sep);

	auto const & [open, high, low, close, volume] = WTI.begin()->second();
	ASSERT_EQ (open, 46.54);
	ASSERT_EQ (high, 46.6);
	ASSERT_EQ (low, 46.5);
	ASSERT_EQ (close, 46.53);
	ASSERT_EQ (volume, 89);

	ASSERT_TRUE (WTI.size() == 100u);
}

TEST(EndToEnd, SingleThread_Upcast) {
	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016 short.csv"};

	char const comma_sep {','};
	using NaturalOHLCV = Serie<Minutes, OHLCV<double>>;
	auto WTI = readFromFile<NaturalOHLCV>(test_file, comma_sep);
	ASSERT_TRUE (WTI.size() == 100u);

	auto upcastedWTI = WTI.upcastTo<TwoMinutes>();
	auto new_duration = std::next(upcastedWTI.begin())->timestamp - upcastedWTI.begin()->timestamp;
	auto const one_period_of_two_minutes = TwoMinutes(1);
	ASSERT_EQ (new_duration, one_period_of_two_minutes);
	ASSERT_TRUE (WTI.size() >= 50u);

	auto it_curr = std::next(WTI.begin());
	auto it_prev = WTI.begin();
	int count{0};
	for (auto const& [ts, ohlcv] : upcastedWTI) {++count;
		auto const & [open_comb, high_comb, low_comb, close_comb, volume_comb] = ohlcv;
		auto const & [open_curr, high_curr, low_curr, close_curr, volume_curr] = it_curr->second();
		auto const & [open_prev, high_prev, low_prev, close_prev, volume_prev] = it_prev->second();
		if (it_prev->first() == it_curr->first()) {
//		ASSERT_EQ (ts, it_prev->first() + one_period_of_two_minutes);
			ASSERT_EQ (ts, it_prev->first());
			ASSERT_EQ (open_comb, open_prev);
			ASSERT_EQ (high_comb, std::max(high_curr, high_prev));
			ASSERT_EQ (low_comb, std::min(low_curr, low_prev));
			ASSERT_EQ (close_comb, close_curr);
			ASSERT_EQ (volume_comb, volume_curr+volume_prev) << ts << ' ' << ohlcv << ' ' << count << '\n';
		}
		it_curr = std::next(it_curr, 2);
		it_prev = std::next(it_prev, 2);
	}
}
TEST (EndToEnd, Playground) {
	using ElemType = OHLCV<double>;
	std::cout << std::boolalpha;
	std::cout << std::is_invocable_v<decltype(std::declval<ElemType>().collideWith), ElemType> << '\n';

}