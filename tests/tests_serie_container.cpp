//
// Created by Andrey Solovyev on 16/02/2023.
//

#include <gtest/gtest.h>
#include "time_series/serie.hpp"


#include <sstream>
#include <cstdint>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <list>
#include <array>
#include <thread>

#include "boost/circular_buffer.hpp"
#include "boost/type_index.hpp"

using namespace std::chrono_literals;
using namespace boost::typeindex;
using Duration = base::Microseconds;
using ElemType = base::Value<std::int64_t>;
using Elem = time_series::Element<Duration, ElemType>;
const auto pause_to_take = 10us;

namespace array {
  template <typename T>
  using Helper = std::array<T, 3u>;
}
TEST(SerieContainer, Array) {
	time_series::Serie<
	        Duration, ElemType,
			array::Helper
			> serie;
	serie[0] = Elem(12);
	serie[1] = Elem(1);
	serie[2] = Elem(3);
	ASSERT_EQ(serie[0].value, 12.0);

	std::stringstream ss;
	for (const auto& [ts, elem] : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"12 1 3 "};
	ASSERT_EQ(ss.str(), check);
}
TEST(SerieContainer, Vector) {
	time_series::Serie<Duration, ElemType,
	        std::vector
			> serie;
	serie.push_back({12});
	serie.push_back({1});
	serie.push_back({3});
	ASSERT_EQ(serie.front().value, 12.0);

	std::stringstream ss;
	for (const auto& elem : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"12 1 3 "};
	ASSERT_EQ(ss.str(), check);
}
TEST(SerieContainer, Deque) {
	time_series::Serie<Duration, ElemType,
	        std::deque
	> serie;
	serie.push_back({12});
	serie.push_back({1});
	serie.push_back({3});
	ASSERT_EQ(serie.front().value, 12.0);

	std::stringstream ss;
	for (const auto& elem : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"12 1 3 "};
	ASSERT_EQ(ss.str(), check);
}
TEST(SerieContainer, List) {
	time_series::Serie<
	        Duration, ElemType,
	        std::list
			> serie;
	serie.push_back({12});
	serie.push_back({1});
	serie.push_back({3});
	ASSERT_EQ(serie.front().value, 12.0);

	std::stringstream ss;
	for (const auto& elem : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"12 1 3 "};
	ASSERT_EQ(ss.str(), check);
}
TEST(SerieContainer, BoostCircularBuffer) {
	time_series::Serie<
	        Duration, ElemType,
			boost::circular_buffer
	> serie;

	int capacity {2};
	serie.set_capacity(capacity);

	serie.push_back({12});
	serie.push_back({1});
	serie.push_back({3});
	ASSERT_EQ(serie.front().value, 1);

	std::stringstream ss;
	for (const auto& elem : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"1 3 "};
	ASSERT_EQ(ss.str(), check);
}

namespace comparators {
  template<typename Duration>
  struct CompTimestampStruct {
	  bool operator()(const base::Timestamp<Duration> &a, const base::Timestamp<Duration> &b) const {
		  return a > b;
	  };
  };
  template<typename Duration>
  [[maybe_unused]] auto comp_timestamp = [](const base::Timestamp<Duration> &a, const base::Timestamp<Duration> &b) {
	return a > b;
  };
  template<typename Duration>
  using CompTimestampLambda = decltype(comp_timestamp<Duration>);


