//
// Created by Andrey Solovyev on 31/01/2023.
//

#pragma once
#include "time_series/value.hpp"

#ifndef FIN_SINGLE_QOUTE_H
#define FIN_SINGLE_QOUTE_H


namespace time_series::financial {

  template <typename ValueType = value::traits::ValueTypeDefault>
  using SingleQuote = Value<ValueType>;

}

#endif //FIN_SINGLE_QOUTE_H
