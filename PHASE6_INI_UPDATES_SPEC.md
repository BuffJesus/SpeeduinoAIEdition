# Phase 6 TunerStudio INI Update Specification

**Purpose**: Document required TunerStudio INI file changes to support 16-bit table values on Teensy 4.1

**Status**: Specification only - INI files not modified in this phase

## Background

Phase 6.2 introduced 16-bit table values on Teensy 4.1 (via conditional `table3d_value_t = uint16_t`). The TunerStudio INI file must be updated to reflect the new page sizes so TunerStudio correctly interprets the binary data sent by the firmware.

**Key Finding from Slice A**: The comms send path (`send_table_values()` in comms_legacy.cpp:1220) already handles 16-bit values correctly via raw memory writes. The issue is that TunerStudio needs to know the new page sizes via INI updates.

## Pages Affected by 16-bit Table Values

Only pages containing 3D table **values** are affected. Axis values remain 1 byte (adequate resolution).

### Table Pages with Size Changes

| Page # | Page Name | Table | Old Size (8-bit) | New Size (16-bit) | Delta |
|--------|-----------|-------|------------------|-------------------|-------|
| 1 | veMapPage | fuelTable (16x16) | 288 bytes | 544 bytes | +256 |
| 3 | ignMapPage | ignitionTable (16x16) | 288 bytes | 544 bytes | +256 |
| 5 | afrMapPage | afrTable (16x16) | 288 bytes | 544 bytes | +256 |
| 7 | boostvvtPage | boostTable (8x8), vvtTable (8x8), stagingTable (8x8) | 240 bytes | 432 bytes | +192 |
| 8 | seqFuelPage | trim1-8 Tables (6x6 each, 8 tables) | 384 bytes | 672 bytes | +288 |
| 11 | fuelMap2Page | fuelTable2 (16x16) | 288 bytes | 544 bytes | +256 |
| 12 | wmiMapPage | wmiTable (8x8), vvt2Table (8x8), dwellTable (4x4) | 192 bytes | 336 bytes | +144 |
| 14 | ignMap2Page | ignitionTable2 (16x16) | 288 bytes | 544 bytes | +256 |
| 15 | boostLookupPage | boostTableLookupDuty (8x8) | 96 bytes | 160 bytes | +64 |

### Size Calculation Formula

For each table page:
```
new_size = old_config_bytes + sum(table_x_size * table_y_size * sizeof(table3d_value_t)) + sum(axis_sizes)
```

Where:
- `sizeof(table3d_value_t)` = 2 on Teensy 4.1, 1 on AVR
- Axis values remain 1 byte each (unchanged)
- Config bytes (non-table data) remain unchanged

### Example: veMapPage (Page 1)

**Old Layout (8-bit values)**:
- fuelTable values: 16 × 16 × 1 = 256 bytes
- fuelTable X-axis: 16 × 1 = 16 bytes
- fuelTable Y-axis: 16 × 1 = 16 bytes
- **Total**: 288 bytes

**New Layout (16-bit values on Teensy 4.1)**:
- fuelTable values: 16 × 16 × 2 = 512 bytes
- fuelTable X-axis: 16 × 1 = 16 bytes (unchanged)
- fuelTable Y-axis: 16 × 1 = 16 bytes (unchanged)
- **Total**: 544 bytes

## Config Pages (No Size Changes)

These pages contain only configuration bytes (no table values), so their sizes remain unchanged:

| Page # | Page Name | Size |
|--------|-----------|------|
| 2 | veSetPage | 128 bytes |
| 4 | ignSetPage | 128 bytes |
| 6 | afrSetPage | 128 bytes |
| 9 | canbusPage | 192 bytes |
| 10 | warmupPage | 192 bytes |
| 13 | progOutsPage | 128 bytes |

## Required INI Changes

### 1. Firmware Signature

The firmware signature must change to prevent TunerStudio from using an old INI with new firmware (or vice versa).

**Current signature** (example):
```ini
[MegaTune]
signature = "speeduino 202501.6"
```

**Recommended new signature**:
```ini
[MegaTune]
signature = "speeduino 202501.6 T41" ; Teensy 4.1 variant
```

**Alternative**: Use a conditional signature based on board capability flags if INI supports runtime conditionals.

### 2. Page Size Definitions

The INI file contains page size definitions that must be updated. These are typically in a `[Constants]` or page definition section.

**Example Current Definition**:
```ini
pageSize = 288  ; Page 1 (veMapPage)
```

**Conditional Update Needed**:
```ini
; Option 1: Separate INI files
pageSize = 544  ; Page 1 (veMapPage) - Teensy 4.1 variant INI

; Option 2: Runtime conditional (if supported by TunerStudio)
pageSize = { boardCapabilities & 0x0080 ? 544 : 288 }  ; Check BOARD_CAP_HIGH_RES_TABLES
```

**Note**: TunerStudio's conditional expression support should be verified. If not supported, separate INI files for Teensy 4.1 and AVR platforms are required.

### 3. Page Checksum Updates

If the INI includes page checksums or CRC values, these will need recalculation after size changes.

### 4. Table Value Scaling

Table values on Teensy 4.1 use 16-bit resolution (0-65535 range). The INI must specify the correct scaling factors.

**Example for VE Table**:
```ini
; Old (8-bit, 0-255 range)
table = fuelTable, "VE Table", 1
   ...
   field = "#", 0-255

; New (16-bit, 0-65535 range, Teensy 4.1 only)
table = fuelTable, "VE Table", 1
   ...
   field = "#", 0-65535  ; Or scaled to 0.0-655.35 for 0.01% resolution
```

