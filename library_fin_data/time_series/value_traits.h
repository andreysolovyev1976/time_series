//
// Created by Andrey Solovyev on 29/01/2023.
//

#pragma once

#ifndef BASE_VALUE_TRAITS_H
#define BASE_VALUE_TRAITS_H

namespace base {

  namespace traits {

	/**
	 * @details
	 * CRTP as it is required to have a quasi-reflection
	 * */

	template <typename Derived>
	struct ValueBase {
		using wrapper_type = Derived;

		virtual ~ValueBase () {}
	};

	//todo: virtual dtor deletes move ctor and assignment operator. Implementation required.


	/**
	 * @details
	 * double as a carrier for time series value was selected after a benchmark research.
	 * See separate project.
	 */

	using ValueTypeDefault = double;


  }//!namespace

}//!namespace
#endif //BASE_VALUE_TRAITS_H
