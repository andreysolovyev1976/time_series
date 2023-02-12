//
// Created by Andrey Solovyev on 24/01/2023.
//

#include "typed_tests_list.h"

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlace) {
	[[maybe_unused]] double v1 {0.000005};
	[[maybe_unused]] double v2 {0.000006};
	[[maybe_unused]] double v3 {0.000005};
}

TYPED_TEST (ElementFnApplication, ApplyFunctionInPlaceChaining) {
	[[maybe_unused]] double v1 {0.000005};
	[[maybe_unused]] double v2 {0.000006};
	[[maybe_unused]] double v3 {0.000005};
}

TYPED_TEST (ElementFnApplication, ApplyFunctionCreateNew) {
	[[maybe_unused]] double v1 {0.000005};
	[[maybe_unused]] double v2 {0.000006};
	[[maybe_unused]] double v3 {0.000005};
}

