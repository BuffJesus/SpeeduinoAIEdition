#pragma once

#include "../trace_fixture.h"

// Harley: a shorter gap after sync maps to tooth 2, and the next longer gap returns to tooth 1.
TRACE_DATA_P TraceEvent kHarleyAlternatingEvents[] = {
    {5000U, TRACE_PRIMARY},
    {3000U, TRACE_PRIMARY},
    {5000U, TRACE_PRIMARY},
};
