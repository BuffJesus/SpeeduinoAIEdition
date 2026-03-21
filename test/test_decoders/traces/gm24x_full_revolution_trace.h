#pragma once

#include "../trace_fixture.h"

// GM 24X: one cam pulse establishes sync, then 24 primary teeth complete a revolution.
// A second cam pulse resets the count for the next revolution, and the next primary wraps to tooth 1.
TRACE_DATA_P TraceRepeatEvent kGm24xFullRevolutionEvents[] = {
    {1000U, TRACE_SECONDARY_RISING, 1U},
    {4200U, TRACE_PRIMARY, 24U},
    {1000U, TRACE_SECONDARY_RISING, 1U},
    {4200U, TRACE_PRIMARY, 1U},
};
