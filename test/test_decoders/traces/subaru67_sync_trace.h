#pragma once

#include "../trace_fixture.h"

// Subaru 6/7: three back-to-back cam pulses immediately before the next primary should identify tooth 2 and establish sync.
TRACE_DATA_P TraceEvent kSubaru67SyncEvents[] = {
    {1000U, TRACE_SECONDARY_RISING},
    {1000U, TRACE_SECONDARY_RISING},
    {1000U, TRACE_SECONDARY_RISING},
    {1000U, TRACE_PRIMARY},
};
