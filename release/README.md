# Release Artifacts

All artifacts in this folder are described by `release_manifest.json`.
Regenerate it any time with:

```
python tools/generate_release_manifest.py
```

Use the release files as matched sets. Mixing the production and experimental
files will break TunerStudio signature matching.

---

## Production (preferred) — `artifact_kind: production`, `preferred: true`

Flash these three files together:

- `speeduino-dropbear-v2.0.1-teensy41.hex`
- `speeduino-dropbear-v2.0.1.ini`
- `speeduino-dropbear-v2.0.1-base-tune.msq`

Expected firmware signature: `speeduino 202501-T41`

This is the normal byte-serialized TunerStudio contract for DropBear / Teensy 4.1.

---

## Experimental Native-U16 Page 2 — `artifact_kind: experimental`, `preferred: true`

Flash these three files together:

- `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental.hex`
- `speeduino-dropbear-v2.0.1-u16p2-experimental.ini`
- `speeduino-dropbear-v2.0.1-u16p2-experimental-base-tune.msq`

Expected firmware signature: `speeduino 202501-T41-U16P2`

This is an alternate experimental TunerStudio contract. Do not use it with the
production INI or an older production TunerStudio project.

---

## Diagnostic Firmware — `artifact_kind: diagnostic`

All files matching `*-diagnostic*.hex` are diagnostic builds. They are **not**
suitable for normal engine operation. Use them only for bench testing and serial
communication validation.

| File | Purpose |
|------|---------|
| `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental-diagnostic.hex` | Base diagnostic trace build |
| `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental-diagnostic-serialtrace.hex` | Serial trace enabled |
| `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental-diagnostic-trace.hex` | Full startup trace |
| `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental-diagnostic-posttriggers.hex` | Diagnostic returns after trigger attach |
| `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental-diagnostic-posttriggerattach.hex` | Diagnostic post trigger-attach only |
| `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental-diagnostic-triggerfix.hex` | Trigger fix diagnostic variant |

The minimal serial diagnostic program (for Airbear / Serial2 link verification)
is in `diagnostics/minimal_teensy41_serial/`. Flash
`minimal_teensy41_serial.ino.hex` to confirm the physical Serial2 link before
running production firmware.

---

## Important Note For Existing TunerStudio Projects

Teensy 4.1 now identifies itself with `speeduino 202501-T41`.

Older projects created against `speeduino 202501` will not attach cleanly after
flashing the newer Teensy firmware unless the project is updated to use the
matching Teensy 4.1 INI.

If TunerStudio reports a signature mismatch or refuses to connect:

1. Verify which `.hex` you flashed.
2. Verify the project is using the matching `.ini`.
3. If the project is older, create a fresh project with the correct INI and
   re-import the matching `.msq`.
