#pragma once

#include "../trace_fixture.h"

// Jeep 2000: 12 crank teeth in 4 groups (3 teeth per group, 20° spacing within group, 60° between groups)
// Cam pulse resets toothCurrentCount to 0, establishing sync
// At 600 RPM: 100ms per revolution
// 20° spacing ≈ 5.5ms, 60° spacing ≈ 16.7ms
TRACE_DATA_P TraceEvent kJeep2000CamSyncEvents[] = {
    {5500U, TRACE_PRIMARY},   // Tooth 1 (group 1)
    {5500U, TRACE_PRIMARY},   // Tooth 2
    {5500U, TRACE_PRIMARY},   // Tooth 3
    {16700U, TRACE_PRIMARY},  // Tooth 4 (group 2, 60° gap)
    {5500U, TRACE_PRIMARY},   // Tooth 5
    {5500U, TRACE_PRIMARY},   // Tooth 6
    {10000U, TRACE_SECONDARY_RISING},  // Cam pulse resets count to 0
    {6700U, TRACE_PRIMARY},   // Tooth 7 (group 3, 60° gap) - now becomes tooth 0, then wraps to tooth 1
    {5500U, TRACE_PRIMARY},   // Tooth 1 (after wrap)
    {5500U, TRACE_PRIMARY},   // Tooth 2
};
