//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include <cstddef>

#ifndef FIN_CONST_VALUES_H
#define FIN_CONST_VALUES_H

namespace time_series::financial {
  namespace const_values {
	static const std::size_t EXPECTED_ERROR_MESSAGE_LENGTH {256u}; //arbitrary figure
	static const std::size_t EXPECTED_OHLCV_LENGTH {32u}; //arbitrary figure
	static const std::size_t EXPECTED_BidAsk_LENGTH {32u}; //arbitrary figure

  }//!namespace
}//!namespace


#endif //FIN_CONST_VALUES_H
