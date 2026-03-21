#pragma once

#include "../trace_fixture.h"

// Audi 135: Resync behavior with useResync enabled
// Cam pulse during running resets tooth count back to 0
TRACE_DATA_P TraceEvent kAudi135ResyncEvents[] = {
    // Initial cam sync
    {1000U, TRACE_SECONDARY_RISING},

    // Several effective teeth (9 physical)
    {740U, TRACE_PRIMARY},   // Physical 1, effective tooth 1
    {740U, TRACE_PRIMARY},   // Physical 2
    {740U, TRACE_PRIMARY},   // Physical 3, systemCount=3, toothCurrentCount=2
    {740U, TRACE_PRIMARY},   // Physical 4
    {740U, TRACE_PRIMARY},   // Physical 5
    {740U, TRACE_PRIMARY},   // Physical 6, systemCount=3, toothCurrentCount=3
    {740U, TRACE_PRIMARY},   // Physical 7
    {740U, TRACE_PRIMARY},   // Physical 8
    {740U, TRACE_PRIMARY},   // Physical 9, systemCount=3, toothCurrentCount=4

    // Resync cam pulse (if useResync=1, resets to 0)
    {1000U, TRACE_SECONDARY_RISING},

    // Next effective tooth becomes tooth 1 again
    {740U, TRACE_PRIMARY},   // Physical 10, effective tooth 1 (after resync)
    {740U, TRACE_PRIMARY},   // Physical 11
    {740U, TRACE_PRIMARY},   // Physical 12, systemCount=3, toothCurrentCount=2
};
