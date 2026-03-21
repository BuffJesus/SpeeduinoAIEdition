#pragma once

#include "../trace_fixture.h"

// Honda D17: 12 normal teeth followed by a short 13th gap establishes sync, and the next normal tooth starts the next revolution.
TRACE_DATA_P TraceRepeatEvent kHondaD17SyncEvents[] = {
    {10000U, TRACE_PRIMARY, 12U},
    {4000U, TRACE_PRIMARY, 1U},
    {10000U, TRACE_PRIMARY, 1U},
};
