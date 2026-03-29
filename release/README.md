# Release Artifact Pairing

Use the release files as matched sets. Mixing the production and experimental files will break TunerStudio signature matching.

## Production DropBear / Teensy 4.1

Use these three files together:

- `speeduino-dropbear-v2.0.1-teensy41.hex`
- `speeduino-dropbear-v2.0.1.ini`
- `speeduino-dropbear-v2.0.1-base-tune.msq`

Expected firmware signature:

- `speeduino 202501-T41`

This is the normal byte-serialized TunerStudio contract.

## Experimental Native-U16 Page 2

Use these three files together:

- `speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental.hex`
- `speeduino-dropbear-v2.0.1-u16p2-experimental.ini`
- `speeduino-dropbear-v2.0.1-u16p2-experimental-base-tune.msq`

Expected firmware signature:

- `speeduino 202501-T41-U16P2`

This is an alternate experimental TunerStudio contract. Do not use it with the production INI or an older production TunerStudio project.

## Important Note For Existing TunerStudio Projects

Teensy 4.1 now identifies itself with:

- `speeduino 202501-T41`

Older projects that were created against:

- `speeduino 202501`

will not attach cleanly after flashing the newer Teensy firmware unless the project is updated to use the matching Teensy 4.1 INI.

If TunerStudio reports a signature mismatch or refuses to connect:

1. verify which `.hex` you flashed
2. verify the project is using the matching `.ini`
3. if the project is older, create a fresh project with the correct INI and re-import the matching `.msq`
