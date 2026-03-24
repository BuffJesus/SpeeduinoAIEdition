# SPI Flash Page Serialization Approach

**Status**: Infrastructure complete, full implementation deferred to future phase

## Current State (Phase 6)

**Completed**:
- SPI flash initialization infrastructure in storage.cpp (initTeensyStorage)
- storage_spi.h/cpp with LittleFS backend and page-level API
- Initialization calls in loadConfig() and loadCalibration()
- EEPROM fallback if SPI flash init fails

**Not Implemented**:
- Page serialization to/from byte buffers
- Integration of serialization with writeConfig()/loadConfig()
- EEPROM-to-SPI-flash migration on first boot

## Serialization Strategy

### High-Level Approach

Use the existing page iterator system (page_begin/advance) that the comms layer uses to ensure byte layout matches TunerStudio expectations.

### Functions Needed

1. **serializePageToBuffer(pageNum, buffer, bufferSize)**
   - Iterate entities with page_begin(pageNum) and advance()
   - For each entity:
     - If Raw: memcpy directly to buffer
     - If Table: serialize values, then X-axis, then Y-axis (same order as sendPage)
   - Return bytes written

2. **deserializePageFromBuffer(pageNum, buffer, bufferSize)**
   - Iterate entities with page_begin(pageNum) and advance()
   - For each entity:
     - If Raw: memcpy from buffer
     - If Table: deserialize values, then axes
   - Validate CRC if available

3. **Integration into writeConfig(pageNum)**
   ```cpp
   #if defined(CORE_TEENSY41)
     if (g_useSPIFlash) {
       uint8_t buffer[getPageSize(pageNum)];
       uint16_t size = serializePageToBuffer(pageNum, buffer, sizeof(buffer));
       if (!saveConfigToFlash(pageNum, buffer, size)) {
         // Fall back to EEPROM on error
       } else {
         return; // Success
       }
     }
   #endif
   // Existing EEPROM path continues...
   ```

4. **Integration into loadConfig()**
   ```cpp
   #if defined(CORE_TEENSY41)
     initTeensyStorage();
     if (g_useSPIFlash) {
       uint8_t buffer[largest_page_size];
       if (loadConfigFromFlash(pageNum, buffer, sizeof(buffer))) {
         deserializePageFromBuffer(pageNum, buffer, getPageSize(pageNum));
         return; // Success
       }
       // On first boot (file not found), fall through to EEPROM load
       // then immediately save to SPI flash to seed it
     }
   #endif
   // Existing EEPROM load...
   ```

### Complexity Assessment

**Estimated effort**: 2-3 sessions
- Session 1: Implement buffer serialization functions and unit tests
- Session 2: Integrate with storage.cpp, test page round-trips
- Session 3: Migration logic and hardware validation

**Risk**: Medium
- Page layout must exactly match TunerStudio expectations (tested via comms tests)
- Mixed 8-bit/16-bit data in same page requires careful sizeof handling
- First-boot migration from EEPROM needs careful testing

**Blocking factors**: None - infrastructure exists, just needs implementation time

## Deferred Rationale

**Why defer**:
1. TunerStudio INI and firmware signature are higher priority (enables tuning)
2. Config pages currently work via EEPROM (functional but not optimal)
3. SPI flash infrastructure is ready but needs dedicated testing time
4. ISR optimization and validation plan deliver more immediate value

**When to resume**:
- After Phase 6 validation with hardware
- When dedicated bench testing time is available
- After TunerStudio confirms 16-bit table values work correctly

## Testing Requirements

When implemented, must verify:
1. Page round-trip: write values via TS, power cycle, read back
2. All page types serialize correctly (config-only, table-only, mixed)
3. Migration from EEPROM to SPI flash preserves tune
4. SPI flash failure falls back to EEPROM gracefully
5. Tune bank switching works with all pages
6. Page CRC validation catches corruption

## Code References

- sendPage() in comms_legacy.cpp:1276 - reference implementation for entity iteration
- send_entity() in comms_legacy.cpp - per-entity send logic
- page_begin() / advance() in pages.cpp - entity iteration API
- storage_spi.h - SPI flash API (loadConfigFromFlash, saveConfigToFlash)
- getPageSize() in pages.cpp - page size calculation with sizeof(table3d_value_t)
