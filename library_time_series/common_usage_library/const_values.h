//
// Created by Andrey Solovyev on 30/01/2023.
//

#pragma once

#include <cstddef>

#ifndef BASE_CONST_VALUES_H
#define BASE_CONST_VALUES_H

  namespace culib::const_values {

	static const double EPSILON_BY_DEFAULT {0.00005};
	static const std::size_t EXPECTED_SINGLE_QUOTE_LENGTH {16u}; //arbitrary figure
	static const std::size_t REQUIRED_ARG_COUNT {5u};

	static const std::size_t ALLOWANCE_FOR_CONTAINER_STATE_DATA {32u};

  }//!namespace


#endif //BASE_CONST_VALUES_H
