#pragma once

#include "../trace_fixture.h"

// 4G63 primary edges alone should not establish sync because the cam phase never marks a valid tooth.
TRACE_DATA_P TraceEvent k4G63NoCamEvents[] = {
    {19000U, TRACE_PRIMARY},
    {30000U, TRACE_PRIMARY_WIDE},
    {19000U, TRACE_PRIMARY},
    {30000U, TRACE_PRIMARY_WIDE},
};
