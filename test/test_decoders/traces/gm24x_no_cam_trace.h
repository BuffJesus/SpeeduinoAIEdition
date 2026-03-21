#pragma once

#include "../trace_fixture.h"

// GM 24X cannot leave its unsynced sentinel without a cam pulse.
TRACE_DATA_P TraceRepeatEvent kGm24xNoCamEvents[] = {
    {4200U, TRACE_PRIMARY, 12U},
};
