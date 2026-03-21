#pragma once

#include "../trace_fixture.h"

// Honda D17: uniform gaps never produce the short 13th-tooth reference gap, so sync must not establish.
TRACE_DATA_P TraceRepeatEvent kHondaD17NoSyncEvents[] = {
    {10000U, TRACE_PRIMARY, 13U},
};
