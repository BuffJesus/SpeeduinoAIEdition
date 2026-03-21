#pragma once

#include "../trace_fixture.h"

// 4G63 / Miata 4/2: a cam-high primary rising edge sets revolutionOne.
// The following primary falling edge with cam still high sets toothCurrentCount to 5.
// A cam falling edge while primary is low then establishes sync, after which primary edges continue from tooth 6.
TRACE_DATA_P TraceEvent k4G63CamSyncEvents[] = {
    {19000U, TRACE_PRIMARY},
    {30000U, TRACE_PRIMARY_WIDE},
    {5000U, TRACE_SECONDARY_FALLING},
    {19000U, TRACE_PRIMARY},
    {30000U, TRACE_PRIMARY_WIDE},
};
