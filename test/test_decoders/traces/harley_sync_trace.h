#pragma once

#include "../trace_fixture.h"

// Harley: the first valid high primary pulse is treated as the long-gap reference and establishes sync on tooth 1.
TRACE_DATA_P TraceEvent kHarleySyncEvents[] = {
    {5000U, TRACE_PRIMARY},
};
