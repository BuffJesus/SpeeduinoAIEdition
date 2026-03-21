#pragma once

#include "../trace_fixture.h"

// Audi 135: 135 physical crank teeth, every 3rd counted (45 effective teeth, 8° each)
// Cam pulse establishes sync by setting toothCurrentCount=0 and toothSystemCount=3
// At 600 RPM: 360°/135 teeth = 2.67° per tooth ≈ 740μs per tooth
TRACE_DATA_P TraceEvent kAudi135CamSyncEvents[] = {
    // Physical teeth 1-6, only teeth 3 and 6 counted
    {740U, TRACE_PRIMARY},    // Physical tooth 1 (toothSystemCount = 1)
    {740U, TRACE_PRIMARY},    // Physical tooth 2 (toothSystemCount = 2)
    {740U, TRACE_PRIMARY},    // Physical tooth 3 (toothSystemCount = 3, counted as effective tooth, but no sync yet)
    {740U, TRACE_PRIMARY},    // Physical tooth 4 (toothSystemCount = 1)
    {740U, TRACE_PRIMARY},    // Physical tooth 5 (toothSystemCount = 2)
    {740U, TRACE_PRIMARY},    // Physical tooth 6 (toothSystemCount = 3, counted)

    // Cam pulse establishes sync
    {1000U, TRACE_SECONDARY_RISING},  // Cam pulse: toothCurrentCount=0, toothSystemCount=3, hasSync=true

    // Next 3 physical teeth - first one wraps to tooth 1
    {740U, TRACE_PRIMARY},    // Physical tooth 7 (systemCount was 3, process as effective tooth 1)
    {740U, TRACE_PRIMARY},    // Physical tooth 8 (systemCount = 1)
    {740U, TRACE_PRIMARY},    // Physical tooth 9 (systemCount = 2)
};
