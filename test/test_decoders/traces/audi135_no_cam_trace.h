#pragma once

#include "../trace_fixture.h"

// Audi 135: Primary-only trace without cam signal
// Demonstrates that decoder increments toothSystemCount but doesn't process effective teeth without sync
// toothCurrentCount stays at UINT8_MAX (255) until cam pulse
TRACE_DATA_P TraceRepeatEvent kAudi135NoCamEvents[] = {
    // 12 physical teeth (4 groups of 3) without cam
    // Without sync, decoder updates toothSystemCount but doesn't increment toothCurrentCount
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 1-3
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 4-6
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 7-9
    {740U, TRACE_PRIMARY, 3U},   // Physical teeth 10-12
    // hasSync should remain false, toothCurrentCount should stay at 255
};
