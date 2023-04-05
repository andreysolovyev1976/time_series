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

	auto raw_data = readFile(path);
	auto lines_of_data = splitStringBy(raw_data, '\n', ::const_values::EXPECTED_LINES_COUNT);

	ReturnType WTI;

	if (lines_of_data.empty()) return WTI;

	auto line_iter = lines_of_data.begin();
	if (headersArePresented(line_iter)) {
		WTI.setField (&ReturnType::headers, makeHeaders(line_iter, sep));
		line_iter = std::next(line_iter);
	}

	for (auto itb = line_iter, ite = lines_of_data.end(); itb!=ite; ++itb) {
		getData(WTI, itb, sep);
	}

	return WTI;
}


TEST(EndToEnd, Read_CSV_SingleThread) {

	using namespace time_series;
	using namespace culib::time;
	using namespace csv_reader;

	std::string data_sample{
R"(Date,Open,High,Low,Close,Volume
9/23/2015 0:00,46.54,46.6,46.5,46.53,89
9/23/2015 0:01,46.53,46.54,46.51,46.52,14
9/23/2015 0:02,46.51,46.51,46.48,46.48,30
)"};
	std::cout << "original data sample:\n" << data_sample;

//	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016.csv"};
	std::filesystem::path test_file {"../../tests/WTI_OHLCVminute sept2016 short.csv"};

	using ValueType = double;
	char const comma_sep {','};
	using SyntheticOHLCV = time_series::financial::Columns<Minutes, ValueType, 5u>;

	auto WTI = readFromFile<SyntheticOHLCV>(test_file, comma_sep);
	WTI.setField (&SyntheticOHLCV::name, "CL OHLCV minute data for Sep 2016");

	auto it = WTI.begin();
	for (int i = 0; i != 3; ++i) {
		std::cout << *it << '\n';
		++it;
	}

	auto column_close = WTI.getColumn ("Close");
	auto column_four = WTI.getColumn (3);

	ASSERT_TRUE (column_close.size() == 100u);
	ASSERT_TRUE (column_four.size() == 100u);
	ASSERT_EQ (column_close, column_four);
}