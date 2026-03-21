#pragma once

#include "../trace_fixture.h"

// Audi 135: Full revolution showing tooth decimation (every 3rd tooth counted)
// 135 physical teeth per revolution, 45 effective teeth
// Uses compressed format - each group of 3 physical teeth produces 1 effective tooth
TRACE_DATA_P TraceRepeatEvent kAudi135FullRevolutionEvents[] = {
    // Cam pulse to establish sync
    {1000U, TRACE_SECONDARY_RISING, 1U},

    // First 9 effective teeth (27 physical teeth) - demonstrates decimation
    // Each effective tooth is 3 physical teeth at 740μs each = 2220μs total
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 1-3 → effective tooth 1
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 4-6 → effective tooth 2
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 7-9 → effective tooth 3
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 10-12 → effective tooth 4
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 13-15 → effective tooth 5
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 16-18 → effective tooth 6
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 19-21 → effective tooth 7
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 22-24 → effective tooth 8
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 25-27 → effective tooth 9
};
