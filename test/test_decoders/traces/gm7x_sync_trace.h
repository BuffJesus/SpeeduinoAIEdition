#pragma once

#include "../trace_fixture.h"

// GM 7X: two normal gaps followed by a gap shorter than half the previous gap.
// That short gap is the sync tooth ("tooth 3" in decoder terminology).
TRACE_DATA_P TraceEvent kGm7xSyncEvents[] = {
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {4000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
};
