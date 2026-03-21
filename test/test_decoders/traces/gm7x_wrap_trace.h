#pragma once

#include "../trace_fixture.h"

// GM 7X: sync on the short gap, then continue through teeth 4..7 and wrap on the next tooth.
TRACE_DATA_P TraceEvent kGm7xWrapEvents[] = {
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {4000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
};
