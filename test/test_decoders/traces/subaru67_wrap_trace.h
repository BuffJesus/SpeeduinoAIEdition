#pragma once

#include "../trace_fixture.h"

// Subaru 6/7: after syncing on tooth 2, eleven more primaries complete the cycle and the twelfth primary wraps to tooth 1.
TRACE_DATA_P TraceRepeatEvent kSubaru67WrapEvents[] = {
    {1000U, TRACE_SECONDARY_RISING, 3U},
    {1000U, TRACE_PRIMARY, 12U},
};
