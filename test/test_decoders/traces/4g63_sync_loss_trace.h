#pragma once

#include "../trace_fixture.h"

// After sync is established, a cam edge while primary is high and toothCurrentCount != 8
// is treated as noise during cranking and drops sync.
TRACE_DATA_P TraceEvent k4G63SyncLossEvents[] = {
    {19000U, TRACE_PRIMARY},
    {30000U, TRACE_PRIMARY_WIDE},
    {5000U, TRACE_SECONDARY_FALLING},
    {19000U, TRACE_PRIMARY},
    {5000U, TRACE_SECONDARY_RISING},
};
