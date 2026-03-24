# Teensy 4.1 PWM Fan Support Blocker

## Status: DEFERRED (Phase 6 Slice D)

PWM fan support remains disabled on Teensy 4.1 (`//#define PWM_FAN_AVAILABLE` in board_teensy41.h:35).

## Reason

**Timer Resource Conflict:** Enabling PWM fan without confirming timer availability could conflict with critical ignition/fuel scheduling timers.

### Current Teensy 4.1 Timer Usage

From `board_teensy41.cpp` and `board_teensy41.h`:

- **TMR1 (Quad Timer 1)**: Fuel channels 1-4
- **TMR2 (Quad Timer 2)**: Fuel channels 5-8
- **TMR3 (Quad Timer 3)**: Ignition channels 1-4
- **TMR4 (Quad Timer 4)**: Ignition channels 5-8
- **PIT (Periodic Interrupt Timers)**: Boost/VVT PWM (`boost_pwm_max_count`, `vvt_pwm_max_count` at board_teensy41.cpp:92-93)

### Available Timer Options (Not Yet Investigated)

Per FIRMWARE_ROADMAP.md line 116 and phase6_teensy41_enablement_roadmap.md line 62:

1. **GPT (General Purpose Timers)**: Teensy 4.1 has GPT1, GPT2
   - Could replace PIT timers for Boost/VVT, freeing PIT for fan PWM
   - Or directly use GPT for fan PWM
   - Requires investigation of GPT resolution and suitability

2. **FTM (FlexTimer Module)**: Available but not currently used
   - May have resolution/precision trade-offs vs Quad Timers

### Risk Assessment

**High Risk:** Enabling PWM fan without confirming timer availability could:
- Steal timer resources from ignition (safety-critical)
- Cause ignition timing jitter or missed events
- Introduce subtle timing bugs that only manifest under specific load conditions

**Per Working Rules:** "Do not compromise ignition safety for fan PWM" (line 50)

## Recommended Path Forward

1. **Audit Available Timers:**
   - Confirm GPT1/GPT2 availability and resolution
   - Check if FTM can provide suitable PWM frequency (typically 25-100 Hz for fan control)

2. **Prototype GPT-Based Fan PWM:**
   - Implement fan PWM using GPT1 (if available)
   - Verify no impact on existing ignition/fuel scheduling
   - Test under high-load conditions (8-cyl, sequential, all outputs active)

3. **Enable Only After Validation:**
   - Add `#define PWM_FAN_AVAILABLE` only after timer conflict confirmed resolved
   - Document which timer is used in board_teensy41.h comment

## Estimated Effort

- Timer audit: 2-4 hours (review Teensy 4.1 timer documentation, check existing usage)
- GPT implementation: 4-6 hours (adapt existing PIT-based PWM logic to GPT)
- Validation testing: 2-4 hours (bench test with all outputs active)

**Total: 8-14 hours**

## Decision for Phase 6 Session

**DEFER:** PWM fan enablement is lower priority than:
- 16-bit table values (user-visible resolution improvement) ✅ COMPLETE
- 12-bit ADC (sensor precision improvement) ✅ COMPLETE
- Board capability declarations (infrastructure for future features)

PWM fan can be addressed in a future Phase 6 continuation session focused on peripheral optimization.

## Files to Modify When Resolved

1. `speeduino/board_teensy41.h:35` — Uncomment `#define PWM_FAN_AVAILABLE`
2. `speeduino/board_teensy41.cpp` — Add GPT-based fan PWM initialization
3. `speeduino/auxiliaries.cpp` — Verify fan PWM logic works with new timer
4. This document — Update with resolution details
