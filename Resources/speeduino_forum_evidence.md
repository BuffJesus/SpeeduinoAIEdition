# Speeduino forum evidence report

This report is generated from targeted public-forum searching and thread scraping.

## Roadmap evidence

### Decoder and trigger-pattern evidence

#### HONDA TRANSALP XL650V
- URL: https://speeduino.com/forum/viewtopic.php?t=7157#p73072
- Date: Sun Sep 07, 2025 8:52 am
- Who posted it: Tutajjestem
- Evidence type: tooth log / composite log / scope capture / wheel description
- Confidence: high
- Summary: [jonbill](./memberlist.php?mode=viewprofile&u=1981) wrote: [↑](./viewtopic.php?p=73071#p73071) Sun Sep 07, 2025 7:35 am I doubt whether the trigger wheel pattern and its relationship to the firing pattern are the cause of the problems. Sync loss means the pattern read by the sensor is not matching the expected pattern. This can be caused by noise, runout, too much variation in crank speed causing a timeout waiting to read the next tooth etc.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 14-5, 36-1, 8-1 Signal roles: cam mentioned, crank mentioned Explicit sync-loss discussion present.
- Matched search terms: composite logger

#### Messing with Subaru 6/7 trigger
- URL: https://speeduino.com/forum/viewtopic.php?t=4175#p72272
- Date: Sat Jun 28, 2025 10:08 pm
- Who posted it: ImprezaRSC
- Evidence type: tooth log / composite log / scope capture
- Confidence: high
- Summary: Hi, guys. I'll be noodling with this. I have the full version of TunerStudio and a pocket oscilloscope.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: Subaru 6/7

#### HONDA TRANSALP XL650V
- URL: https://speeduino.com/forum/viewtopic.php?t=7157#p73068
- Date: Sat Sep 06, 2025 5:49 pm
- Who posted it: Tutajjestem
- Evidence type: scope capture / wheel description
- Confidence: high
- Summary: Engine Specs: Model: Honda Transalp 650 Configuration: V-twin, 52° cylinder angle Ignition: Odd-fire, 232° firing interval Trigger Wheel / Crank Sensor: Trigger pattern: 14-5 (14 teeth total, 5 missing) — 9 physical teeth and 5 gaps Trigger wheel rotates clockwise Ignition for the first cylinder (front cylinder) according to the mark on the magnetic wheel: 10° BTDC aligns exactly with tooth #9 Ignition for the second cylinder (rear cylinder) according to the mark on the magnetic wheel: 10° BTDC aligns exactly with tooth #4 Service manual specifies: first cylinder ignition at 232° and second cylinder ignition at 488° This creates an asymmetric odd-fire pattern that must be correctly mapped in Speeduino for accurate ignition timing. Trigger Wheel Explanation (Honda Transalp 650) The engine uses a 14-5 trigger wheel as the crank position reference. The wheel is divided into 14 equal segments, each covering ≈25.71° of crankshaft rotation.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 14-5, 36-1, 8-1 Signal roles: crank mentioned Attachments: DataLogs.zip -> https://speeduino.com/forum/download/file.php?id=14402 Image or capture links found: 1
- Matched search terms: composite logger

#### [strange project] Turbo Honda sh 150cc
- URL: https://speeduino.com/forum/viewtopic.php?t=7236#p73958
- Date: Fri Jan 16, 2026 11:13 pm
- Who posted it: PSIG
- Evidence type: tooth log / scope capture / wheel description
- Confidence: high
- Summary: I see your tooth log showing only 8 teeth and 3 missing, not 9 (12-3). From this I assume your decoder is missing tooth #1 for the reasons posted earlier. I also see in your last run-log 254 sync losses.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 12-3 Signal roles: crank mentioned Explicit sync-loss discussion present.
- Matched search terms: sync loss

#### [strange project] Turbo Honda sh 150cc
- URL: https://speeduino.com/forum/viewtopic.php?t=7236#p73504
- Date: Thu Nov 06, 2025 9:06 pm
- Who posted it: Alessio
- Evidence type: tooth log / scope capture
- Confidence: high
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=73487#p73487) Wed Nov 05, 2025 2:28 am Please take a Tooth Log cranking the engine. TS Tooth Logger.png The MAX9926 VR conditioner chip often does not do well with certain missing tooth gaps. It is a smart chip, but can be too smart, and can see the end of the gap with a large signal.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: crank mentioned
- Matched search terms: sync loss

#### [strange project] Turbo Honda sh 150cc
- URL: https://speeduino.com/forum/viewtopic.php?t=7236#p73487
- Date: Wed Nov 05, 2025 2:28 am
- Who posted it: PSIG
- Evidence type: tooth log / scope capture
- Confidence: high
- Summary: Please take a Tooth Log cranking the engine. TS Tooth Logger.png (18.47 KiB) Viewed 37039 times The MAX9926 VR conditioner chip often does not do well with certain missing tooth gaps. It is a smart chip, but can be too smart, and can see the end of the gap with a large signal.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: crank mentioned
- Matched search terms: sync loss

#### Speeduino Beetle
- URL: https://speeduino.com/forum/viewtopic.php?t=917#p72823
- Date: Thu Aug 14, 2025 4:13 pm
- Who posted it: michaellatini
- Evidence type: tooth log / composite log
- Confidence: high
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=72805#p72805) Thu Aug 14, 2025 12:55 am [michaellatini](./memberlist.php?mode=viewprofile&u=5673) wrote: [↑](./viewtopic.php?p=72801#p72801) Wed Aug 13, 2025 9:11 pm Would that mean that with speeduino the cam sync should be timed to cylinder 1 TDC??? Yes, but to confirm we are on the same page... Cam sync is the signal to begin the 720° 4-stroke cycle at TDC Cyl1.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Edge or poll details: rising, change Signal roles: cam mentioned, crank mentioned
- Matched search terms: composite logger

#### Sync code question
- URL: https://speeduino.com/forum/viewtopic.php?t=7359#p74149
- Date: Tue Feb 24, 2026 12:59 am
- Who posted it: Banjo
- Evidence type: tooth log / composite log / wheel description
- Confidence: high
- Summary: I'm having lots of problems understanding the "Synching" & "Re-Synching" approach of the code, when it comes to resynching, in particular, in Dual Wheel setups. Now I understand that the C.A.S. pulse is generated by the camshaft; & that the fact that it is driven by the chains & sprockets, or belts that stretch under severe acceleration; that it can have the jitters.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 24-1 Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger decoder

#### Classic mini 1098cc cam trigger problem without any crank input
- URL: https://speeduino.com/forum/viewtopic.php?t=7264#p73593
- Date: Wed Nov 19, 2025 3:12 am
- Who posted it: Omarbadar
- Evidence type: tune file / wheel description / image attachment
- Confidence: high
- Summary: Hello all, I am new to Speeduino. I am trying to install Speeduino Mini Pro24 on my classic mini with 1098cc engine. I have replaced the distributor+ign coil with NB miata coil, but still using the original SU carb while sorting out the ign timing issue.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 7-1 Signal roles: cam mentioned, crank mentioned Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=14577 Image or capture links found: 1
- Matched search terms: trigger decoder

#### Messing with Subaru 6/7 trigger
- URL: https://speeduino.com/forum/viewtopic.php?t=4175#p47872
- Date: Thu Jan 28, 2021 5:44 pm
- Who posted it: Yoosyn
- Evidence type: scope capture / wheel description
- Confidence: high
- Summary: jonathanlawley wrote: thats strange that its no different. Ive just ordered an oscilloscope to see if I can make a different type of filter with the code. In the mean time I made this little loop that should (hopefully) find the correct filter value for you.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Edge or poll details: rising, falling, change Signal roles: crank mentioned
- Matched search terms: Subaru 6/7

#### HONDA TRANSALP XL650V
- URL: https://speeduino.com/forum/viewtopic.php?t=7157#p73147
- Date: Sun Sep 14, 2025 4:34 pm
- Who posted it: PSIG
- Evidence type: composite log / scope capture
- Confidence: high
- Summary: Usable logs. While there is no data log (cranking-start-run log), there is enough to suspect the VR conditioner is too 'smart' and not reading teeth after the gap. This is a problem with the smart MAX conditioners in A2 mode, which automatically adjusts the trigger threshold based on previous teeth.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: composite logger

#### Mazda Mx5 Miata instability issues (and workaround)
- URL: https://speeduino.com/forum/viewtopic.php?t=6281#p67660
- Date: Tue Mar 26, 2024 9:43 pm
- Who posted it: AlexE
- Evidence type: image attachment
- Confidence: high
- Summary: Hello, I took my friends mx5 (2001 mk2.5 1.8vvt) to dyno to make test what can I achieve on stock car using my PnP board. And I found following issue: car runs unstable especially on high rpm, see attached dyno plots. Above 5000rpm there was massive instability.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned Explicit sync-loss discussion present. Attachments: mx5 mk25 18vvt issue.zip -> https://speeduino.com/forum/download/file.php?id=12831; mx51.8vvt problem.pdf -> https://speeduino.com/forum/download/file.php?id=12815 Image or capture links found: 2
- Matched search terms: sync loss

#### Honda K20 trigger pattern
- URL: https://speeduino.com/forum/viewtopic.php?t=7277#p73689
- Date: Sun Nov 30, 2025 9:16 pm
- Who posted it: Michal327
- Evidence type: wheel description
- Confidence: high
- Summary: Hi folks, I've created a new thread about k20 trigger because two years ago @miker wrote that the new pattern was ready but untested. To avoid deeming my project a failure, I really need any code or files you have for this engine so it can run in full sequential mode on Speeduino v0.4.3d. I'm trying to write my own custom decoder, but it's not working.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1, honda d17 Edge or poll details: change Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger decoder

#### Honda K20 trigger pattern
- URL: https://speeduino.com/forum/viewtopic.php?t=7277#p73812
- Date: Wed Dec 24, 2025 1:04 pm
- Who posted it: Michal327
- Evidence type: wheel description
- Confidence: high
- Summary: Sorry for not responding @delion This is crank and cam trigger pattern which I followed when creating the decoder: Honda D17 crank trigger wheel: 12+1 Honda K20 crank trigger wheel: 12+1 Honda K20 cam trigger wheel: 4+1 The D17 decoder is semi-sequential (wasted spark) and there is no script written for triggerSec_HondaD17 (for reading cam). Just to be clear, the principle of operation of the cam trigger is almost the same as the crank trigger ( it remembers the last tooth gap and checks if it is half the current gap, because it has +1 extra tooth in between ). I made sure that sync is triggered correctly in my code.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: honda d17 Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger decoder

#### Progress report - August 2017
- URL: https://speeduino.com/forum/viewtopic.php?t=1387#p21114
- Date: Sat Sep 09, 2017 2:20 pm
- Who posted it: noisymime
- Evidence type: wheel description / maintainer explanation / image attachment
- Confidence: high
- Summary: Time again for another monthly dose of firmware goodness! Much of the effort this month has been around little tweaks specific to the MX5 / Miata PNP units, but there are a HEAP of other changes as well. Most significant of these is a complete rewrite of the closed loop boost control.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Pattern mentions: harley Edge or poll details: change Attachments: speeduino.ini -> https://speeduino.com/forum/download/file.php?id=2418 Image or capture links found: 1
- Matched search terms: Harley decoder

#### Tooth and Composit Logging Is Fun!
- URL: https://speeduino.com/forum/viewtopic.php?t=7182#p73272
- Date: Thu Oct 02, 2025 2:38 pm
- Who posted it: michaellatini
- Evidence type: wheel description
- Confidence: high
- Summary: Oh my goodness!!! Between my memory and not knowing what I'm doing... The crank sensor is a VR and not Hall, just looked at what I bought on the website and it shows that the VR sensor has 3 wires that I bought, and the Hall has only 2 wires.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 4-1 Edge or poll details: rising, falling Signal roles: cam mentioned, crank mentioned
- Matched search terms: sync loss

#### Wheel Decoder and VVT coder for Honda K series
- URL: https://speeduino.com/forum/viewtopic.php?t=5816#p73651
- Date: Tue Nov 25, 2025 8:29 pm
- Who posted it: Michal327
- Evidence type: wheel description
- Confidence: high
- Summary: Hi folks, @miker if you already have a sequential decoder, could you please upload it to github? I'm trying to write my own custom decoder, but it's not working. Please help me, I'm out of ideas.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1, honda d17 Edge or poll details: change Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger decoder

#### Trying to build a trigger for uneven flywheel.
- URL: https://speeduino.com/forum/viewtopic.php?t=4907#p52998
- Date: Thu Sep 09, 2021 10:58 am
- Who posted it: RempageR1
- Evidence type: wheel description
- Confidence: high
- Summary: Hi, I`m trying to get this flywheel working using the ardustim but I`m having some issues: Wheel.JPG (35.01 KiB) Viewed 22218 times I`m using the change trigger in the setup and the code below syncs on the wider lobe. So there is a routine checking the width of a lobe compare with the others. When the wider lobe is found, the secondaryToothCount is set to 1.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Pattern mentions: 36-1 Edge or poll details: rising, falling, change Signal roles: crank mentioned
- Matched search terms: Harley decoder

#### 3sge - 91 toyota mr2 SW20 - struggling to get a crank signal.
- URL: https://speeduino.com/forum/viewtopic.php?t=6554#p69680
- Date: Sat Sep 28, 2024 9:00 pm
- Who posted it: andyC
- Evidence type: tooth log
- Confidence: high
- Summary: Well spotted, i should be have used the words trigger signal not crank. My engine is fitted with a 4 wire vr sensor at the cam and nothing at the crank. It has two wheels, a 24 tooth wheel, and a single tooth wheel.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger trace

#### Speeduino Beetle
- URL: https://speeduino.com/forum/viewtopic.php?t=917#p73221
- Date: Wed Sep 24, 2025 7:24 pm
- Who posted it: michaellatini
- Evidence type: tune file / image attachment
- Confidence: high
- Summary: Had to take a month + off of the project but, back on it again. It turns out the Cam sync is 52* BTDC cyl. 1 when it reaches the leading edge of the magnet and 43* at the center of the magnet and not 240.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned, crank mentioned Attachments: 2025-09-24_10.33.08.mlg -> https://speeduino.com/forum/download/file.php?id=14437; CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=14438 Image or capture links found: 2
- Matched search terms: composite logger

#### Tooth and Composit Logging Is Fun!
- URL: https://speeduino.com/forum/viewtopic.php?t=7182#p73237
- Date: Sat Sep 27, 2025 3:43 pm
- Who posted it: michaellatini
- Evidence type: tooth log / composite log / wheel description
- Confidence: high
- Summary: Hello All, Logging some tooth and composite logs to try and chase down issues of course. Working on understanding what I have so far, searching, DIYAutotune and there is sync loss for the entire composite log. Tooth log shows what I think is 2 missing teeth maybe, for a 36-1 wheel.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1 Edge or poll details: change Explicit sync-loss discussion present. Attachments: TUNE & LOG 10-1-25.rar -> https://speeduino.com/forum/download/file.php?id=14465; TUNE AND LOG.zip -> https://speeduino.com/forum/download/file.php?id=14463 Image or capture links found: 2
- Matched search terms: sync loss

#### J35a4 Triggers
- URL: https://speeduino.com/forum/viewtopic.php?t=6027#p65009
- Date: Mon Sep 25, 2023 1:15 am
- Who posted it: OldKamikaze
- Evidence type: scope capture
- Confidence: high
- Summary: Unfortunately my ECU declined to steam pulses, but this is what oscilloscope gave me. See attached office file, making the cam gear
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned Attachments: signal.ods -> https://speeduino.com/forum/download/file.php?id=12116 Image or capture links found: 1
- Matched search terms: Honda J32

#### A cobbled together oddfire (Harley)
- URL: https://speeduino.com/forum/viewtopic.php?t=1022#p17301
- Date: Sat Mar 18, 2017 1:10 pm
- Who posted it: Jcwood3
- Evidence type: scope capture / wheel description
- Confidence: high
- Summary: Had a little more success with the setup. I was having trouble with ardustim so I wrote a simple program that maps a pot output to time delay on a square wave, effectively giving me rpm control. Works great and I'm getting sync, though there is no missing tooth on the signal, so I'm not sure what that's about.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 32-2
- Matched search terms: Harley decoder

#### Subaru EJ25 DOHC
- URL: https://speeduino.com/forum/viewtopic.php?t=6731#p70688
- Date: Mon Feb 03, 2025 8:48 pm
- Who posted it: PSIG
- Evidence type: tooth log / scope capture / wheel description
- Confidence: high
- Summary: Reviewing an image of the wheel, the tooth log makes more sense. You have to think in ONLY rising or falling edges (depending on decoder spec), and that a sensed edge is required to end a time bar in a tooth log. Using this and the 'scope image earlier, we can count 36 total teeth (real and missing) like this: If it helps to see what the sensor is reading, perhaps this may help.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 36-2-2-2, 36-2-4 Edge or poll details: rising, falling Signal roles: crank mentioned
- Matched search terms: Subaru 6/7

#### Sync code question
- URL: https://speeduino.com/forum/viewtopic.php?t=7359#p74204
- Date: Wed Mar 04, 2026 12:08 am
- Who posted it: Banjo
- Evidence type: technical discussion
- Confidence: high
- Summary: On the 24th February, I posted a query regarding the way that Speeduino firmware handles/ or presumably; doesn't handle resynching issues. This post have garnered 18,500 views in this short time, so it must be of importance to others, beside myself. I summary, I have come to these conclusions, (which may be incorrect), but would appreciate anyone please answering my queries.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: trigger decoder

#### Trying to build a trigger for uneven flywheel.
- URL: https://speeduino.com/forum/viewtopic.php?t=4907#p53326
- Date: Wed Sep 29, 2021 6:29 pm
- Who posted it: RempageR1
- Evidence type: wheel description
- Confidence: high
- Summary: I`ve written a working trigger that can sync on the width of a tooth. It`s been validated and works like a charm including the trigger filter . Sharing here so hopefully it can benefit someone else.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1, harley, vmax Edge or poll details: rising, falling, change Signal roles: crank mentioned
- Matched search terms: Harley decoder

#### 3sge - 91 toyota mr2 SW20 - struggling to get a crank signal.
- URL: https://speeduino.com/forum/viewtopic.php?t=6554#p70074
- Date: Wed Nov 20, 2024 2:53 am
- Who posted it: stum
- Evidence type: wheel description
- Confidence: high
- Summary: I think you have the wrong trigger pattern. Looking at your trace and my experiences you should not have missing tooth selected. As it has no missing tooth.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger trace

#### Messing with Subaru 6/7 trigger
- URL: https://speeduino.com/forum/viewtopic.php?t=4175#p47863
- Date: Thu Jan 28, 2021 1:07 am
- Who posted it: jonathanlawley
- Evidence type: scope capture
- Confidence: high
- Summary: thats strange that its no different. Ive just ordered an oscilloscope to see if I can make a different type of filter with the code. In the mean time I made this little loop that should (hopefully) find the correct filter value for you.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: None extracted
- Matched search terms: Subaru 6/7

#### Reading RPM off 4 tooth irregular cam
- URL: https://speeduino.com/forum/viewtopic.php?t=6726#p70614
- Date: Sat Jan 25, 2025 9:35 pm
- Who posted it: Lex_GTX
- Evidence type: scope capture
- Confidence: high
- Summary: Excellent eye! The falling edges of the oscilloscope trace are indeed symmetrical. That means we should be able to set this up in the software as a Distributor with Falling Edge trigger.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Edge or poll details: falling
- Matched search terms: trigger trace

#### Subaru EJ25 DOHC
- URL: https://speeduino.com/forum/viewtopic.php?t=6731#p70668
- Date: Sun Feb 02, 2025 1:53 pm
- Who posted it: Laudper
- Evidence type: composite log / tune file / wheel description
- Confidence: high
- Summary: I am using the 36-2-2-2, so see no problems using that pattern? Current tune uploaded. I haven't got a tooth/composite logger to share currently as I am doing a bit of wiring.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 36-2-2-2 Signal roles: crank mentioned Attachments: 2025-02-01_13.01.19.msq -> https://speeduino.com/forum/download/file.php?id=13649 Image or capture links found: 1
- Matched search terms: Subaru 6/7

#### A cobbled together oddfire (Harley)
- URL: https://speeduino.com/forum/viewtopic.php?t=1022#p17362
- Date: Mon Mar 20, 2017 8:43 pm
- Who posted it: PSIG
- Evidence type: scope capture / wheel description
- Confidence: high
- Summary: Random spewing among your quotes for simplicity: [Jcwood3](./memberlist.php?mode=viewprofile&u=940) wrote: [↑](./viewtopic.php?p=17344#p17344) Mon Mar 20, 2017 1:47 am ... A couple of differences I think I'll use will be the use of only one O2 sensor and no security system (not shown). Also the "active intake" is not used - pretty sure that was an emissions/noise thing used outside the US and is not installed.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 4-6 Signal roles: cam mentioned, crank mentioned
- Matched search terms: Harley decoder

#### Speeduino Beetle
- URL: https://speeduino.com/forum/viewtopic.php?t=917#p72805
- Date: Thu Aug 14, 2025 12:55 am
- Who posted it: PSIG
- Evidence type: tooth log / composite log
- Confidence: high
- Summary: [michaellatini](./memberlist.php?mode=viewprofile&u=5673) wrote: [↑](./viewtopic.php?p=72801#p72801) Wed Aug 13, 2025 9:11 pm Would that mean that with speeduino the cam sync should be timed to cylinder 1 TDC??? Yes, but to confirm we are on the same page... Cam sync is the signal to begin the 720° 4-stroke cycle at TDC Cyl1.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: rising, change Signal roles: cam mentioned, crank mentioned
- Matched search terms: composite logger

#### What else do I need? Nub question.
- URL: https://speeduino.com/forum/viewtopic.php?t=2035#p28025
- Date: Thu Aug 23, 2018 9:16 am
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: high
- Summary: [doleckijohn](./memberlist.php?mode=viewprofile&u=2003) wrote: [↑](./viewtopic.php?p=28017#p28017) Wed Aug 22, 2018 11:37 am Also if I jut mount and aftermarket trigger wheel to the engine is the Speeduino capable of using it to run the odd firing order of a harley? Hey John — Twin Cam? I think that's a 32-2 trigger on the crank, right?
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 32-2, harley Signal roles: cam mentioned, crank mentioned
- Matched search terms: Harley decoder

#### How to re-open a composite logger file
- URL: https://speeduino.com/forum/viewtopic.php?t=7263#p73583
- Date: Mon Nov 17, 2025 9:07 pm
- Who posted it: Donald G
- Evidence type: tooth log / composite log
- Confidence: high
- Summary: When I do a composite log file, I can view the results. If I close the file and reopen it later, it opens up as a tooth log. How can I reopen the composite files to see the cam, crank and sync signals?
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned, crank mentioned Attachments: 2025-11-17_14.46.53.csv -> https://speeduino.com/forum/download/file.php?id=14575 Image or capture links found: 1
- Matched search terms: composite logger

#### Firmware update for Jeep engines (4 and 6 cylinder support, sequential operation)
- URL: https://speeduino.com/forum/viewtopic.php?t=7323#p73986
- Date: Tue Jan 20, 2026 5:11 pm
- Who posted it: FlashStopFall
- Evidence type: wheel description
- Confidence: high
- Summary: I thought I'd make a quick post about this in case anyone is looking to run sequential injection on their Jeep 4.0 or 2.5 in the future. The trigger decoder (Jeep2000) did not support sequential operation, and there was no 4 cylinder support before. I added sequential support, and added a new decoder for the 4 cylinder (Jeep2000_4cyl) because it was significantly different, although it works in the same way.
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations.
- Specific details: Edge or poll details: change Signal roles: cam mentioned
- Matched search terms: trigger decoder

#### J35a4 Triggers
- URL: https://speeduino.com/forum/viewtopic.php?t=6027#p64990
- Date: Sat Sep 23, 2023 6:35 pm
- Who posted it: PSIG
- Evidence type: scope capture / wheel description
- Confidence: high
- Summary: [OldKamikaze](./memberlist.php?mode=viewprofile&u=7985) wrote: [↑](./viewtopic.php?p=64958#p64958) Thu Sep 21, 2023 9:05 pm ZCrank: 12+12+0 (No missing) Cam: 6+4+2 (2missing) I don't have that set of patterns on file. Your description does not appear to follow naming convention for + or - teeth, and does not translate to a valid pattern. Please provide a drawing, or image, or oscilloscope pattern, etc.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: Honda J32

#### J35a4 Triggers
- URL: https://speeduino.com/forum/viewtopic.php?t=6027#p64996
- Date: Sat Sep 23, 2023 11:02 pm
- Who posted it: OldKamikaze
- Evidence type: scope capture / wheel description
- Confidence: high
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=64990#p64990) Sat Sep 23, 2023 6:35 pm [OldKamikaze](./memberlist.php?mode=viewprofile&u=7985) wrote: [↑](./viewtopic.php?p=64958#p64958) Thu Sep 21, 2023 9:05 pm ZCrank: 12+12+0 (No missing) Cam: 6+4+2 (2missing) I don't have that set of patterns on file. Your description does not appear to follow naming convention for + or - teeth, and does not translate to a valid pattern. Please provide a drawing, or image, or oscilloscope pattern, etc.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: Honda J32

#### Tooth and Composit Logging Is Fun!
- URL: https://speeduino.com/forum/viewtopic.php?t=7182#p73260
- Date: Tue Sep 30, 2025 8:42 pm
- Who posted it: michaellatini
- Evidence type: composite log
- Confidence: high
- Summary: Today I changed the location of the cam sensor from -42* to -340*. I read that if it's too close to TDC #1 cylinder it may not read correctly. Finally got a tune and composite log to share.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: change Signal roles: cam mentioned Explicit sync-loss discussion present. Attachments: TUNE AND LOG.zip -> https://speeduino.com/forum/download/file.php?id=14463 Image or capture links found: 1
- Matched search terms: sync loss

#### (Hardware test Inj 5-8 and Ign 5-8 grayed out) Help setting up v0.4.3d board
- URL: https://speeduino.com/forum/viewtopic.php?t=7217#p73405
- Date: Thu Oct 23, 2025 7:36 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: high
- Summary: [FlashStopFall](./memberlist.php?mode=viewprofile&u=10039) wrote: [↑](./viewtopic.php?p=73404#p73404) Thu Oct 23, 2025 7:27 am [FlashStopFall](./memberlist.php?mode=viewprofile&u=10039) wrote: [↑](./viewtopic.php?p=73401#p73401) Thu Oct 23, 2025 6:46 am Actually, I found that the injector count is reliant upon boardFuelOutputs in speeduino.ini. Changing it to 6 for v0.4 boards got me exactly the control I wanted. I can change the pins in init.cpp to the coil pins, and see the lights go on and off when I toggle them.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Directly informs tune and INI compatibility handling plus migration expectations.
- Specific details: Edge or poll details: change
- Matched search terms: 36-2-1

#### Spark only Subaru Project EJ22
- URL: https://speeduino.com/forum/viewtopic.php?t=7073#p72364
- Date: Mon Jul 07, 2025 4:16 pm
- Who posted it: Donald G
- Evidence type: tune file / image attachment
- Confidence: high
- Summary: Hello to all here, I am trying to use a v0.4.3d controller with an Arduino MEGA 2560 to control spark only on a Subaru Ej22. It uses a 6/7 crank cam trigger. I also have VR Conditioner to convert the cam and crank signals to DC out.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned, crank mentioned Attachments: 2025-07-07_16.27.02.msq -> https://speeduino.com/forum/download/file.php?id=14145 Image or capture links found: 1
- Matched search terms: Subaru 6/7

#### A cobbled together oddfire (Harley)
- URL: https://speeduino.com/forum/viewtopic.php?t=1022#p16446
- Date: Sun Feb 05, 2017 7:12 pm
- Who posted it: Jcwood3
- Evidence type: tune file / wheel description
- Confidence: high
- Summary: So here's where I'm at and still working, of course. I'll edit this post to maintain a running log or what I know/don't know, done/haven't done. I look forward to many, many hours of hair-pulling frustration punctuated by moments of jumping for joy.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 32-2 Signal roles: cam mentioned, crank mentioned
- Matched search terms: Harley decoder

#### What else do I need? Nub question.
- URL: https://speeduino.com/forum/viewtopic.php?t=2035#p28017
- Date: Wed Aug 22, 2018 11:37 am
- Who posted it: doleckijohn
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: I have to split the cases anyway to machine some parts for the blower drive. So I can see the crank then an how its machined for the crank sensor. Or I'll just put the scope on the sensor before i split the cases.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: harley Signal roles: crank mentioned
- Matched search terms: Harley decoder

#### Subaru EJ25 DOHC
- URL: https://speeduino.com/forum/viewtopic.php?t=6731#p70702
- Date: Tue Feb 04, 2025 6:20 pm
- Who posted it: PSIG
- Evidence type: tooth log / tune file / wheel description
- Confidence: medium
- Summary: [Laudper](./memberlist.php?mode=viewprofile&u=9265) wrote: [↑](./viewtopic.php?p=70694#p70694) Tue Feb 04, 2025 11:40 am If the tooth log tallies with the wheel, what is my best next move? Just to eliminate the possibility of different versions, I would first compare the decoder's coded 36-2-2-2 pattern to your physical wheel, for any differences in (-2) placement. This is why I posted all that info.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 2-2, 36-2-2-2 Attachments: 2025-02-08_15.37.13_composite.csv -> https://speeduino.com/forum/download/file.php?id=13683; 2025-02-08_15.38.22.mlg -> https://speeduino.com/forum/download/file.php?id=13682; 2025-02-08_15.47.35.msq -> https://speeduino.com/forum/download/file.php?id=13684 Image or capture links found: 3
- Matched search terms: Subaru 6/7

#### V0.4.3c accidentally shorted 12v now no rpm signal or inj LED's. NA6 MX5
- URL: https://speeduino.com/forum/viewtopic.php?t=6388#p68573
- Date: Sat Jun 08, 2024 10:22 am
- Who posted it: ~Slideways
- Evidence type: technical discussion
- Confidence: medium
- Summary: Hello, I have a 0.4.3c and an NA6 adapter board (adapting to the 48 pin ECU plug) that I built a few years ago, just getting round to testing out an ITB setup now. Had some good success getting it running, but the accidental shorting happened after I had installed a ULN2003A for fuel pump control, I was using a multi meter to confirm a few things then I stupidly shorted the 12v and ground on the adapter board. It let out the magic smoke from the adapter board.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: change Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger trace

#### Wheel Decoder and VVT coder for Honda K series
- URL: https://speeduino.com/forum/viewtopic.php?t=5816#p63897
- Date: Sat Jul 08, 2023 1:59 am
- Who posted it: NickZ
- Evidence type: wheel description
- Confidence: medium
- Summary: I was working on a ford engine with an uneven 4 tooth cam trigger, I was able to isolate 1 tooth out of the pattern and use that as a sync for sequential operation on that engine. the way i did it was using the crank trigger teeth, i had a 36-1 crank trigger, I added code if it saw a cam tooth between crank tooth between 13-16 to use that cam tooth as sync. But I'm not a coder either so may not be the best solution.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1 Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger decoder

#### How to re-open a composite logger file
- URL: https://speeduino.com/forum/viewtopic.php?t=7263#p73585
- Date: Tue Nov 18, 2025 2:24 am
- Who posted it: Donald G
- Evidence type: tooth log / composite log
- Confidence: medium
- Summary: I am able to log the composite signals and save them, but I can't open them up again as the composite log that shows the triggers of the cam and crank signals along with the sync. It only opens up as a tooth log. [2025-11-17_14.46.53.csv](./download/file.php?id=14575) (85.57 KiB) Downloaded 95 times
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: composite logger

#### Single tooth trigger pattern for Ignition only
- URL: https://speeduino.com/forum/viewtopic.php?t=1494#p22216
- Date: Wed Nov 08, 2017 9:59 pm
- Who posted it: PSIG
- Evidence type: scope capture
- Confidence: medium
- Summary: I would first explore the signals the current box receives and sends. Unless you can find very specific data it would require an oscilloscope. That will tell you what input you need, and the parameters of the outputs, such as each signal's voltage range, coil dwell if IDI, trigger polarity if CDI, leading or trailing action, etc.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: None extracted
- Matched search terms: Harley decoder

#### First tooth logs
- URL: https://speeduino.com/forum/viewtopic.php?t=7052#p72253
- Date: Sat Jun 28, 2025 4:32 am
- Who posted it: ImprezaRSC
- Evidence type: tooth log / wheel description
- Confidence: medium
- Summary: I think I finally have what looks to be my first tooth logs. The crank was spun by hand and I see what looks like a tooth pattern. Playback leaves a little to be desired, but maybe it's just Linux.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: crank mentioned Attachments: Compositelogger2025-06-27_22.26.31.csv -> https://speeduino.com/forum/download/file.php?id=14109 Image or capture links found: 1
- Matched search terms: Subaru 6/7

#### Wheel Decoder and VVT coder for Honda K series
- URL: https://speeduino.com/forum/viewtopic.php?t=5816#p63042
- Date: Wed May 17, 2023 7:44 pm
- Who posted it: choyr
- Evidence type: wheel description
- Confidence: medium
- Summary: Hi all, As you're aware, I do plug and play ecus based on UA4C. You can check my website at pnpduino.com Currently I am modifying the existing Honda D17 decoder since the K20 has the same crank. The D17 only uses semi sequential and not using cam signal to trigger Home (AFAIK).
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: honda d17 Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger decoder

#### Mazda Mx5 Miata instability issues (and workaround)
- URL: https://speeduino.com/forum/viewtopic.php?t=6281#p73623
- Date: Sat Nov 22, 2025 9:09 pm
- Who posted it: PSIG
- Evidence type: tooth log
- Confidence: medium
- Summary: This appears to be a different issue from the previous one in the thread? What was the issue? Do you have a Tooth log or other reference?
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Explicit sync-loss discussion present.
- Matched search terms: sync loss

#### Progress report - August 2017
- URL: https://speeduino.com/forum/viewtopic.php?t=1387#p21201
- Date: Wed Sep 13, 2017 11:41 pm
- Who posted it: noisymime
- Evidence type: wheel description / maintainer explanation
- Confidence: medium
- Summary: [stormfinder](./memberlist.php?mode=viewprofile&u=1310) wrote: [↑](./viewtopic.php?p=21199#p21199) Wed Sep 13, 2017 9:58 pm Possible bug in august stable, and sept dev firmware. I have collaborated this bug with one other person here (PSIG), and another member on miataturbo.net Since updating to the august (or in my case september) firmware, when writing a tune. i have to re write the tables, particularily the VE table most often, 3-5 times before it will stick.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Directly informs tune and INI compatibility handling plus migration expectations.
- Specific details: Pattern mentions: 3-5 Edge or poll details: change
- Matched search terms: Harley decoder

#### Sensor for cam wheel
- URL: https://speeduino.com/forum/viewtopic.php?t=7296#p73803
- Date: Tue Dec 23, 2025 6:42 pm
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: medium
- Summary: There are some old OptiSpark threads here for Speeduino background, or a ton of info on the web. The OptiSpark is a version of Mitsubishi-designed ignition, often seen here on Nissans and others. The current decoder is the [Nissan-360](https://wiki.speeduino.com/en/decoders/Nissan_360) , and would need code mods to work directly with OptiSpark.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 2-2, 72-1 Signal roles: cam mentioned, crank mentioned
- Matched search terms: 36-2-1

#### Single tooth trigger pattern for Ignition only
- URL: https://speeduino.com/forum/viewtopic.php?t=1494#p22580
- Date: Wed Nov 22, 2017 1:52 pm
- Who posted it: Bleeker
- Evidence type: wheel description
- Confidence: medium
- Summary: Nice to see that others are also interested in this topic. As far as I understood now, L-Twin Engines and all other multi cylinder Engines with crankpin offset, require 2 or more Channels which are triggered separately, as a single tooth does not allow precise calculation of the 2nd cylinder. On my OEM Ducati Ignition I have two Pickups, which have an offset of 90° and flywheel has only one tooth.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1 Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: Harley decoder

#### Classic mini 1098cc cam trigger problem without any crank input
- URL: https://speeduino.com/forum/viewtopic.php?t=7264#p73602
- Date: Thu Nov 20, 2025 8:00 am
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: medium
- Summary: Offhand, two probable issues. First is that the distributor signal is a crankshaft position signal at half-speed, whereas the cam signal is only a cycle signal (where to start the sequence in the cycle). Second, your 7-1 trigger pattern does not divide evenly into 360° as an 8-1 would, for example.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 7-1, 8-1 Signal roles: cam mentioned, crank mentioned
- Matched search terms: trigger decoder

#### Single tooth trigger pattern for Ignition only
- URL: https://speeduino.com/forum/viewtopic.php?t=1494#p22230
- Date: Thu Nov 09, 2017 2:01 pm
- Who posted it: Bleeker
- Evidence type: scope capture
- Confidence: medium
- Summary: The system is IDI and has an inductive Pick-Up which I have already used for testing with speeduino and it worked well. Unfortunatelly I do not have an oscilloscope. I could use the wiring and coil driver which I have built for my Ducati for testing and drive the BMW coils with it.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: None extracted
- Matched search terms: Harley decoder

#### Injector Timing Benchtop Testing Issue
- URL: https://speeduino.com/forum/viewtopic.php?t=7242#p73503
- Date: Thu Nov 06, 2025 6:23 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: medium
- Summary: Always your tune and a log of the issue with it. Silly things like cranking going over and under threshold rpm can cause erratic injections as it switches modes, or sync loss, and several other things. We can't guess until we see what it is doing (log), compared to what it was told to do (tune).
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: crank mentioned Explicit sync-loss discussion present.
- Matched search terms: sync loss

#### V0.4.3c accidentally shorted 12v now no rpm signal or inj LED's. NA6 MX5
- URL: https://speeduino.com/forum/viewtopic.php?t=6388#p68604
- Date: Thu Jun 13, 2024 10:01 pm
- Who posted it: ~Slideways
- Evidence type: technical discussion
- Confidence: medium
- Summary: Hi PSIG, Thanks for the reply and leads to try. I'll get some logs this weekend hopefully. The main concern is that it was running and idling etc until the shorted 12v.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: trigger trace

#### First tooth logs
- URL: https://speeduino.com/forum/viewtopic.php?t=7052#p72296
- Date: Wed Jul 02, 2025 3:54 am
- Who posted it: ImprezaRSC
- Evidence type: tooth log / wheel description
- Confidence: medium
- Summary: Digging a little deeper into these first tooth logs, I'm just getting used to how they read. Consider these signals were created by turning the crank by hand. Just the fact that they are there is a good thing.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: Subaru 6/7

#### (Hardware test Inj 5-8 and Ign 5-8 grayed out) Help setting up v0.4.3d board
- URL: https://speeduino.com/forum/viewtopic.php?t=7217#p73406
- Date: Thu Oct 23, 2025 7:59 am
- Who posted it: FlashStopFall
- Evidence type: technical discussion
- Confidence: medium
- Summary: [dazq](./memberlist.php?mode=viewprofile&u=186) wrote: [↑](./viewtopic.php?p=73405#p73405) Thu Oct 23, 2025 7:36 am [FlashStopFall](./memberlist.php?mode=viewprofile&u=10039) wrote: [↑](./viewtopic.php?p=73404#p73404) Thu Oct 23, 2025 7:27 am [FlashStopFall](./memberlist.php?mode=viewprofile&u=10039) wrote: [↑](./viewtopic.php?p=73401#p73401) Thu Oct 23, 2025 6:46 am Actually, I found that the injector count is reliant upon boardFuelOutputs in speeduino.ini. Changing it to 6 for v0.4 boards got me exactly the control I wanted. I can change the pins in init.cpp to the coil pins, and see the lights go on and off when I toggle them.
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations.
- Specific details: Edge or poll details: change
- Matched search terms: 36-2-1

#### V0.4.3c accidentally shorted 12v now no rpm signal or inj LED's. NA6 MX5
- URL: https://speeduino.com/forum/viewtopic.php?t=6388#p68578
- Date: Sun Jun 09, 2024 3:38 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: medium
- Summary: Injectors and coils are functioned by RPM, so I would start there, looking for signal and if it gets to the processor. However, the injectors also trigger on power-up (priming shot) before rpm. So: Do the injectors prime?
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: trigger trace

#### Reading RPM off 4 tooth irregular cam
- URL: https://speeduino.com/forum/viewtopic.php?t=6726#p70609
- Date: Sat Jan 25, 2025 5:45 am
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: medium
- Summary: [Lex_GTX](./memberlist.php?mode=viewprofile&u=9342) wrote: [↑](./viewtopic.php?p=70608#p70608) Sat Jan 25, 2025 12:00 am The simplest trigger is the cam. It has 4 teeth (like a distributor) but their spacing is NOT symmetrical as the OEM ECU uses this to know the position of the engine. Hmm.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned
- Matched search terms: trigger trace

#### 3sge - 91 toyota mr2 SW20 - struggling to get a crank signal.
- URL: https://speeduino.com/forum/viewtopic.php?t=6554#p69649
- Date: Fri Sep 27, 2024 11:11 am
- Who posted it: andyC
- Evidence type: technical discussion
- Confidence: low
- Summary: Hi and thanks for reading, I have been to struggling to wire my ECU into my car. In particular, I have been struggling with the crank signal. I was hoping someone with a similar engine could give some guidance.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: crank mentioned
- Matched search terms: trigger trace

#### Spark only Subaru Project EJ22
- URL: https://speeduino.com/forum/viewtopic.php?t=7073#p72380
- Date: Tue Jul 08, 2025 9:26 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [Donald G](./memberlist.php?mode=viewprofile&u=9822) wrote: [↑](./viewtopic.php?p=72374#p72374) Mon Jul 07, 2025 8:46 pm I believe this is the file you're asking for. I also figured out how to do the hardware test and it won't turn on either the ignition or injector Led outputs on the board. If the outputs don't operate in hardware test mode then it's not likely a setup issue .
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: Subaru 6/7

#### Classic mini 1098cc cam trigger problem without any crank input
- URL: https://speeduino.com/forum/viewtopic.php?t=7264#p73614
- Date: Fri Nov 21, 2025 4:21 pm
- Who posted it: digmorepaka
- Evidence type: wheel description
- Confidence: low
- Summary: [Omarbadar](./memberlist.php?mode=viewprofile&u=4325) wrote: [↑](./viewtopic.php?p=73609#p73609) Fri Nov 21, 2025 6:43 am Thanks PSIG on your feedback. Shall the camshaft signal be connected to pin 18(RPM2) or pin 19(RPM1) when running CAM SPEED as primary with no secondary? Noted on the 7-1 tooth issue.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 7-1, 8-1 Signal roles: cam mentioned
- Matched search terms: trigger decoder

#### Injector Timing Benchtop Testing Issue
- URL: https://speeduino.com/forum/viewtopic.php?t=7242#p73493
- Date: Wed Nov 05, 2025 1:02 pm
- Who posted it: Patrickco16
- Evidence type: wheel description
- Confidence: low
- Summary: I’m currently bench testing a Speeduino Ocelot board on my desktop setup using Tunerstudio and have run into an issue with injector firing. Setup: Power: 12 V bench supply, Function generator output: 0–5 V square wave @ 67 Hz that is connected to Primary Trigger (Crank) input, Trigger mode: Basic Distributor, Falling edge, TPS knob connected and working with 5V. Using 1 cylinder settings, 1 injector that I am attempting to fire.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: basic distributor Edge or poll details: falling Signal roles: crank mentioned
- Matched search terms: sync loss

#### Reading RPM off 4 tooth irregular cam
- URL: https://speeduino.com/forum/viewtopic.php?t=6726#p70608
- Date: Sat Jan 25, 2025 12:00 am
- Who posted it: Lex_GTX
- Evidence type: wheel description
- Confidence: low
- Summary: Hi Speeduino Community, I am building an aux fuel controller for a direct injected Ecoboost 4 cylinder. I want this to be batch fire past a certain load under higher boost so sequential injection is not needed. The simplest trigger is the cam.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned
- Matched search terms: trigger trace

#### Sensor for cam wheel
- URL: https://speeduino.com/forum/viewtopic.php?t=7296#p73851
- Date: Sun Jan 04, 2026 12:08 pm
- Who posted it: SonOfSaturn
- Evidence type: technical discussion
- Confidence: low
- Summary: That is exactly what I have now. Although the original optispark is bad, that is what sent me down this path. So the conditioner can read the 360 tooth wheel?
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: crank mentioned
- Matched search terms: 36-2-1

#### Trying to build a trigger for uneven flywheel.
- URL: https://speeduino.com/forum/viewtopic.php?t=4907#p54762
- Date: Mon Jan 17, 2022 2:15 pm
- Who posted it: RempageR1
- Evidence type: technical discussion
- Confidence: low
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=54703#p54703) Thu Jan 13, 2022 6:33 pm [RempageR1](./memberlist.php?mode=viewprofile&u=4011) wrote: [↑](./viewtopic.php?p=54691#p54691) Thu Jan 13, 2022 8:13 am … This was for my V3 VR-conditioner reference board using the MAX9926. OK, then relative to the edge you wish to read, and the wired polarity of your VR sensor, the MAX module will invert the signal. So, to step through the inversions; if you wish to read the leading edge of a tooth for position, and assuming correct VR sensor polarity wiring (rising voltage on leading edge, but which can also be inverted) , your module-inverted signal detection would be Falling .
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: rising, falling
- Matched search terms: Harley decoder

#### What else do I need? Nub question.
- URL: https://speeduino.com/forum/viewtopic.php?t=2035#p28018
- Date: Wed Aug 22, 2018 11:49 am
- Who posted it: LPG2CV
- Evidence type: wheel description
- Confidence: low
- Summary: Speedy loves trigger wheels and supports odd fire. You may also want to consider a cam position sensor unless you can do wasted spark on your odd fire.. On that note, you may also need a coil ignitor.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned
- Matched search terms: Harley decoder

#### (Hardware test Inj 5-8 and Ign 5-8 grayed out) Help setting up v0.4.3d board
- URL: https://speeduino.com/forum/viewtopic.php?t=7217#p73400
- Date: Thu Oct 23, 2025 6:24 am
- Who posted it: pazi88
- Evidence type: technical discussion
- Confidence: low
- Summary: You can look the init.cpp file to see what pins are used for ign/inj pins on v0.4 board: case 3: //Pin mappings as per the v0.4 shield pinInjector1 = 8; //Output pin injector 1 is on pinInjector2 = 9; //Output pin injector 2 is on pinInjector3 = 10; //Output pin injector 3 is on pinInjector4 = 11; //Output pin injector 4 is on pinInjector5 = 12; //Output pin injector 5 is on pinInjector6 = 50; //CAUTION: Uses the same as Coil 4 below. pinCoil1 = 40; //Pin for coil 1 pinCoil2 = 38; //Pin for coil 2 pinCoil3 = 52; //Pin for coil 3 pinCoil4 = 50; //Pin for coil 4 pinCoil5 = 34; //Pin for coil 5 PLACEHOLDER value for now The speeduino.ini for tunerstudio defines how many ign and inj outputs each board has on the "boardFuelOutputs" and "boardIgnOutputs" -parameters. For 0.4 it defines both to be 4, so you can only test up to 4 outputs.
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations.
- Specific details: Edge or poll details: change
- Matched search terms: 36-2-1

#### Progress report - August 2017
- URL: https://speeduino.com/forum/viewtopic.php?t=1387#p21126
- Date: Sun Sep 10, 2017 5:44 am
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [turbonetics](./memberlist.php?mode=viewprofile&u=1200) wrote: [↑](./viewtopic.php?p=21123#p21123) Sun Sep 10, 2017 12:11 am I'm intrested in hardware changes . Do you plan 0.5 or something? The v0.5 has been 'planned' for a long time, it's making it happen that's difficult
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Edge or poll details: change
- Matched search terms: Harley decoder

### Idle advance, idle taper, and air-con defaults

#### Adventures in tuning and driving a UA4C, Supercharged BMW V8
- URL: https://speeduino.com/forum/viewtopic.php?t=5471#p58294
- Date: Tue Aug 23, 2022 2:31 am
- Who posted it: Mykk
- Evidence type: wheel description
- Confidence: high
- Summary: Hello Tuners. I've wanted to make this thread sharing my various ideas, tests, experiments and tuning strategies I have tried and currently use over the last year since getting my project running, driving, racing, cruising and getting better at all of those things every day. These are just things I have tried.
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations. Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 1-5, 1-5-4-8, 1-6, 4-7, 4-8, 5-3, 6-3, 6-3-7-2, 60-2, 7-2, 8-2 Edge or poll details: change Signal roles: cam mentioned, crank mentioned
- Matched search terms: delay before idle control

#### HugoW’s 1998 MX5 NB with 2004 1.8 VVT engine
- URL: https://speeduino.com/forum/viewtopic.php?t=4413#p55107
- Date: Wed Feb 02, 2022 9:24 am
- Who posted it: HugoW
- Evidence type: technical discussion
- Confidence: high
- Summary: Going back to ye olde forum, I still prefer this over any fast and quick chat medium. On those media, I am often accused of just copying other people's tunes and wondering why they don't work. Let me quickly give the reader some background on me and my ways of approaching Speeduino tuning.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Signal roles: cam mentioned, crank mentioned
- Matched search terms: delay before idle control

#### Opel Astra G project, original Ecu + speeduino (Parallel/piggyback)
- URL: https://speeduino.com/forum/viewtopic.php?t=5644#p67936
- Date: Sun Apr 14, 2024 4:21 pm
- Who posted it: lovosal
- Evidence type: scope capture / tune file
- Confidence: high
- Summary: Hello Today I was with the car again. Knowing that I couldn't do anything with my oscilloscope since I'm missing the ammeter clamp and without it I haven't seen how I could do the checking of the coil dwell times, so I've done it the old way with the test method and mistake. I have removed the fuel relay so as not to choke or waste gasoline and I have started to touch the dwell menu...
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: crank mentioned Attachments: ASTRA G 2024-03-20_23.09.17.msq -> https://speeduino.com/forum/download/file.php?id=12915; DataLogs.zip -> https://speeduino.com/forum/download/file.php?id=12920 Image or capture links found: 2
- Matched search terms: idle taper

#### Suzuki RE-5 EFI Conversion
- URL: https://speeduino.com/forum/viewtopic.php?t=5324#p67461
- Date: Sun Mar 10, 2024 3:55 pm
- Who posted it: MGR550
- Evidence type: tooth log / wheel description
- Confidence: high
- Summary: Figured out the fuel pump issue, was bad crimp on the wiring on my part. For the trigger wheel, I hadn't actually ever counted the reduction from crank to distributor housing shafts; turns out it is 2:1, so I cut 1 more tooth off my 36-1 trigger wheel and made it into sortof a pair of 18-1 wheels which seems to work well in the tooth logger. Got the wiring onto the bike nicely, intake adapter and throttle body on and a temporary fuel feed hooked up and managed to fire it up!
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 18-1, 36-1 Signal roles: crank mentioned Attachments: 2024-03-10_12.21.17 Good Short Run.mlg -> https://speeduino.com/forum/download/file.php?id=12705 Image or capture links found: 1
- Matched search terms: idle taper

#### 1985 Honda Goldwing Limited Edition FI Model ECU Replacement
- URL: https://speeduino.com/forum/viewtopic.php?t=5544#p59264
- Date: Tue Nov 01, 2022 5:46 pm
- Who posted it: Rednaxs60
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: PSIG - Thanks for the reply. The reason I have thought about just using a ground and not reassigning any pins is that when I trace out the power distribution on the CFI schematic, there is 12 VDC power supply to relay #5 from the Main Relay #4. Following the schematic wiring to the ECU from the Fuel Pump Relay #5 indicates to me that it is going for an ECU ground, completing the circuit.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 8-3
- Matched search terms: idle taper

#### Suzuki RE-5 EFI Conversion
- URL: https://speeduino.com/forum/viewtopic.php?t=5324#p67237
- Date: Mon Feb 19, 2024 9:18 pm
- Who posted it: MGR550
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: Hello, Re-did the wiring a bit in terms of fusing and relays, with an updated diagram to match. Got it wired into the bike, turning on and off with key and kill switch, reading sensors, and tested inj output so far. A bit of debugging and I got what I think is a clean signal from the trigger wheel .
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: rising Signal roles: crank mentioned Attachments: 2024-02-19_15.39.46.csv -> https://speeduino.com/forum/download/file.php?id=12604; 2024-02-19_16.08.03.msq -> https://speeduino.com/forum/download/file.php?id=12603 Image or capture links found: 2
- Matched search terms: idle taper

#### Crank to Run Taper
- URL: https://speeduino.com/forum/viewtopic.php?t=5910#p63337
- Date: Thu Jun 01, 2023 8:56 am
- Who posted it: 92Yata
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: So I do have good cold and hot starts. The problem was when the car was hot and then sat for some time 10-15 minutes it would start up and then just die. If I worked the throttle for 30 sec or more it engine would then maintain idle.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: crank mentioned Attachments: 10f closed loop.msq -> https://speeduino.com/forum/download/file.php?id=11593; Baldwin-10f-OPEN-LOOP J modified.msq -> https://speeduino.com/forum/download/file.php?id=11594; DataLogs.rar -> https://speeduino.com/forum/download/file.php?id=11595 Image or capture links found: 3
- Matched search terms: crank to run taper

#### Difficult Miata Startups
- URL: https://speeduino.com/forum/viewtopic.php?t=5923#p63513
- Date: Wed Jun 14, 2023 10:52 am
- Who posted it: 92Yata
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Hey Speeduino Users, I know its not uncommon to hear of a Miata owner with difficult startup issues but this has been beat to to the ground without a solution for many months and a few good tuners. So currently I have a map on the car that does start the car somewhat decent and what I noticed in the last few days playing around with the tuners map revisions is.......... If I key on and let the pump prime and injector prime, then give the engine a very short burst of crank (short), then stop cranking and leaving the key on, then resume cranking it starts right away.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Signal roles: cam mentioned, crank mentioned Attachments: 640 cc miata 6-14 .msq -> https://speeduino.com/forum/download/file.php?id=11662; Good Start June 14.msq -> https://speeduino.com/forum/download/file.php?id=11659; Good Start June 14.rar -> https://speeduino.com/forum/download/file.php?id=11658 Image or capture links found: 10
- Matched search terms: idle taper

#### Difficult Miata Startups
- URL: https://speeduino.com/forum/viewtopic.php?t=5923#p63552
- Date: Fri Jun 16, 2023 9:45 am
- Who posted it: 92Yata
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Well with persistence and the heavy testing this past week. And with more thought on the history. This car just needs more fuel to start.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: crank mentioned Attachments: More Prime More Crank Good.msq -> https://speeduino.com/forum/download/file.php?id=11674; More Prime More Crank.rar -> https://speeduino.com/forum/download/file.php?id=11675; jason tweaked 09-may-23 no2.msq -> https://speeduino.com/forum/download/file.php?id=11679 Image or capture links found: 3
- Matched search terms: idle taper

#### 1985 Honda Goldwing Limited Edition FI Model ECU Replacement
- URL: https://speeduino.com/forum/viewtopic.php?t=5544#p59227
- Date: Mon Oct 31, 2022 3:45 pm
- Who posted it: Rednaxs60
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Thanks - much easier. The firmware I loaded is 202207.1. I am in the process of consolidating the information I have gathered regarding this project into one succinct folder.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 2-3
- Matched search terms: idle taper

#### 1985 Honda Goldwing Limited Edition FI Model ECU Replacement
- URL: https://speeduino.com/forum/viewtopic.php?t=5544#p65073
- Date: Sun Oct 01, 2023 5:17 pm
- Who posted it: Rednaxs60
- Evidence type: wheel description / image attachment
- Confidence: medium
- Summary: Progress with the new sensors. Have made brackets and installed the Hall effect sensors on the engine. Use the OEM 8 tooth crank trigger wheel.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Signal roles: cam mentioned, crank mentioned Explicit sync-loss discussion present. Attachments: Archive 6.zip -> https://speeduino.com/forum/download/file.php?id=12143; Cherry Hall Effect Sensor GS100701.pdf -> https://speeduino.com/forum/download/file.php?id=12141 Image or capture links found: 2
- Matched search terms: idle taper

#### TunerStudio 3.3 causing Speeduino to lock-up (fairly sure)
- URL: https://speeduino.com/forum/viewtopic.php?t=7387#p74248
- Date: Mon Mar 16, 2026 12:22 pm
- Who posted it: sako243
- Evidence type: scope capture
- Confidence: medium
- Summary: I haven't managed to (not really had the time) to find the cause of this but it appears that something between TunerStudio 3.2.0.5 and 3.3.0.1 (paid up version but that shouldn't matter) is dropping the connection and also (not always) causing the ECU to lock up. I've got far more anecdotal information but the general scenario has been this - I'm more than happy to try and provide more evidence if there's specific things that would help. Context / Setup I've got an ECU built around the DIY-EFI Core4 module running a 4.6V8 in a 6x6 Land Rover, it's effectively my daily driver now.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Signal roles: cam mentioned
- Matched search terms: idle up

#### Ignition rollback
- URL: https://speeduino.com/forum/viewtopic.php?t=7164#p73110
- Date: Thu Sep 11, 2025 11:31 am
- Who posted it: Philippov Pavel
- Evidence type: scope capture / tune file / wheel description
- Confidence: medium
- Summary: Hello, dear community members, creators of speeduino, this is a wonderful project, I really like it. I have already assembled 3 ECUs based on standard Toyota ECUs and 2560 modules (as a so-called motherboard, using factory power supply circuits, filtering input analog signals, processing rotation sensors and power output circuits for load control. Injectors and so on.) Everything works great except for one annoying nuance - ignition rollbacks.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 36-2, 6-7 Signal roles: cam mentioned, crank mentioned Attachments: 1jz-ge_speeduino.msq -> https://speeduino.com/forum/download/file.php?id=14448 Image or capture links found: 1
- Matched search terms: idle advance

#### Suzuki RE-5 EFI Conversion
- URL: https://speeduino.com/forum/viewtopic.php?t=5324#p66088
- Date: Fri Dec 01, 2023 4:11 am
- Who posted it: MGR550
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: Hello again, over a year later and little progress On the bright side, there now exists: a bike in the living room for convenient access, a colour matched wiring diagram, 3d printed 36-1 trigger wheel, brackets, an intake manifold, exhaust w/ WBO2 bung, a rough map of ignition timing and other settings, and last but not least a complete speeduino-side wiring harness that needs to be lightly spliced into the main bike harness. Please let me know if I've made any obvious errors in the wiring. I'd tested the TPS, CLT, IAT, sensors in the harness through tuner studio and all seemed to be responding well.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1 Attachments: 2024-02-19_15.39.46.csv -> https://speeduino.com/forum/download/file.php?id=12604; 2024-02-19_16.08.03.msq -> https://speeduino.com/forum/download/file.php?id=12603 Image or capture links found: 2
- Matched search terms: idle taper

#### HugoW’s 1998 MX5 NB with 2004 1.8 VVT engine
- URL: https://speeduino.com/forum/viewtopic.php?t=4413#p54619
- Date: Sun Jan 09, 2022 10:50 am
- Who posted it: HugoW
- Evidence type: technical discussion
- Confidence: medium
- Summary: Update and next issue... With a new starter battery the car is flung to life a lot more enhousiastically, less voltage drop. Adding a sh*tload of fuel was the final fix to the bad starting.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Signal roles: cam mentioned
- Matched search terms: delay before idle control

#### PCB for BMW m42/m50/m60 engine
- URL: https://speeduino.com/forum/viewtopic.php?t=2638#p63408
- Date: Tue Jun 06, 2023 9:38 pm
- Who posted it: LoStSOul
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: vcnt_m wrote: [pazi88](./memberlist.php?mode=viewprofile&u=2291) wrote: [↑](./viewtopic.php?p=62848#p62848) Fri May 05, 2023 12:23 pm [M5222](./memberlist.php?mode=viewprofile&u=6887) wrote: [↑](./viewtopic.php?p=62831#p62831) Thu May 04, 2023 3:38 pm vnld 5300-e or vnld 5300tr-e? right now, with the m40 2.1 bom at least, we are going to have problem with VNLD5090TR-E MPX4250AP LM2940T-5.0/NOPB STP75NS04Z VNLD5090TR-E: BM2LC105FJ-CE2 MPX4250AP: MPXA4250AC6U (2.5bar), MPXH6400AC6U (4bar) with adapter LM2940T-5.0/NOPB: L4941BV STP75NS04Z: STP62NS04Z Thank you very much! I have welded these components.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Helps separate universal defaults from context-specific tune choices.
- Specific details: Pattern mentions: 1-2, 2-3 Signal roles: cam mentioned
- Matched search terms: air con polarity

#### Crank to Run Taper
- URL: https://speeduino.com/forum/viewtopic.php?t=5910#p63320
- Date: Wed May 31, 2023 5:15 pm
- Who posted it: 92Yata
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Hey, I am running a speedyefi with firmware 202207.01 in my 1992 turbo 1,8 miata. I am not a tuner nor a coder but have been dealing with heatsoak engine starts that are not IAC related. Using the crank to run taper in open loop is beneficial here.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: crank mentioned Attachments: 10f closed loop.msq -> https://speeduino.com/forum/download/file.php?id=11593; Baldwin-10f-OPEN-LOOP J modified.msq -> https://speeduino.com/forum/download/file.php?id=11594; DataLogs.rar -> https://speeduino.com/forum/download/file.php?id=11595 Image or capture links found: 3
- Matched search terms: crank to run taper

#### Difficult Miata Startups
- URL: https://speeduino.com/forum/viewtopic.php?t=5923#p63515
- Date: Wed Jun 14, 2023 1:14 pm
- Who posted it: 92Yata
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: So after re-reading my post I thought I would try the map I was using with lower cranking enrich and a bit more prime. The car is warm but it also starts the car well when warm pretty well. This is closer to the fueling I should be using.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: crank mentioned Attachments: Lower Fuel Good Start.msq -> https://speeduino.com/forum/download/file.php?id=11655; Lower Fuel Start.rar -> https://speeduino.com/forum/download/file.php?id=11656; Same tune bad start.rar -> https://speeduino.com/forum/download/file.php?id=11657 Image or capture links found: 3
- Matched search terms: idle taper

#### Opel Astra G project, original Ecu + speeduino (Parallel/piggyback)
- URL: https://speeduino.com/forum/viewtopic.php?t=5644#p60901
- Date: Sun Jan 29, 2023 6:51 pm
- Who posted it: lovosal
- Evidence type: scope capture
- Confidence: medium
- Summary: Well, I finally finished the test bench and it has worked!! but with some problems. I turn on the coils and the spark plugs are already sparking.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: None extracted
- Matched search terms: idle taper

#### Opel Astra G project, original Ecu + speeduino (Parallel/piggyback)
- URL: https://speeduino.com/forum/viewtopic.php?t=5644#p61061
- Date: Fri Feb 03, 2023 10:42 pm
- Who posted it: lovosal
- Evidence type: scope capture
- Confidence: medium
- Summary: yes of course. The main error is the injector, which I thought was not working. I tested it by connecting 12v directly to the injector and it worked, but with the speeduino I can't get the injector to work.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: None extracted
- Matched search terms: idle taper

#### Crank to run IGNITION TAPER
- URL: https://speeduino.com/forum/viewtopic.php?t=5952#p64278
- Date: Tue Aug 01, 2023 7:12 pm
- Who posted it: PSIG
- Evidence type: scope capture
- Confidence: medium
- Summary: Use any method that will indicate the source of the noise. While I've used pocket AM radios, and oscilloscopes with short wire "antennas", you can also power or disable certain circuits to focus the search for noisemakers. For example, turn everything off you can, and crank.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: crank mentioned
- Matched search terms: crank to run taper

#### Firmware 202305 things
- URL: https://speeduino.com/forum/viewtopic.php?t=5917#p63419
- Date: Wed Jun 07, 2023 6:40 pm
- Who posted it: Ilotalo
- Evidence type: image attachment
- Confidence: medium
- Summary: I have some issues in my car after i put this newest firmware. My PWM open loop is cutting after idle raise over 1100 rpm. Why?
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Attachments: 2023-06-07_19.42.54.mlg.zip -> https://speeduino.com/forum/download/file.php?id=11619; 2023-06-09_18.12.22.csv.zip -> https://speeduino.com/forum/download/file.php?id=11627; 2023-06-09_20.50.29.mlg.zip -> https://speeduino.com/forum/download/file.php?id=11626 Image or capture links found: 5
- Matched search terms: delay before idle control

#### HugoW’s 1998 MX5 NB with 2004 1.8 VVT engine
- URL: https://speeduino.com/forum/viewtopic.php?t=4413#p55114
- Date: Wed Feb 02, 2022 8:50 pm
- Who posted it: HugoW
- Evidence type: technical discussion
- Confidence: medium
- Summary: I fixed the alternator controller connector this afternoon and did two tests today. First I switched off the VVT control and then I ran a 30 minute drive with varying conditions while autotuning. The tune was adjusted and a second drive proved the tune was faily accurate.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: None extracted
- Matched search terms: delay before idle control

#### PCB for BMW m42/m50/m60 engine
- URL: https://speeduino.com/forum/viewtopic.php?t=2638#p66011
- Date: Tue Nov 28, 2023 8:47 am
- Who posted it: pazi88
- Evidence type: wheel description
- Confidence: medium
- Summary: Yeah, this is the m50 firing order and how wasted spark works: 14770640155_91ea8d9d6d_b.jpg (73.98 KiB) Viewed 27972 times This is the M50NV wiring diagram: M50NV.png (65.9 KiB) Viewed 27972 times And here's the schematic for this board: schematic.png (19.92 KiB) Viewed 27972 times So: IGN Output 1 - pin 25 - Cyl 1 IGN Output 2 - pin 52 - Cyl 5 IGN Output 3 - pin 24 - Cyl 3 IGN Output 4 - pin 51 - Cyl 6 IGN Output 5 - pin 23 - Cyl 2 IGN Output 6 - pin 50 - Cyl 4 So all is correct for M50NV. In M50TU it's like this: M50TU.png (69.19 KiB) Viewed 27972 times So: IGN Output 1 - pin 25 - Cyl 5 IGN Output 2 - pin 52 - Cyl 3 IGN Output 3 - pin 24 - Cyl 6 IGN Output 4 - pin 51 - Cyl 2 IGN Output 5 - pin 23 - Cyl 4 IGN Output 6 - pin 50 - Cyl 1 Which means that the order is correct, but it's just shifted by 120 degrees. But so is the crank trigger wheel too in m50tu.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: Signal roles: crank mentioned
- Matched search terms: air con polarity

#### Ignition rollback
- URL: https://speeduino.com/forum/viewtopic.php?t=7164#p73243
- Date: Sun Sep 28, 2025 9:15 am
- Who posted it: Philippov Pavel
- Evidence type: tune file / wheel description / image attachment
- Confidence: low
- Summary: Hello, community members, thank you to everyone who responded; your opinions are important. I measured the crankshaft and camshaft signals. The signal processor clearly inverts the signals.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 3-5, 36-2 Edge or poll details: rising, falling, change Signal roles: cam mentioned, crank mentioned Attachments: 1jz-ge_speeduino.msq -> https://speeduino.com/forum/download/file.php?id=14448 Image or capture links found: 1
- Matched search terms: idle advance

#### Firmware 202305 things
- URL: https://speeduino.com/forum/viewtopic.php?t=5917#p63426
- Date: Thu Jun 08, 2023 5:24 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: low
- Summary: [Ilotalo](./memberlist.php?mode=viewprofile&u=2021) wrote: [↑](./viewtopic.php?p=63419#p63419) Wed Jun 07, 2023 6:40 pm I have some issues in my car after i put this newest firmware. You did not include your tune file, so I cannot see more info, but you appear to have tune stability issues. Before looking at it again, you should do some tuning, then post the new tune with a new log and any questions at that point.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change
- Matched search terms: delay before idle control

#### 6 and 8 cylinder sequential injection PR#280
- URL: https://speeduino.com/forum/viewtopic.php?t=3838#p46201
- Date: Tue Nov 03, 2020 4:34 pm
- Who posted it: Sunglasses
- Evidence type: technical discussion
- Confidence: low
- Summary: Yes, @pazi88 is right. There have been a few contributions since then (including mine) to get this feature working. You will need to edit the globals.h.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: None extracted
- Matched search terms: idle up

#### TunerStudio 3.3 causing Speeduino to lock-up (fairly sure)
- URL: https://speeduino.com/forum/viewtopic.php?t=7387#p74263
- Date: Tue Mar 17, 2026 7:53 am
- Who posted it: sako243
- Evidence type: scope capture
- Confidence: low
- Summary: I still don't know if it's related to the Silicon Labs chip that's on board the Core4. Originally when we had the engine running I just drilled a hole through the case to gain access to the USB port. I ran like this for a while with the USB cable coming through the bulkhead from the engine bay but started getting resets of the ECU whilst the engine was running.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: None extracted
- Matched search terms: idle up

#### Adventures in tuning and driving a UA4C, Supercharged BMW V8
- URL: https://speeduino.com/forum/viewtopic.php?t=5471#p58304
- Date: Tue Aug 23, 2022 9:13 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: low
- Summary: An awesome post of tips and trials. I've found that most "issues" with aftermarket ECMs have creative solutions, and often better than the coded ones that may approach from theory rather than reality. Post as much as you have time for.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: None extracted
- Matched search terms: delay before idle control

#### Pitbike EFI conversion
- URL: https://speeduino.com/forum/viewtopic.php?t=7380#p74256
- Date: Mon Mar 16, 2026 7:58 pm
- Who posted it: peca
- Evidence type: wheel description
- Confidence: low
- Summary: Thanks, that is now clear. Some information regarding this hobby project. Pitbike is a small supermoto bike with a single cylinder engine.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 1-2 Signal roles: cam mentioned
- Matched search terms: idle up

#### TunerStudio 3.3 causing Speeduino to lock-up (fairly sure)
- URL: https://speeduino.com/forum/viewtopic.php?t=7387#p74252
- Date: Mon Mar 16, 2026 6:03 pm
- Who posted it: sako243
- Evidence type: technical discussion
- Confidence: low
- Summary: I have not yet, I've been battling / debugging it a bit most of this morning and got some further anecdotal notes. After I posted that message I'd uninstalled 3.3 on my laptop and replaced it with 3.2 which I had an installer saved for (couldn't seem to find an archive of the older versions on EFI Analytics' website). When I came back from making a cup of tea TunerStudio had gone offline.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: cam mentioned
- Matched search terms: idle up

#### Coolant temperature issue
- URL: https://speeduino.com/forum/viewtopic.php?t=7372#p74181
- Date: Sat Feb 28, 2026 6:02 pm
- Who posted it: scudderfish
- Evidence type: technical discussion
- Confidence: low
- Summary: 82C is likely a default value if the ECU thinks the sensor (or something in between) is toast. Do you have another Mega to swap in? Another idea, do you have a working IAT?
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: idle up

#### PCB for BMW m42/m50/m60 engine
- URL: https://speeduino.com/forum/viewtopic.php?t=2638#p72514
- Date: Sun Jul 20, 2025 11:46 pm
- Who posted it: dkauffman
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: (the m50 vanos will have cam input too using stock hall sensor. The stock VR-crank sensors can be used and there is option to also use hall sensor for crank.) Hello, the previous paragraph means: that the m50 vanos uses a VR sensor on the crankshaft, and a hall sensor for the cam, my m50b25tu has a 2.3 plate with sequential Fw, the VR conditioner is a mini max A2, which was supplied to me by Weaver from WTM Tronic and the entire kit, when I set the trigger edge to rising as it should be for this conditioner, the engine rarely manages to start and makes a very loud sound, as if the flywheel was dragging the starter drive pinion, but when it manages to start the engine works perfectly, and when I set the trigger edge to falling the engine always starts perfectly, but this configuration is not the right one for this VR conditioner, I would like to ask what problem I may have, or if I can continue as is without having problems, thank you.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: rising, falling Signal roles: cam mentioned, crank mentioned Attachments: 2025-07-20_18.52.42.msq -> https://speeduino.com/forum/download/file.php?id=14179 Image or capture links found: 1
- Matched search terms: air con polarity

#### Firmware 202305 things
- URL: https://speeduino.com/forum/viewtopic.php?t=5917#p63442
- Date: Fri Jun 09, 2023 3:16 pm
- Who posted it: Ilotalo
- Evidence type: tooth log
- Confidence: low
- Summary: And tooth log loogs good.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: 2023-06-09_18.12.22.csv.zip -> https://speeduino.com/forum/download/file.php?id=11627 Image or capture links found: 1
- Matched search terms: delay before idle control

#### Crank to Run Taper
- URL: https://speeduino.com/forum/viewtopic.php?t=5910#p63323
- Date: Wed May 31, 2023 7:02 pm
- Who posted it: 92Yata
- Evidence type: technical discussion
- Confidence: low
- Summary: I did not write this correctly. I should have said it is not IAT related as this seems like a common issue. My apologies.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: crank mentioned
- Matched search terms: crank to run taper

### Knock and pin/default policy

#### Just another Speedy ECU
- URL: https://speeduino.com/forum/viewtopic.php?t=7329#p74172
- Date: Sat Feb 28, 2026 1:05 am
- Who posted it: Black Knight
- Evidence type: tooth log / scope capture
- Confidence: high
- Summary: Finalizing VR After a few revisions of the VR circuits we did come up with 4 that worked very well. The pics and wire diagrams are posted above. What we missed was posting the tooth logger so you can see how clean it looks and we also posted a pic of the oscope reading the AC input signal and the VR circuit output square wave.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Attachments: bayview posting.mlg -> https://speeduino.com/forum/download/file.php?id=14815; tooth logger cold start and run.csv -> https://speeduino.com/forum/download/file.php?id=14818; up moose.mlg -> https://speeduino.com/forum/download/file.php?id=14816 Image or capture links found: 3
- Matched search terms: knock sensor

#### E21 BMW 320i goes sequential spark
- URL: https://speeduino.com/forum/viewtopic.php?t=2893#p35872
- Date: Sat Jun 22, 2019 2:57 pm
- Who posted it: BigJ
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: I now have Speeduino lighting the fire with LS2 coils in an old BMW 320i and doing a splendid job of it so add me to your list of success stories. Josh, you rock. I know how much work this took.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Helps separate universal defaults from context-specific tune choices. Relevant to pin-selection policy and board-specific packaged settings. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 36-1 Edge or poll details: rising, falling, change Signal roles: cam mentioned, crank mentioned
- Matched search terms: external knock controller

#### Missing Knock Pins in tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7094#p72568
- Date: Sat Jul 26, 2025 8:32 am
- Who posted it: Patrick10072
- Evidence type: image attachment
- Confidence: medium
- Summary: Im planning a project using a dropbear v2. I've uploaded the latest firmware (20251.4) onto a teensy 4.1 i have. Ive been thoroughly looking into knock control for this project.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: Attachments: 202501.4 - Copy.ini -> https://speeduino.com/forum/download/file.php?id=14184 Image or capture links found: 1
- Matched search terms: knock_pin

#### Just another Speedy ECU
- URL: https://speeduino.com/forum/viewtopic.php?t=7329#p74068
- Date: Sat Feb 07, 2026 3:16 am
- Who posted it: Black Knight
- Evidence type: tooth log / scope capture / wheel description
- Confidence: medium
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=74065#p74065) Fri Feb 06, 2026 2:02 am The input impedance as around 100 Megohms. However, there is a signal capacitor that requires charging to read analog signals, and they suggest input at 10K ohms or less. I read that as 500 microamps (1/2 mA) minimum per-channel.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 26-8
- Matched search terms: knock sensor

#### Just another Speedy ECU
- URL: https://speeduino.com/forum/viewtopic.php?t=7329#p74153
- Date: Wed Feb 25, 2026 3:39 am
- Who posted it: Black Knight
- Evidence type: tooth log / scope capture
- Confidence: medium
- Summary: As we are learning various ways to design and build different circuits, like VR, power supply and others, we decided to build a pre prototype board to make all our mistakes on, also various circuits are mounted to the mother board by using header pins so we can play with circuit designs and plug them into the mother board for full testing. We took a couple of days to build this pre prototype board, build it into a project box with the correct connections to tie into the existing wire harness. Now we can test these circuits under real driving conditions.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: crank mentioned
- Matched search terms: knock sensor

#### Direct Injection Project - Let me be the first!
- URL: https://speeduino.com/forum/viewtopic.php?t=4088#p44461
- Date: Mon Jul 27, 2020 2:28 pm
- Who posted it: DeeeeC
- Evidence type: scope capture
- Confidence: medium
- Summary: JHolland wrote: You need to measure the injector voltage and current. The DC-DC converter stage can be tricky if you have high current because you can get voltage sag with the turn-on transient, big caps help but can affect the stability. You may also find that you only need high voltage for the peak and then battery voltage for the hold stage.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned
- Matched search terms: Teensy 4.1 knock

#### PCB for Electromotive Tec3r
- URL: https://speeduino.com/forum/viewtopic.php?t=4250#p45694
- Date: Tue Oct 06, 2020 8:29 pm
- Who posted it: R42BUG
- Evidence type: image attachment
- Confidence: medium
- Summary: Hi I buy along time ago a Tec3r (about 10 years) never installed until this year, I'm working in a mid engine v8 vw bug, after all the wire is done I check inputs and all looks find, and decide to upgrade firmware and Tec3r bricked (never run car) I contact electromotive support and they said this is common in this Ecu's due the serial comunication problems and need to send unit to repair, the repair cost is about $100 + $313 to hardware upgrade to WinTec4 to avoid serial communication trouble + shipping (from Mexico) $120 for Mex-Usa and Usa-Mex for that mony I think I go to made my own board. My engine have Low Z injectors and 4 waste spark dumb coils. The goal is to keep current harness (I dont want to rewire ecu) I try to mach this board to tec3r pinout.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings. Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: cam mentioned, crank mentioned Attachments: R42_ECU2_rev0.1_2020-10-07.zip -> https://speeduino.com/forum/download/file.php?id=7437; R42_ECU2_rev0.1_SCH.pdf -> https://speeduino.com/forum/download/file.php?id=7438 Image or capture links found: 2
- Matched search terms: Teensy 4.1 knock

#### NO2C 420cc Predator powered Honda Insight used on Robot Cantina video series
- URL: https://speeduino.com/forum/viewtopic.php?t=5177#p55729
- Date: Sun Mar 06, 2022 2:37 am
- Who posted it: Jimbo
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: Hi everyone, This post is for anyone interested in the 420 CC Predator powered Honda Insight used in the Robot Cantina youtube video series. In the most recent video in the series I have invited the viewers over to this forum to discuss possible improvements to the tune on the car. Predator 420 cc engine Speeduino NO2C VR conditioner Mini max A2 V12 AEM 30-300 Wideband Ford 4.6 36-1 crank wheel Jeep 4.0 VR crank position sensor 30mm throttle body 310cc/min injector ignition by GM HEI 4 pin module Ford 4.6 ignition coil We built the car for the single purpose of going as fast as possible with a small engine.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Pattern mentions: 36-1 Signal roles: crank mentioned Attachments: 2022-03-04_11.18.12.txt.txt -> https://speeduino.com/forum/download/file.php?id=9426; 2022-03-04_12.09.51.msq -> https://speeduino.com/forum/download/file.php?id=9425; 2022-03-04_12.09.51.msq -> https://speeduino.com/forum/download/file.php?id=9430 Image or capture links found: 3
- Matched search terms: external knock controller

#### Direct Injection Project - Let me be the first!
- URL: https://speeduino.com/forum/viewtopic.php?t=4088#p44194
- Date: Sat Jul 11, 2020 1:18 pm
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [DeeeeC](./memberlist.php?mode=viewprofile&u=3486) wrote: [↑](./viewtopic.php?p=44184#p44184) Fri Jul 10, 2020 5:57 pm Yes. Alfa were really ahead of the curve with direct injection but this engine was much-hated for general GDI issues like coking of the inlet valves and high oil consumption. I think I have one of the last 100 or so JTS models still running.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Signal roles: cam mentioned
- Matched search terms: Teensy 4.1 knock

#### Direct Injection Project - Let me be the first!
- URL: https://speeduino.com/forum/viewtopic.php?t=4088#p44170
- Date: Fri Jul 10, 2020 12:58 am
- Who posted it: DeeeeC
- Evidence type: technical discussion
- Confidence: medium
- Summary: Hi all, I introduced myself in the General Forum. I have an Alfa Romeo 156 JTS 2004 model year 2.0L. [0] Introduction I was told that Speeduino doesn't do direct injection - the post has had 200 views.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Relevant to pin-selection policy and board-specific packaged settings. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Edge or poll details: change Signal roles: cam mentioned
- Matched search terms: Teensy 4.1 knock

#### PCB for BMW m52 engine
- URL: https://speeduino.com/forum/viewtopic.php?t=2527#p31340
- Date: Fri Jan 18, 2019 11:59 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [pazi88](./memberlist.php?mode=viewprofile&u=2291) wrote: [↑](./viewtopic.php?p=31339#p31339) Fri Jan 18, 2019 7:41 am [janithdg](./memberlist.php?mode=viewprofile&u=1812) wrote: [↑](./viewtopic.php?p=31338#p31338) Fri Jan 18, 2019 7:02 am Hi Pazi88, i think by using unused io's of the mega, it should be possible to have 6 inj seq fuel and this it will be cool if flat shift input and fan control outs in a separate socket Yes, but that would require code changes and I'm not currently capable of doing those. Maybe in future But I don't know if mega has the grunt to drive 6 cyl sequential. Not so actually, you could have 6cyl seq fuel OR 6cyl sequential spark with a mega, but not both.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Edge or poll details: change
- Matched search terms: Teensy 4.1 knock

#### 87 f-150 5.0 supercharged build using UA4C vC5
- URL: https://speeduino.com/forum/viewtopic.php?t=6229#p72508
- Date: Sun Jul 20, 2025 6:00 am
- Who posted it: Richard Burnside
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: I had been wondering about the balance issue with twin turbos and how much of an issue it would be. I haven't decided on what I'm going to do yet, but leaving it NA is firmly on the table. So far I'm liking what I'm seeing with it being NA and I'm seeing improvements every time I'm able to test it.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: 2025-08-19_18.18.10.mlg -> https://speeduino.com/forum/download/file.php?id=14357; 2025-11-11_16.59.19.mlg -> https://speeduino.com/forum/download/file.php?id=14542; CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=14358 Image or capture links found: 4
- Matched search terms: knock input

#### NO2C 420cc Predator powered Honda Insight used on Robot Cantina video series
- URL: https://speeduino.com/forum/viewtopic.php?t=5177#p55735
- Date: Sun Mar 06, 2022 5:01 pm
- Who posted it: digmorepaka
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: Hi Jimbo! I've been following the series for a while and I'm quite exctied that you have posted these here. As other forum members have mentioned, it is running quite rich, and you could get more out of it with extra ignition timing, a knock detection setup, either detcans, a discrete amplifier, or even a full controller like speedyknock(once that's released) would be able to help quite a lot.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: Attachments: 2022-03-04_12.09.51.msq -> https://speeduino.com/forum/download/file.php?id=9430 Image or capture links found: 1
- Matched search terms: external knock controller

#### PCB for Electromotive Tec3r
- URL: https://speeduino.com/forum/viewtopic.php?t=4250#p45952
- Date: Sun Oct 18, 2020 10:03 pm
- Who posted it: R42BUG
- Evidence type: image attachment
- Confidence: low
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=45908#p45908) Fri Oct 16, 2020 9:50 pm Random comments: "Absolute Maximum Ratings" is not useful for planning suitability of the component, except when something goes horribly wrong. While the part indicates 3V to 5V signal voltage, the ratings clearly indicate 5V is intended, 3V will work for function, but will have a large impact on the current it will carry (large reduction in current before overheat). Capacity in most of these cases is not strictly "amps" but RDS (ON) , which limits by calculation how much the device can handle without overheating.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: vns3nv04dp-e.pdf -> https://speeduino.com/forum/download/file.php?id=7468 Image or capture links found: 1
- Matched search terms: Teensy 4.1 knock

#### blitzbox
- URL: https://speeduino.com/forum/viewtopic.php?t=2900#p42868
- Date: Thu May 14, 2020 6:04 pm
- Who posted it: chaoschris
- Evidence type: image attachment
- Confidence: low
- Summary: Hi folks, after fiddling a while with the atmega2561 design tooly and me decided to change the project to the standard atmega2560. We also changed the Molex connector to a TE variant with 34 pins. There is also a wideband lambda controller onboard (LSU 4.9) with the Bosch CJ125 and a separate Atmega328P to relieve the speeduino cpu.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Signal roles: cam mentioned, crank mentioned Attachments: schematics.pdf -> https://speeduino.com/forum/download/file.php?id=6823 Image or capture links found: 1
- Matched search terms: external knock controller

#### blitzbox
- URL: https://speeduino.com/forum/viewtopic.php?t=2900#p47641
- Date: Mon Jan 18, 2021 9:22 am
- Who posted it: Chris Wolfson
- Evidence type: image attachment
- Confidence: low
- Summary: So this diode issue is hardly one. I do not know any factory ECU with external diodes. Fine.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: BOM_blitzboxBL49sp_V0.1.0.xlsx -> https://speeduino.com/forum/download/file.php?id=8735; CPL_blitzboxBL49sp_V0.1.0.xlsx -> https://speeduino.com/forum/download/file.php?id=8736; V0.1.0.zip -> https://speeduino.com/forum/download/file.php?id=8737 Image or capture links found: 3
- Matched search terms: external knock controller

#### blitzbox
- URL: https://speeduino.com/forum/viewtopic.php?t=2900#p52268
- Date: Tue Aug 03, 2021 5:39 pm
- Who posted it: chaoschris
- Evidence type: image attachment
- Confidence: low
- Summary: Hi guys, i wanted to publish the fabrication files for the newer version 0.1.0 of the blitzbox, but something is going wrong with the the positioning of the parts during the order process. There must have been something changed at JLCPCB or i am too blind that i cannot see my error. All is working fine until the review page where you can inspect the position and alignment of the assembled parts.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: change Attachments: BOM_blitzboxBL49sp_V0.1.0.xlsx -> https://speeduino.com/forum/download/file.php?id=8735; CPL_blitzboxBL49sp_V0.1.0.xlsx -> https://speeduino.com/forum/download/file.php?id=8736; V0.1.0.zip -> https://speeduino.com/forum/download/file.php?id=8737 Image or capture links found: 3
- Matched search terms: external knock controller

#### Speeduino (atmega) vs dropbear for rover V8
- URL: https://speeduino.com/forum/viewtopic.php?t=5660#p60488
- Date: Thu Jan 12, 2023 2:17 pm
- Who posted it: prelude
- Evidence type: technical discussion
- Confidence: low
- Summary: I am currently in the process of converting a P38 range rover into an overland rig. It is powered by the 4.6 rover V8 (GEMS) engine. Preferably I would like to make a drop in replacement for the EFI.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: DropBear knock

#### 87 f-150 5.0 supercharged build using UA4C vC5
- URL: https://speeduino.com/forum/viewtopic.php?t=6229#p71981
- Date: Wed Jun 11, 2025 7:04 pm
- Who posted it: jonbill
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: under tuning -> afr02, set the algorithm to 'no correction' - the tunes far too raw for any closed loop afr control. in engine constants, squirts per cycle should be 2 (or more) . do you have a knock sensor?
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: Attachments: 2025-06-18_15.15.51.mlg -> https://speeduino.com/forum/download/file.php?id=14098; 2025-06-18_16.17.16.mlg -> https://speeduino.com/forum/download/file.php?id=14095; 2025-06-18_19.55.07.mlg -> https://speeduino.com/forum/download/file.php?id=14096 Image or capture links found: 8
- Matched search terms: knock input

#### 87 f-150 5.0 supercharged build using UA4C vC5
- URL: https://speeduino.com/forum/viewtopic.php?t=6229#p71976
- Date: Wed Jun 11, 2025 4:44 pm
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: low
- Summary: +1 to post bundled logs of issues with the exact tune used to take them. IMO, there is no such thing as "conservative" or "safe" timing, except correct timing. Whether you call it optimal, correct or most-efficient timing is safest.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: knock input

#### Alfa Romeo 147 2.0 TS 16v - race car - V0.4 board
- URL: https://speeduino.com/forum/viewtopic.php?t=6916#p71962
- Date: Mon Jun 09, 2025 9:38 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: As you have the part numbers for the sensors you will most likely find the pin outs online easily. I find searching parts suppliers on eBay very useful for pin outs as they often post these details and images of connectors to help identification. The easiest way to connect to serial3 is to fit a stackable type through header on the vo4 rather than just pins .
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: knock input

#### PCB for BMW m52 engine
- URL: https://speeduino.com/forum/viewtopic.php?t=2527#p31382
- Date: Sun Jan 20, 2019 3:21 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [pazi88](./memberlist.php?mode=viewprofile&u=2291) wrote: [↑](./viewtopic.php?p=31376#p31376) Sun Jan 20, 2019 12:11 pm [dazq](./memberlist.php?mode=viewprofile&u=186) wrote: [↑](./viewtopic.php?p=31344#p31344) Fri Jan 18, 2019 5:19 pm None of the outputs need to be pwm or any other kind of dedicated pin! The pwm waveform for vvt ,boost and idle is created within the firmware and thus can be pointed anywhere. Same goes for the pulse outputs to injectors and the spark drives.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: Teensy 4.1 knock

#### PCB for BMW m52 engine
- URL: https://speeduino.com/forum/viewtopic.php?t=2527#p32120
- Date: Fri Feb 15, 2019 1:15 pm
- Who posted it: dazq
- Evidence type: wheel description / maintainer explanation
- Confidence: low
- Summary: [pazi88](./memberlist.php?mode=viewprofile&u=2291) wrote: [↑](./viewtopic.php?p=32071#p32071) Wed Feb 13, 2019 4:41 pm With careful massaging I was able to squeeze in the 6-pin minifit connector. And still keep the MAP sensor and MEGA so that there is no need to cut big holes for those. So I think the rev1.2 is now ready: [https://easyeda.com/pazi88/speeduino-v0 ...
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Pattern mentions: 4-3 Signal roles: cam mentioned
- Matched search terms: Teensy 4.1 knock

#### PCB for Electromotive Tec3r
- URL: https://speeduino.com/forum/viewtopic.php?t=4250#p45859
- Date: Wed Oct 14, 2020 10:41 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [Dagaz_Psi](./memberlist.php?mode=viewprofile&u=3059) wrote: [↑](./viewtopic.php?p=45850#p45850) Tue Oct 13, 2020 11:42 pm Why the dual max9924 and not one 9926? Some overvoltage calmping on the analog inputs? In josh latest stream he said he had som problems with vnld5160, the teensy didnt supply enough current to open the gate fully.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: Teensy 4.1 knock

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7211#p73386
- Date: Sat Oct 18, 2025 12:17 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [PVK](./memberlist.php?mode=viewprofile&u=10031) wrote: [↑](./viewtopic.php?p=73385#p73385) Fri Oct 17, 2025 9:40 pm I see now that Knock control is a feature of 202501: [https://github.com/speeduino/speeduino ... tag/202501](https://github.com/speeduino/speeduino/releases/tag/202501) Ik don't understand that I can't select the right Tunerstudio pin... As it says in that version , initial implementation, it is still under development by josh , that's what I meant by it not being a feature , as it's still WIP Suggest you post an issue on GitHub for the pin thing to be looked into (assuming it is not already logged)
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Teensy based mainboard
- URL: https://speeduino.com/forum/viewtopic.php?t=4657#p50189
- Date: Sun May 09, 2021 10:00 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: There is support for teensy 3.6 and even a few boards designed and in use. External data stream out is possible via serial and via canbus at this time , but data input is only via serial right now though work is under way to support can too. Sounds like an exciting project, you should consider joining the slack group for techy discussions.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: Teensy 4.1 knock

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7211#p73377
- Date: Fri Oct 17, 2025 11:47 am
- Who posted it: PVK
- Evidence type: technical discussion
- Confidence: low
- Summary: I have a Speeduino Dropbear V2 and I want to add a Knock sensor with an external controller that puts out a digital High/Low signal. As seen here: [https://wiki.speeduino.com/en/boards/dropbear](https://wiki.speeduino.com/en/boards/dropbear) there is no dedicated pin available so I have to use one of the spare pins: Spare Digital 1: tunerstudio pin 35 Spare Digital 2: tunerstudio pin 34 Spare Analog 1 : Tunerstudio pin A16 Spare Analog 2 : Tunerstudio pin A17 The problem is that in Tunerstudio the pin that can be selected are: digital pins 2,3,18,19 and 20 or analog pins A0 - A15. I don’t understand that the firmware has the option for Knock sense but the right pin can’t be selected.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Missing Knock Pins in tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7094#p72569
- Date: Sat Jul 26, 2025 9:19 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: You won't find the board pin list in the ini or TS. The pins for each board type are set in the init CPP in the firmware. Please note that adding options in the ini won't necessarily add a working feature or option in the firmware.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7211#p73378
- Date: Fri Oct 17, 2025 12:11 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: Knock sense is not yet an actual feature as it is still under development at this time.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: knock_pin

#### Teensy based mainboard
- URL: https://speeduino.com/forum/viewtopic.php?t=4657#p50217
- Date: Mon May 10, 2021 11:29 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [JRockland](./memberlist.php?mode=viewprofile&u=4641) wrote: [↑](./viewtopic.php?p=50215#p50215) Mon May 10, 2021 9:58 pm OpelPanFan have a board who work with 3.6 on easyeda. Also im in for a ton of sensor as Im trying to make a board for a M73 (v12)... so 2 map sensors, 2 wideband sensors, 2 air intake sensors..
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Signal roles: cam mentioned
- Matched search terms: Teensy 4.1 knock

### Tune compatibility, base tune, and INI/default drift

#### LibreTune - Rust and Tauri powered FOSS Tuining Suite - GPLv2
- URL: https://speeduino.com/forum/viewtopic.php?t=7313#p73904
- Date: Sun Jan 11, 2026 7:46 pm
- Who posted it: RallyPat
- Evidence type: tooth log / composite log / tune file
- Confidence: high
- Summary: I have the nightly releases setup now. You can get them here: [https://github.com/RallyPat/LibreTune/r ... ag/nightly](https://github.com/RallyPat/LibreTune/releases/tag/nightly) Heres everything implemented since our first commit.
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations. Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change
- Matched search terms: msq

#### Speeduino system VW vanagon WBX 2.1 DJ with hiccups
- URL: https://speeduino.com/forum/viewtopic.php?t=7123#p72735
- Date: Sat Aug 09, 2025 12:39 pm
- Who posted it: Iskander
- Evidence type: tooth log / scope capture / tune file
- Confidence: high
- Summary: Hi I installed a speeduino system on my VW vanagon that still runs the original WBX 2.1 engine. I use TS version:MSv3.2.05 De system works fine but I have troubles with EMI. The system I built is similar as the Speeduino V0.4.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: cam mentioned Attachments: 4_2025-05-03_14.19.29.zip -> https://speeduino.com/forum/download/file.php?id=14294; CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=14295; ECU grounding do s & don’t s by Adaptronic.pdf -> https://speeduino.com/forum/download/file.php?id=14310 Image or capture links found: 5
- Matched search terms: msq

#### Abnormal values on TunerStudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7104#p72698
- Date: Tue Aug 05, 2025 9:24 pm
- Who posted it: Russell Diao
- Evidence type: tune file / image attachment
- Confidence: high
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=72664#p72664) Mon Aug 04, 2025 5:30 pm You appear to be missing a basic step. Follow the [Wiki](https://wiki.speeduino.com/en/Installing_Firmware) process (or SpeedyLoader below) of loading firmware onto the Mega, then loading a Base Tune onto the Mega, then connecting to TunerStudio and loading the matching INI file in Project Properties , for selecting board and calibrating sensors. When completed – with the Mega connected to TunerStudio, flick File > Save Tune As and name the tune Russell Diao.msq and save.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=14249 Image or capture links found: 1
- Matched search terms: speeduino 202501

#### Progress report - August 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=2062#p28275
- Date: Tue Sep 04, 2018 1:16 pm
- Who posted it: noisymime
- Evidence type: wheel description / maintainer explanation
- Confidence: high
- Summary: It's been one of those times when 1 month just wasn't enough for everything I wanted to get done in this month! The biggest change by far this month are the huge modifications to the ignition timing and scheduling system. At a high level, this has extended the older per tooth timing mode and brings a significant increase in timing accuracy.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Helps separate universal defaults from context-specific tune choices. Relevant to pin-selection policy and board-specific packaged settings. Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: basic distributor Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: tune migration

#### Speeduino system VW vanagon WBX 2.1 DJ with hiccups
- URL: https://speeduino.com/forum/viewtopic.php?t=7123#p73714
- Date: Mon Dec 08, 2025 11:13 am
- Who posted it: Iskander
- Evidence type: tooth log / wheel description
- Confidence: high
- Summary: Hi I have done some test again. The new spark plugs arrived (Denso PK20PR-P8) and installed. I changed the WUE that its end at 100% Drived a short trip.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Edge or poll details: rising, change Attachments: VR signal test 560RPM.zip -> https://speeduino.com/forum/download/file.php?id=14644 Image or capture links found: 1
- Matched search terms: msq

#### 1981 VW Rabbit here for research (Build)
- URL: https://speeduino.com/forum/viewtopic.php?t=4492#p67619
- Date: Sun Mar 24, 2024 10:48 am
- Who posted it: soultron
- Evidence type: technical discussion
- Confidence: high
- Summary: I had the car running on a version of this tune but you're likely onto something. Starting from scratch could help. I've made a lot of hardware changes since then and I also haven't updated the firmware.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change
- Matched search terms: tunerstudio defaults

#### NO2C Fuel and spark LEDs
- URL: https://speeduino.com/forum/viewtopic.php?t=7281#p73727
- Date: Wed Dec 10, 2025 11:57 pm
- Who posted it: PeterJones
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: Yes I assembled this board from a WM Kit. It has done many hours of testing on ardustim, and It had run, for up to 15 minutes at a time previously. The Tuner Studio hardware tests for injector and spark run fine (apart from the ignition LED, which is where we came in) A base tune has been downloaded and applied numerous times, and then modified from there, but my project and the base are so far apart about the only thing in common is they are both four stroke.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 18-1 Edge or poll details: rising Signal roles: cam mentioned Attachments: 2025-12-11_12.55.31.msq.zip -> https://speeduino.com/forum/download/file.php?id=14661 Image or capture links found: 1
- Matched search terms: base tune

#### UA4C Board NO SPARK!!
- URL: https://speeduino.com/forum/viewtopic.php?t=7169#p73136
- Date: Sat Sep 13, 2025 11:37 am
- Who posted it: klialex
- Evidence type: scope capture / tune file
- Confidence: medium
- Summary: Hi, friends! I am assembling a project on UA4C. 4 cylinders, 2+2 ignition coil - wasted spark.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned Attachments: sierra_sr20det.msq -> https://speeduino.com/forum/download/file.php?id=14407 Image or capture links found: 1
- Matched search terms: base map

#### Triumph TR5 Speeduino build / frequent resets
- URL: https://speeduino.com/forum/viewtopic.php?t=7108#p72628
- Date: Sat Aug 02, 2025 3:57 pm
- Who posted it: newyankee
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: It's my first EFI build and my first post. 1968 Triumph TR5 PI. TR5 20210718_153857.jpg (89.35 KiB) Viewed 385921 times 2.5 litre 6-in-line engine, originally with a LUCAS mechanical injection.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 60-2 Attachments: 2025-08-03_18.25.31.csv -> https://speeduino.com/forum/download/file.php?id=14231; 2025-08-03_18.26.48.csv -> https://speeduino.com/forum/download/file.php?id=14232; 2025-08-03_18.27.45.csv -> https://speeduino.com/forum/download/file.php?id=14233 Image or capture links found: 4
- Matched search terms: speeduino 202501

#### Where find the MSQ base tuning file for version 202305?
- URL: https://speeduino.com/forum/viewtopic.php?t=7112#p72675
- Date: Tue Aug 05, 2025 9:14 am
- Who posted it: LPG2CV
- Evidence type: image attachment
- Confidence: medium
- Summary: In SpeedyLoader, select the version of firmware you have (but don't download it), then click on the Get Base Tune button. It should enable you to download the relevant tune. It's also important to have the correct .INI file.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Attachments: 202305.ini -> https://speeduino.com/forum/download/file.php?id=14246 Image or capture links found: 1
- Matched search terms: ini mismatch

#### 1981 VW Rabbit here for research (Build)
- URL: https://speeduino.com/forum/viewtopic.php?t=4492#p67674
- Date: Wed Mar 27, 2024 3:47 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: medium
- Summary: [soultron](./memberlist.php?mode=viewprofile&u=4308) wrote: [↑](./viewtopic.php?p=67666#p67666) Wed Mar 27, 2024 3:38 am ok, That's a parameter I didn't set. … There's either a very specific thing I'm missing … This is the default setting in TunerStudio: DE_default_100.jpg (8.42 KiB) Viewed 5126 times If you did not change it: the tune is corrupted, or the tune was not built on a Base Tune. See my [earlier post](https://speeduino.com/forum/viewtopic.php?p=67617#p67617) .
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change
- Matched search terms: tunerstudio defaults

#### 1981 VW Rabbit here for research (Build)
- URL: https://speeduino.com/forum/viewtopic.php?t=4492#p67679
- Date: Thu Mar 28, 2024 2:47 am
- Who posted it: soultron
- Evidence type: technical discussion
- Confidence: medium
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=67674#p67674) Wed Mar 27, 2024 3:47 pm See my [earlier post](https://speeduino.com/forum/viewtopic.php?p=67617#p67617) . The Base Tune sets the defaults that you then change specific settings for your specific engine, injectors, sensors, etc. I strongly suggest you download a Base Tune and modify it, in order to avoid missing settings that could prevent proper operation.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change
- Matched search terms: tunerstudio defaults

#### Abnormal values on TunerStudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7104#p72604
- Date: Thu Jul 31, 2025 1:24 pm
- Who posted it: Russell Diao
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Hi all, I'm experiencing a problem with TunerStudio displaying incorrect or extreme sensor values after uploading firmware. I'm using the 202501.4 official release of Speeduino on an Arduino Mega 2560, and uploading the firmware using Arduino IDE 2.3.6 — the upload completes without any errors. I'm also using the matching .ini file from the /reference folder of the same firmware version.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: change Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=14199 Image or capture links found: 1
- Matched search terms: speeduino 202501

#### VW Golf Mk3 2.0 8V NO2C Build
- URL: https://speeduino.com/forum/viewtopic.php?t=6742#p71008
- Date: Thu Feb 27, 2025 4:32 am
- Who posted it: sandedmk.3
- Evidence type: technical discussion
- Confidence: medium
- Summary: Reinstalled a clean version of 202501 that I did not code any inputs or outputs on so hopefully I can get this to come around. I got an error when loading in the base tune through TS that said it was for a different firmware and i loaded it anyways. It had a knock sensor set to pin 3 as an analog sensor but I turned it off as its not needed quiet yet and can be figured out later I just want to make sure that the tune being loaded onto this firmware is going to work properly.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: None extracted
- Matched search terms: signature

#### Progress report - August 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=2062#p28957
- Date: Mon Sep 17, 2018 10:51 pm
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [jmunkdk](./memberlist.php?mode=viewprofile&u=1301) wrote: [↑](./viewtopic.php?p=28954#p28954) Mon Sep 17, 2018 8:50 pm I have tried your update and paid attention to the aux inputs, and I am pleased to tell that it now runs good. Thank you for looking into this. Thanks mate, good to hear!
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change
- Matched search terms: tune migration

#### IDLE Control via Advance When Sparktable2 is Enabled
- URL: https://speeduino.com/forum/viewtopic.php?t=7105#p72618
- Date: Fri Aug 01, 2025 6:39 pm
- Who posted it: @deywde
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Hello everyone, First of all, I’d like to thank you all for your dedication in keeping this project running — you’re amazing. I’d like to share an issue I’ve been experiencing with my Speeduino setup. When I activate the secondary ignition table ( Sparktable2 ) via digital input pin 36, I’ve noticed that the idle control through ignition advance stops working, and the “AdvanceCurrent” value simply mirrors what’s set in Sparktable2.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=14205; SECOND SPARK TABLE_16.28.24.mlg -> https://speeduino.com/forum/download/file.php?id=14271 Image or capture links found: 2
- Matched search terms: speeduino 202501

#### O2 Control PID settings
- URL: https://speeduino.com/forum/viewtopic.php?t=6799#p71179
- Date: Wed Mar 12, 2025 2:31 am
- Who posted it: BlackBird_SR71
- Evidence type: tune file / wheel description
- Confidence: medium
- Summary: Having problems getting a starting point for O2 control PID settings. I've tried many different settings and I seem to have big oscillation problems with even the the Base Tunes that are listed in Speedyloaderx64 ( Base, Miata and Various BMW's . Using an AEM 30-0300 X series AFR Gauge and Controller.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Pattern mentions: 02-2
- Matched search terms: signature

#### Progress report - August 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=2062#p28917
- Date: Mon Sep 17, 2018 3:29 am
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [jmunkdk](./memberlist.php?mode=viewprofile&u=1301) wrote: [↑](./viewtopic.php?p=28757#p28757) Tue Sep 11, 2018 6:35 pm I have tried this, but the result is the same. Kind of like it is running on 3 cylinders. Just to be sure, I have rolled back the last version, and it then runs fine again.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: tune migration

#### Triumph TR5 Speeduino build / frequent resets
- URL: https://speeduino.com/forum/viewtopic.php?t=7108#p72815
- Date: Thu Aug 14, 2025 8:02 am
- Who posted it: newyankee
- Evidence type: scope capture
- Confidence: medium
- Summary: So I followed up the suggestion by PSIG from further up. [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=72635#p72635) Sat Aug 02, 2025 7:12 pm Your reset idea has merit. While this could be due to a power glitch, I see no logged voltage change.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Edge or poll details: change Attachments: 2025-08-14_19.50.48.mlg -> https://speeduino.com/forum/download/file.php?id=14348 Image or capture links found: 1
- Matched search terms: speeduino 202501

#### VSS input to serial3.
- URL: https://speeduino.com/forum/viewtopic.php?t=6675#p71822
- Date: Fri May 09, 2025 10:11 am
- Who posted it: kettlekev
- Evidence type: scope capture
- Confidence: medium
- Summary: So. I went to github and downloaded the speeduino-202501.3 files. Then I modified init.cpp file again for the UA4C case moving the flex pin to pin 21 and in TS assigning VSS to pin 20.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: speeduino 202501

#### Speeduino system VW vanagon WBX 2.1 DJ with hiccups
- URL: https://speeduino.com/forum/viewtopic.php?t=7123#p73674
- Date: Sat Nov 29, 2025 10:15 pm
- Who posted it: Iskander
- Evidence type: image attachment
- Confidence: medium
- Summary: I‘ve made some improvements but there rise also more questions? I’ve tried several things to solve my hiccup problem. -2 Ferite bead over the complete loom entrée at ECU -MSD spark cables far away from speeduino wiring loom -DIS bobine earth cable outside the loom connected to other cylinder head -New VR sensor with separate cable outside the loom, shielded twisted pair.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Explicit sync-loss discussion present. Attachments: Test drive 29112025.mlg -> https://speeduino.com/forum/download/file.php?id=14622 Image or capture links found: 1
- Matched search terms: msq

#### Triumph TR5 Speeduino build / frequent resets
- URL: https://speeduino.com/forum/viewtopic.php?t=7108#p72721
- Date: Fri Aug 08, 2025 8:09 am
- Who posted it: newyankee
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: LPG2CV wrote: Check you haven't pushed back a pin in the connector. And pay particular attention to the screw connectors for ground and supply. I can't see any calibration in clt and iat!
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: change Attachments: 2025-08-03_09.50.52.msq -> https://speeduino.com/forum/download/file.php?id=14265; 2025-08-07_20.54.57.csv -> https://speeduino.com/forum/download/file.php?id=14263; 2025-08-07_21.05.20.csv -> https://speeduino.com/forum/download/file.php?id=14264 Image or capture links found: 3
- Matched search terms: speeduino 202501

#### VW Golf Mk3 2.0 8V NO2C Build
- URL: https://speeduino.com/forum/viewtopic.php?t=6742#p70979
- Date: Tue Feb 25, 2025 11:09 pm
- Who posted it: LPG2CV
- Evidence type: technical discussion
- Confidence: medium
- Summary: Hi First off, are you sure you loaded a base tune? The reason I say this is because looking at the I/O summary, you only have one default vaule, and I would anticipate a lot more of these,! A standard value for a setting is often 100% Less than and you get less fuel or dwell, and more idicates more fuel or dwell.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: signature

#### VW Golf Mk3 2.0 8V NO2C Build
- URL: https://speeduino.com/forum/viewtopic.php?t=6742#p71031
- Date: Fri Feb 28, 2025 12:33 am
- Who posted it: sandedmk.3
- Evidence type: technical discussion
- Confidence: medium
- Summary: put the 202310 firmware on my arduino and uploaded a newly downloaded generic base tune. I only got error messages for the FPR and oil pressure. The I/O summary only had 4 things highlited which were the fan, fuel pump, tach and reset pins all labeled "Board default" .
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: signature

#### Abnormal values on TunerStudio
- URL: https://speeduino.com/forum/viewtopic.php?t=7104#p72664
- Date: Mon Aug 04, 2025 5:30 pm
- Who posted it: PSIG
- Evidence type: tune file
- Confidence: medium
- Summary: You appear to be missing a basic step. Follow the [Wiki](https://wiki.speeduino.com/en/Installing_Firmware) process (or SpeedyLoader below) of loading firmware onto the Mega, then loading a Base Tune onto the Mega, then connecting to TunerStudio and loading the matching INI file in Project Properties , for selecting board and calibrating sensors. When completed – with the Mega connected to TunerStudio, flick File > Save Tune As and name the tune Russell Diao.msq and save.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: speeduino 202501

#### Where find the MSQ base tuning file for version 202305?
- URL: https://speeduino.com/forum/viewtopic.php?t=7112#p72666
- Date: Tue Aug 05, 2025 1:21 am
- Who posted it: keanecn
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Where can I find the MSQ base tuning file for version **202305**? I have an old commercial ECU (see post: [viewtopic.php?t=7084](https://speeduino.com/forum/viewtopic.php?t=7084) ), and its firmware version is 202305. I want to restore factory settings and reset parameters.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: 202305.ini -> https://speeduino.com/forum/download/file.php?id=14246 Image or capture links found: 1
- Matched search terms: ini mismatch

#### not getting 5v
- URL: https://speeduino.com/forum/viewtopic.php?t=7270#p73738
- Date: Fri Dec 12, 2025 7:30 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: medium
- Summary: Which PCB version? Which pin? (IDC, processor, or... ?) Is firmware loaded? Is a base tune loaded? Have you selected your PCB version in TunerStudio? Is the board powered with clean 12V? While the PCB may have activity on some pins, many will not be correctly functional or any function for testing until all the above is complete.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: base tune

#### VSS input to serial3.
- URL: https://speeduino.com/forum/viewtopic.php?t=6675#p71865
- Date: Wed May 21, 2025 8:15 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [kettlekev](./memberlist.php?mode=viewprofile&u=3980) wrote: [↑](./viewtopic.php?p=71864#p71864) Wed May 21, 2025 5:12 am OK some progress. I started with confirming that my Honda VSS only puts out a 1v square wave. It is a 3 wire sensor (12v, 0v and signal) so I am guessing a hall effect sensor.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Edge or poll details: change
- Matched search terms: speeduino 202501

#### Issue with flex fuel settings
- URL: https://speeduino.com/forum/viewtopic.php?t=5765#p61544
- Date: Tue Feb 28, 2023 7:15 am
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: medium
- Summary: [Discovette](./memberlist.php?mode=viewprofile&u=5586) wrote: [↑](./viewtopic.php?p=61539#p61539) Tue Feb 28, 2023 3:44 am … you would think the default values would be close enough to work, but guess not. It is important to assume that the default values in Speeduino base tunes are not good for anything. They are simply placeholders filling empty values so the tune does not corrupt and features are functional (but not correct).
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: tunerstudio defaults

#### 'Start Doctor' is in the house
- URL: https://speeduino.com/forum/viewtopic.php?t=7357#p74128
- Date: Fri Feb 20, 2026 2:14 pm
- Who posted it: scudderfish
- Evidence type: technical discussion
- Confidence: medium
- Summary: [DStage](./memberlist.php?mode=viewprofile&u=5328) wrote: [↑](./viewtopic.php?p=74127#p74127) Fri Feb 20, 2026 12:37 pm Interesting idea. I would add a clear information about the criteria your automated diagnostic will use. For example the thresholds for rate of changes on one or the other signal, perhaps a possibility to tweak them from default.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change
- Matched search terms: tunerstudio defaults

#### NO2C Fuel and spark LEDs
- URL: https://speeduino.com/forum/viewtopic.php?t=7281#p73720
- Date: Wed Dec 10, 2025 1:40 am
- Who posted it: PeterJones
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Does the intensity / brightness or lack of same indicate a problem with the system. My NO2C, (685cc single cylinder, single spark plug) KLR650 has a (what I would call normal) LED brightness for the fuel injector circuit, but the spark LED is so dim as to require the garage lights to be off in order to see it, however the hardware test delvers a healthy spark - about 8 - 10 mm between the end of the plug lead and the cylinder head. Using standard resistor plug lead and cap, standard NGK DPR8eA9 resistor plug.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: crank mentioned Attachments: 2025-12-11_12.55.31.msq.zip -> https://speeduino.com/forum/download/file.php?id=14661 Image or capture links found: 1
- Matched search terms: base tune

#### Bluetooth to phone for RealDash or Shadow dash
- URL: https://speeduino.com/forum/viewtopic.php?t=6621#p70082
- Date: Wed Nov 20, 2024 8:33 pm
- Who posted it: darreng
- Evidence type: technical discussion
- Confidence: medium
- Summary: I've looked in my downloads folder and have this: #unset enablehardware_test [MegaTune] MTversion = 2.25 queryCommand = "Q" signature = "speeduino 202402" versionInfo = "S" ;This info is what is displayed to user [TunerStudio] iniSpecVersion = 3.64 I've tried this ini file before and the apps don't like it This is why I'm still asking for guidance. I've just received 2 new HC-05. I'm going to reinstall the apps and go from there.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: None extracted
- Matched search terms: signature

#### E46 M50/M54 Frankenstien Build
- URL: https://speeduino.com/forum/viewtopic.php?t=7284#p73754
- Date: Mon Dec 15, 2025 6:25 am
- Who posted it: uppy1273
- Evidence type: technical discussion
- Confidence: medium
- Summary: So my project is a 2002 e46 330xi with and junk yard m50b25tu engine that I used for the head and the block with m54b30 crank, rods, pistons, cams, dual vanos. With the use of the m52b25tu throttle body and the M50 manifold to be compatible with the DIY-EFI PnP M54 ECU, I have Siemens deka injectors and the stock coils that came with the car, then im also using the wiring harness from the earlier e46's to be able to use the throttle body with both electronic and cable throttle and I have to re wire a few things but when through and checked the pinouts from both harnesses and changed anything that needed to be changed to make it correct for my current chassis, then also this is a turbo setup but I am having issues starting my car at all. it will fire but just won't fully turn over, and I'm just at a loss of where to go from here, and also I have burned the base tune provided from DIY-EfI that's meant for this engine setup, since I've kept it all the same in terms of the ecu, just a different block and modified head to fit the dual vanos, also to note I do have 40psi of fuel pressure at the regulator and the turbo spins when it fires but for testing to get it running I have the boot for the throttle body disconnected for the time being.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change Signal roles: cam mentioned, crank mentioned
- Matched search terms: base tune

#### E46 M50/M54 Frankenstien Build
- URL: https://speeduino.com/forum/viewtopic.php?t=7284#p73773
- Date: Fri Dec 19, 2025 12:45 pm
- Who posted it: digmorepaka
- Evidence type: technical discussion
- Confidence: medium
- Summary: [uppy1273](./memberlist.php?mode=viewprofile&u=10136) wrote: [↑](./viewtopic.php?p=73754#p73754) Mon Dec 15, 2025 6:25 am So my project is a 2002 e46 330xi with and junk yard m50b25tu engine that I used for the head and the block with m54b30 crank, rods, pistons, cams, dual vanos. With the use of the m52b25tu throttle body and the M50 manifold to be compatible with the DIY-EFI PnP M54 ECU, I have Siemens deka injectors and the stock coils that came with the car, then im also using the wiring harness from the earlier e46's to be able to use the throttle body with both electronic and cable throttle and I have to re wire a few things but when through and checked the pinouts from both harnesses and changed anything that needed to be changed to make it correct for my current chassis, then also this is a turbo setup but I am having issues starting my car at all. it will fire but just won't fully turn over, and I'm just at a loss of where to go from here, and also I have burned the base tune provided from DIY-EfI that's meant for this engine setup, since I've kept it all the same in terms of the ecu, just a different block and modified head to fit the dual vanos, also to note I do have 40psi of fuel pressure at the regulator and the turbo spins when it fires but for testing to get it running I have the boot for the throttle body disconnected for the time being.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change Signal roles: cam mentioned, crank mentioned
- Matched search terms: base tune

#### VSS input to serial3.
- URL: https://speeduino.com/forum/viewtopic.php?t=6675#p70373
- Date: Wed Jan 01, 2025 11:32 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [kettlekev](./memberlist.php?mode=viewprofile&u=3980) wrote: [↑](./viewtopic.php?p=70360#p70360) Tue Dec 31, 2024 3:34 pm Thanks, Yes I understand flex or knock could be used. My main question is how does that then get routed to serial3 as a speed output for the xml to recognise? I suppose I could just make the gauge source in RD to be flex?
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Relevant to pin-selection policy and board-specific packaged settings. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: speeduino 202501

#### Problems Connecting STM32F401 Blackpill
- URL: https://speeduino.com/forum/viewtopic.php?t=6116#p65954
- Date: Fri Nov 24, 2023 11:18 am
- Who posted it: digmorepaka
- Evidence type: technical discussion
- Confidence: low
- Summary: [damichi](./memberlist.php?mode=viewprofile&u=8150) wrote: [↑](./viewtopic.php?p=65933#p65933) Thu Nov 23, 2023 2:34 pm Hi folks, for a small pcb project, I wanted to use a blackpill board with a STM32F401CC. I am able to build the software release 202310 and load it to the board with no errors using VSC and PlatformIO as well as Arduino IDE. Both result in the chip not reporting a Serial Connection via USB and therefore not being recognized by Tunerstudio.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change
- Matched search terms: tunerstudio defaults

#### Bugs on Tunerstudio update
- URL: https://speeduino.com/forum/viewtopic.php?t=5127#p55277
- Date: Fri Feb 11, 2022 6:21 am
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: low
- Summary: As mentioned above, if they're purely Warnings they can typically be ignored. It looks like the new version of TS provides Warnings on a bunch more things than the older version. I've just pushed some minor changes to the ini file in master now to resolve these, but for the current released version you can simply turn the Warnings off using the option mentioned by jdlaugh
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Edge or poll details: change
- Matched search terms: ini mismatch

#### Dropbear bluetooth connection
- URL: https://speeduino.com/forum/viewtopic.php?t=5371#p57499
- Date: Wed Jun 22, 2022 11:33 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: You can do a simple response test using the serial monitor in the Arduino IDE. Enter s or q and you will get the firmware version as a response . This is the first thing TS looks for.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: ini mismatch

#### Issue with flex fuel settings
- URL: https://speeduino.com/forum/viewtopic.php?t=5765#p61460
- Date: Sat Feb 25, 2023 6:02 pm
- Who posted it: Discovette
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: Hello, I hope someone can point me in the right direction on setting my flex fuel correctly. The issue I am having is that when I turn the flex fuel setting on, my car starts to run extremely rich, to the extreme of wanting to die. Under flex fuel adjustments I believe 100 percent should be 0 ethanol content and my engine should run the same with the flex option turned off or on.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: Tuned flex off.msq -> https://speeduino.com/forum/download/file.php?id=11077 Image or capture links found: 1
- Matched search terms: tunerstudio defaults

#### Bluetooth to phone for RealDash or Shadow dash
- URL: https://speeduino.com/forum/viewtopic.php?t=6621#p70083
- Date: Wed Nov 20, 2024 9:40 pm
- Who posted it: jonbill
- Evidence type: technical discussion
- Confidence: low
- Summary: That is the start of the 202402 firmware speeduino.ini. So if you're using 202402 firmware, it's the right ini file. If the apps 'don't like it' then either you've not successfully configured it as the ini file in RealDash, or there's another of the frequently occuring incompatibilities between speeduino firmware & ini file & RealDash,
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations.
- Specific details: None extracted
- Matched search terms: signature

#### 'Start Doctor' is in the house
- URL: https://speeduino.com/forum/viewtopic.php?t=7357#p74143
- Date: Mon Feb 23, 2026 6:59 am
- Who posted it: scudderfish
- Evidence type: tooth log / wheel description
- Confidence: low
- Summary: I got the tooth logger working last night, but only against ardustim so no pressure variation. Thinking about it, it will only ever be able to say 'your timing is grossly off' anyway as a 36-1 wheel only has at best 10 degrees of precision, which comes back to (the correct philosophy) of tell not fix. I think getting someone who is struggling to get their shiny new PnP ECU working on their MX5 to plumb and wire in extra fuel pressure sensors might be a bit of a stretch
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Pattern mentions: 36-1
- Matched search terms: tunerstudio defaults

#### Bluetooth serial, signature info problem. "FIXED"
- URL: https://speeduino.com/forum/viewtopic.php?t=4959#p53595
- Date: Fri Oct 15, 2021 9:17 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [Zeppa](./memberlist.php?mode=viewprofile&u=5339) wrote: [↑](./viewtopic.php?p=53590#p53590) Fri Oct 15, 2021 2:55 pm Thanks, I was afraid of that being the case..have to keep looking for the solution, if its some weird hw issue.. Is the serial communication "format" still same in these ports or am I missing some settings, meaning if I have ok communication in Serial3 I should have it from primary serial also? I tried to seek info about the AT commands, as how is this different from normal serial traffic if I have no issue configurating HC-05 but I cant communicate normally with it when I swap the lines back..
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: signature

#### Dropbear bluetooth connection
- URL: https://speeduino.com/forum/viewtopic.php?t=5371#p57494
- Date: Wed Jun 22, 2022 4:53 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: If TS doesn't see the teensy com port then it has nothing to do with an ini mismatch. If TS is connecting it will show a ini mismatch error if there is one. If TS won't find or connect t the com port it is likely something is holding the port open , thus preventing TS from being able to use it.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Edge or poll details: change
- Matched search terms: ini mismatch

#### 93 miata 1.6 supercharge unable to start
- URL: https://speeduino.com/forum/viewtopic.php?t=7244#p73532
- Date: Mon Nov 10, 2025 10:10 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: low
- Summary: Good advice, and my comment is that superchargers and turbos have little effect on starting and warmup. From that perspective, you should be able to use a 'stock' Miata base tune, only modified for your fuel and spark changes, in order to start and run in your driveway. Once you get it running for a stable warm idle (only!), you can then alter tables and other settings in order to accommodate for the boost and other factors specific to your mods.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change
- Matched search terms: base map

#### Morris Minor 1098cc Single point injection with C2C.
- URL: https://speeduino.com/forum/viewtopic.php?t=6910#p72466
- Date: Wed Jul 16, 2025 2:35 pm
- Who posted it: Morris
- Evidence type: technical discussion
- Confidence: low
- Summary: It worked fine in ignition only, however I noticed that the fuel pump relay wasn't triggering, and the fan output (when set to pin 6 as suggested by DIY EFI) actually operates the fuel pump relay. Also, when I try to calibrate the TPS, nothing happens, but I noticed the MAP gauge going up and down as I move the throttle. I've cleared the firmware a number of times and re loaded the base tune but have the same issues.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Signal roles: crank mentioned
- Matched search terms: msq

#### Only will run with Laptop Connected.
- URL: https://speeduino.com/forum/viewtopic.php?t=5080#p54684
- Date: Wed Jan 12, 2022 8:34 pm
- Who posted it: MattSkelton
- Evidence type: technical discussion
- Confidence: low
- Summary: I used Speedyloaderx64 and chose 202202 I downloaded a base tune too (did i need to do this?) Anway I am not connected to the ECU atm as its in the garage and fiddling with TS in the house on my TR7a current project; The title bar seems to indicate its speeduino 2022.02 firmware Screenshot 2022-01-12 202411.png (14.77 KiB) Viewed 2367 times If I try to load my current tune from my TR7a folder I can see the mismatch Screenshot 2022-01-12 202734.png (45.22 KiB) Viewed 2367 times However even loading the base tune obtained from Speedyloader there is a mismatch Screenshot 2022-01-12 202828.png (55.2 KiB) Viewed 2367 times
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: ini mismatch

#### Something simple
- URL: https://speeduino.com/forum/viewtopic.php?t=7178#p73230
- Date: Fri Sep 26, 2025 8:06 am
- Who posted it: Mark850
- Evidence type: technical discussion
- Confidence: low
- Summary: Thanks for the detailed reply. Unfortunately wiring diagrams aren't available for these engines, even the few diagrams that are available for other systems in the car are quite poor. I guess that leaves me testing each one manually.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: base map

#### Something simple
- URL: https://speeduino.com/forum/viewtopic.php?t=7178#p73246
- Date: Sun Sep 28, 2025 5:16 pm
- Who posted it: Mark850
- Evidence type: technical discussion
- Confidence: low
- Summary: Good to know it can be done, thanks for the information. I need to get a measure off the sensors to be sure they're not 12v and see if I can find a suitable base map before I start ordering. I can get a VR conditioner added to the board and buy a smart coil pack from the same place.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: rising
- Matched search terms: base map

#### 'Start Doctor' is in the house
- URL: https://speeduino.com/forum/viewtopic.php?t=7357#p74139
- Date: Sun Feb 22, 2026 7:24 pm
- Who posted it: PSIG
- Evidence type: tooth log
- Confidence: low
- Summary: +1 to a flow chart or diagnostic tree, however IMO that should be part of documentation, where I was under impression you are looking for software solutions and indicators for those diagnostics? For example in documentation; . .
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Signal roles: crank mentioned
- Matched search terms: tunerstudio defaults

#### 93 miata 1.6 supercharge unable to start
- URL: https://speeduino.com/forum/viewtopic.php?t=7244#p73506
- Date: Fri Nov 07, 2025 5:43 pm
- Who posted it: Jeff98
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: Hi all. I have been looking for many posts about supercharged miata tune but still unable to get my car started I have tried with several timing tables and this feel like the closest to get it started. It did start and kinda idle before but I accidentally removed the tune.
- Why it matters: Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: crank mentioned Attachments: 2025-11-07_17.27.02.msq -> https://speeduino.com/forum/download/file.php?id=14533; sadmiatalogs.zip -> https://speeduino.com/forum/download/file.php?id=14532 Image or capture links found: 2
- Matched search terms: base map

#### Issue with flex fuel settings
- URL: https://speeduino.com/forum/viewtopic.php?t=5765#p63022
- Date: Tue May 16, 2023 1:45 pm
- Who posted it: nwalk7800
- Evidence type: technical discussion
- Confidence: low
- Summary: [LPG2CV](./memberlist.php?mode=viewprofile&u=1236) wrote: [↑](./viewtopic.php?p=61531#p61531) Mon Feb 27, 2023 10:12 pm Did you correct the Flex fuel temperature table? I thank you for that as well. I see it in TunerStudio as just "fuel temp correction" and I don't see any mention of it in the wiki.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: tunerstudio defaults

#### Only will run with Laptop Connected.
- URL: https://speeduino.com/forum/viewtopic.php?t=5080#p54687
- Date: Wed Jan 12, 2022 9:15 pm
- Who posted it: jonbill
- Evidence type: technical discussion
- Confidence: low
- Summary: 202202 is the right new firmware. It does look like the default tune signatures haven't been updated from 202201, but I think that's a small thing. IF that tune properties you're showing of your tr7a after updating to the new firmware and successfully syncing with TunerStudio suggests a problem - something thinks your firmware is still 202103.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: None extracted
- Matched search terms: ini mismatch

#### Bugs on Tunerstudio update
- URL: https://speeduino.com/forum/viewtopic.php?t=5127#p55261
- Date: Thu Feb 10, 2022 4:33 pm
- Who posted it: jdlaugh
- Evidence type: technical discussion
- Confidence: low
- Summary: I just updated Tunerstudio and I'm now getting a warning every time I open TS. Message says there are 0 errors and 25 warnings. I'm running 202012 firmware and now running v3.1.08 of TS, the latest version.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: None extracted
- Matched search terms: ini mismatch

#### Bugs on Tunerstudio update
- URL: https://speeduino.com/forum/viewtopic.php?t=5127#p55273
- Date: Fri Feb 11, 2022 4:19 am
- Who posted it: jdlaugh
- Evidence type: technical discussion
- Confidence: low
- Summary: Got this from Josh Stewart, the admin of the Speeduino Facebook group. "Warnings can almost always be safely ignored in TS. It's only Errors you really need to worry about.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: None extracted
- Matched search terms: ini mismatch

#### Problems Connecting STM32F401 Blackpill
- URL: https://speeduino.com/forum/viewtopic.php?t=6116#p66048
- Date: Wed Nov 29, 2023 1:06 pm
- Who posted it: damichi
- Evidence type: technical discussion
- Confidence: low
- Summary: Thanks you for your reply. I tried to change the pin defaults in the init.ino file under case 60 and #elif (definedSTM32F401xC), where the fuel pump output was definded at an USB pin. This however seems not to be all of the issue, as I still have the same problem.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change
- Matched search terms: tunerstudio defaults

#### Problems Connecting STM32F401 Blackpill
- URL: https://speeduino.com/forum/viewtopic.php?t=6116#p66102
- Date: Sat Dec 02, 2023 9:34 pm
- Who posted it: ptownsin
- Evidence type: technical discussion
- Confidence: low
- Summary: Hi Michael, I recently did exactly the same as you, and initially had the same result. I debugged the code and it was crashing initialising the eeprom. I got things working by adding a winbond spi chip and enabling it in the build by uncommenting the line In board_stm32_official.h starting #define USE_SPI_EEPROM There was one more gotcha.
- Why it matters: Helps separate universal defaults from context-specific tune choices.
- Specific details: Edge or poll details: change
- Matched search terms: tunerstudio defaults

#### Bluetooth serial, signature info problem. "FIXED"
- URL: https://speeduino.com/forum/viewtopic.php?t=4959#p53589
- Date: Fri Oct 15, 2021 2:13 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: You cannot connect msdroid or TS to serial3 as the actual protocol differs and the data available is not formatted the same as it is meant for use with dashboards/loggers etc
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: signature

#### Bluetooth to phone for RealDash or Shadow dash
- URL: https://speeduino.com/forum/viewtopic.php?t=6621#p70062
- Date: Mon Nov 18, 2024 4:25 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: You need to use the matching ini file for the firmware you have installed on your speeduino board. You cannot mix them about. The ini would normally be downloaded with the firmware package.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: signature

#### New tuning software in development
- URL: https://speeduino.com/forum/viewtopic.php?t=7312#p73892
- Date: Sat Jan 10, 2026 11:30 am
- Who posted it: scudderfish
- Evidence type: tune file
- Confidence: low
- Summary: Screenshot 2026-01-10 at 11-21-03 VE Tuner.png (373.05 KiB) Viewed 53533 times This is where I currently am. This is general purpose software (like TS) so it parses INI and MSQ files and evaluates the expressions in real time. It talks to ECUs either over serial if the browser supports it, or web sockets.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Edge or poll details: change
- Matched search terms: msq

### VSS, launch, rolling cut, and related protection behavior

#### 6G72 6-cyl on a 4G63 4-cyl trigger wheel
- URL: https://speeduino.com/forum/viewtopic.php?t=5023#p54523
- Date: Sat Jan 01, 2022 8:29 pm
- Who posted it: jonbill
- Evidence type: scope capture
- Confidence: high
- Summary: Yeah, you're right - the wiki isn't super clear - in the code primary wheel is defined as crank speed and so if it is on the cam it needs to be halved. so that isn't the problem. Have you tried 'New' ignition mode?
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Helps separate universal defaults from context-specific tune choices. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: launch speed

#### 2001 Forester NA/Turbo build
- URL: https://speeduino.com/forum/viewtopic.php?t=5944#p63741
- Date: Sat Jul 01, 2023 7:48 pm
- Who posted it: jonbill
- Evidence type: technical discussion
- Confidence: high
- Summary: A few observations: 1) You've got lots of advanced features on, which won't help when trying to build the tune - I'd advise turning it all off (I'm talking about DFCO, Launch Control, VSS, AirCon etc). DFCO in particular is set to 'on' below 10% TPS, so that's definitely going to make the throttle on/off. 2) your injector voltage correction looks a bit mad.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Signal roles: crank mentioned Explicit sync-loss discussion present.
- Matched search terms: launch control VSS

#### Speeduino and Realdash using USB cable
- URL: https://speeduino.com/forum/viewtopic.php?t=6624#p70351
- Date: Mon Dec 30, 2024 11:38 pm
- Who posted it: kettlekev
- Evidence type: wheel description
- Confidence: high
- Summary: Well, I have just spent 4 days working it out with help from the original author of the code and I still dont fully understand it in detail yet but it does function and I can add extra inputs to the dash screens. 1. Use an ESP32 (I used a Doit ESP32Dev1 which is available in the IDE board manager).
- Why it matters: Helps separate universal defaults from context-specific tune choices. Relevant to pin-selection policy and board-specific packaged settings. Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 0-3, 0-7, 1-9 Signal roles: crank mentioned
- Matched search terms: launch speed

#### Temporary fix for anyone wanting a working rolling limiter :)
- URL: https://speeduino.com/forum/viewtopic.php?t=5511#p59199
- Date: Sun Oct 30, 2022 9:28 pm
- Who posted it: ngt
- Evidence type: technical discussion
- Confidence: medium
- Summary: For me testing today showed that whatever choice that included rolling limiter was overshooting the limiter set (hard or soft) On the launch the only cure was setting retard 35 degrees or more. No cure for the revlimiter so i removed the rolling cut for keeping the engine below the rev limiter. Also the timer for the soft cut was not working.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: rolling cut

#### Speeduino and Realdash using USB cable
- URL: https://speeduino.com/forum/viewtopic.php?t=6624#p71970
- Date: Mon Jun 09, 2025 9:18 pm
- Who posted it: kettlekev
- Evidence type: wheel description
- Confidence: medium
- Summary: So working with the original code designer (thanks Peleppe) we came up with a fix for everything. The new RD XML file that is needed is; Code: [Select all](#) ``` <?xml version="1.0" encoding="utf-8"?> <!-- RealDash CAN XML description file, version 2 Requires RealDash version 1.5.8 or newer --> <RealDashCAN version="2"> <!-- baseId [optional] is added to each frame canId. --> <!-- frames baseId="3200" --> <frames> <!-- PARAMETERS FOR 'frame' --> <!-- 'id' can identifier (= baseId + id).
- Why it matters: Helps separate universal defaults from context-specific tune choices. Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 0-3, 0-7, 1-9 Signal roles: cam mentioned, crank mentioned
- Matched search terms: launch speed

#### 1 sided pcb
- URL: https://speeduino.com/forum/viewtopic.php?t=9#p931
- Date: Tue Jun 09, 2015 3:26 am
- Who posted it: noisymime
- Evidence type: tune file / maintainer explanation / image attachment
- Confidence: medium
- Summary: Once you've got the latest code on (Assuming it works for you), then I'll be interested to see a similar log. I just remembered that there was a fix go in recently to the duty cycle calculation in TunerStudio, the values being shown were too low. This didn't affect the pulsewidth itself, purely the DC% calculation on the gauge/log.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Attachments: 2015-06-09_15.50.05.msl -> https://speeduino.com/forum/download/file.php?id=171; 2015-06-13_21.21.46.msl -> https://speeduino.com/forum/download/file.php?id=175; CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=172 Image or capture links found: 4
- Matched search terms: VSS input mode

#### SCG-ATC / Automatic Transmission Controller
- URL: https://speeduino.com/forum/viewtopic.php?t=1827#p29239
- Date: Fri Sep 28, 2018 12:18 pm
- Who posted it: dazq
- Evidence type: maintainer explanation / image attachment
- Confidence: medium
- Summary: [tmoble](./memberlist.php?mode=viewprofile&u=2090) wrote: [↑](./viewtopic.php?p=29237#p29237) Fri Sep 28, 2018 11:03 am Hi, Tom from US here. how is this project proceeding? any hope of getting a unit for the popular GM 4L60E any time soon?
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Attachments: 56-8(1)(1).rar -> https://speeduino.com/forum/download/file.php?id=3894 Image or capture links found: 1
- Matched search terms: VSS input mode

#### HELP! - Engine Protection Not *actually* turning on
- URL: https://speeduino.com/forum/viewtopic.php?t=5788#p61857
- Date: Tue Mar 14, 2023 4:01 pm
- Who posted it: Zserbo98
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Allrighty, I attached the tune that is 100% from today and in use. Also attached a new log - from engine start up until I got to work (little less than 9 minutes). In the beginning it includes a little testing around 2500 - 3000 - 4000 rpm rev limiting while I sat in the parking lot.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Attachments: 2023-03-14_06.56.40.msl -> https://speeduino.com/forum/download/file.php?id=11197; Tune_20230314.msq -> https://speeduino.com/forum/download/file.php?id=11198 Image or capture links found: 2
- Matched search terms: rolling cut

#### Problems with switched inputs
- URL: https://speeduino.com/forum/viewtopic.php?t=5097#p54921
- Date: Thu Jan 27, 2022 12:36 pm
- Who posted it: LPG2CV
- Evidence type: technical discussion
- Confidence: medium
- Summary: looking at your tune ( ), and this is simply my first thought. it's not impossible that even though flat shift is disabled, that the pin selected for flat shift is not being assigned in the code but held high. On the board selected, pin 9 is an injector pin, so it could mess up the fueling on one, or a pair of cylinders.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: launch control VSS

#### 1 sided pcb
- URL: https://speeduino.com/forum/viewtopic.php?t=9#p567
- Date: Tue May 05, 2015 11:28 pm
- Who posted it: noisymime
- Evidence type: scope capture / maintainer explanation
- Confidence: high
- Summary: Really sorry to here about the igniter, well, igniting When you change the ignition setting between inverted and non-inverted, did you restart the arduino? I just realised that this setting was missing a restart warning (Have added it now) as it does require this to function. I've just tried now and the inverting/non-inverting seems to be working OK here.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Edge or poll details: change
- Matched search terms: VSS input mode

#### Problems with switched inputs
- URL: https://speeduino.com/forum/viewtopic.php?t=5097#p54894
- Date: Wed Jan 26, 2022 11:55 am
- Who posted it: Kalns
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Hello. I have made pazi88 m42 ecu, revision 2.3 firmware 202108 I wanted to enable input for launch control, and noticed that after ecu restart, engine will start poorly, has sync etc, but has so poor idle it stalls, missfires. It feels like heavily retatrded ignition, like launch would be enabled.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=9256 Image or capture links found: 1
- Matched search terms: launch control VSS

#### Help With Speeduino + Automatic Transmission Signals
- URL: https://speeduino.com/forum/viewtopic.php?t=6866#p71714
- Date: Mon Apr 28, 2025 8:59 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [AlcaTech](./memberlist.php?mode=viewprofile&u=9579) wrote: [↑](./viewtopic.php?p=71712#p71712) Sun Apr 27, 2025 10:28 pm Hi AlexE. Thank you for your reply! [AlexE](./memberlist.php?mode=viewprofile&u=7151) wrote: [↑](./viewtopic.php?p=71644#p71644) Sun Apr 20, 2025 6:23 am 1.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: None extracted
- Matched search terms: flat shift

#### electronic park brake
- URL: https://speeduino.com/forum/viewtopic.php?t=5184#p55949
- Date: Sun Mar 13, 2022 12:49 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=55939#p55939) Sat Mar 12, 2022 6:43 pm Cool. As you create the project outline, I would like to add a few "universal" features that could be applied to specific projects. Wish List examples could be: basic switch (full on/off from high or low input, serial 0/1, etc) rotary input (dial the force and press (switch above) to engage/disengage) multi-button panel operation (switches or touch data) for per-wheel braking (cutting), Serial and CAN options to integrate to whatever else.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: launch speed

#### Temporary fix for anyone wanting a working rolling limiter :)
- URL: https://speeduino.com/forum/viewtopic.php?t=5511#p58783
- Date: Tue Oct 04, 2022 11:11 am
- Who posted it: Linus Giwjorra
- Evidence type: wheel description / image attachment
- Confidence: medium
- Summary: Hey, as I just set up my launch control it was really bothering me that the rolling limiter method wasn't working. The full limiter method seems to be very unsuitable for launch control applications, at least in my case. I've heard that the rolling limiter worked on earlier releases, so I've compared the code from the current release and the one of feb.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Pattern mentions: 2-6 Edge or poll details: change Attachments: speeduino.txt -> https://speeduino.com/forum/download/file.php?id=10236 Image or capture links found: 1
- Matched search terms: rolling cut

#### Mazda MX5 Miata mk1 mk2 mk2.5 PnP ECUs
- URL: https://speeduino.com/forum/viewtopic.php?t=6198#p66849
- Date: Fri Jan 26, 2024 9:46 am
- Who posted it: AlexE
- Evidence type: technical discussion
- Confidence: medium
- Summary: Hello, here are PnP ECUs for Mazda Mx5 I designed. They all fit original ECU cases, come with step-by-step manual and have preloaded maps for particular car model/engine to have as much as possible features running out of the box. In stock car installation time is within 15 minutes.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: None extracted
- Matched search terms: launch control VSS

#### 1 sided pcb
- URL: https://speeduino.com/forum/viewtopic.php?t=9#p54
- Date: Wed Jan 14, 2015 10:23 pm
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: I really like this. I must admit that my own hardware design skills are pretty basic, so it's nice to see someone with some experience whipping up clever things like a single sided board Using D12 as a spare output should be fine. It's one I'm thinking about myself, so not a bad bet.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: VSS input mode

#### 6G72 6-cyl on a 4G63 4-cyl trigger wheel
- URL: https://speeduino.com/forum/viewtopic.php?t=5023#p54520
- Date: Sat Jan 01, 2022 7:38 pm
- Who posted it: jonbill
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: that still sounds like the same thing to me - aren't they just either end of a wire? , but never mind , its probably not important. I'm not sure if the trigger pattern is configured properly - my head hurts your tune says cam speed dual wheel with 12 primary and a single tooth secondary.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: launch speed

#### Problems with switched inputs
- URL: https://speeduino.com/forum/viewtopic.php?t=5097#p54920
- Date: Thu Jan 27, 2022 12:09 pm
- Who posted it: Kalns
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Well I think its obvious, but maybe not everyone checks everything befor posting in forum.. Here is my current tune, with flatshift enabled, and when i disable only that engine works fine with no problems. One thing I have found out in bmw oem wiring is that I acctualy have semi-sequential injection, but these engine constants work fine for me when launch or any other switched input is not enabled.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=9256 Image or capture links found: 1
- Matched search terms: launch control VSS

#### Help With Speeduino + Automatic Transmission Signals
- URL: https://speeduino.com/forum/viewtopic.php?t=6866#p71638
- Date: Sat Apr 19, 2025 4:55 pm
- Who posted it: AlcaTech
- Evidence type: scope capture
- Confidence: medium
- Summary: Hello Everyone! My name is Luiz (aka Alcatech), currently I own an 1995 Daewoo Espero with automatic trasnmission (for years this model was Brazil’s most popular imported car). I'm planning to install a sequential EFI system with Coils-on-Plugs Ignition based on Speeduino, I'm using the 0.4.3D Board, but the automatic transmission is causing some issues.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: flat shift

#### 6G72 6-cyl on a 4G63 4-cyl trigger wheel
- URL: https://speeduino.com/forum/viewtopic.php?t=5023#p54521
- Date: Sat Jan 01, 2022 7:43 pm
- Who posted it: FWLRmusic
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Hahaha believe me I know how you feel I've included an image of the trigger wheel in the post with the scope images. It's a 24 tooth running under the distributor at cam speed. The guy who made the trigger wheel told me I had to divide the tooth count in two because it is running at cam speed.
- Why it matters: Contains concrete trace-oriented evidence that can support replay tests and decoder validation.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: launch speed

#### rev limiter cut method is switching itself
- URL: https://speeduino.com/forum/viewtopic.php?t=6060#p65228
- Date: Wed Oct 11, 2023 12:37 pm
- Who posted it: jonbill
- Evidence type: technical discussion
- Confidence: medium
- Summary: -25 degrees is a bit mad isn't it? thats sparks at 25 degrees AFTER TDC. It wouldn't surprise me if that feels like a pretty full cut.
- Why it matters: Helps separate universal defaults from context-specific tune choices. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: rolling cut

#### SCG-ATC / Automatic Transmission Controller
- URL: https://speeduino.com/forum/viewtopic.php?t=1827#p28177
- Date: Sat Sep 01, 2018 4:20 pm
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: medium
- Summary: Darren - here is basic info I sent before on the Ford 4R70/75 (AOD-E) family that may help on outlining operational control. Note I have converted my current controller (US Shift) from TPS to MAP load input for operation in turbocharged conditions. MCCC = PWM TCC.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: None extracted
- Matched search terms: VSS input mode

#### launch control based on speed input
- URL: https://speeduino.com/forum/viewtopic.php?t=382#p32151
- Date: Sat Feb 16, 2019 5:01 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: medium
- Summary: Launch Control and power/flat-shift currently use RPM threshold as input. In the current form, you do not need a multi-position switch in order to have multiple rev-limit points. Only a single-position on/off switch and the clutch switch or button is necessary, and rev-limit is established by the switch-point set in TunerStudio.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: VSS input mode

#### rev limiter cut method is switching itself
- URL: https://speeduino.com/forum/viewtopic.php?t=6060#p65222
- Date: Tue Oct 10, 2023 11:06 pm
- Who posted it: hytrozion
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: hi guys, I can't figure this out, in tunerstudio, the parameter for the rev limiter cut method is set to full. and it seems like above 2000rpm, the rev limiter cut method set itself to rolling. If at idle, I press the clutch and 100% tps, the launch is in full method.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Attachments: rev_limiter_cut_method_messed_up.msq -> https://speeduino.com/forum/download/file.php?id=12166; test_launch__shiftcut_at_different_rpm.mlg -> https://speeduino.com/forum/download/file.php?id=12169; test_launch_off.mlg -> https://speeduino.com/forum/download/file.php?id=12168 Image or capture links found: 4
- Matched search terms: rolling cut

#### rev limiter cut method is switching itself
- URL: https://speeduino.com/forum/viewtopic.php?t=6060#p65227
- Date: Wed Oct 11, 2023 9:47 am
- Who posted it: hytrozion
- Evidence type: image attachment
- Confidence: medium
- Summary: thanks you for helping me. I made three logs, one with nothing changed on the tune, one without launch control and one without shiftcut. the log with nothing changed, I was static, with slighty pressing the throttle more and more to gain rpm, and it was in full cut method all the time (guessing by the sound) when launch control was deactivated (with shiftcut still on), I couldn't rev above 4krpm.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Edge or poll details: change Attachments: test_launch__shiftcut_at_different_rpm.mlg -> https://speeduino.com/forum/download/file.php?id=12169; test_launch_off.mlg -> https://speeduino.com/forum/download/file.php?id=12168; test_shiftcut_off.mlg -> https://speeduino.com/forum/download/file.php?id=12170 Image or capture links found: 3
- Matched search terms: rolling cut

#### Speeduino and Realdash using USB cable
- URL: https://speeduino.com/forum/viewtopic.php?t=6624#p70355
- Date: Tue Dec 31, 2024 1:36 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [kettlekev](./memberlist.php?mode=viewprofile&u=3980) wrote: [↑](./viewtopic.php?p=70353#p70353) Tue Dec 31, 2024 11:02 am When you have done all of the above then the standard elements like RPM, CLT, IAT etc (as in the XML) will all be visible in RD. For the GPIO inputs you have used on the ESP, they will not be visible yet. To active them you need to go to garage in RD.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: Edge or poll details: change
- Matched search terms: launch speed

#### electronic park brake
- URL: https://speeduino.com/forum/viewtopic.php?t=5184#p55769
- Date: Mon Mar 07, 2022 5:59 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: I am currently working on a replacement EPB (electronic park brake or e-brake). I am using calipers from TRW who supply many manufacturers Inc BMW,VAG ,Fiat ,Ford etc. The calipers are of the type with the motor fitted directly onto the actual caliper itself though I see no reason the motor system that uses a Bowden cable and a single motor should be any issues in fact my early work used just such an setup from a jaguar s type(also f and xf).
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: launch speed

#### 2001 Forester NA/Turbo build
- URL: https://speeduino.com/forum/viewtopic.php?t=5944#p63739
- Date: Sat Jul 01, 2023 2:47 pm
- Who posted it: FozzyMan
- Evidence type: tune file / wheel description / image attachment
- Confidence: low
- Summary: Hey all! I am working on a Subaru build and have run into some issues. I am completely new to tuning and have a few issues.
- Why it matters: Relevant to pin-selection policy and board-specific packaged settings. Touches wiring-dependent versus universal idle and A/C behavior.
- Specific details: Pattern mentions: 1-2, 3-4 Signal roles: cam mentioned Attachments: 2023-07-01_10.44.26.zip -> https://speeduino.com/forum/download/file.php?id=11721; Subie-base.msq -> https://speeduino.com/forum/download/file.php?id=11720 Image or capture links found: 2
- Matched search terms: launch control VSS

#### SCG-ATC / Automatic Transmission Controller
- URL: https://speeduino.com/forum/viewtopic.php?t=1827#p29244
- Date: Fri Sep 28, 2018 5:23 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: I'm not sure how far off the existing code is from running your box , David (Psig) sent over the data on them a while back Would need details on your shifter and it's switching mechanism, also the shift paddles (steering)if you have any. Hardware wise the v1 board is working fine , when the V2 will be ready I can't say
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: VSS input mode

#### Speed input (VSS). Traction Control.
- URL: https://speeduino.com/forum/viewtopic.php?t=3240#p38957
- Date: Fri Nov 08, 2019 1:43 pm
- Who posted it: warhead
- Evidence type: technical discussion
- Confidence: low
- Summary: [Falcao](./memberlist.php?mode=viewprofile&u=2854) wrote: [↑](./viewtopic.php?p=38945#p38945) Thu Nov 07, 2019 7:57 pm Great work ! How did you do to read vss signal from another pin than crank or cam ? Arduino Mega has 6 external interrupts inputs (pins 2, 3, 18, 19, 20, 21), pins 18 and 19 are for crank and cam signals.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: Signal roles: cam mentioned, crank mentioned
- Matched search terms: VSS input mode

#### electronic park brake
- URL: https://speeduino.com/forum/viewtopic.php?t=5184#p55840
- Date: Thu Mar 10, 2022 1:13 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=55833#p55833) Wed Mar 09, 2022 9:05 pm Some early electric Park brakes simply used the trans Park position (physical or signal) to enable-disable. I haven't thought about it, but does it need to be more complicated? That's a useful option of activation to have in addition , but could cause issues if the driven axle is not the braked axle.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: launch speed

#### 2001 Forester NA/Turbo build
- URL: https://speeduino.com/forum/viewtopic.php?t=5944#p63740
- Date: Sat Jul 01, 2023 3:52 pm
- Who posted it: FozzyMan
- Evidence type: image attachment
- Confidence: low
- Summary: log from a drive a couple minutes ago. It has a very on/off feel at low throttle input. It runs "pretty well" when feathering the throttle, but roll into it and it spits and sputters.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: 2023-07-01_10.44.26.zip -> https://speeduino.com/forum/download/file.php?id=11721 Image or capture links found: 1
- Matched search terms: launch control VSS

#### launch control based on speed input
- URL: https://speeduino.com/forum/viewtopic.php?t=382#p68399
- Date: Sun May 19, 2024 1:22 am
- Who posted it: LeeTaylor
- Evidence type: technical discussion
- Confidence: low
- Summary: I saw this suggested on GitHub. I've used programmable outputs to essentially stop the ECU looking at the clutch switch pin above 5 km/h so launch works when stopped, and then re-enables over 5,000rpm which is where I have my flat shift set to start working. If you only set by VSS you'll hit the rev limiter trying to flat shift, ask me how I know
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: VSS input mode

#### How to: Add an additional sensor input to Speeduino using unused A2D channels on the Arduino
- URL: https://speeduino.com/forum/viewtopic.php?t=2809#p49487
- Date: Tue Apr 06, 2021 10:52 pm
- Who posted it: Black Knight
- Evidence type: technical discussion
- Confidence: low
- Summary: [pazi88](./memberlist.php?mode=viewprofile&u=2291) wrote: [↑](./viewtopic.php?p=49481#p49481) Tue Apr 06, 2021 7:00 am The aux in channels are only read in 4Hz pace. So really slow for any kind of knock detection. To actually detect that at the moment it happens, you need to have the knock signal to be detected with interrupt.
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: Edge or poll details: change
- Matched search terms: flat shift

#### speedometer input
- URL: https://speeduino.com/forum/viewtopic.php?t=2612#p32209
- Date: Tue Feb 19, 2019 8:38 am
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: If you convert the vss signal to analog(lm2917?) Then feed it to speeduino aux input , you could see it in TS.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions. Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: vss pulses per km

#### Speed input (VSS). Traction Control.
- URL: https://speeduino.com/forum/viewtopic.php?t=3240#p38715
- Date: Tue Oct 22, 2019 4:17 pm
- Who posted it: warhead
- Evidence type: technical discussion
- Confidence: low
- Summary: Hi Josh and Friends!, first, I want to congratulate you again for your work. I'm curently working in a external traction control combined with Speeduino's launch control . This device (Arduino Nano) measures forward and rear wheels speeds and calculates slip percentaje.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: VSS input mode

#### Speed input (VSS). Traction Control.
- URL: https://speeduino.com/forum/viewtopic.php?t=3240#p38969
- Date: Fri Nov 08, 2019 11:05 pm
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: low
- Summary: [evo_lucian](./memberlist.php?mode=viewprofile&u=103) wrote: [↑](./viewtopic.php?p=38950#p38950) Fri Nov 08, 2019 4:35 am I have always felt that first step in traction control should be ignition retard and followed by cutting of cylinders if it really gets out of hand. Cutting 1 out of 4 cylinders is an automatic 25% reduction in power. Correct, and why you may want to expand the range simply, by adding more "n" as I outlined.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: VSS input mode

#### HELP! - Engine Protection Not *actually* turning on
- URL: https://speeduino.com/forum/viewtopic.php?t=5788#p61822
- Date: Mon Mar 13, 2023 4:19 pm
- Who posted it: Zserbo98
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: Hello! My problem in short is the following: My engine protection doesn't exactly work. I was doing a pull for testing, but I noticed that it wasn't really limiting at the set 7000 rpm, so I tried it around 4000rpm with a soft limit value of 10 degrees upwards from 3500rpm.
- Why it matters: Contains technical evidence that may affect firmware behavior, tests, or documentation.
- Specific details: Attachments: 2023-03-14_06.56.40.msl -> https://speeduino.com/forum/download/file.php?id=11197; CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=11187; RevNotLimiting.msl -> https://speeduino.com/forum/download/file.php?id=11188 Image or capture links found: 4
- Matched search terms: rolling cut

#### Flat shift usage
- URL: https://speeduino.com/forum/viewtopic.php?t=6547#p69609
- Date: Wed Sep 25, 2024 5:54 pm
- Who posted it: jonbill
- Evidence type: technical discussion
- Confidence: low
- Summary: Yes, I think flat shift is the feature intended for this. Have you seen the wiki page on it? I think it explains it ok.
- Why it matters: Useful for protection-mode expectations and disabled or off-state behavior.
- Specific details: None extracted
- Matched search terms: flat shift

#### How to: Add an additional sensor input to Speeduino using unused A2D channels on the Arduino
- URL: https://speeduino.com/forum/viewtopic.php?t=2809#p49623
- Date: Tue Apr 13, 2021 6:51 am
- Who posted it: Black Knight
- Evidence type: technical discussion
- Confidence: low
- Summary: [pazi88](./memberlist.php?mode=viewprofile&u=2291) wrote: [↑](./viewtopic.php?p=49481#p49481) Tue Apr 06, 2021 7:00 am The aux in channels are only read in 4Hz pace. So really slow for any kind of knock detection. To actually detect that at the moment it happens, you need to have the knock signal to be detected with interrupt.
- Why it matters: Directly informs tune and INI compatibility handling plus migration expectations. Relevant to pin-selection policy and board-specific packaged settings.
- Specific details: Edge or poll details: change Signal roles: crank mentioned
- Matched search terms: flat shift

#### How to: Add an additional sensor input to Speeduino using unused A2D channels on the Arduino
- URL: https://speeduino.com/forum/viewtopic.php?t=2809#p34526
- Date: Wed May 08, 2019 2:22 pm
- Who posted it: dazq
- Evidence type: maintainer explanation
- Confidence: low
- Summary: Fantastic work ! Maybe you would consider writing a how to that we can add into the wiki? If you don't have wiki access just post the files and txt here and one of us will add it to the appropriate section.
- Why it matters: Includes maintainer guidance, which is more reliable for roadmap decisions.
- Specific details: None extracted
- Matched search terms: flat shift

## Strongest evidence by roadmap area

- **Decoder and trigger-pattern evidence**: [HONDA TRANSALP XL650V](https://speeduino.com/forum/viewtopic.php?t=7157#p73072) — Tutajjestem, Sun Sep 07, 2025 8:52 am, confidence high.
- **Idle advance, idle taper, and air-con defaults**: [Adventures in tuning and driving a UA4C, Supercharged BMW V8](https://speeduino.com/forum/viewtopic.php?t=5471#p58294) — Mykk, Tue Aug 23, 2022 2:31 am, confidence high.
- **Knock and pin/default policy**: [Just another Speedy ECU](https://speeduino.com/forum/viewtopic.php?t=7329#p74172) — Black Knight, Sat Feb 28, 2026 1:05 am, confidence high.
- **Tune compatibility, base tune, and INI/default drift**: [LibreTune - Rust and Tauri powered FOSS Tuining Suite - GPLv2](https://speeduino.com/forum/viewtopic.php?t=7313#p73904) — RallyPat, Sun Jan 11, 2026 7:46 pm, confidence high.
- **VSS, launch, rolling cut, and related protection behavior**: [6G72 6-cyl on a 4G63 4-cyl trigger wheel](https://speeduino.com/forum/viewtopic.php?t=5023#p54523) — jonbill, Sat Jan 01, 2022 8:29 pm, confidence high.

## Unresolved contradictions

- Decoder and trigger-pattern evidence: some posts imply a universal default while others frame it as context-specific.
- Decoder and trigger-pattern evidence: conflicting edge or polarity references appear across posts.
- Idle advance, idle taper, and air-con defaults: some posts imply a universal default while others frame it as context-specific.
- Idle advance, idle taper, and air-con defaults: conflicting edge or polarity references appear across posts.
- Knock and pin/default policy: some posts imply a universal default while others frame it as context-specific.
- Knock and pin/default policy: conflicting edge or polarity references appear across posts.
- Tune compatibility, base tune, and INI/default drift: some posts imply a universal default while others frame it as context-specific.
- VSS, launch, rolling cut, and related protection behavior: some posts imply a universal default while others frame it as context-specific.

## Places the forum suggests code changes, tune changes, test additions, or documentation updates

- Documentation and test addition: add explicit firmware, INI, and base-tune compatibility matrix checks plus migration guidance.
- Test addition: build replay and regression coverage from real tooth/composite traces and sync-loss cases.
- Code and documentation review: verify knock-pin enumerations, board pin exposure, and packaged tune defaults for board variants.
- Documentation change: clarify that A/C polarity is wiring-dependent, not a universal default.
- Test and documentation change: confirm disabled or off-state semantics for VSS, launch control, and rolling cut settings.

## Trigger decoder and sync-behavior evidence

### 1-3-4-2

#### Issues trying to start an EJ16 with Speeduino v0.4
- URL: https://speeduino.com/forum/viewtopic.php?t=6880#p71697
- Date: Fri Apr 25, 2025 4:22 pm
- Who posted it: George Tsotsos
- Evidence type: composite log / tune file / wheel description
- Confidence: medium
- Summary: We're having issues trying to start the engine, currently its setup as sequential injection with wasted spark, while cranking we do get Full Sync, and the car tries to run with one cylinder only, that being CYL3, the injectors seem to be correctly installed as the attachments show I am unsure about the firing order setup because in the speeduino documentation it shows that the coils should be wired this way for a typical i4 engine with a 1-3-4-2 firing order: IGN1 -> CYL1 + CYL4 IGN2 -> CYL2 + CYL3 Meaning it does the pattern IGN 1-2-1-2 The boxer engine has a different firing order: 1-3-2-4, meaning the IGN would have to be in a different order?: IGN 1-2-2-1 , thats what i believe the stock ECU does. Any ideas on what we should try/change? I've attached a picture of how the Injectors are wired wired up and the composite logger while running on 1 cylinder I can send a video of how it behaves when it runs on 1 cylinder if needed
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Pattern references: 1-2-1-2, 1-2-2-1, 1-3-2-4, 1-3-4-2 Includes: change Includes: crank Includes: sync Image or capture links: 2 Attachments: cranking.csv -> https://speeduino.com/forum/download/file.php?id=14091; subarugc4.msq -> https://speeduino.com/forum/download/file.php?id=14092
- Matched search terms: 36-2-2-2 Speeduino trigger

### 3-4

#### 4age 16v smallport
- URL: https://speeduino.com/forum/viewtopic.php?t=5986#p64671
- Date: Wed Aug 30, 2023 12:16 pm
- Who posted it: CamJam
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: More updates - Car now runs and drives surprisingly stable, but still getting consistent sync loss in the data logs. - I initially set the timing to fixed 10deg, warmed the car up, played with the idle till it was pretty stable, and then went for a drive around the block. Played a little with the VE table and although it was sluggish due to fixed timing, there was no bucking or dips from the engine, which I would expect from sync loss. - I then turned off fixed timing and have been playing with the spark table.
- Why it matters: Discusses sync-loss conditions that can inform negative test cases.
- Replay-test details: Pattern references: 3-4 Includes: sync Includes: sync loss Image or capture links: 3 Attachments: 2023-08-29_08.11.16.mlg -> https://speeduino.com/forum/download/file.php?id=11955; 2023-08-29_12.02.14.mlg -> https://speeduino.com/forum/download/file.php?id=11956; 2023-08-29_17.24.22.msq -> https://speeduino.com/forum/download/file.php?id=11954
- Matched search terms: Basic distributor sync

### 36-1

#### Trying to build a trigger for uneven flywheel.
- URL: https://speeduino.com/forum/viewtopic.php?t=4907#p52998
- Date: Thu Sep 09, 2021 10:58 am
- Who posted it: RempageR1
- Evidence type: wheel description
- Confidence: low
- Summary: Hi, I`m trying to get this flywheel working using the ardustim but I`m having some issues: Wheel.JPG (35.01 KiB) Viewed 22222 times I`m using the change trigger in the setup and the code below syncs on the wider lobe. So there is a routine checking the width of a lobe compare with the others. When the wider lobe is found, the secondaryToothCount is set to 1. This syncs just fine, no problems.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 36-1 Includes: rising edge Includes: change Includes: crank Includes: sync
- Matched search terms: Harley Speeduino trigger

### 36-2-2-2

#### Fresh build, no sync. Subaru EJ20.
- URL: https://speeduino.com/forum/viewtopic.php?t=6013#p64751
- Date: Tue Sep 05, 2023 1:23 am
- Who posted it: rozap
- Evidence type: scope capture / tune file / wheel description
- Confidence: medium
- Summary: Hi everyone, This is my second speeduino install and the first one went really smoothly (on a VW VR6) while this one is kicking my ass. Engine is a Subaru EJ20 with the 36-2-2-2 wheel. I've been unable to get any sync to occur. I've tried two different VR conditioners, one mini max A2, and one regular DSC conditioner.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: 36-2-2-2 Includes: cam Includes: crank Includes: sync Image or capture links: 2 Attachments: 2023-09-04_18.08.38.csv -> https://speeduino.com/forum/download/file.php?id=11985; CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=11984
- Matched search terms: Subaru 6/7 Speeduino trigger

#### Subaru EJ25 DOHC
- URL: https://speeduino.com/forum/viewtopic.php?t=6731#p70668
- Date: Sun Feb 02, 2025 1:53 pm
- Who posted it: Laudper
- Evidence type: composite log / tune file / wheel description
- Confidence: medium
- Summary: I am using the 36-2-2-2, so see no problems using that pattern? Current tune uploaded. I haven't got a tooth/composite logger to share currently as I am doing a bit of wiring. Will share as soon as I can crank the engine.
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Pattern references: 36-2-2-2 Includes: crank Image or capture links: 1 Attachments: 2025-02-01_13.01.19.msq -> https://speeduino.com/forum/download/file.php?id=13649
- Matched search terms: 36-2-2-2 tooth logger

#### Issues trying to start an EJ16 with Speeduino v0.4
- URL: https://speeduino.com/forum/viewtopic.php?t=6880#p71709
- Date: Sat Apr 26, 2025 6:52 am
- Who posted it: dazq
- Evidence type: wheel description / maintainer explanation
- Confidence: medium
- Summary: [George Tsotsos](./memberlist.php?mode=viewprofile&u=9594) wrote: [↑](./viewtopic.php?p=71705#p71705) Fri Apr 25, 2025 6:30 pm Still no luck with getting it to run, it still sometimes misfires and causes the engine to brake while cranking or it will misfire from the intake. I used the 36-2-2-2 trigger pattern and it does fully sync, I don't know exactly how the trigger wheel looks as you need to take off the crank pulley to get the timing covers off to see What should I try next? Set it to locked timing and check timing with a strobe light and adjust trigger offset so it is correct .
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Pattern references: 36-2-2-2 Includes: crank Includes: sync
- Matched search terms: 36-2-2-2 Speeduino trigger

#### Subaru EJ25 DOHC
- URL: https://speeduino.com/forum/viewtopic.php?t=6731#p70688
- Date: Mon Feb 03, 2025 8:48 pm
- Who posted it: PSIG
- Evidence type: tooth log / scope capture / wheel description
- Confidence: medium
- Summary: Reviewing an image of the wheel, the tooth log makes more sense. You have to think in ONLY rising or falling edges (depending on decoder spec), and that a sensed edge is required to end a time bar in a tooth log. Using this and the 'scope image earlier, we can count 36 total teeth (real and missing) like this: If it helps to see what the sensor is reading, perhaps this may help. Please pardon the crude drawing .
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: 36-2-2-2, 36-2-4 Includes: falling edge Includes: crank
- Matched search terms: 36-2-2-2 tooth logger

#### Subaru EJ25 DOHC
- URL: https://speeduino.com/forum/viewtopic.php?t=6731#p70705
- Date: Tue Feb 04, 2025 9:04 pm
- Who posted it: Laudper
- Evidence type: tooth log / wheel description
- Confidence: low
- Summary: jonbill wrote: I've just realised that the EJ25 is a 4 cylinder - I thought it was a 6 : but anyway - the (first) tooth log pattern looks right to me. the data log is very strange : very little changes through the log - the only thing thats interesting is at around 6.7 seconds vbat goes from 0 to 5.5 and then flits around up to 11 and then back down to 0, and at the same time MAP goes from 100 to 93 and back to 100. what's actually going on here? are you connecting a battery and cranking, or?
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Includes: change Includes: crank Matched decoder family: 36-2-2-2
- Matched search terms: 36-2-2-2 tooth logger

#### Issues trying to start an EJ16 with Speeduino v0.4
- URL: https://speeduino.com/forum/viewtopic.php?t=6880#p72017
- Date: Mon Jun 16, 2025 3:53 pm
- Who posted it: George Tsotsos
- Evidence type: tooth log / wheel description
- Confidence: low
- Summary: We tried to fix the timing but i noticed that the timing mark keeps moving ahead while cranking, sometimes its at 0 and other times it goes off like 90 ahead, i believe the trigger wheel is not correct or something The tooth logger looks weird as well, any suggestions?
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Includes: crank Matched decoder family: 36-2-2-2
- Matched search terms: 36-2-2-2 Speeduino trigger

### 420a

#### How to configure Primary Base Teeth and Trigger Angle on the 420A engine?
- URL: https://speeduino.com/forum/viewtopic.php?t=6739#p70720
- Date: Wed Feb 05, 2025 10:36 pm
- Who posted it: PedroV9
- Evidence type: scope capture / tune file
- Confidence: medium
- Summary: Would anyone be able to tell me, based on these images, the correct Primary Base Teeth and Trigger Angle configuration? Using a timing light, I found a value of -3 degrees, but I’m experiencing Lost Sync at intermediate RPMs. Checking with an oscilloscope, the signal looks clean, and there is also no reading fluctuation in TunerStudio. I believe it might be an issue with the decoder settings.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: sync Matched decoder family: 420a Image or capture links: 4 Attachments: 2025-02-05_11.05.39.mlg -> https://speeduino.com/forum/download/file.php?id=13677; 2025-02-06_18.52.31.csv -> https://speeduino.com/forum/download/file.php?id=13678; noovo.csv -> https://speeduino.com/forum/download/file.php?id=13675
- Matched search terms: 420a Speeduino trigger

#### How to configure Primary Base Teeth and Trigger Angle on the 420A engine?
- URL: https://speeduino.com/forum/viewtopic.php?t=6739#p70755
- Date: Sat Feb 08, 2025 3:50 pm
- Who posted it: PedroV9
- Evidence type: composite log / scope capture
- Confidence: medium
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=70746#p70746) Sat Feb 08, 2025 12:14 am Are you using any VR or signal conditioner? My first guess is noise. You have 164 sync losses in 23 seconds. My next clue is your run log, which shows rpm spikes when the engine is off and not cranking.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation. Discusses sync-loss conditions that can inform negative test cases.
- Replay-test details: Includes: change Includes: cam Includes: crank Includes: sync Includes: sync loss Includes: cas Matched decoder family: 420a
- Matched search terms: 420a Speeduino trigger

#### How to configure Primary Base Teeth and Trigger Angle on the 420A engine?
- URL: https://speeduino.com/forum/viewtopic.php?t=6739#p70746
- Date: Sat Feb 08, 2025 12:14 am
- Who posted it: PSIG
- Evidence type: composite log / scope capture
- Confidence: medium
- Summary: Are you using any VR or signal conditioner? My first guess is noise. You have 164 sync losses in 23 seconds. My next clue is your run log, which shows rpm spikes when the engine is off and not cranking.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation. Discusses sync-loss conditions that can inform negative test cases.
- Replay-test details: Includes: cam Includes: crank Includes: sync Includes: sync loss Matched decoder family: 420a
- Matched search terms: 420a Speeduino trigger

### 8-0

#### 4age 16v smallport
- URL: https://speeduino.com/forum/viewtopic.php?t=5986#p64903
- Date: Sun Sep 17, 2023 3:03 am
- Who posted it: CamJam
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: So i checked the sparkplug leads and spark plugs. All leads read 2.2kOhms, and all sparkplugs are resistor type and gapped correctly (all around 0.8-0.9mm) So i have narrowed this down to when the car is cranking with compression Current setup is the max9926 has a 10kOhm shunt resistor on VR1 signal. Secondary signal is disconnected at the board) When i crank the car with no accessories and no spark plugs installed. Only thing hooked up is the coil and spark leads, i get a perfect waveform out of the scope and in TS.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: 8-0 Includes: change Includes: crank Image or capture links: 1 Attachments: 2023-09-17_13.42.23_WithSparkPLugsIn_2xbat.mlg -> https://speeduino.com/forum/download/file.php?id=12064
- Matched search terms: Basic distributor sync

### Basic distributor

#### 1987 Subaru XT turbo, trouble syncing RPM
- URL: https://speeduino.com/forum/viewtopic.php?t=2474#p30601
- Date: Thu Dec 06, 2018 1:03 pm
- Who posted it: JoostXT
- Evidence type: scope capture / tune file / wheel description
- Confidence: high
- Summary: Hello everyone! I recently decided to replace the aging/failing ECU in my Subaru with something MS. I've used megasquirt based EFI before so it was a easy decision and seeing as speeduino was new and cheap I ordered one online and started soldering. Since I kind of need the car to be reliable and useable throughout the project I decided to reuse all sensors and wiring harnasses and built the speeduino inside a spare ECU housing.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: basic distributor Includes: sync Includes: cas Image or capture links: 2 Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=4140; Subaru XT turbo.msq -> https://speeduino.com/forum/download/file.php?id=4160
- Matched search terms: Basic distributor CAS

#### 1990 Turbo MX-5 on PNP Beta 2
- URL: https://speeduino.com/forum/viewtopic.php?t=1671#p25056
- Date: Tue Mar 20, 2018 6:10 pm
- Who posted it: bond007
- Evidence type: scope capture
- Confidence: medium
- Summary: Not sure if this attachment shows up on Slack as it said that the space is low on storage. Attachment is the trace from the idle valve on the PC Oscilloscope. I'll try to fire the car back up again and look at what the Idle Load parameter in TS realtime view is versus what I see on the scope and what has been input into the table.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Matched decoder family: Basic distributor Image or capture links: 1 Attachments: Idle Valve Trace.pdf -> https://speeduino.com/forum/download/file.php?id=3152
- Matched search terms: Basic distributor CAS

#### Trigger setup for 1 cylinder engine with just VR on crank shaft
- URL: https://speeduino.com/forum/viewtopic.php?t=6473#p69226
- Date: Thu Aug 15, 2024 8:55 am
- Who posted it: Hich
- Evidence type: scope capture / tune file
- Confidence: medium
- Summary: Thanks for the good input. Actually I haven't looked at the VE / Spark table values yet. (but probably should have). I have changed this to match 100% TPS now.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: change Includes: cam Includes: crank Matched decoder family: Basic distributor Image or capture links: 1 Attachments: 2024-08-15_10.50.00.msq -> https://speeduino.com/forum/download/file.php?id=13275
- Matched search terms: Basic distributor scope

#### Tachometer output as Speeduino trigger?
- URL: https://speeduino.com/forum/viewtopic.php?t=5521#p58915
- Date: Thu Oct 13, 2022 5:28 am
- Who posted it: RonS
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Thanks for the reply! “Basic Distributor” is where I started, but I get that crazy high/erratic reading. Which is odd since I would assume that the tach output is sending 1 pulse/rotation, and with TS expecting to see 6 pulses/rotation (v6 - or I guess 3 pulses) I would expect TS to show low RPMs. I thought about piggybacking off the distributor output, but I’m concerned about interference or some sort of bad interaction between the two systems.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: Basic Distributor
- Matched search terms: Basic distributor oscilloscope

#### 1990 Turbo MX-5 on PNP Beta 2
- URL: https://speeduino.com/forum/viewtopic.php?t=1671#p24607
- Date: Tue Feb 27, 2018 5:57 am
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: What method are you using for the fuel pump control? Just the standard one with the ST_SIG fuse pulled? Either way it sounds like it's working given that you're hearing fuel pump priming on startup, but worth double checking. bond007 wrote: It seems to be firing ignition close to 5 degrees BTDC on the timing light, which should get it started This is what is expected with that Fixed cranking timing option on, so no problem there (CAS has a trigger at 5* BTDC that is used when cranking) What pulsewidth are you seeing when cranking?
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Includes: crank Includes: cas Matched decoder family: Basic distributor
- Matched search terms: Basic distributor CAS

#### Basic distributor GM HEI
- URL: https://speeduino.com/forum/viewtopic.php?t=4835#p52098
- Date: Wed Jul 28, 2021 5:48 am
- Who posted it: chester240
- Evidence type: tooth log / scope capture
- Confidence: medium
- Summary: Ok what know signal issue? The trigger signal i almost perfect. Its the output ignition signal thats the problem. On tooth logger the Trigger signal is ok with bypass disconected.
- Why it matters: Provides replay-test-relevant trigger traces. Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Matched decoder family: Basic distributor
- Matched search terms: Basic distributor tooth logger

#### Basic Distributor cranking/filtering issue
- URL: https://speeduino.com/forum/viewtopic.php?t=3039#p50980
- Date: Thu Jun 10, 2021 5:35 am
- Who posted it: PSIG
- Evidence type: scope capture
- Confidence: medium
- Summary: Hey, Sodium (you make whistles?) Without specifics of the circuit, I generally suggest to ground the shielding only at the ECM end, on a power (not sensor) ground. That could help, but keep in-mind, defending from noise is simpler if there is no noise to shield from. This again assumes it is a noise issue. Noise also affects different sensors in different ways.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Matched decoder family: Basic distributor
- Matched search terms: Basic distributor oscilloscope

#### CHEVY LS USING BASIC DISTRIBUTOR SETTING AND THE CRANK SENSOR ONLY
- URL: https://speeduino.com/forum/viewtopic.php?t=6764#p70922
- Date: Thu Feb 20, 2025 7:36 pm
- Who posted it: Built2runmaybe
- Evidence type: tooth log / wheel description
- Confidence: medium
- Summary: Hello all, working on my LS project and have completed the wiring. Using the black sensor on 5v (it is supposed to run on 12, but it seems to be the only way I can get an RPM reading), I am able to get readings on both the tooth logger and the Tunerstudio RPM gauge. The 5v grey sensor did not produce any results no matter what pull up or pulldown configuration tested. But this is for another topic.
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Pattern references: basic distributor Includes: crank
- Matched search terms: Basic distributor tooth logger

#### Trigger setup for 1 cylinder engine with just VR on crank shaft
- URL: https://speeduino.com/forum/viewtopic.php?t=6473#p69123
- Date: Mon Aug 05, 2024 9:50 am
- Who posted it: Hich
- Evidence type: tooth log / scope capture / tune file
- Confidence: medium
- Summary: Hi, please find attached the current tune and a tooth log. Not sure if this is the correct log you need. Only in the first few seconds the starter is cranking the engine. The yellow line is the ignition output from the ECU.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: crank Matched decoder family: Basic distributor Image or capture links: 2 Attachments: 2024-08-04_16.15.56.csv -> https://speeduino.com/forum/download/file.php?id=13239; CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=13238
- Matched search terms: Basic distributor scope

#### 4age 16v smallport
- URL: https://speeduino.com/forum/viewtopic.php?t=5986#p64745
- Date: Mon Sep 04, 2023 8:53 pm
- Who posted it: PSIG
- Evidence type: composite log / scope capture / wheel description
- Confidence: medium
- Summary: [CamJam](./memberlist.php?mode=viewprofile&u=7865) wrote: [↑](./viewtopic.php?p=64734#p64734) Mon Sep 04, 2023 5:07 am - setup the trigger to be Falling(Crank)-Rising(Cam), retimed the car and it starts and runs but the timing is not that accurate, like oscillates between 0-10degs *video* - Sync loss was gone too Speeduino only reports sync loss if it think it has lost sync. The up-side to this is that the thing screwing with signal is something that Speeduino does not see as an issue - so no loss reported. <hint> Can you get a composite log so we can see what Speeduino thinks it sees? Assuming the probes were correct polarity (+ 'scope probe on VR+), the cam sensor is correct polarity and should use Rising edge.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation. Discusses sync-loss conditions that can inform negative test cases.
- Replay-test details: Includes: falling edge Includes: cam Includes: crank Includes: sync Includes: sync loss Matched decoder family: Basic distributor
- Matched search terms: Basic distributor sync

#### 4age 16v smallport
- URL: https://speeduino.com/forum/viewtopic.php?t=5986#p64375
- Date: Sun Aug 13, 2023 6:54 am
- Who posted it: CamJam
- Evidence type: composite log
- Confidence: medium
- Summary: Hey Everyone, I have a 0.4 board running 202305FW, and installed in the car hooked up the 4age 16v and have all my sensors calibrated. I am running a stock distributor running 24+1 through a max2966. The only changes were breaking the grounds of the 24t and 1t and running a shielded cable to the ECU. I have tested this with a drill and while cranking using the composite logger.
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Includes: change Includes: crank Matched decoder family: Basic distributor Image or capture links: 2 Attachments: 4age_202305FW.zip -> https://speeduino.com/forum/download/file.php?id=11884; 4age_202305FW.zip -> https://speeduino.com/forum/download/file.php?id=11892
- Matched search terms: Basic distributor sync

#### Basic Distributor x Sequential injector
- URL: https://speeduino.com/forum/viewtopic.php?t=6065#p65249
- Date: Fri Oct 13, 2023 6:44 am
- Who posted it: PSIG
- Evidence type: scope capture
- Confidence: medium
- Summary: I am not sure what your question is? Yes, you can run timed with a distributor and cycle sensor. There are several ways to do this. Other functions are explainable, such as all modes have adjustable injection end point, so you will always see a Lambda change in any case including batch fire.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: change Includes: sync Includes: cas Matched decoder family: Basic distributor
- Matched search terms: Basic distributor oscilloscope

#### Problems with Basic Distributor - Start of injection the solution?
- URL: https://speeduino.com/forum/viewtopic.php?t=7194#p73320
- Date: Fri Oct 10, 2025 1:56 pm
- Who posted it: Jannik Panda 16V
- Evidence type: scope capture
- Confidence: medium
- Summary: Hey! The method with the diode works. But there are some anomalies. When I have a falling edge, the rev counter jerks (rev counter is connected to coil) when the ignition is switched on and the fuel pump is only activated very briefly.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: rising edge Includes: falling edge Includes: change Matched decoder family: Basic distributor
- Matched search terms: Basic distributor oscilloscope

#### 4 to 6 additional sparks after trigger stops - problem
- URL: https://speeduino.com/forum/viewtopic.php?t=6779#p71050
- Date: Sun Mar 02, 2025 3:46 pm
- Who posted it: yugokoral
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Hi guys. I need some help. I have basic distributor with VR - pickup coil connected to VR conditioner on speeduino. I was doing some tests on a scope and watching ignition coil and trigger signals.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: basic distributor Includes: cas
- Matched search terms: Basic distributor scope

#### 1990 Turbo MX-5 on PNP Beta 2
- URL: https://speeduino.com/forum/viewtopic.php?t=1671#p24521
- Date: Fri Feb 23, 2018 7:57 am
- Who posted it: PSIG
- Evidence type: technical discussion
- Confidence: medium
- Summary: [bond007](./memberlist.php?mode=viewprofile&u=1253) wrote: [↑](./viewtopic.php?p=24520#p24520) Fri Feb 23, 2018 4:37 am Is the prevailing understanding that the base map loaded onto the PNP, if properly wired, will start and run? One would like to think so, but it is far from guaranteed. Some do and some don't. There are so many factors that can change, from model or version, to various production changes, to previous owner adjustments or changes.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Includes: change Includes: crank Includes: cas Matched decoder family: Basic distributor
- Matched search terms: Basic distributor CAS

#### Problems with Basic Distributor - Start of injection the solution?
- URL: https://speeduino.com/forum/viewtopic.php?t=7194#p73297
- Date: Tue Oct 07, 2025 8:06 pm
- Who posted it: Jannik Panda 16V
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: Hey Jonbill, thanks for answer! Yes, here it is! In the Log data you can see, the RPM is sometimes fluctuating. Ignore AFR - there is at moment a narrowband sensor.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Basic Distributor Includes: cam Includes: crank Image or capture links: 2 Attachments: 2025-10-05_15.55.06.msl -> https://speeduino.com/forum/download/file.php?id=14474; Tune VW Golf2.msq -> https://speeduino.com/forum/download/file.php?id=14473
- Matched search terms: Basic distributor oscilloscope

#### Trigger setup for 1 cylinder engine with just VR on crank shaft
- URL: https://speeduino.com/forum/viewtopic.php?t=6473#p69121
- Date: Mon Aug 05, 2024 12:57 am
- Who posted it: PSIG
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Thank you, Karsten. Please post your tune, and a log of cranking. The o-scope image is odd, that the MAX inverts the incoming signal on the output. I assume that is what the yellow line is?
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: Basic Distributor Includes: missing tooth Includes: falling edge Includes: crank
- Matched search terms: Basic distributor scope

#### 1990 Turbo MX-5 on PNP Beta 2
- URL: https://speeduino.com/forum/viewtopic.php?t=1671#p25055
- Date: Tue Mar 20, 2018 12:31 pm
- Who posted it: noisymime
- Evidence type: scope capture / maintainer explanation
- Confidence: high
- Summary: Just had a play with this hear and everything seems to be working OK. The idle DC I'm seeing matches perfectly what is being requested in TS, and at the correct frequency. Just to check, in TS go to the Tuning Menu->Realtime Display->All Output Channels and then look for the one called Idle Load. Assuming you have open loop PWM as the control method, that value will be the DC that the board is trying to set.
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior. Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Matched decoder family: Basic distributor
- Matched search terms: Basic distributor CAS

#### 1987 Subaru XT turbo, trouble syncing RPM
- URL: https://speeduino.com/forum/viewtopic.php?t=2474#p30698
- Date: Wed Dec 12, 2018 9:19 am
- Who posted it: JoostXT
- Evidence type: tune file / image attachment
- Confidence: medium
- Summary: Thanks for the replies guys! I actually had another go at this whilst waiting for your replies and figured out some stuff already. Apparently the CAS or coil is powered of a relais somewhere, I can hear it clicking whilst cranking and the "sync" light blinks in the same rythym So I powered the CAS directly of the ignition barrel and it now syncs perfectly! Still doesn't start though until the relais decides to stick and actually power the coil, this needs some digging in the wiring.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Includes: change Includes: crank Includes: sync Includes: cas Matched decoder family: Basic distributor Image or capture links: 1 Attachments: Subaru XT turbo.msq -> https://speeduino.com/forum/download/file.php?id=4160
- Matched search terms: Basic distributor CAS

#### 3rd project: land rover series 110.
- URL: https://speeduino.com/forum/viewtopic.php?t=735#p11658
- Date: Sat Aug 20, 2016 7:58 pm
- Who posted it: cx500tc
- Evidence type: wheel description
- Confidence: medium
- Summary: PSIG wrote: mattsabun wrote: ... I am going to considered this to be my next project. not sure it is series 88 or 110, it comes with 2200 petrol/contact breaker/carburetor engine. so, what trigger wheel should i use?
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Basic Distributor Includes: cam Includes: crank Includes: cas
- Matched search terms: Basic distributor CAS

#### Erratic engine speed from hall effect crank trigger - Datsun A12
- URL: https://speeduino.com/forum/viewtopic.php?t=3084#p37248
- Date: Wed Aug 21, 2019 9:10 am
- Who posted it: Offthewheel
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Through each iteration of this project (basic distributor with optical isolator then electronic distributor, and now hall effect crank sensor), I have had an unstable rpm that the speeduino interprets from my crank trigger. Each change I have made is in an effort to get a clean reliable engine speed input. In this most recent setup I have recessed three neodymium magnets into the the front flange of the crank pulley at 90, 180, and 270 degrees with - photo attached, with the hall sensor solidly mounted in front of them. I have also attached the 'scope of the incoming signal - there is some noise on the signal but I don't know if that is the cause of the unstable indicated RPM I am getting.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: 4-1, basic distributor Includes: missing tooth Includes: change Includes: cam Includes: crank
- Matched search terms: Basic distributor CAS

#### 3rd project: land rover series 110.
- URL: https://speeduino.com/forum/viewtopic.php?t=735#p11668
- Date: Sat Aug 20, 2016 11:46 pm
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: medium
- Summary: noisymime wrote: Accuracy in degrees is dependant on the resolution of the primary wheel and the RPM. RPM is a factor because the system isn't accurate to a fixed number of degrees, it's accurate to a (roughly) fixed period of time. The faster the RPM, the more degrees that represents. Understood.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 60-2, Basic Distributor Includes: cas
- Matched search terms: Basic distributor CAS

#### 3rd project: land rover series 110.
- URL: https://speeduino.com/forum/viewtopic.php?t=735#p11689
- Date: Sun Aug 21, 2016 6:53 am
- Who posted it: mattsabun
- Evidence type: wheel description
- Confidence: medium
- Summary: PSIG wrote: mattsabun wrote: ... I am going to considered this to be my next project. not sure it is series 88 or 110, it comes with 2200 petrol/contact breaker/carburetor engine. so, what trigger wheel should i use?
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Basic Distributor Includes: change Includes: cas
- Matched search terms: Basic distributor CAS

#### Question about speeduino ignition control for Chrysler 318 V8
- URL: https://speeduino.com/forum/viewtopic.php?t=7374#p74192
- Date: Sun Mar 01, 2026 5:32 pm
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: medium
- Summary: Much of the current Wiki version assumes you have some basic understanding of engine components, signals, and required outputs. Our hope is for you to find the answers in the documentation as they are (minimally) all there, although much of the fundamental info is far reduced from previous Wiki versions, such as the difference in analog versus digital distributor signals. More of the basic requirements info is in the archived Wiki [here](https://web.archive.org/web/20161216123200/http://speeduino.com/wiki/index.php/Speeduino) , and you will minimally need to review [Crank Sensors](https://web.archive.org/web/20161204093009/http://speeduino.com/wiki/index.php/Hardware_requirements#Crank_sensor) , [MAP](https://web.archive.org/web/20161204093009/http://speeduino.com/wiki/index.php/Hardware_requirements#MAP_.28Manifold_Pressure.29) , and [Coils](https://web.archive.org/web/20161204093009/http://speeduino.com/wiki/index.php/Hardware_requirements#Coils) output info. The short answer is yes.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Basic Distributor Includes: cam Includes: crank
- Matched search terms: Basic distributor crank

#### Problems with Basic Distributor - Start of injection the solution?
- URL: https://speeduino.com/forum/viewtopic.php?t=7194#p73293
- Date: Tue Oct 07, 2025 12:53 pm
- Who posted it: Jannik Panda 16V
- Evidence type: tune file / wheel description / image attachment
- Confidence: low
- Summary: Hello, I'm Jannik from Germany and I'm having trouble controlling an old VW engine. (4 cylinders, 4 injectors, ignition distributor with Hall sensor) While driving, the engine repeatedly has short, minor misfires, which disappear when I set the trigger edge from falling to rising and adjust the offset and EOI so that the start of injection is close to the trigger offset (close to the trigger edge) and switch from semi-injection (1+4 2+3) to simultaneous injection. But as soon as I turn off the car and it stands for a longer period of time, the car does not want to start properly. It does start, but runs very poorly and stalls immediately without throttle.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 1-2, 60-2 Includes: rising edge Includes: falling edge Includes: crank Matched decoder family: Basic distributor Image or capture links: 2 Attachments: 2025-10-05_15.55.06.msl -> https://speeduino.com/forum/download/file.php?id=14474; Tune VW Golf2.msq -> https://speeduino.com/forum/download/file.php?id=14473
- Matched search terms: Basic distributor oscilloscope

#### Trigger setup for 1 cylinder engine with just VR on crank shaft
- URL: https://speeduino.com/forum/viewtopic.php?t=6473#p69127
- Date: Mon Aug 05, 2024 6:22 pm
- Who posted it: Hich
- Evidence type: scope capture
- Confidence: low
- Summary: Ok, I have tapped the MAX pins directly and I think there are two problems: On the scope is blue - VR+ as input to MAX9926 and yellow the output (pin 4). 1.Tunerstudio shows about 2000rpm while it is 1000rpm, so something is definitly wrong defined. V+ (blue), Max9926 output (yellow) @ 1000rpm 20240805_Max9926_In_out_1.JPEG (1.19 MiB) Viewed 9181 times close up, 20240805_Max9926_In_out_2.JPEG (1.03 MiB) Viewed 9181 times 2. When cranking stops there is no real signal (besides noise) on the VR+ pin, but there is still output from Max9926 with ignition coil connected 20240805_Max9926_cranking_with_coil.JPEG (932.94 KiB) Viewed 9181 times If I disconnect the ignition coil, the signal looks clean: ignition coil disconnected 20240805_Max9926_cranking_no_coil.JPEG (1.18 MiB) Viewed 9181 times So obviously the Max9926 is catching some noise due to the induction of the ignition coil.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: crank Matched decoder family: Basic distributor
- Matched search terms: Basic distributor scope

#### Basic distributor GM HEI
- URL: https://speeduino.com/forum/viewtopic.php?t=4835#p52081
- Date: Tue Jul 27, 2021 4:03 pm
- Who posted it: chester240
- Evidence type: scope capture
- Confidence: low
- Summary: Good day and hello to all. Im new to speeduino but not to other ECM's. I have a problem with the GM HEI setup. The engine starts fine with bypass disconected and the cam signal is clean.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: cam Matched decoder family: Basic distributor Image or capture links: 1
- Matched search terms: Basic distributor tooth logger

#### Erratic engine speed from hall effect crank trigger - Datsun A12
- URL: https://speeduino.com/forum/viewtopic.php?t=3084#p37282
- Date: Thu Aug 22, 2019 10:23 pm
- Who posted it: Offthewheel
- Evidence type: wheel description
- Confidence: low
- Summary: Thanks for that - I am very tight for space and at the front of the engine and can't mount a trigger wheel on the front of the crankshaft pulley. If I go with an OEM trigger I will have to adapt a CAS to go on the distributor drive. The reason I thought this would work is that the speeduino is set up to run off a basic distributor as a trigger (only 2 signals per crankshaft revolution in that case) I did have the engine running on an old school distributor but had engine speed instability issues, so changed to an electronic distributor thinking it was ignition noise from the points or induction. Had the instability problem again The "weird shit" comment isn't helpful - if I wanted an off the shelf project I wouldn't be using a Speeduino - an EFI conversion of 40 year old A12 needs a bit of creativity.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: basic distributor Includes: change Includes: crank Includes: cas
- Matched search terms: Basic distributor CAS

#### Basic Distributor x Sequential injector
- URL: https://speeduino.com/forum/viewtopic.php?t=6065#p65244
- Date: Fri Oct 13, 2023 1:33 am
- Who posted it: marciohiteck
- Evidence type: wheel description
- Confidence: low
- Summary: Hello everybody, How are you. I'm here with some questions, but before I ask them, I have to explain my configuration that I use nowadays, that is, day-to-day.I have a 1983 Beetle with a 16000 air cooled. I use an original Vw manifold for these injected engines, so far with the Speeduino everything works perfectly, but I have to make it clear here that my rotation sensor is my distributor, this is the inductive type. So the configuration is practically this, the other sensors are engine and factory defaults.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: basic distributor Includes: change Includes: cas
- Matched search terms: Basic distributor oscilloscope

#### 1987 Subaru XT turbo, trouble syncing RPM
- URL: https://speeduino.com/forum/viewtopic.php?t=2474#p30655
- Date: Sun Dec 09, 2018 5:39 pm
- Who posted it: Yoosyn
- Evidence type: wheel description
- Confidence: low
- Summary: JoostXT wrote: Since I kind of need the car to be reliable and useable throughout the project I decided to reuse all sensors and wiring harnasses I've done same on my Impreza. Its my daily driver and its happy with Speede JoostXT wrote: - car is running stock distributor with nissan 360 CAS with a unique Subaru pattern - speeduino triggers on the low-res signal in basic distributor mode In trigger settings: Try lowering trigger signal filter. You can also lower "skip revolutions" 4 is quite big number. JoostXT wrote: - low Z injectors with resistorpack You are using low Zs.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: basic distributor Includes: cas
- Matched search terms: Basic distributor CAS

#### Fuel Injected Paramotor
- URL: https://speeduino.com/forum/viewtopic.php?t=7187#p73274
- Date: Thu Oct 02, 2025 9:28 pm
- Who posted it: digmorepaka
- Evidence type: wheel description
- Confidence: low
- Summary: If it's a single cylinder I would advise against speeduino and recommend rusefi/fome instead. Speeduino's triggering as it stands is insufficient for a 2 stroke 1cylinder to drive ignition, only fuel with a basic distributor pattern is what I would consider usable. No problem on 2 cylinder 2T. The ~30L/h scooter pumps tend need around 1.5A in normal operation at 3 bar.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: basic distributor Includes: crank Includes: cas
- Matched search terms: Basic distributor crank

#### Fuel Injected Paramotor
- URL: https://speeduino.com/forum/viewtopic.php?t=7187#p73276
- Date: Fri Oct 03, 2025 12:53 am
- Who posted it: MrDieselTwitch
- Evidence type: wheel description
- Confidence: low
- Summary: Thank you for the insight! I’ll look into rusefi. Currently the ignition is TCI. I had high hopes of going CDI so I can can control the spark advance.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: basic distributor Includes: crank Includes: cas
- Matched search terms: Basic distributor crank

### Daihatsu

#### Progress report - February 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=1706#p24914
- Date: Wed Mar 14, 2018 10:44 am
- Who posted it: noisymime
- Evidence type: wheel description / maintainer explanation
- Confidence: medium
- Summary: OK, so this one has taken me a LOT longer to get out than normal. Turns out one of the things I've been meaning to sit down and write took a lot longer than I'd first thought and early in Feb around the time I'd normally be looking to do a monthly release, the whole thing was essentially in a million pieces. Rather than trying to cobble it back together in a hurry (Which typically leads to more problems than it solves) I elected to give myself a few more weeks. I can here you all asking "What could possibly be so big that it took so damn long to finish?".
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Pattern references: Daihatsu Includes: change Includes: crank Includes: sync
- Matched search terms: Daihatsu sync

#### Progress report - June 2017
- URL: https://speeduino.com/forum/viewtopic.php?t=1278#p19733
- Date: Tue Jul 11, 2017 11:32 am
- Who posted it: noisymime
- Evidence type: wheel description / maintainer explanation
- Confidence: medium
- Summary: Ok, so I fully admit that this is the latest I've gotten a monthly firmware out, but in my defence it's been a huge month of work! Before anyone gets too excited, a lot of the work in the June firmware is 'under the covers' rather than being sexy new features, but personally I consider the changes made this month to be some of the most important I've done in at least the last 12 months. From a reliability and accuracy perspective, there is no question that this firmware is the best since the project started. Firstly, as mentioned last month, I have been working to make the firmware [MISRA compliant](https://en.wikipedia.org/wiki/MISRA_C) .
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Pattern references: Daihatsu Includes: missing tooth Includes: change Includes: cam Includes: cas
- Matched search terms: Daihatsu Speeduino trigger

#### New firmware 202103 Trigger 36-2-2-2 not working for Daihatsu after update
- URL: https://speeduino.com/forum/viewtopic.php?t=4523#p48796
- Date: Sun Mar 07, 2021 12:07 am
- Who posted it: theonewithin
- Evidence type: tooth log
- Confidence: medium
- Summary: Do any EEPRM clear and update and load base tune and then enter values manually. Check tooth logger and see what you get there Post both a log and your tune.
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Matched decoder family: Daihatsu
- Matched search terms: Daihatsu tooth logger

#### Daihatsu HD Carb to ITB Speeduino
- URL: https://speeduino.com/forum/viewtopic.php?t=5610#p59945
- Date: Mon Dec 12, 2022 6:19 am
- Who posted it: RedRallyRoza
- Evidence type: wheel description
- Confidence: medium
- Summary: Hi, Been researching how to convert my carburated Daihatsu Feroza for EFI, on and off for the past decade. Originally looked into buying the EFI parts (not worth the effort to get something 30 years old) but now I'm settled on either speeduino or MS. It's a Daihatsu HD-C engine, 1600. I think I've seen a few on the forujm but they seem to all be coming from the factory EFI HD-E or HC-E My current plan, and absolutely feel free to tell me where I'm wrong or making things hard for myself, is to use a 4AGE silvertop ITB, and a trigger wheel replacing the guts of the distributor.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 36-1, Daihatsu Includes: change Includes: cam
- Matched search terms: Daihatsu Speeduino trigger

#### V0.3 Daihatsu HC-E/HD-E Build
- URL: https://speeduino.com/forum/viewtopic.php?t=1070#p16943
- Date: Sun Feb 26, 2017 10:09 am
- Who posted it: brendon.beckett
- Evidence type: wheel description
- Confidence: medium
- Summary: So, I figured I'd better start a build thread. I'm hoping to learn a fair bit from the members on this forum, so it's best if I let everyone know where I'm coming from and more importantly where I'm headed with my project. Might turn into a bit of a narrative initially, so apologies for that. First things first.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Daihatsu Includes: cam Includes: crank
- Matched search terms: Daihatsu cam

#### Daihatsu K3-VE
- URL: https://speeduino.com/forum/viewtopic.php?t=1186#p18487
- Date: Wed May 10, 2017 4:42 am
- Who posted it: PSIG
- Evidence type: scope capture
- Confidence: medium
- Summary: Indeed it appears the VVT control is PWM at a fixed frequency, using higher duty cycle to advance the cam, and lower DC to retard it. Somewhere in there it has to pass zero. It would be simple enough to use a multimeter or o-scope to check the PWM frequency, and the o-scope would also tell you the DC under various conditions in operation. If I were to tackle this, I'd put a Speeduino together and use it for a data collector to log sensor inputs while also 'scoping the VVT in order to determine when the PWM does what.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: cam Includes: cas Matched decoder family: Daihatsu
- Matched search terms: Daihatsu scope

#### Decoder for Nissan Micra K11 Distributor Disc
- URL: https://speeduino.com/forum/viewtopic.php?t=6166#p66630
- Date: Mon Jan 08, 2024 10:28 am
- Who posted it: baguete
- Evidence type: wheel description
- Confidence: medium
- Summary: Hi guys, new here. I'm getting my 0.4.3d board ready but still getting my head around the decoder needed for my car. It uses an optical sensor like most Nissans but with a different pattern, 4 big slots and 1 small slot for 1st cylinder TDC. This is talked here: [viewtopic.php?p=33047](https://speeduino.com/forum/viewtopic.php?p=33047) but no further discussion was made.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Basic Distributor, Daihatsu Includes: sync
- Matched search terms: Daihatsu sync

#### Progress report - February 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=1706#p25189
- Date: Wed Mar 28, 2018 1:42 am
- Who posted it: noisymime
- Evidence type: maintainer explanation
- Confidence: medium
- Summary: [stum](./memberlist.php?mode=viewprofile&u=1391) wrote: [↑](./viewtopic.php?p=25133#p25133) Sun Mar 25, 2018 3:47 am What I have found: On the temperature (water) enrichment I had my last dot on the graph at 85'C Every thing worked as expected up to that temp. Enrichment dial sitting at approx. 90%. all good Once it got over 85'C the enrichment jumped up to 110% so 20% more gas.
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Includes: change Includes: cas Matched decoder family: Daihatsu
- Matched search terms: Daihatsu sync

#### Daihatsu K3-VE
- URL: https://speeduino.com/forum/viewtopic.php?t=1186#p18734
- Date: Mon May 22, 2017 11:46 am
- Who posted it: fazren
- Evidence type: scope capture
- Confidence: low
- Summary: [dazq](./memberlist.php?mode=viewprofile&u=186) wrote: [↑](./viewtopic.php?p=18525#p18525) Fri May 12, 2017 1:40 am Your vvt sounds very similar in operation to that on my aj30:Jag v6. The hold or dither position is approx 50% Less DC decreases and more DC increases the timing. The further the DC away from 50% the faster the timing shift. If you keep the valve at a constant DC away from the dither it will advance or retard all the way to its limits.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: cam Includes: crank Matched decoder family: Daihatsu
- Matched search terms: Daihatsu scope

#### Daihatsu
- URL: https://speeduino.com/forum/viewtopic.php?t=1213#p18851
- Date: Fri May 26, 2017 2:11 pm
- Who posted it: twobarboost
- Evidence type: wheel description
- Confidence: low
- Summary: Hi there everyone. I was wondering if anyone could give me some advice on Speeduino and it's suitability with regards to my Daihatsu EJ-DE 989cc three cylinder. I'm interested in experimenting with a home built ECU. From the factory the engine uses a 3+1 CAMSHAFT sensor (only) and runs sequential injection and sequential (coil on plug) ignition.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Daihatsu Includes: cam Includes: crank
- Matched search terms: Daihatsu crank

#### Decoder for Nissan Micra K11 Distributor Disc
- URL: https://speeduino.com/forum/viewtopic.php?t=6166#p66663
- Date: Thu Jan 11, 2024 11:57 am
- Who posted it: baguete
- Evidence type: wheel description
- Confidence: low
- Summary: [Raku](./memberlist.php?mode=viewprofile&u=845) wrote: [↑](./viewtopic.php?p=66631#p66631) Mon Jan 08, 2024 11:23 am just done a k11. daihatsu +1 works fine running fully sequential. It doesn't get sync problems? The decoders.cpp file has this comment: //We need to try and find the extra tooth (#2) which is located 30 degrees after tooth #1 //Aim for tooth times less than about 60 degrees Can this be a problem or the firmware can go to the 80 degrees offset that the Nissan disc has?
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Daihatsu, daihatsu Includes: falling edge Includes: sync
- Matched search terms: Daihatsu sync

#### Decoder for Nissan Micra K11 Distributor Disc
- URL: https://speeduino.com/forum/viewtopic.php?t=6166#p66681
- Date: Sat Jan 13, 2024 1:34 pm
- Who posted it: Raku
- Evidence type: wheel description
- Confidence: low
- Summary: [baguete](./memberlist.php?mode=viewprofile&u=8275) wrote: [↑](./viewtopic.php?p=66663#p66663) Thu Jan 11, 2024 11:57 am [Raku](./memberlist.php?mode=viewprofile&u=845) wrote: [↑](./viewtopic.php?p=66631#p66631) Mon Jan 08, 2024 11:23 am just done a k11. daihatsu +1 works fine running fully sequential. It doesn't get sync problems? The decoders.cpp file has this comment: //We need to try and find the extra tooth (#2) which is located 30 degrees after tooth #1 //Aim for tooth times less than about 60 degrees Can this be a problem or the firmware can go to the 80 degrees offset that the Nissan disc has?
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Daihatsu, daihatsu Includes: falling edge Includes: sync
- Matched search terms: Daihatsu sync

#### Progress report - February 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=1706#p24944
- Date: Thu Mar 15, 2018 4:14 am
- Who posted it: Foordy
- Evidence type: wheel description
- Confidence: low
- Summary: [noisymime](./memberlist.php?mode=viewprofile&u=48) wrote: [↑](./viewtopic.php?p=24941#p24941) Wed Mar 14, 2018 11:28 pm [Foordy](./memberlist.php?mode=viewprofile&u=1558) wrote: [↑](./viewtopic.php?p=24931#p24931) Wed Mar 14, 2018 9:11 pm Great work as usual, well done! When you say the Daihatsu +1 issues are fixed for 4cyl, what about 3cyl? The problem fixed didn't occur on 3 cylinder engines. They were (and are) working fine to the best of my knowledge.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Daihatsu Includes: sync
- Matched search terms: Daihatsu sync

#### Daihatsu
- URL: https://speeduino.com/forum/viewtopic.php?t=1213#p18899
- Date: Sun May 28, 2017 9:04 am
- Who posted it: twobarboost
- Evidence type: wheel description
- Confidence: low
- Summary: That's the exact trigger wheel. It certainly is low res.... but i'd rather leave the engine as standard as possible so i'll give it a go and see what sort of results I have. If it's absolute garbage, i'll swap to a missing tooth crank sensor I guess.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Includes: missing tooth Includes: crank Matched decoder family: Daihatsu
- Matched search terms: Daihatsu crank

#### Daihatsu 3cylinder trigger wheel support?
- URL: https://speeduino.com/forum/viewtopic.php?t=1072#p16946
- Date: Sun Feb 26, 2017 2:52 pm
- Who posted it: Tom Wood
- Evidence type: wheel description
- Confidence: low
- Summary: Hey, just wondering if its possible to include compatibility for the Daihatsu 3 cylinder trigger wheel. It has 3 teeth for TDC on each cylinder and an extra tooth by one that acts as a home signal. I have included a picture of the trigger wheel.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Daihatsu
- Matched search terms: Daihatsu trigger wheel

### Harley

#### A cobbled together oddfire (Harley)
- URL: https://speeduino.com/forum/viewtopic.php?t=1022#p17301
- Date: Sat Mar 18, 2017 1:10 pm
- Who posted it: Jcwood3
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Had a little more success with the setup. I was having trouble with ardustim so I wrote a simple program that maps a pot output to time delay on a square wave, effectively giving me rpm control. Works great and I'm getting sync, though there is no missing tooth on the signal, so I'm not sure what that's about. Assembled my 14point7 wide band controller and had everything working great until I fiddled with the capacitor that bumps the display and now the display doesn't read.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: 32-2 Includes: missing tooth Includes: sync Matched decoder family: Harley
- Matched search terms: Harley scope

#### A cobbled together oddfire (Harley)
- URL: https://speeduino.com/forum/viewtopic.php?t=1022#p17362
- Date: Mon Mar 20, 2017 8:43 pm
- Who posted it: PSIG
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: Random spewing among your quotes for simplicity: [Jcwood3](./memberlist.php?mode=viewprofile&u=940) wrote: [↑](./viewtopic.php?p=17344#p17344) Mon Mar 20, 2017 1:47 am ... A couple of differences I think I'll use will be the use of only one O2 sensor and no security system (not shown). Also the "active intake" is not used - pretty sure that was an emissions/noise thing used outside the US and is not installed. No cats?
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: 4-6 Includes: cam Includes: crank Matched decoder family: Harley
- Matched search terms: Harley scope

#### What else do I need? Nub question.
- URL: https://speeduino.com/forum/viewtopic.php?t=2035#p28017
- Date: Wed Aug 22, 2018 11:37 am
- Who posted it: doleckijohn
- Evidence type: scope capture / wheel description
- Confidence: medium
- Summary: I have to split the cases anyway to machine some parts for the blower drive. So I can see the crank then an how its machined for the crank sensor. Or I'll just put the scope on the sensor before i split the cases. Also if I jut mount and aftermarket trigger wheel to the engine is the Speeduino capable of using it to run the odd firing order of a harley?
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: harley Includes: crank Includes: cas
- Matched search terms: Harley Speeduino trigger

#### Trying to build a trigger for uneven flywheel.
- URL: https://speeduino.com/forum/viewtopic.php?t=4907#p53326
- Date: Wed Sep 29, 2021 6:29 pm
- Who posted it: RempageR1
- Evidence type: wheel description
- Confidence: low
- Summary: I`ve written a working trigger that can sync on the width of a tooth. It`s been validated and works like a charm including the trigger filter . Sharing here so hopefully it can benefit someone else. Code: [Select all](#) ``` /** Rewritten Harley code to use on the Vmax.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 36-1, Harley, Vmax Includes: change Includes: crank Includes: sync Includes: cas
- Matched search terms: Harley Speeduino trigger

#### Sync with dual inductive pick-up coils on single trigger wheel
- URL: https://speeduino.com/forum/viewtopic.php?t=4612#p49796
- Date: Sat Apr 24, 2021 7:22 pm
- Who posted it: iiminov
- Evidence type: wheel description
- Confidence: low
- Summary: Just wanted to confirm if speeduino is capable of syncing from dual inductive pick-up coil setups that come out from the factory. To provide a bit of perspective I am working with Honda NT650 V-Twin 4 stroke engine. The trigger wheel and pickup coils should look like the image I found in the Honda Hawk manual. source: http://hawkworks.net/manual/images/7/7-10-img03.jpg NT650_Pulse_Generators_2.png (65.11 KiB) Viewed 5051 times It's an 8-1 trigger wheel but with two pick-up coils spaced apart 130 or 230 degrees depending on how you measure.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 8-1, Harley Includes: missing tooth Includes: sync
- Matched search terms: Harley Speeduino trigger

#### What else do I need? Nub question.
- URL: https://speeduino.com/forum/viewtopic.php?t=2035#p28025
- Date: Thu Aug 23, 2018 9:16 am
- Who posted it: PSIG
- Evidence type: wheel description
- Confidence: low
- Summary: [doleckijohn](./memberlist.php?mode=viewprofile&u=2003) wrote: [↑](./viewtopic.php?p=28017#p28017) Wed Aug 22, 2018 11:37 am Also if I jut mount and aftermarket trigger wheel to the engine is the Speeduino capable of using it to run the odd firing order of a harley? Hey John — Twin Cam? I think that's a 32-2 trigger on the crank, right? Speeduino can do odd-tooth wheels like that (see ["Non-360" decoder](https://speeduino.com/wiki/index.php/Non-360) ), and yes, it can do odd-fire cylinder spacing like your Harley.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 32-2, Harley, harley Includes: cam Includes: crank
- Matched search terms: Harley Speeduino trigger

#### Early Harley Davidson trigger cup.
- URL: https://speeduino.com/forum/viewtopic.php?t=5378#p57445
- Date: Sat Jun 18, 2022 5:02 pm
- Who posted it: jdolecki
- Evidence type: wheel description
- Confidence: low
- Summary: I see that the listed Harley trigger patterns works off the later crank sensor sensor. I want to add fi to the early engine. My earlier harley has the timing rotor in the cone. I previously read that the two slot rotor does not work well.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Harley, harley Includes: crank
- Matched search terms: Harley crank

#### Predator 670 parts list
- URL: https://speeduino.com/forum/viewtopic.php?t=7303#p73845
- Date: Fri Jan 02, 2026 12:12 pm
- Who posted it: Claycity
- Evidence type: wheel description
- Confidence: low
- Summary: ok so im deployed and ordering part for my return. so far I have ECU wide band o2 stepper motor controler trigger wheel, sensor, pigtail coils, and pigtails Deutsch kits rolled of wire different colors fuse relay box 2015 harley sportster throttle body. it has the TPS, IAT, IAC, injector (ill be moving them closer to the head) all in one housing.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: harley
- Matched search terms: Harley trigger wheel

### Honda D17

#### Help With Honda D17A1 Project
- URL: https://speeduino.com/forum/viewtopic.php?t=4304#p46940
- Date: Mon Dec 07, 2020 5:03 am
- Who posted it: Stars
- Evidence type: tune file / wheel description
- Confidence: medium
- Summary: Okay, so it has been quite a while since I have initially posted this tread. I have made quite a bit of progress, even getting the first """start.""" Although there are a bunch of confusions I am having with getting this to work, and will be going over all of them in this post! For reference, I have been following this thread, also using the basemap from it: [viewtopic.php?f=18&t=1323&p=20375](https://speeduino.com/forum/viewtopic.php?f=18&t=1323&p=20375) Again, this is the D17A1, but it is basically the same as the JDM D17 model besides a few small differences. And without the VTEC .
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Honda D17 Includes: change Includes: crank Includes: sync
- Matched search terms: Honda D17 sync

#### Secondary Edge Changing itself from falling to rising (Dual Wheel Trigger)
- URL: https://speeduino.com/forum/viewtopic.php?t=5283#p56690
- Date: Thu Apr 28, 2022 2:11 am
- Who posted it: Wfulk25
- Evidence type: tune file / wheel description / image attachment
- Confidence: medium
- Summary: Hi all! New to the forum, so apologies if this has been discussed/answered already. I'm running a speeduino v4.3 on my Honda D17 engine with modified trigger wheels, so it has a crank trigger with 12 evenly spaced teeth and a cam wheel with one tooth on it running the dual wheel trigger pattern. I have been daily driving the car like this for about 5 months now and it has been running really good (And getting better gas mileage than it did before), although I am a tuning novice so still a work in progress all the time.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Honda D17 Includes: rising edge Includes: falling edge Includes: change Includes: cam Includes: crank Includes: sync Image or capture links: 1 Attachments: Honda D17 Tune.msq -> https://speeduino.com/forum/download/file.php?id=9656
- Matched search terms: Honda D17 Speeduino trigger

#### Help With Honda D17A1 Project
- URL: https://speeduino.com/forum/viewtopic.php?t=4304#p46096
- Date: Thu Oct 29, 2020 11:57 pm
- Who posted it: Stars
- Evidence type: tune file / image attachment
- Confidence: low
- Summary: Hello all! I am excited for my PCB and the kit to arrive.. I am going to be putting this in my 2002 Honda Civic to increase it's horsepower a bit, and hopefully make it more fuel efficient. The idea is that I want to have a open source computer on my car so I can have complete control over what my vehicle does.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Includes: crank Matched decoder family: Honda D17 Image or capture links: 1 Attachments: CurrentTune.msq -> https://speeduino.com/forum/download/file.php?id=7679
- Matched search terms: Honda D17 sync

#### Honda K20 trigger pattern
- URL: https://speeduino.com/forum/viewtopic.php?t=7277#p73689
- Date: Sun Nov 30, 2025 9:16 pm
- Who posted it: Michal327
- Evidence type: wheel description
- Confidence: low
- Summary: Hi folks, I've created a new thread about k20 trigger because two years ago @miker wrote that the new pattern was ready but untested. To avoid deeming my project a failure, I really need any code or files you have for this engine so it can run in full sequential mode on Speeduino v0.4.3d. I'm trying to write my own custom decoder, but it's not working. Please help me, I'm out of ideas.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 36-1, Honda D17 Includes: change Includes: cam Includes: crank Includes: sync
- Matched search terms: Honda D17 Speeduino trigger

#### Honda K20 trigger pattern
- URL: https://speeduino.com/forum/viewtopic.php?t=7277#p73812
- Date: Wed Dec 24, 2025 1:04 pm
- Who posted it: Michal327
- Evidence type: wheel description
- Confidence: low
- Summary: Sorry for not responding @delion This is crank and cam trigger pattern which I followed when creating the decoder: Honda D17 crank trigger wheel: 12+1 Honda K20 crank trigger wheel: 12+1 Honda K20 cam trigger wheel: 4+1 The D17 decoder is semi-sequential (wasted spark) and there is no script written for triggerSec_HondaD17 (for reading cam). Just to be clear, the principle of operation of the cam trigger is almost the same as the crank trigger ( it remembers the last tooth gap and checks if it is half the current gap, because it has +1 extra tooth in between ). I made sure that sync is triggered correctly in my code. It seems like it should work, but it doesn't.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Honda D17 Includes: cam Includes: crank Includes: sync
- Matched search terms: Honda D17 Speeduino trigger

#### Wheel Decoder and VVT coder for Honda K series
- URL: https://speeduino.com/forum/viewtopic.php?t=5816#p73651
- Date: Tue Nov 25, 2025 8:29 pm
- Who posted it: Michal327
- Evidence type: wheel description
- Confidence: low
- Summary: Hi folks, @miker if you already have a sequential decoder, could you please upload it to github? I'm trying to write my own custom decoder, but it's not working. Please help me, I'm out of ideas. Tell me what to change.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: 36-1, Honda D17 Includes: change Includes: cam Includes: crank Includes: sync
- Matched search terms: Honda D17 Speeduino trigger

#### Coils getting hot
- URL: https://speeduino.com/forum/viewtopic.php?t=4363#p46665
- Date: Tue Nov 24, 2020 4:41 pm
- Who posted it: civic_duty
- Evidence type: wheel description
- Confidence: low
- Summary: so i have a speeduino installed on my Toyota 4age 20v, 0.4.3 board retrofitted a Mitsubishi 1g cas sensor for crank/cam signal. it running sequential fuel and spark. board is setup for hall sensor input (no VR conditioner), JP1 i set to 5v . Tunerstudio software spark settings set to going low and sequential, cranking dwell 6ms and running is 4ms, spark duration 1ms.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: honda D17 Includes: cam Includes: crank Includes: cas
- Matched search terms: Honda D17 CAS

#### Any idea?
- URL: https://speeduino.com/forum/viewtopic.php?t=6709#p70506
- Date: Fri Jan 17, 2025 7:13 am
- Who posted it: Thencerest1947
- Evidence type: wheel description
- Confidence: low
- Summary: Have an pevision to add the decoder for 4+1 cam pulley to run up full sequential injection? Product: Honda D17 Cam sensor
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Honda D17 Includes: cam
- Matched search terms: Honda D17 cam

#### Any idea?
- URL: https://speeduino.com/forum/viewtopic.php?t=6709#p70514
- Date: Fri Jan 17, 2025 2:54 pm
- Who posted it: digmorepaka
- Evidence type: wheel description
- Confidence: low
- Summary: [Thencerest1947](./memberlist.php?mode=viewprofile&u=9316) wrote: [↑](./viewtopic.php?p=70506#p70506) Fri Jan 17, 2025 7:13 am Have an pevision to add the decoder for 4+1 cam pulley to run up full sequential injection? Product: Honda D17 Cam sensor You need to add more information and a little more cohesively.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: Honda D17 Includes: cam
- Matched search terms: Honda D17 cam

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

#### Classic Mini SPI Automtic
- URL: https://speeduino.com/forum/viewtopic.php?t=4776#p52362
- Date: Sat Aug 07, 2021 5:56 am
- Who posted it: dazq
- Evidence type: wheel description / maintainer explanation
- Confidence: medium
- Summary: Have a search on here there is a thread running for the testing of the rover mems trigger patterns. It is not yet included in the release download but there are links there to the test release. Yes this will mean you could use the OEM trigger wheel and not need a replacement 36-1. [minitw](./memberlist.php?mode=viewprofile&u=5145) wrote: [↑](./viewtopic.php?p=52360#p52360) Sat Aug 07, 2021 1:23 am Hi dazq, could you point where I can find the trigger for rover?
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Pattern references: 36-1, rover mems Includes: crank
- Matched search terms: Rover MEMS Speeduino trigger

#### Classic Mini SPI Automtic
- URL: https://speeduino.com/forum/viewtopic.php?t=4776#p51599
- Date: Sun Jul 04, 2021 7:54 am
- Who posted it: dazq
- Evidence type: wheel description / maintainer explanation
- Confidence: medium
- Summary: [Mightyminimods](./memberlist.php?mode=viewprofile&u=4810) wrote: [↑](./viewtopic.php?p=51582#p51582) Sat Jul 03, 2021 10:05 am Hi Folks, I am trying to find out if it is possible to: 1. Is it possible to use a Speeduino ECU with a single point injection classic mini? 2. Is it possible to use a Speeduino ECU with a Rover Mini SPI with an automatic gearbox (Ap2 transmission)?
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior.
- Replay-test details: Pattern references: rover mems
- Matched search terms: Rover MEMS Speeduino trigger

#### Rover MGF K series MEMs 1.9 trigger pattern decoder
- URL: https://speeduino.com/forum/viewtopic.php?t=4770#p51548
- Date: Thu Jul 01, 2021 1:19 pm
- Who posted it: Trevor Getty
- Evidence type: scope capture / wheel description
- Confidence: low
- Summary: Thanks very much Mike for raising this feature and testing branch. I've hopefully got each of the 4 patterns in various cars in my collection, but time maybe a different story So I'll defo test this for you as soon as my t16 has finished its speedy tuning. Should be within the next week. For anyone that is interested this decoder combination is for a dizzy based k series which will cover a lot of the early k series models that had mems1.9 ecus which are the type with the vacuum line coming out of them and pg1 gearbox and crank sensor only.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: crank Matched decoder family: Rover MEMS
- Matched search terms: Rover MEMS Speeduino trigger

### Subaru 6/7

#### Firmware release - October 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=2456#p30403
- Date: Thu Nov 29, 2018 12:21 pm
- Who posted it: noisymime
- Evidence type: composite log / scope capture / wheel description / maintainer explanation
- Confidence: high
- Summary: OK, so this has been a long, long while coming (In fact, at 2.5 months, it's the longest there has ever been between Speeduino firmware releases), but there are a few tasty little additions that make it worth the wait! The biggest one straight off the bat is the addition of composite logging! This has been one of the most requested features for a long, long time and is a tremendous help in diagnosing issues with trigger issues. For those not familiar with the composite logging function in Tuner Studio, this presents a scope like view of the crank and cam inputs, eg: Screen Shot 2018-10-15 at 2.29.22 pm.png (243.53 KiB) Viewed 17380 times In terms of other large changes, the other one this time around is the addition of customisable analog filter values.
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior. Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: Subaru 6/7 Includes: change Includes: cam Includes: crank
- Matched search terms: Subaru 6/7 scope

#### Firmware release - October 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=2456#p30910
- Date: Sun Dec 30, 2018 12:19 am
- Who posted it: noisymime
- Evidence type: scope capture / wheel description / maintainer explanation
- Confidence: high
- Summary: [Yoosyn](./memberlist.php?mode=viewprofile&u=1591) wrote: [↑](./viewtopic.php?p=30504#p30504) Sun Dec 02, 2018 6:33 pm Reporting problems with Subaru 6/7 First: it is hard to start engine. One time it got sync, other time not. Is there any chance to choose vr signal filters? It is unavailable after update.
- Why it matters: Maintainer explanation increases confidence in expected decoder behavior. Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: Subaru 6/7 Includes: change Includes: sync
- Matched search terms: Subaru 6/7 scope

#### Fresh build, no sync. Subaru EJ20.
- URL: https://speeduino.com/forum/viewtopic.php?t=6013#p64768
- Date: Wed Sep 06, 2023 3:03 am
- Who posted it: rozap
- Evidence type: composite log / scope capture / wheel description
- Confidence: high
- Summary: PSIG wrote: Polarity is important. Your 'scope traces appear to be tested in correct polarity. If connected in same polarity to Speeduino, the Trigger Edge settings with inverting MAX conditioners is Rising , and with non-inverting conditioners such as the DSC is Falling . These combinations are required for the decoder to read the signals and gaps correctly.
- Why it matters: Provides replay-test-relevant trigger traces. Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Pattern references: subaru 6/7 Includes: crank
- Matched search terms: Subaru 6/7 Speeduino trigger

#### Subaru EJ20G do not start
- URL: https://speeduino.com/forum/viewtopic.php?t=5841#p62555
- Date: Thu Apr 20, 2023 3:59 pm
- Who posted it: cyrille
- Evidence type: composite log / tune file / wheel description
- Confidence: medium
- Summary: Good morning, Above all, excuse me for the faults, I am French and a bit lazy with languages. My name is Cyrille, I live in the south of France. I have a 1998 Subaru WRX which drive perfectly with an Apexi Power FC, but I would like to try the Speeduino for the challenge. I have been registered on the forum for some time, I bought a Speeduino v0.4.4c some time ago with a max conditioner.
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Pattern references: Subaru 6/7 Image or capture links: 4 Attachments: 1997 ej20k jdm ecu wiring ver3-4.pdf -> https://speeduino.com/forum/download/file.php?id=11357; 2023-04-20_15.12.25.csv -> https://speeduino.com/forum/download/file.php?id=11356; 2023-04-21_08.16.46.csv -> https://speeduino.com/forum/download/file.php?id=11360
- Matched search terms: Subaru 6/7 Speeduino trigger

#### Subaru EJ20G do not start
- URL: https://speeduino.com/forum/viewtopic.php?t=5841#p62556
- Date: Thu Apr 20, 2023 5:59 pm
- Who posted it: miker
- Evidence type: tooth log / composite log
- Confidence: medium
- Summary: You may have a problem with noise. The very small teeth in the log file could be electrical noise. You may need better shielded cable between the sensor and Speeduino. Remember to only ground the shielding at the Speeduino ground.
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Matched decoder family: Subaru 6/7
- Matched search terms: Subaru 6/7 Speeduino trigger

#### Subaru EJ20G do not start
- URL: https://speeduino.com/forum/viewtopic.php?t=5841#p62560
- Date: Thu Apr 20, 2023 7:08 pm
- Who posted it: cyrille
- Evidence type: tooth log / composite log
- Confidence: medium
- Summary: [miker](./memberlist.php?mode=viewprofile&u=2011) wrote: [↑](./viewtopic.php?p=62556#p62556) Thu Apr 20, 2023 5:59 pm You may have a problem with noise. The very small teeth in the log file could be electrical noise. You may need better shielded cable between the sensor and Speeduino. Remember to only ground the shielding at the Speeduino ground.
- Why it matters: Provides replay-test-relevant trigger traces.
- Replay-test details: Matched decoder family: Subaru 6/7
- Matched search terms: Subaru 6/7 Speeduino trigger

#### Fresh build, no sync. Subaru EJ20.
- URL: https://speeduino.com/forum/viewtopic.php?t=6013#p64770
- Date: Wed Sep 06, 2023 5:55 am
- Who posted it: rozap
- Evidence type: composite log / scope capture
- Confidence: medium
- Summary: Digging into the code a bit, would I be right in thinking that this line sets the flag that the composite logger ultimately uses to display the blue square wave? Code: [Select all](#) ``` BIT_SET(decoderState, BIT_DECODER_VALID_TRIGGER); ``` [https://github.com/noisymime/speeduino/ ... .ino#L3587](https://github.com/noisymime/speeduino/blob/master/speeduino/decoders.ino#L3587) If so, then that'd narrow things down a bit since I know at this point that there are trigger pulses but we never enter a statement where hasSync is set to true. According to the scope, it's doing one rotation every (approx) 300ms, so about 200 RPM.
- Why it matters: Provides replay-test-relevant trigger traces. Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: sync Matched decoder family: Subaru 6/7
- Matched search terms: Subaru 6/7 Speeduino trigger

#### Subaru EJ20G do not start
- URL: https://speeduino.com/forum/viewtopic.php?t=5841#p62621
- Date: Sun Apr 23, 2023 9:04 pm
- Who posted it: miker
- Evidence type: image attachment
- Confidence: medium
- Summary: updated code here -> [https://github.com/mike501/speeduino/tree/Subaru67Fixes](https://github.com/mike501/speeduino/tree/Subaru67Fixes) Instructions attached (to use this you'll have to download a compile and compile the firmware) This fixes/changes - resync (the code always does a resync now) adds error checking / reporting errors (sync loss counter) tries to recover syncloss on nearly every occurance - should within half a revolution get sync back adds noise filtering for every tooth - due to different tooth gaps, this isn't a brilliant implementation but its better than nothing. It may need more work depending how much the rpm changes per 180 degrees. no promises it will work on a real engine, but it works against the ardustim development version of the 6/7 decoder. Please make backups of your current tune before starting.
- Why it matters: Discusses sync-loss conditions that can inform negative test cases.
- Replay-test details: Includes: change Includes: sync Includes: sync loss Matched decoder family: Subaru 6/7 Image or capture links: 1 Attachments: Speeduino Installing custom firmware.docx -> https://speeduino.com/forum/download/file.php?id=11374
- Matched search terms: Subaru 6/7 Speeduino trigger

#### Firmware release - October 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=2456#p30411
- Date: Thu Nov 29, 2018 4:26 pm
- Who posted it: PSIG
- Evidence type: composite log / scope capture
- Confidence: medium
- Summary: Awesome! Nice batch of big changes that should be very helpful. A couple questions: Will the new filters affect input and processing function in each/every case, e.g., O2 corrections less aggressive with greater filtering? Separately, will input logging and gauge response be less defined with greater filtering?
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: change Includes: cam Includes: crank Includes: cas Matched decoder family: Subaru 6/7
- Matched search terms: Subaru 6/7 scope

#### Firmware release - October 2018
- URL: https://speeduino.com/forum/viewtopic.php?t=2456#p30958
- Date: Wed Jan 02, 2019 12:31 pm
- Who posted it: Broke4speed
- Evidence type: scope capture
- Confidence: medium
- Summary: [PSIG](./memberlist.php?mode=viewprofile&u=149) wrote: [↑](./viewtopic.php?p=30411#p30411) Thu Nov 29, 2018 4:26 pm • Rising and Falling are relative to which signal edge you want Speeduino to detect incoming on the processor pin — after the sensor action and any input conditioner or circuit effects. For examples; if you want to detect the trailing-edges of a missing-tooth wheel, most (not all) Hall sensors ground on the tooth detection, and so the input edge for correct reading would be Rising in that case. Alternatively, a correct-polarity ('correctly' wired) VR sensor also looking for tooth trailing-edge detection would typically have a rising voltage as the tooth approaches and falling on the trailing edge, however if using an inverting signal conditioner (MAX992x, LM1815, etc), the signal at the Mega will be Rising in order to properly detect the tooth trailing edge. I would strongly suggest using the tooth-logger, or a meter/'scope on the Mega input pins, or the output LEDs on some conditioner modules (MiniMAX-A2, DSC, etc) in order to test and verify the correct selection for the tooth edge detection required — no different than before, but now less confusing with the Rising/Falling terms.
- Why it matters: Includes hardware-signal evidence, useful for edge and polarity interpretation.
- Replay-test details: Includes: change Includes: cam Includes: crank Includes: cas Matched decoder family: Subaru 6/7
- Matched search terms: Subaru 6/7 scope

### Vmax

#### Ardustim decoder help
- URL: https://speeduino.com/forum/viewtopic.php?t=5708#p61149
- Date: Thu Feb 09, 2023 8:51 pm
- Who posted it: chris900f
- Evidence type: wheel description
- Confidence: low
- Summary: Thanks for the link. I've been looking at the wheels.def in Speeduino with VS Studio. I'm intrigued by the VMAX pattern that identifies the widest tooth on the rotor for sync,( and doesn't need a cam signal). One thing I can't figure out is how they set-up to read both the rising and falling edges on the same wheel.
- Why it matters: Contains decoder-specific technical details that may support test coverage.
- Replay-test details: Pattern references: VMAX Includes: rising edge Includes: falling edge Includes: cam Includes: sync
- Matched search terms: Vmax sync

## Strongest evidence by decoder

- **1-3-4-2**: [Issues trying to start an EJ16 with Speeduino v0.4](https://speeduino.com/forum/viewtopic.php?t=6880#p71697) — George Tsotsos, Fri Apr 25, 2025 4:22 pm, confidence medium.
- **3-4**: [4age 16v smallport](https://speeduino.com/forum/viewtopic.php?t=5986#p64671) — CamJam, Wed Aug 30, 2023 12:16 pm, confidence medium.
- **36-1**: [Trying to build a trigger for uneven flywheel.](https://speeduino.com/forum/viewtopic.php?t=4907#p52998) — RempageR1, Thu Sep 09, 2021 10:58 am, confidence low.
- **36-2-2-2**: [Fresh build, no sync. Subaru EJ20.](https://speeduino.com/forum/viewtopic.php?t=6013#p64751) — rozap, Tue Sep 05, 2023 1:23 am, confidence medium.
- **420a**: [How to configure Primary Base Teeth and Trigger Angle on the 420A engine?](https://speeduino.com/forum/viewtopic.php?t=6739#p70720) — PedroV9, Wed Feb 05, 2025 10:36 pm, confidence medium.
- **8-0**: [4age 16v smallport](https://speeduino.com/forum/viewtopic.php?t=5986#p64903) — CamJam, Sun Sep 17, 2023 3:03 am, confidence medium.
- **Basic distributor**: [1987 Subaru XT turbo, trouble syncing RPM](https://speeduino.com/forum/viewtopic.php?t=2474#p30601) — JoostXT, Thu Dec 06, 2018 1:03 pm, confidence high.
- **Daihatsu**: [Progress report - February 2018](https://speeduino.com/forum/viewtopic.php?t=1706#p24914) — noisymime, Wed Mar 14, 2018 10:44 am, confidence medium.
- **Harley**: [A cobbled together oddfire (Harley)](https://speeduino.com/forum/viewtopic.php?t=1022#p17301) — Jcwood3, Sat Mar 18, 2017 1:10 pm, confidence medium.
- **Honda D17**: [Help With Honda D17A1 Project](https://speeduino.com/forum/viewtopic.php?t=4304#p46940) — Stars, Mon Dec 07, 2020 5:03 am, confidence medium.
- **Honda J32**: [J35a4 Triggers](https://speeduino.com/forum/viewtopic.php?t=6027#p65009) — OldKamikaze, Mon Sep 25, 2023 1:15 am, confidence medium.
- **Rover MEMS**: [rover MEMs decoder](https://speeduino.com/forum/viewtopic.php?t=1427#p51307) — Trevor Getty, Wed Jun 23, 2021 1:34 am, confidence high.
- **Subaru 6/7**: [Firmware release - October 2018](https://speeduino.com/forum/viewtopic.php?t=2456#p30403) — noisymime, Thu Nov 29, 2018 12:21 pm, confidence high.
- **Vmax**: [Ardustim decoder help](https://speeduino.com/forum/viewtopic.php?t=5708#p61149) — chris900f, Thu Feb 09, 2023 8:51 pm, confidence low.

## Decoders with enough information to build replay tests safely

- Basic distributor
- Rover MEMS
- Subaru 6/7

## Decoders still blocked by missing real trace evidence

- 1-3-4-2
- 3-4
- 36-1
- 36-2-2-2
- 420a
- 8-0
- Daihatsu
- Harley
- Honda D17
- Honda J32
- Vmax

## Suggested next priorities for test coverage

- 1-3-4-2: still needs stronger real-trace evidence before high-confidence replay coverage.
- 3-4: still needs stronger real-trace evidence before high-confidence replay coverage.
- 36-1: still needs stronger real-trace evidence before high-confidence replay coverage.
- 36-2-2-2: convert existing trace-backed maintainer guidance into replay tests first.
- 420a: add edge and polarity-focused tests derived from scope evidence.
- 8-0: add edge and polarity-focused tests derived from scope evidence.
- Basic distributor: convert existing trace-backed maintainer guidance into replay tests first.
- Daihatsu: convert existing trace-backed maintainer guidance into replay tests first.
- Harley: add edge and polarity-focused tests derived from scope evidence.
- Honda D17: still needs stronger real-trace evidence before high-confidence replay coverage.
- Honda J32: add edge and polarity-focused tests derived from scope evidence.
- Rover MEMS: convert existing trace-backed maintainer guidance into replay tests first.

## JSON export

- Structured JSON dataset written to: `C:\Users\Cornelio\Desktop\speeduino-202501.6\Resources\speeduino_forum_evidence.json`
