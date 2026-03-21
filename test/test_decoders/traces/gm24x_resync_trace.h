#pragma once

#include "../trace_fixture.h"

// GM 24X: a later cam pulse forces toothCurrentCount back to 0.
// The following primary becomes tooth 1 again.
TRACE_DATA_P TraceEvent kGm24xResyncEvents[] = {
    {1000U, TRACE_SECONDARY_RISING},
    {4200U, TRACE_PRIMARY},
    {4200U, TRACE_PRIMARY},
    {4200U, TRACE_PRIMARY},
    {4200U, TRACE_PRIMARY},
    {1000U, TRACE_SECONDARY_RISING},
    {4200U, TRACE_PRIMARY},
};
