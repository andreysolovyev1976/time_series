//
// Created by Andrey Solovyev on 23/02/2023.
//

#pragma once



#include "time_series/serie.hpp"
#include "utils/randomer.hpp"

#include <cstddef>
#include <thread>

#ifndef FIN_DATA_TIME_SERIE_RANDOM_GENERATOR_H
#define FIN_DATA_TIME_SERIE_RANDOM_GENERATOR_H

namespace time_series::utils {

  template <
		  typename Duration = base::Nanoseconds
		  , typename ElemType = base::Value<int>
		  , template <typename...> typename Container = std::vector
  >
  auto generateSerie (std::size_t required_size = 100u) {
	  using namespace std::chrono_literals;
	  const auto step = 100ns;

	  Serie<Duration, ElemType, Container> serie;

	  for (std::size_t idx = 0; idx != required_size; ++idx) {
		  std::this_thread::sleep_for(step);
		  serie.insert({base::utils::randomer(
				  static_cast<typename ElemType::value_type>(-required_size * 2),
				  static_cast<typename ElemType::value_type>(required_size * 2))
		  });
	  }

	  return serie;
  }


}//namespace

#endif //FIN_DATA_TIME_SERIE_RANDOM_GENERATOR_H
