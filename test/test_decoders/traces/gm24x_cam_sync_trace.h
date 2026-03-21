#pragma once

#include "../trace_fixture.h"

// GM 24X: primary edges do nothing until a cam pulse resets toothCurrentCount to 0.
// The next primary becomes tooth 1 and establishes sync.
// At 600 RPM: 100 ms per revolution, about 4.17 ms between 24 crank teeth.
TRACE_DATA_P TraceEvent kGm24xCamSyncEvents[] = {
    {4200U, TRACE_PRIMARY},
    {4200U, TRACE_PRIMARY},
    {1000U, TRACE_SECONDARY_RISING},
    {4200U, TRACE_PRIMARY},
    {4200U, TRACE_PRIMARY},
};
