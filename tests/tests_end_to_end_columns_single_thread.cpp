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

namespace const_values {
  static const std::size_t EXPECTED_LINES_COUNT {330'000u};
}

template <typename ReturnType>
ReturnType readFromFile (std::filesystem::path path, char const sep) {
	using namespace csv_reader;
	using namespace culib::time;
	using namespace culib::utils;
	using namespace time_series::requirements;
	using namespace time_series::financial;
	using namespace time_series;

	using ohlcv_t = OHLCV<double>;
	using duration_t = Minutes;
	using elem_t = Element<duration_t, ohlcv_t>;


	auto raw_data = readFile(path);
	auto lines_of_data = splitStringBy(raw_data, '\n', ::const_values::EXPECTED_LINES_COUNT);

	ReturnType WTI;

	if (lines_of_data.empty()) return WTI;
	auto line_iter = lines_of_data.begin();

	if constexpr (is_serie_synthetic_v<ReturnType>) {
		if (headersArePresented(line_iter)) {
			WTI.setField(&ReturnType::headers, makeHeaders(line_iter, sep));
			line_iter = std::next(line_iter);
		}
	}

	for (auto itb = line_iter, ite = lines_of_data.end(); itb!=ite; ++itb) {
		auto [ts, values] = parseData<duration_t, double>(itb, sep);
		if constexpr (is_serie_synthetic_v<ReturnType>) {
			WTI.emplaceBack(ts, std::move(values));
		}
		else if constexpr (is_serie_v<ReturnType>) {
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
)"};
	std::cout << "original data sample:\n" << data_sample;
}

TEST(EndToEnd, SingleThread_Read_CSV_Synthetic) {

	using namespace time_series;
	using namespace culib::time;
	using namespace csv_reader;


//	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016.csv"};
	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016 short.csv"};

	using ValueType = double;
	char const comma_sep {','};
	using SyntheticOHLCV = time_series::SerieSynthethic<Minutes, ValueType, 5u>;

	auto WTI = readFromFile<SyntheticOHLCV>(test_file, comma_sep);
	WTI.setField (&SyntheticOHLCV::name, "CL OHLCV minute data for Sep 2016");

	auto it = WTI.begin();
	auto & [ts, open, high, low, close, volume] = it;
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

#if 0
TEST(EndToEnd, SingleThread_Read_CSV_Natural) {

	using namespace time_series;
	using namespace time_series::financial;
	using namespace culib::time;
	using namespace csv_reader;

//	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016.csv"};
	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016 short.csv"};

	char const comma_sep {','};
	using NaturalOHLCV = Serie<Minutes, OHLCV<double>>;

	auto WTI = readFromFile<NaturalOHLCV>(test_file, comma_sep);

	auto it = WTI.begin();
	auto const & [open, high, low, close, volume] = it->second();
	ASSERT_EQ (open, 46.54);
	ASSERT_EQ (high, 46.6);
	ASSERT_EQ (low, 46.5);
	ASSERT_EQ (close, 46.53);
	ASSERT_EQ (volume, 89);

	ASSERT_TRUE (WTI.size() == 100u);

}
#endif
TEST(EndToEnd, SingleThread_Upcast) {
	ASSERT_EQ(true, false);
}