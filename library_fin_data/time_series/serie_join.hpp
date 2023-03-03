//
// Created by Andrey Solovyev on 01/03/2023.
//

#pragma once

#include "time_series/types_requirements/serie.h"
#include "utils/itertools.hpp"
#include "utils/joins.hpp"

#include <algorithm>
#include <type_traits>
#include <cstddef>

#ifndef FIN_DATA_SERIE_JOIN_H
#define FIN_DATA_SERIE_JOIN_H

namespace time_series {

  /*
  // FULL OUTER JOIN returns matched and unmatched rows from both tables (it's an union of both). If there is no match, the missing side will contain null.
  // so it extends each table up to n + m rows
  full outer join

  //A LEFT OUTER JOIN returns all rows from the left table (TableA) with the matching rows from the right table (TableB) or null – if there is no match in the right table.
  // it either has a letf table value or null from B, so table doesn't increase
  left outer join
  //A RIGHT OUTER JOIN returns all rows from the right table (TableB) with the matching rows from the left table (TableA) or null – if there is no match in the left table.
  // it either has a value or null if no match from table B, so resulting table may be up to n + m rows
  right outer join

  left-anti-join
  right-anti-join
  full anti-join

   inner join //An INNER JOIN merges ONLY the matching rows in BOTH tables


   OUTER EXCLUDING JOIN
   */

  /*
  		27
   3	12
   2	16
   3	20
   17
   25


   Full Outer Join
   0	27
   3	12
   2	16
   3	20
   17	0
   25	0
   Inner Join
   3	12
   2	16
   3	20
   Outer Excluding Join
   0	27
   17	0
   25	0

   Left Outer join
   3 	12
   2	16
   3	20
   17	0
   25	0
   Left Excluding Join
   17	0
   25	0
  Right Outer join
   0	27
   3 	12
   2	16
   3	20
   Right Excluding Join
   0	27

   */

  /*
  where primary key (timestamp)
   is less than
   ...
   or any other complicated logic

   comparator is required



   */

  namespace join {


	//todo: add serie requirementes
	//todo: reduce IMPLEMENT OPERATION... macro by removing VA_ARGS
	//todo: add UI

	void outerFull () {}
	void outerExcluding () {}
	void inner () {}

	void leftOuter () {}
	void leftExcluding () {}

	void rightOuter () {}
	void rightExcluding () {}

/**
 * @brief
 * A Serie must be
 * a) not empty
 * b) sorted
 * otherwise you'll have your UB
 *
 * */


  }//!namespace
}//!namespace
#endif //FIN_DATA_SERIE_JOIN_H
