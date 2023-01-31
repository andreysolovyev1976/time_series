//
// Created by Andrey Solovyev on 31/01/2023.
//

#pragma once
#include "time_series/value.h"

#ifndef FIN_SINGLE_QOUTE_H
#define FIN_SINGLE_QOUTE_H


namespace finacial {

  template <typename Duration>
  using SingleQuote = base::Value<Duration>;

}

#endif //FIN_SINGLE_QOUTE_H
