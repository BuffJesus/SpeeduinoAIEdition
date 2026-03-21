#pragma once

#include "../trace_fixture.h"

// Subaru 6/7: a lone cam pulse before the next primary at tooth 2 is invalid and should realign the decoder to tooth 5 without syncing.
TRACE_DATA_P TraceEvent kSubaru67SingleCamMisalignedEvents[] = {
    {1000U, TRACE_SECONDARY_RISING},
    {1000U, TRACE_PRIMARY},
};
