# Speeduino forum evidence report

This report is generated from targeted public-forum searching and thread scraping.

## Roadmap evidence

### Knock and pin/default policy

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?p=73386#p73386
- Date: Sat Oct 18, 2025 12:17 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [PVK](./memberlist.php?mode=viewprofile&u=10031) wrote: [↑](./viewtopic.php?p=73385#p73385) Fri Oct 17, 2025 9:40 pm I see now that Knock control is a feature of 202501: [https://github.com/speeduino/speeduino ... tag/202501](https://github.com/speeduino/speeduino/releases/tag/202501) Ik don't understand that I can't select the right Tunerstudio pin... As it says in that version , initial implementation, it is still under development by josh , that's what I meant by it not being a feature , as it's still WIP Suggest you post an issue on GitHub for the pin thing to be looked into (assuming it is not already logged)
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?p=73387#p73386
- Date: Sat Oct 18, 2025 12:17 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [PVK](./memberlist.php?mode=viewprofile&u=10031) wrote: [↑](./viewtopic.php?p=73385#p73385) Fri Oct 17, 2025 9:40 pm I see now that Knock control is a feature of 202501: [https://github.com/speeduino/speeduino ... tag/202501](https://github.com/speeduino/speeduino/releases/tag/202501) Ik don't understand that I can't select the right Tunerstudio pin... As it says in that version , initial implementation, it is still under development by josh , that's what I meant by it not being a feature , as it's still WIP Suggest you post an issue on GitHub for the pin thing to be looked into (assuming it is not already logged)
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?p=73386#p73377
- Date: Fri Oct 17, 2025 11:47 am
- Who posted it: PVK
- Evidence type: technical discussion
- Confidence: low
- Summary: I have a Speeduino Dropbear V2 and I want to add a Knock sensor with an external controller that puts out a digital High/Low signal. As seen here: [https://wiki.speeduino.com/en/boards/dropbear](https://wiki.speeduino.com/en/boards/dropbear) there is no dedicated pin available so I have to use one of the spare pins: Spare Digital 1: tunerstudio pin 35 Spare Digital 2: tunerstudio pin 34 Spare Analog 1 : Tunerstudio pin A16 Spare Analog 2 : Tunerstudio pin A17 The problem is that in Tunerstudio the pin that can be selected are: digital pins 2,3,18,19 and 20 or analog pins A0 - A15. I don’t understand that the firmware has the option for Knock sense but the right pin can’t be selected.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?p=73387#p73377
- Date: Fri Oct 17, 2025 11:47 am
- Who posted it: PVK
- Evidence type: technical discussion
- Confidence: low
- Summary: I have a Speeduino Dropbear V2 and I want to add a Knock sensor with an external controller that puts out a digital High/Low signal. As seen here: [https://wiki.speeduino.com/en/boards/dropbear](https://wiki.speeduino.com/en/boards/dropbear) there is no dedicated pin available so I have to use one of the spare pins: Spare Digital 1: tunerstudio pin 35 Spare Digital 2: tunerstudio pin 34 Spare Analog 1 : Tunerstudio pin A16 Spare Analog 2 : Tunerstudio pin A17 The problem is that in Tunerstudio the pin that can be selected are: digital pins 2,3,18,19 and 20 or analog pins A0 - A15. I don’t understand that the firmware has the option for Knock sense but the right pin can’t be selected.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?p=73386#p73378
- Date: Fri Oct 17, 2025 12:11 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: Knock sense is not yet an actual feature as it is still under development at this time.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?p=73387#p73378
- Date: Fri Oct 17, 2025 12:11 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: Knock sense is not yet an actual feature as it is still under development at this time.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

## Strongest evidence by roadmap area

- **Knock and pin/default policy**: [Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio](https://speeduino.com/forum/viewtopic.php?p=73386#p73386) — dazq, Sat Oct 18, 2025 12:17 am, confidence low.

## Unresolved contradictions

- No obvious contradictions were detected automatically; review medium-confidence entries manually.

## Places the forum suggests code changes, tune changes, test additions, or documentation updates

- Code and documentation review: verify knock-pin enumerations, board pin exposure, and packaged tune defaults for board variants.

## JSON export

- Structured JSON dataset written to: `C:\Users\Cornelio\Desktop\speeduino-202501.6\Resources\speeduino_forum_evidence.json`
