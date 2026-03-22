# Speeduino forum evidence report

This report is generated from targeted public-forum searching and thread scraping.

## Trigger decoder and sync-behavior evidence

### Honda J32

#### J35a4 Triggers
- URL: https://speeduino.com/forum/viewtopic.php?t=6027#p65009
- Date: Mon Sep 25, 2023 1:15 am
- Who posted it: OldKamikaze
- Evidence type: scope capture
- Confidence: medium
- Summary: Unfortunately my ECU declined to steam pulses, but this is what oscilloscope gave me. See attached office file, making the cam gear
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: cam Matched decoder family: Honda J32 Image or capture links: 1 Attachments: signal.ods -> https://speeduino.com/forum/download/file.php?id=12116
- Matched search terms: Honda J32 Speeduino trigger

#### J35a4 Triggers
- URL: https://speeduino.com/forum/viewtopic.php?t=6027#p64990
- Date: Sat Sep 23, 2023 6:35 pm
- Who posted it: PSIG
- Evidence type: scope capture / wheel description
- Confidence: low
- Summary: [OldKamikaze](./memberlist.php?mode=viewprofile&u=7985) wrote: [↑](./viewtopic.php?p=64958#p64958) Thu Sep 21, 2023 9:05 pm ZCrank: 12+12+0 (No missing) Cam: 6+4+2 (2missing) I don't have that set of patterns on file. Your description does not appear to follow naming convention for + or - teeth, and does not translate to a valid pattern. Please provide a drawing, or image, or oscilloscope pattern, etc.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: cam Includes: crank Matched decoder family: Honda J32
- Matched search terms: Honda J32 Speeduino trigger

#### J35a4 Triggers
- URL: https://speeduino.com/forum/viewtopic.php?t=6027#p64996
- Date: Sat Sep 23, 2023 11:02 pm
- Who posted it: OldKamikaze
- Evidence type: scope capture / wheel description
- Confidence: low
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=64990#p64990) Sat Sep 23, 2023 6:35 pm [OldKamikaze](./memberlist.php?mode=viewprofile&u=7985) wrote: [↑](./viewtopic.php?p=64958#p64958) Thu Sep 21, 2023 9:05 pm ZCrank: 12+12+0 (No missing) Cam: 6+4+2 (2missing) I don't have that set of patterns on file. Your description does not appear to follow naming convention for + or - teeth, and does not translate to a valid pattern. Please provide a drawing, or image, or oscilloscope pattern, etc. Will do, have oscilloscope
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: cam Includes: crank Matched decoder family: Honda J32
- Matched search terms: Honda J32 Speeduino trigger

### Rover MEMS

#### rover MEMs decoder
- URL: https://speeduino.com/forum/viewtopic.php?t=1427#p51307
- Date: Wed Jun 23, 2021 1:34 am
- Who posted it: Trevor Getty
- Evidence type: tooth log / composite log / scope capture
- Confidence: high
- Summary: Hi mike good and bad news. so new vr now has crank and cam showing in composite logger see attached pics and logger output. now for some reason it logs and saves fine but i can not reopen without failure , see attached pic. finally i am getting wasted spark fine but no inj due to high sync loss i think.
- Why it matters: Provides replay-test-relevant trigger traces. Includes hardware-signal evidence, useful for edge and polarity interpretation. Discusses sync-loss conditions that can inform negative test cases.
- Replay-test details: Includes: cam Includes: crank Includes: sync Includes: sync loss Matched decoder family: Rover MEMS Image or capture links: 1 Attachments: T16-RoverMemsTesting.rar -> https://speeduino.com/forum/download/file.php?id=8530
- Matched search terms: Rover MEMS Speeduino trigger

#### rover MEMs decoder
- URL: https://speeduino.com/forum/viewtopic.php?t=1427#p21484
- Date: Sat Oct 07, 2017 2:57 pm
- Who posted it: dazq
- Evidence type: wheel description / maintainer explanation / image attachment
- Confidence: high
- Summary: the rover MEMs trigger pattern is like this(extract from manual) 32 poles spaced at 10 degree intervals, with 4 missing poles,at 30, 60,210 and 250 degrees. i dont think our generic decoder will do this one ? The MEMs was fitted to a lot of rover engines , 4cyl to V8 i have one for testing with its oem ecu in the car and would like to speedy it (im sampling the signals to the gearboc tcu as well before removing the oem engine ecu). Could we do this??
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Pattern references: rover MEMs Image or capture links: 4 Attachments: mems 2 crank nn cam.pdf -> https://speeduino.com/forum/download/file.php?id=2590; mems1.9 trigger description.pdf -> https://speeduino.com/forum/download/file.php?id=2589; mems3 crank and cam.pdf -> https://speeduino.com/forum/download/file.php?id=2585
- Matched search terms: Rover MEMS Speeduino trigger

#### rover MEMs decoder
- URL: https://speeduino.com/forum/viewtopic.php?t=1427#p51340
- Date: Wed Jun 23, 2021 5:05 pm
- Who posted it: Trevor Getty
- Evidence type: composite log / scope capture / wheel description
- Confidence: medium
- Summary: Regarding posts above: Without things connected run hardware test and check lights light up. (With connected you risk injectors filling your cylinders with fuel) I have done this and the injectors turn on fine - leds fire and I can see output no probs. Code: [Select all](#) ``` Will look later, until then done standard questions, when cranking generating the logs did you have the spark plugs out? Are the cables shielded cables?
- Why it matters: Provides replay-test-relevant trigger traces. Includes hardware-signal evidence, useful for edge and polarity interpretation. Discusses sync-loss conditions that can inform negative test cases.
- Replay-test details: Includes: rising edge Includes: cam Includes: crank Includes: sync Includes: sync loss Matched decoder family: Rover MEMS
- Matched search terms: Rover MEMS Speeduino trigger

#### rover MEMs decoder
- URL: https://speeduino.com/forum/viewtopic.php?t=1427#p51343
- Date: Wed Jun 23, 2021 5:28 pm
- Who posted it: Trevor Getty
- Evidence type: tooth log / scope capture
- Confidence: medium
- Summary: Quick PS note. I have looked at my scope picture, and I am going to make sure the VR+ and VR- aren't mixed up. I see that as the VR approaches voltage should increase and as it leaves it should decrease - we see this for the crank signal. On cam it seems inverted - going down then up per tooth logger - so I will quickly confirm which is going into speedy.
- Why it matters: Provides replay-test-relevant trigger traces. Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: cam Includes: crank Matched decoder family: Rover MEMS
- Matched search terms: Rover MEMS Speeduino trigger

## Strongest evidence by decoder

- **Honda J32**: [J35a4 Triggers](https://speeduino.com/forum/viewtopic.php?t=6027#p65009) — OldKamikaze, Mon Sep 25, 2023 1:15 am, confidence medium.
- **Rover MEMS**: [rover MEMs decoder](https://speeduino.com/forum/viewtopic.php?t=1427#p51307) — Trevor Getty, Wed Jun 23, 2021 1:34 am, confidence high.

## Decoders with enough information to build replay tests safely

- Rover MEMS

## Decoders still blocked by missing real trace evidence

- Honda J32

## Suggested next priorities for test coverage

- Honda J32: add edge and polarity-focused tests derived from scope evidence.
- Rover MEMS: convert existing trace-backed maintainer guidance into replay tests first.

## JSON export

- Structured JSON dataset written to: `C:\Users\Cornelio\Desktop\speeduino-202501.6\Resources\speeduino_forum_evidence.json`
