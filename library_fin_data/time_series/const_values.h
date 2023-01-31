//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include <cstddef>

#ifndef BASE_CONST_VALUES_H
#define BASE_CONST_VALUES_H

namespace base {
  namespace const_values {

	static const double EPSILON_BY_DEFAULT {0.000001};
	static const std::size_t EXPECTED_SINGLE_QUOTE_LENGTH {16u}; //arbitrary figure
	static const std::size_t REQUIRED_ARG_COUNT {5u};

  }//!namespace
}//!namespace

namespace time_series {
  namespace const_values {
	static const std::size_t EXPECTED_ELEMENT_LENGTH {64u}; //arbitrary figure

  }//!namespace
}//!namespace


#endif //BASE_CONST_VALUES_H
