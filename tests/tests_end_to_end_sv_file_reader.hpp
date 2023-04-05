//
// Created by Andrey Solovyev on 05/04/2023.
//

#include "culib.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <optional>


namespace csv_reader {

  [[nodiscard]]
  inline
  std::string readFile(std::filesystem::path const& file_path) {
	  std::ifstream in_file{file_path, std::ios::in | std::ios::binary};
	  if (!in_file) {
		  throw std::runtime_error("Cannot open "+file_path.string());
	  }

	  std::string str(static_cast<std::size_t>(std::filesystem::file_size(file_path)), '\0');

	  in_file.read(str.data(), str.size());
	  if (!in_file) {
		  throw std::runtime_error("Could not read the full contents from "+file_path.filename().string());
	  }

	  return str;
  }

  template<typename Iter>
  void freakingWindows (Iter iter) {
	  if (iter->back()=='\r') { iter->remove_suffix(1); }//freaking windows
  }

  template <typename Iter>
  bool headersArePresented(Iter line_iter) {
	  auto symbol = line_iter->begin();
	  while (std::isspace(static_cast<int>(*symbol))) ++symbol;
	  return std::isalpha(static_cast<int>(*symbol));
  }

  template <typename Iter>
  std::vector<std::string> makeHeaders(Iter line_iter, const char sep){
	  freakingWindows(line_iter);
	  auto tmp = culib::utils::splitStringViewBy(*line_iter, sep);
	  return {std::make_move_iterator(tmp.begin()),
			  std::make_move_iterator(tmp.end())};
  }



  template<typename Duration, typename ValueType, typename Iter>
  std::pair<culib::time::Timestamp<Duration>,std::vector<ValueType>>
  parseData (Iter iter, const char sep) {
	  //surely can be optimized, these entities are created every iteration
	  std::stringstream ss;
	  std::vector<ValueType> ohlcv_tmp; ohlcv_tmp.reserve(5);
	  culib::time::Timestamp<Duration> ts;

	  freakingWindows(iter);
	  auto data_points = culib::utils::splitStringViewBy(*iter, sep);
	  if (data_points.empty()) return {};
	  for (auto data_point : data_points) {
		  if (data_point == *data_points.begin()) {
			  ss.write(data_point.data(), data_point.size());
			  date::from_stream(ss, "%m/%d/%Y %H:%M", ts.time_point);
//			  ss.str() = std::string();
		  }
		  else {
			  auto d = culib::utils::fromChars<ValueType>(data_point); //no exception
			  if (d) { ohlcv_tmp.push_back(*d); }
			  else { break; }
		  }
	  }
	  return std::pair{std::move(ts), std::move(ohlcv_tmp)};
  }

}//!namespace