  template<typename Duration, typename ElemType>
  struct CompElemStruct {
	  bool operator()(const time_series::Element<Duration, ElemType> &a, const time_series::Element<Duration, ElemType> &b) const {
		  return a > b;
	  };
  };
  template<typename Duration, typename ElemType>
  [[maybe_unused]] auto comp_elem = [](const time_series::Element<Duration, ElemType> &a, const time_series::Element<Duration, ElemType> &b) {
	return a > b;
  };
  template<typename Duration, typename ElemType>
  using CompElemLambda = decltype(comp_elem<Duration, ElemType>);

}//!namespace
TEST(SerieContainer, Set_DefaultComparator) {
	time_series::Serie<Duration, ElemType,
	        std::set
			> serie;
	serie.insert({12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({3});
	ASSERT_EQ(*serie.begin(), 12.0);

	std::stringstream ss;
	for (const auto& elem : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"12 1 3 "};
	ASSERT_EQ(ss.str(), check);
}
TEST(SerieContainer, Set_UserDefinedComparatorStruct) {
	time_series::Serie<
	        Duration, ElemType,
			std::set,
			comparators::CompElemStruct<Duration, ElemType>
	> serie;
	serie.insert({12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({3});
	ASSERT_EQ(serie.begin()->value, 3.0);

	std::stringstream ss;
	for (const auto& elem : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"3 1 12 "};
	ASSERT_EQ(ss.str(), check);
}
#if 0
TEST(SerieContainer, Set_UserDefinedComparatorLambda) {
	time_series::Serie<
			Duration, ElemType,
			std::set,
			comparators::CompElemLambda<Duration, ElemType>
	> serie;
	serie.insert({12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({3});
	ASSERT_EQ(serie.begin()->value, 3.0);

	std::stringstream ss;
	for (const auto& elem : serie) {
		ss << elem.value << ' ';
	}
	const std::string check {"3 1 12 "};
	ASSERT_EQ(ss.str(), check);
}
#endif
TEST(SerieContainer, Map_DefaultComparator) {
	time_series::Serie<
			Duration, ElemType,
			std::map
	> serie;

	serie.insert({base::Timestamp<Duration>(), 12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 3});
	ASSERT_EQ(serie.begin()->second, 12.0);

	std::stringstream ss;
	for (const auto& [timestamp, value] : serie) {
		ss << value << ' ';
	}
	const std::string check {"12 1 3 "};
	ASSERT_EQ(ss.str(), check);
}
TEST(SerieContainer, Map_UserDefinedComparatorStruct) {
	time_series::Serie<
			Duration, ElemType,
			std::map,
			comparators::CompTimestampStruct<Duration>
	> serie;

	serie.insert({base::Timestamp<Duration>(), 12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 3});
	ASSERT_EQ(serie.begin()->second, 3.0);

	std::stringstream ss;
	for (const auto& [timestamp, value] : serie) {
		ss << value << ' ';
	}
	const std::string check {"3 1 12 "};
	ASSERT_EQ(ss.str(), check);
}
#if 0
TEST(SerieContainer, Map_UserDefinedComparatorLambda) {
	time_series::Serie<
		Duration, ElemType,
		std::map,
		comparators::CompTimestampLambda<Duration>
	> serie;

	serie.insert({base::Timestamp<Duration>(), 12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 3});
	ASSERT_EQ(serie.begin()->second, 3.0);

	std::stringstream ss;
	for (const auto& [timestamp, value] : serie) {
		ss << value << ' ';
	}
	const std::string check {"3 1 12 "};
	ASSERT_EQ(ss.str(), check);
}
#endif

namespace hashers {
  template <typename Duration>
  using HasherTimestamp = typename base::TimestampHasher<Duration>;

  template <typename Duration, typename ElemType>
  struct HasherElem {
	  std::size_t operator () (const time_series::Element<Duration, ElemType> &elem) const {
		  return hasher_timestamp(elem.timestamp);
	  }
	  HasherTimestamp<Duration> hasher_timestamp;
  };
}//!namespace
TEST(SerieContainer, Exotics_UnorderedSet) {
	time_series::Serie<
			Duration, ElemType,
			std::unordered_set,
			hashers::HasherElem<Duration, ElemType>
	> serie;

	serie.insert({12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({3});

	std::set assigned_values {1, 3, 12};
	for (const auto& [ts, elem]: serie) {
		if (auto found = assigned_values.find(elem.value);
				found != assigned_values.end()) {
			assigned_values.erase(found);
		}
	}
	ASSERT_TRUE(assigned_values.empty());
}
TEST(SerieContainer, Exotics_UnorderedMap) {
	time_series::Serie<
			Duration, ElemType,
			std::unordered_map,
			hashers::HasherTimestamp<Duration>
			> serie;

	serie.insert({base::Timestamp<Duration>(), 12});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 1});
	std::this_thread::sleep_for(pause_to_take);
	serie.insert({{}, 3});

	std::set assigned_values {1, 3, 12};
	for (const auto& [ts, elem] : serie) {
		if (auto found = assigned_values.find(elem.value);
				found != assigned_values.end()) {
			assigned_values.erase(found);
		}
	}
	ASSERT_TRUE(assigned_values.empty());
}

namespace user_defined_containers {
  template <typename T>
  struct OkContainer {
	  T data [3];
	  const T& begin() const {return data[0];}
	  const T& end() const {return data[4];}
  };
}//!namespace
TEST(SerieContainer, UserDefinedContainer) {
	time_series::Serie<
			Duration, ElemType,
			user_defined_containers::OkContainer
	> serie;

	serie.data[0] = Elem(12);
	serie.data[1] = Elem(1);
	serie.data[2] = Elem(3);

	std::set assigned_values {1, 3, 12};
	for (const auto& [ts, elem] : serie.data) {
		if (auto found = assigned_values.find(elem.value);
				found != assigned_values.end()) {
			assigned_values.erase(found);
		}
	}
	ASSERT_TRUE(assigned_values.empty());
}
