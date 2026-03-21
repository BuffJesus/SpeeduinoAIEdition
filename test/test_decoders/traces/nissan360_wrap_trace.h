#pragma once

#include "../trace_fixture.h"

TRACE_DATA_P TraceRepeatEvent kNissan360WrapEvents[] = {
    {2000U, TRACE_SECONDARY_RISING, 1U},
    {1000U, TRACE_PRIMARY, 16U},
    {2000U, TRACE_SECONDARY_FALLING, 1U},
    {1000U, TRACE_PRIMARY, 345U},
};
