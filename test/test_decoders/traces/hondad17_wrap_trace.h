#pragma once

#include "../trace_fixture.h"

// Honda D17: sync on the short 13th gap, then a full synced cycle wraps on the next reference tooth and the following tooth starts revolution two.
TRACE_DATA_P TraceRepeatEvent kHondaD17WrapEvents[] = {
    {10000U, TRACE_PRIMARY, 12U},
    {4000U, TRACE_PRIMARY, 1U},
    {10000U, TRACE_PRIMARY, 14U},
};
