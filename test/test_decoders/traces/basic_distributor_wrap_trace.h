#pragma once

#include "../trace_fixture.h"

// Basic distributor, 4-cylinder 4-stroke: four evenly spaced cam teeth per 720 degrees, so the fifth tooth wraps to tooth 1.
TRACE_DATA_P TraceRepeatEvent kBasicDistributorWrapEvents[] = {
    {10000U, TRACE_PRIMARY, 5U},
};
