#pragma once

#include "../trace_fixture.h"

// Basic distributor: the first valid tooth immediately establishes sync and starts revolution counting.
TRACE_DATA_P TraceRepeatEvent kBasicDistributorSyncEvents[] = {
    {10000U, TRACE_PRIMARY, 1U},
};
