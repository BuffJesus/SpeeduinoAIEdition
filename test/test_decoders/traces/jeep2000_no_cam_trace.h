#pragma once

#include "../trace_fixture.h"

// Jeep 2000: Primary-only trace without cam signal
// Without cam pulse, toothCurrentCount stays at 13 (no sync)
// Shows that decoder requires cam signal for sync
TRACE_DATA_P TraceRepeatEvent kJeep2000NoCamEvents[] = {
    // Group 1: teeth (20° spacing)
    {5500U, TRACE_PRIMARY, 3U},

    // Group 2 (60° gap, then 20° spacing)
    {16700U, TRACE_PRIMARY, 1U},
    {5500U, TRACE_PRIMARY, 2U},

    // Group 3 (60° gap, then 20° spacing)
    {16700U, TRACE_PRIMARY, 1U},
    {5500U, TRACE_PRIMARY, 2U},

    // Group 4 (60° gap, then 20° spacing)
    {16700U, TRACE_PRIMARY, 1U},
    {5500U, TRACE_PRIMARY, 2U},

    // No cam pulse - should remain without sync
};
