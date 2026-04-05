# Page Verification Protocol (FW-005)

External tools can verify that the ECU's current in-memory page state matches a
known-good tune by comparing a locally-computed CRC32 against the ECU's response.

---

## CRC Command

Both the legacy and modern TS framing expose the same CRC query.

### Legacy (`comms_legacy.cpp`)

| Direction | Bytes | Meaning |
|-----------|-------|---------|
| Tool → ECU | `'d'` | Command byte |
| Tool → ECU | `0x00 <pageNum>` | 2 payload bytes; byte 0 is ignored, byte 1 is the page number |
| ECU → Tool | 4 bytes, big-endian | CRC32 of the page |

Example for `veMapPage` (page 2):

```
→  64 00 02
←  XX XX XX XX   (4-byte big-endian CRC32)
```

### Modern TS framing (`comms.cpp`)

The same page number goes into `serialPayload[2]` of a standard TS `'d'` message.
The response is `0x00 <4-byte LE CRC32>` (5 bytes, RC_OK + little-endian CRC32).

---

## Serialization Mode Selection

`calculatePageCRC32(pageNum)` automatically selects the active serialization mode
via `getTunerStudioPageSerializationMode(pageNum)`:

| Mode | Enum value | When active | Page 2 (veMapPage) size |
|------|-----------|-------------|------------------------|
| Production | `TS_PAGE_SERIALIZATION_CURRENT_BYTES` | default | 288 bytes |
| Experimental U16P2 | `TS_PAGE_SERIALIZATION_NATIVE_U16` | DropBear + `TS_EXPERIMENTAL_NATIVE_U16_PAGE2` compile flag | 544 bytes |

In **production** mode the CRC covers each table value as a single byte (the
low byte of the internal `uint16_t`), plus axis values converted to bytes via
the domain-specific converter.  This matches the byte stream a TS session reads
via the `'r'`/`'p'` page-read commands.

In **U16P2** mode the CRC covers the full page copied to a flat buffer using
`copyTunerStudioPageValuesToBufferForMode(..., TS_PAGE_SERIALIZATION_NATIVE_U16)`.
Each table value occupies 2 bytes (little-endian), axes are still 1 byte each.

---

## How an External Tool Should Verify a Page

1. **Connect and issue a `'K'` capability query** (FW-003).  Read byte [3]
   (`feature_flags`) of the 39-byte response.

2. **Check `CAP_FEATURE_U16P2`** (bit 0 of `feature_flags`).
   - Clear → use `TS_PAGE_SERIALIZATION_CURRENT_BYTES` (288 bytes for veMapPage).
   - Set   → use `TS_PAGE_SERIALIZATION_NATIVE_U16`   (544 bytes for veMapPage).

3. **Serialize the locally-held page** using the matching mode into a flat byte
   buffer.

4. **Compute `~CRC32(buffer, size)`** using the FastCRC32 convention (seed = 0,
   final XOR = 0xFFFFFFFF).

5. **Send the `'d'` command** for the target page and receive the 4-byte
   big-endian CRC32 from the ECU.

6. **Compare**. A mismatch means the ECU page differs from the local copy or the
   wrong mode was used.

---

## `encodeLegacyCrc32Response` Wire Encoding

The legacy response encodes the CRC32 in **big-endian** order:

```
buffer[0] = (crc >> 24) & 0xFF;   // MSB
buffer[1] = (crc >> 16) & 0xFF;
buffer[2] = (crc >>  8) & 0xFF;
buffer[3] = (crc      ) & 0xFF;   // LSB
```

The modern TS response uses **little-endian** (standard TS convention).

---

## Page Numbers Reference

| Page constant | Number | Production size | U16P2 size |
|---------------|--------|----------------|------------|
| `veMapPage`   | 2      | 288 bytes      | 544 bytes  |
| `ignMapPage`  | 3      | 288 bytes      | 544 bytes  |
| `afrMapPage`  | 4      | 288 bytes      | 544 bytes  |
| `boostvvtPage`| 5      | 240 bytes      | 240 bytes (no table values in U16P2 scope) |

Only page-2 table values (VE, ign, AFR, fuelMap2, ignMap2) are widened in U16P2
mode; config-page-only pages are unaffected.

---

## Source References

| Symbol | File |
|--------|------|
| `calculatePageCRC32()` | `speeduino/page_crc.cpp` |
| `calculatePageCRC32ForMode()` | `speeduino/page_crc.cpp` |
| `encodeLegacyCrc32Response()` | `speeduino/comms_legacy.cpp` |
| `parseLegacyCrcRequest()` | `speeduino/comms_legacy.cpp` |
| `getTunerStudioPageSerializationMode()` | `speeduino/pages.cpp` |
| `CAP_FEATURE_U16P2` | `speeduino/comms_legacy.h` |
| Test coverage | `test/test_comms/test_page_verification.cpp` |
|               | `test/test_comms/test_page_serialization.cpp` |
