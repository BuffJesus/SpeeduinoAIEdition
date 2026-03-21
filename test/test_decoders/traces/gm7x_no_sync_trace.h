#pragma once

#include "../trace_fixture.h"

// GM 7X: uniform gaps should never hit the short-gap sync condition.
TRACE_DATA_P TraceEvent kGm7xNoSyncEvents[] = {
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
    {10000U, TRACE_PRIMARY},
};
