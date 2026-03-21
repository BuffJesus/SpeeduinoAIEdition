#pragma once

#include "../trace_fixture.h"

TRACE_DATA_P TraceEvent kK6aSyncNoiseEvents[] = {
    {3000U, TRACE_PRIMARY},
    {2000U, TRACE_PRIMARY},
    {1000U, TRACE_PRIMARY},
    {4000U, TRACE_PRIMARY},
    {2000U, TRACE_PRIMARY},
    {50U, TRACE_PRIMARY},
    {2000U, TRACE_PRIMARY},
};
