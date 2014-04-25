// File: core/pin_util.hpp - Define PIN utility functions.

#ifndef CORE_PIN_UTIL_HPP_
#define CORE_PIN_UTIL_HPP_

#include "pin.H"

#include "core/basictypes.h"

// Global definitions

// Find RTN by function name in an image.
extern RTN FindRTN(IMG img, const char *func_name);

// Get the IMG that contains the TRACE.
extern IMG GetImgByTrace(TRACE trace);

// Return whether the given bbl contains non-stack memory access.
extern bool BBLContainMemOp(BBL bbl);

#endif

