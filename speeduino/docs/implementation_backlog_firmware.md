# Firmware Backlog

- [x] FW-001 Release manifest generator
  Summary: Generate `release/release_manifest.json` from the `release` folder.
  Deliverables:
  - manifest schema
  - generator script
  - deterministic output
  Acceptance criteria:
  - every shipped `.hex` has a manifest entry
  - each entry includes `board_family`, `firmware_signature`, `artifact_kind`, `paired_ini`, `paired_msq`, `preferred`
  - generator can be rerun without manual edits
  Files:
  - new tool under `tools`
  - `release`
  Estimate:
  - `0.5-1 day`
  Dependencies:
  - none

- [x] FW-002 Release artifact classification cleanup
  Summary: Explicitly mark production, experimental, and diagnostic artifacts.
  Deliverables:
  - manifest metadata
  - README alignment
  Acceptance criteria:
  - diagnostic builds are flagged as diagnostic
  - one preferred production build is marked
  - one preferred experimental build is marked
  - README matches manifest
  Files:
  - `release/README.md`
  - manifest generator/output
  Estimate:
  - `0.5 day`
  Dependencies:
  - `FW-001`

- [x] FW-003 Capability query protocol
  Summary: Add a compact serial capability response for external tools.
  Deliverables:
  - wire format
  - command handler
  - compatibility note
  Acceptance criteria:
  - returns firmware signature
  - returns stable board id
  - returns output-channel size/version
  - returns board capability flags
  - returns flash-health support flag
  - returns `runtimeStatusA` support flag
  - returns experimental page-mode flags
  - older clients still connect normally
  Files:
  - `comms.cpp`
  - possibly `comms_legacy.cpp`
  - supporting headers/docs
  Estimate:
  - `1-2 days`
  Dependencies:
  - none

- [x] FW-004 Stable board identifier
  Summary: Expose a board id enum instead of relying on string parsing.
  Deliverables:
  - board id constants
  - mapping for current supported boards
  Acceptance criteria:
  - DropBear Teensy 4.1 is uniquely identifiable
  - board id is available through capability query
  Files:
  - board/globals/comms helpers
  Estimate:
  - `0.5-1 day`
  Dependencies:
  - `FW-003 preferred`

- [x] FW-005 External page verification path
  Summary: Expose page verification for external tools.
  Deliverables:
  - CRC query or equivalent page verification command
  - docs for production and experimental page modes
  Acceptance criteria:
  - external tool can verify a page against ECU state
  - CRC/readback semantics are documented against TS-visible serialization
  - experimental `U16` page 2 is covered
  Files:
  - `page_crc.cpp`
  - comms layer
  - docs/handoff notes
  Estimate:
  - `1-2 days`
  Dependencies:
  - `FW-003`

- [x] FW-006 Live-data contract metadata
  Summary: Make output-channel contract machine-readable.
  Deliverables:
  - packet size/version metadata
  - optional channel presence metadata
  Acceptance criteria:
  - tools can learn `ochBlockSize` equivalent without parsing INI
  - bytes `130`, `131`, `147` are machine-discoverable
  Files:
  - logger/comms/live-data docs and/or capability response
  Estimate:
  - `0.5-1 day`
  Dependencies:
  - `FW-003`

- [x] FW-007 Experimental mode metadata
  Summary: Expose `U16P2` experimental mode as a feature flag.
  Deliverables:
  - feature flag in capability response and/or manifest
  Acceptance criteria:
  - external tools do not need to infer experimental mode only from signature text
  Files:
  - capability response and/or manifest generator
  Estimate:
  - `0.5 day`
  Dependencies:
  - `FW-001 or FW-003`

# Tuner Backlog Dependency Reference

These firmware items are intended to support the tuner backlog in the separate tuner repo.

# Sprint Plan

## Sprint 1: Packaging And Safety

Goal: Remove the highest-risk operator errors in firmware selection and connection handling.

Scope:

- `FW-001`
- `FW-002`

Estimate:

- `4-6 days`

Exit criteria:

- manifest exists
- tuner consumes it
- diagnostic firmware no longer pollutes normal selection
- failed connects leave clean state

## Sprint 2: Capability Handshake

Goal: Make the tuner trust firmware-advertised facts instead of string heuristics.

Scope:

- `FW-003`
- `FW-004`
- `FW-006`
- `FW-007`

Estimate:

- `4-5 days`

Exit criteria:

- connect path retrieves capability metadata
- session stores capabilities
- UI/services can use actual board id and feature flags

## Sprint 3: Verification

Goal: Make "verified" actually mean something.

Scope:

- `FW-005`

Estimate:

- `3-4 days`

Exit criteria:

- page verification works for production path
- expected behavior for experimental `U16P2` is documented and test-covered
- sync state distinguishes verified vs unsupported

## Sprint 4: UX Integration

Goal: Use the new firmware contract to improve daily tuning and bench workflows.

Scope:

- depends on tuner repo items `TN-008` and `TN-007`

Estimate:

- `2-4 days`

Exit criteria:

- runtime UI shows `runtimeStatusA` and board capability data
- telemetry/capability features are not inferred from naming
- endianness contract is explicit and clean

# Dependency Map

- `FW-001 -> TN-002 -> TN-003 -> TN-004`
- `FW-003 -> FW-004/FW-006/FW-007 -> TN-006 -> TN-008`
- `FW-005 -> TN-005`
- `TN-001` is independent and should be done immediately
- `TN-007` is independent and can be slotted wherever convenient

# Recommended Execution Order

1. `TN-001`
2. `FW-001`
3. `FW-002`
4. `TN-002`
5. `TN-003`
6. `TN-004`
7. `FW-003`
8. `FW-004`
9. `FW-006`
10. `FW-007`
11. `TN-006`
12. `FW-005`
13. `TN-005`
14. `TN-008`
15. `TN-007`
