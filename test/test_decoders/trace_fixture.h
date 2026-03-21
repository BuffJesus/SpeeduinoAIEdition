#pragma once

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>
#if defined(PROGMEM)
#include <avr/pgmspace.h>
#endif

enum TraceEventKind : uint8_t
{
    TRACE_PRIMARY = 0U,
    TRACE_PRIMARY_WIDE = 1U,
    TRACE_SECONDARY_RISING = 2U,
    TRACE_SECONDARY_FALLING = 3U,
};

struct TraceEvent
{
    uint32_t deltaUs;
    TraceEventKind kind;
};

struct TriggerTrace
{
    const TraceEvent *events;
    size_t count;
};

struct TraceRepeatEvent
{
    uint32_t deltaUs;
    TraceEventKind kind;
    uint16_t repeatCount;
};

struct RepeatedTriggerTrace
{
    const TraceRepeatEvent *events;
    size_t count;
};

#if defined(PROGMEM)
#define TRACE_DATA_P static constexpr PROGMEM
#else
#define TRACE_DATA_P static constexpr
#endif

template <size_t N>
static inline TriggerTrace makeTriggerTrace(const TraceEvent (&events)[N])
{
    TriggerTrace trace = {events, N};
    return trace;
}

template <size_t N>
static inline RepeatedTriggerTrace makeRepeatedTriggerTrace(const TraceRepeatEvent (&events)[N])
{
    RepeatedTriggerTrace trace = {events, N};
    return trace;
}

struct TraceReplayCallbacks
{
    void (*primary)();
    void (*primaryWide)();
    void (*secondaryRising)();
    void (*secondaryFalling)();
};

static void __attribute__((noinline)) dispatchTraceEventKind(TraceEventKind kind, const TraceReplayCallbacks &callbacks)
{
    switch (kind)
    {
        case TRACE_PRIMARY:
            if (callbacks.primary != nullptr)
            {
                callbacks.primary();
            }
            break;

        case TRACE_PRIMARY_WIDE:
            if (callbacks.primaryWide != nullptr)
            {
                callbacks.primaryWide();
            }
            else if (callbacks.primary != nullptr)
            {
                callbacks.primary();
            }
            break;

        case TRACE_SECONDARY_RISING:
            if (callbacks.secondaryRising != nullptr)
            {
                callbacks.secondaryRising();
            }
            break;

        case TRACE_SECONDARY_FALLING:
            if (callbacks.secondaryFalling != nullptr)
            {
                callbacks.secondaryFalling();
            }
            break;

        default:
            break;
    }
}

static inline void replayTriggerTrace(const TriggerTrace &trace, const TraceReplayCallbacks &callbacks)
{
    for (size_t index = 0; index < trace.count; ++index)
    {
        TraceEvent event;
#if defined(PROGMEM)
        memcpy_P(&event, trace.events + index, sizeof(event));
#else
        event = trace.events[index];
#endif

        delayMicroseconds(event.deltaUs);
        dispatchTraceEventKind(event.kind, callbacks);
    }
}

static inline void replayRepeatedTriggerTrace(const RepeatedTriggerTrace &trace, const TraceReplayCallbacks &callbacks)
{
    for (size_t index = 0; index < trace.count; ++index)
    {
        TraceRepeatEvent event;
#if defined(PROGMEM)
        memcpy_P(&event, trace.events + index, sizeof(event));
#else
        event = trace.events[index];
#endif

        for (uint16_t repeatIndex = 0; repeatIndex < event.repeatCount; ++repeatIndex)
        {
            delayMicroseconds(event.deltaUs);
            dispatchTraceEventKind(event.kind, callbacks);
        }
    }
}
