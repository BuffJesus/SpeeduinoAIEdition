#pragma once

#include "../trace_fixture.h"

TRACE_DATA_P TraceRepeatEvent kFordSt170CamNoiseEvents[] = {
    {2000U, TRACE_SECONDARY_RISING, 1U},
    {50U, TRACE_SECONDARY_RISING, 1U},
    {2000U, TRACE_SECONDARY_RISING, 1U},
    {2000U, TRACE_PRIMARY, 35U},
    {4000U, TRACE_PRIMARY, 1U},
};
