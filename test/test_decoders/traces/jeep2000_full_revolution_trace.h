#pragma once

#include "../trace_fixture.h"

// Jeep 2000: Complete revolution with cam sync and wrap
// Shows full 12-tooth pattern plus cam reset
// Uses compressed format for repeated 20° teeth within groups
TRACE_DATA_P TraceRepeatEvent kJeep2000FullRevolutionEvents[] = {
    // Group 1: teeth 1-3 (20° spacing)
    {5500U, TRACE_PRIMARY, 3U},

    // Group 2: teeth 4-6 (60° gap, then 20° spacing)
    {16700U, TRACE_PRIMARY, 1U},
    {5500U, TRACE_PRIMARY, 2U},

    // Group 3: teeth 7-9 (60° gap, then 20° spacing)
    {16700U, TRACE_PRIMARY, 1U},
    {5500U, TRACE_PRIMARY, 2U},

    // Group 4: teeth 10-12 (60° gap, then 20° spacing)
    {16700U, TRACE_PRIMARY, 1U},
    {5500U, TRACE_PRIMARY, 2U},

    // Cam pulse between tooth 12 and next tooth 1
    {10000U, TRACE_SECONDARY_RISING, 1U},

    // Back to tooth 1 (60° gap from tooth 12)
    {6700U, TRACE_PRIMARY, 1U},  // toothCurrentCount becomes 0, then wraps to 1
};
