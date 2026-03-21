#pragma once

#include "../trace_fixture.h"

// Basic distributor: once synced, a very short third gap should be rejected by the lite even-tooth filter.
TRACE_DATA_P TraceEvent kBasicDistributorShortGapEvents[] = {
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {1000U, TRACE_PRIMARY},
};