**Recommended Scaling**: Use `0-65535` internal range, display as `0.0-655.35` with 0.01% VE resolution.

## Implementation Approach

### Option A: Separate INI Files (Recommended)

Create separate INI files:
- `speeduino.ini` - AVR platforms (existing 8-bit tables)
- `speeduino_teensy41.ini` - Teensy 4.1 (16-bit tables)

**Pros**:
- Clean separation, no conditional complexity
- Easier to maintain and debug
- Guaranteed compatibility with all TunerStudio versions

**Cons**:
- Must maintain two INI files
- Users must select correct INI for their board

### Option B: Single INI with Conditionals

Use TunerStudio's conditional expression support (if available):
```ini
#if TEENSY41
  pageSize1 = 544
#else
  pageSize1 = 288
#endif
```

**Pros**:
- Single file to maintain
- Automatic selection based on board detection

**Cons**:
- Requires TunerStudio conditional support verification
- More complex INI structure
- Potential compatibility issues with older TunerStudio versions

### Option C: Runtime Page Size Negotiation (Future)

Extend the comms protocol to allow firmware to send page sizes to TunerStudio at connection time.

**Pros**:
- Most flexible, supports any page size changes
- No INI edits required for page size changes

**Cons**:
- Requires TunerStudio protocol extension
- Not supported in current protocol
- Would require TunerStudio software update

## Verification Procedure

After INI updates:

1. **Size Verification**:
   - Send 'Q' command from TunerStudio
   - Verify received byte count matches new page sizes
   - Check all table pages (1, 3, 5, 7, 8, 11, 12, 14, 15)

2. **Table Value Round-Trip**:
   - Write known 16-bit value to table cell (e.g., 32768)
   - Read back value
   - Verify value matches (no truncation to 8-bit)

3. **Backward Compatibility**:
   - Flash AVR firmware with old INI
   - Verify 8-bit tables still work correctly
   - Ensure no INI changes affect AVR platforms

4. **Axis Verification**:
   - Verify axis values still read/write correctly (1 byte each)
   - Check RPM and Load axis bins

## Migration Notes

**For Users Upgrading from AVR to Teensy 4.1**:
- Table values will be automatically upscaled from 8-bit to 16-bit on first load
- Existing tunes remain compatible (values multiplied by 256 for 16-bit representation)
- Higher resolution tuning available after migration

**For Users Downgrading from Teensy 4.1 to AVR**:
- Table values will be downscaled from 16-bit to 8-bit
- Resolution loss expected (divide by 256, round down)
- Backup tune before downgrade recommended

## Code References

**Page Size Calculations**:
- `speeduino/pages.cpp:27` - `ini_page_sizes[]` array (hardcoded constants)
- `speeduino/pages.cpp:34-38` - `get_table_value_end()` template (accounts for `sizeof(table3d_value_t)`)

**Comms Send Path**:
- `speeduino/comms_legacy.cpp:1220` - `send_table_values()` (already handles 16-bit correctly)

**Table Type Definitions**:
- `speeduino/table3d_typedefs.h` - Conditional `table3d_value_t` typedef

**Board Capabilities**:
- `speeduino/globals.h:135` - `BOARD_CAP_HIGH_RES_TABLES` flag (0x0080)

## Next Steps

1. **Coordinate with TunerStudio/MegaLogViewer maintainers** to determine:
   - Conditional INI support availability
   - Preferred approach (separate files vs. conditionals)
   - Runtime page size negotiation feasibility

2. **Create Teensy 4.1 INI variant** with updated page sizes

3. **Test with TunerStudio**:
   - Verify connection and page reads
   - Test table value writes (16-bit range)
   - Validate axis value handling (still 1 byte)

4. **Document user-facing changes** in release notes:
   - New INI file requirement for Teensy 4.1
   - Higher resolution table values (0.01% VE resolution)
   - Migration procedure for existing tunes

## Appendix: Complete Page Size Table

| Page # | Name | AVR Size | Teensy 4.1 Size | Notes |
|--------|------|----------|-----------------|-------|
| 0 | (reserved) | 0 | 0 | Not used |
| 1 | veMapPage | 288 | 544 | 16×16 fuel table |
| 2 | veSetPage | 128 | 128 | Config only (no change) |
| 3 | ignMapPage | 288 | 544 | 16×16 ign table |
| 4 | ignSetPage | 128 | 128 | Config only (no change) |
| 5 | afrMapPage | 288 | 544 | 16×16 AFR table |
| 6 | afrSetPage | 128 | 128 | Config only (no change) |
| 7 | boostvvtPage | 240 | 432 | 3× 8×8 tables |
| 8 | seqFuelPage | 384 | 672 | 8× 6×6 trim tables |
| 9 | canbusPage | 192 | 192 | Config only (no change) |
| 10 | warmupPage | 192 | 192 | Config only (no change) |
| 11 | fuelMap2Page | 288 | 544 | 16×16 fuel table 2 |
| 12 | wmiMapPage | 192 | 336 | 2× 8×8 + 1× 4×4 tables |
| 13 | progOutsPage | 128 | 128 | Config only (no change) |
| 14 | ignMap2Page | 288 | 544 | 16×16 ign table 2 |
| 15 | boostLookupPage | 256 | 256 | Config (128) + 8×8 table (96→160) = 256→320? Needs verification |

**Note**: Page 15 size needs verification - the config portion and table portion sizing should be checked in code.
