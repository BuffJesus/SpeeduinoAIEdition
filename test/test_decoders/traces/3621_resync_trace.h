#pragma once

#include "../trace_fixture.h"

TRACE_DATA_P TraceRepeatEvent k3621ResyncEvents[] = {
    {2000U, TRACE_PRIMARY, 2U},
    {4000U, TRACE_PRIMARY, 1U},
    {2000U, TRACE_PRIMARY, 15U}, // 15 more teeth after single-gap tooth: 1+15 = 16 total (real 16-tooth run, count 19->34)
    {7000U, TRACE_PRIMARY, 1U},
};
