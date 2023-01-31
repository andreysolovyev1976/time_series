//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#ifndef BASE_VALUE_TYPE_H
#define BASE_VALUE_TYPE_H

namespace base {

	namespace traits {
	  /**
	   * @details
	   * double as a carrier for time series value was selected after a benchmark research.
	   * See separate project.
	   */

	  using ValueTypeDefault = double;

  }//!namespace

}//!namespace
#endif //BASE_VALUE_TYPE_H
