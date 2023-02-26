//
// Created by Andrey Solovyev on 31/01/2023.
//

#pragma once
#include "time_series/value.hpp"

#ifndef FIN_SINGLE_QOUTE_H
#define FIN_SINGLE_QOUTE_H


namespace financial {

  template <typename ValueType = base::traits::ValueTypeDefault>
  using SingleQuote = base::Value<ValueType>;

}

#endif //FIN_SINGLE_QOUTE_H
