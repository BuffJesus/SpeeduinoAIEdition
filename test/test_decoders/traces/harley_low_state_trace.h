#pragma once

#include "../trace_fixture.h"

// Harley: after syncing on a high pulse, a later low primary state forces sync loss.
TRACE_DATA_P TraceEvent kHarleyLowStateEvents[] = {
    {5000U, TRACE_PRIMARY},
    {5000U, TRACE_PRIMARY_WIDE},
};
