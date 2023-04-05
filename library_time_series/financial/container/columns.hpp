//
// Created by Andrey Solovyev on 26/03/2023.
//

#pragma once

#include "common_usage_library/itertools.hpp"
#include "time_series/serie.hpp"
#include <list>

#ifndef FIN_DATA_CONTAINER_H
#define FIN_DATA_CONTAINER_H

namespace time_series::financial {


  template <typename Duration, typename ValueType, std::size_t ColumnCount = 5u>
  class Columns {
  public:
	  using IndexElement = typename culib::time::Timestamp<Duration>;
	  using Index = typename std::vector<IndexElement>;
	  using DataElement = typename time_series::Value<ValueType>;
	  using DataColumn = typename time_series::Serie<Duration, DataElement, std::list>;
	  using Data = typename std::vector<DataColumn>;
	  using NewValues = typename std::vector<ValueType>;
	  using Name = typename std::string;
	  using Headers = typename std::vector<Name>;

	  Columns () { data.resize(ColumnCount); }
	  Columns (Name n) : name (std::move(n)) { data.resize(ColumnCount); }

	  template <typename Field, typename FieldType>
	  void setField (Field field, FieldType new_value) { this->*field = std::move(new_value); }

	  void emplaceBack(IndexElement idx, NewValues v) {
		  if (v.size() != data.size()) return; //no exception
		  index.emplace_back(idx);
		  std::size_t size = v.size();
		  for (std::size_t i = 0; i != size; ++i) {
			  data[i].emplace_back(idx, v[i]);
		  }
	  }

	  template <typename By>
	  auto& getColumn (By what) {
		  if constexpr (culib::requirements::is_converible_or_constructible_v<By, Name>) {
			  if (!headersAreSet() || headers.size() != (data.size() + 1)) { return empty_column; }
			  int idx {0};
			  for (auto h = std::next(headers.begin()), he = headers.end(); h != he; ++h ) { //skip Index
				  if (*h == what) { return data[idx]; }
				  else { ++idx; }
			  }
			  return empty_column;
		  }
		  else if constexpr (std::is_integral_v<By>) {
			  auto idx = static_cast<std::size_t>(what);
			  if (idx < data.size()) { return data[idx]; }
		  }
		  return empty_column;
	  }

	  //todo looks bad, will require some work if column added
	  auto begin() { return makeBegin<ColumnCount>(); }
	  auto end() { return makeEnd<ColumnCount>(); }

	  Name name;
	  Headers headers;
	  Index index;
	  Data data;

  private:
	  DataColumn empty_column;

	  template <std::size_t... Is>
	  auto makeBeginImpl (std::index_sequence<Is...>) {
		  return culib::itertools::zip(index.begin(), data[Is].begin()...);
	  }

	  template <std::size_t N>
	  auto makeBegin () {
		  return makeBeginImpl(std::make_index_sequence<N>{});
	  }
	  template <std::size_t... Is>
	  auto makeEndImpl (std::index_sequence<Is...>) {
		  return culib::itertools::zip(index.end(), data[Is].end()...);
	  }

	  template <std::size_t N>
	  auto makeEnd () {
		  return makeEndImpl(std::make_index_sequence<N>{});
	  }

	  bool headersAreSet () const { return !headers.empty(); }

  };


}//!namespace

#endif //FIN_DATA_CONTAINER_H
