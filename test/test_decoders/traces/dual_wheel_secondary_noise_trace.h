#pragma once

#include "../trace_fixture.h"

TRACE_DATA_P TraceEvent kDualWheelSecondaryNoiseEvents[] = {
    {6000U, TRACE_SECONDARY_RISING},
    {1000U, TRACE_SECONDARY_RISING},
    {3000U, TRACE_PRIMARY},
};
