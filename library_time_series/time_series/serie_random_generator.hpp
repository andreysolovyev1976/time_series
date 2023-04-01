//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once

#include "time_series/serie.hpp"
#include "common_usage_library/randomer.hpp"

#include <cstddef>
#include <thread>

#ifndef FIN_DATA_TIME_SERIE_RANDOM_GENERATOR_H
#define FIN_DATA_TIME_SERIE_RANDOM_GENERATOR_H

namespace time_series::utils {

  template <
		  typename Duration = culib::time::Microseconds
		  , typename ElemType = Value<int>
		  , template <typename...> typename Container = std::vector
  >
  auto generateSerie (std::size_t required_size = 100u) {
	  using namespace std::chrono_literals;
	  const auto kstep = 100us;
	  const auto klower_limit {static_cast<typename ElemType::value_type>(-required_size * 2)};
	  const auto kupper_limit {static_cast<typename ElemType::value_type>(required_size * 2)};

	  Serie<Duration, ElemType, Container> serie;

	  for (std::size_t idx = 0; idx != required_size; ++idx) {
		  std::this_thread::sleep_for(kstep);
		  serie.insert(serie.end(), {culib::utils::randomer(klower_limit, kupper_limit)});
	  }

	  return serie;
  }

  template <
		  typename Duration = culib::time::Microseconds
		  , typename ElemType = Value<int>
		  , template <typename...> typename Container = std::vector
  >
  auto generateSerie (int value, std::size_t required_size = 3u) {
	  using namespace std::chrono_literals;
	  const auto kstep = 100us;

	  Serie<Duration, ElemType, Container> serie;

	  for (std::size_t idx = 0; idx != required_size; ++idx) {
		  std::this_thread::sleep_for(kstep);
		  serie.insert(serie.end(), {value++});
	  }

	  return serie;
  }


}//namespace

#endif //FIN_DATA_TIME_SERIE_RANDOM_GENERATOR_H
