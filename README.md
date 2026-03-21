<div align="center">

# Speeduino AI Edition

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)
[![Based on Speeduino](https://img.shields.io/badge/based%20on-Speeduino%20202501.6-green.svg)](https://github.com/noisymime/speeduino)
[![Platform](https://img.shields.io/badge/platform-PlatformIO-orange.svg)](https://platformio.org/)

**An AI-Enhanced Fork of Speeduino with Advanced Testing, Safety Improvements, and Teensy 4.1 Optimizations**

</div>

---

## 🎯 Project Overview

**Speeduino AI Edition** is a research and development fork of the [Speeduino Engine Management System](https://speeduino.com), focused on improving firmware safety, correctness, and test coverage through systematic refactoring and AI-assisted development. This fork implements a structured roadmap emphasizing defensive programming, comprehensive testing, and modern embedded development practices.

### Key Differences from Upstream Speeduino

This fork diverges from mainline Speeduino to pursue experimental improvements that may not align with upstream stability requirements:

- **Enhanced Safety & Correctness**: Systematic fixes to ignition, fueling, and protection logic
- **Comprehensive Test Coverage**: Extensive unit tests for decoders, corrections, engine protection, and config migrations
- **Decoder Trace Replay System**: Novel trace-based regression testing for trigger decoder validation
- **Teensy 4.1 Platform Enablement**: First-class support with optimized peripherals, storage, and capabilities
- **Structured Development Roadmap**: Phased approach prioritizing safety before features (see [FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md))

---

## 📋 Table of Contents

- [What is Speeduino?](#what-is-speeduino)
- [AI Edition Features](#ai-edition-features)
- [Current Development Phase](#current-development-phase)
- [Quick Start](#quick-start)
- [Building the Firmware](#building-the-firmware)
- [Running Tests](#running-tests)
- [Project Structure](#project-structure)
- [Development Roadmap](#development-roadmap)
- [Session Handoffs](#session-handoffs)
- [Contributing](#contributing)
- [Important Warnings](#important-warnings)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## 🚗 What is Speeduino?

Speeduino is an open-source, Arduino-based Engine Management System (EMS/ECU) that provides:

- Full engine control (fuel injection and ignition timing)
- Support for 1-12 cylinder engines
- Multiple trigger decoder patterns (60+ supported patterns)
- Tuning via TunerStudio
- Low-cost DIY-friendly hardware designs
- Active community support

For more information about the upstream Speeduino project:
- **Official Website**: https://speeduino.com
- **Documentation**: https://wiki.speeduino.com
- **Original Repository**: https://github.com/noisymime/speeduino

---

## ✨ AI Edition Features

### Phase 1: Safety and Correctness ✅
- Fixed high-risk ignition, fueling, and protection defects
- Corrected knock handling and limiter/protection interactions
- Replaced placeholder telemetry with accurate runtime data
- Added focused regression coverage for corrected logic

### Phase 2: Regression Harness (In Progress) 🔄
- **Decoder Trace Replay System**: Novel test fixture for trigger pattern validation
  - Externalized trace headers stored in flash memory
  - Support for both simple and compressed repeated-event traces
  - Coverage for 8+ decoder families with real/semi-real waveform patterns
- **Config Migration Tests**: End-to-end validation of EEPROM version migrations
- **Runtime State-Machine Tests**: Direct validation of decoder sync/wrap/filtering behavior

**Current Decoder Coverage** (164/164 tests passing):
- Dual-Wheel (clean sync, noise filtering, wrap, resync)
- Ford ST170 (half-sync, cam-before-gap, noise, regroup)
- Suzuki K6A (descending-gap sync, wrap, filtering)
- Missing-Tooth 36-1 (gap sync, noise, half-sync, cam promotion, early-gap loss)
- NGC4 (half-sync, cam phase variants)
- Nissan360 (4/6/8-cylinder windows, invalid rejection, resync, wrap)
- Renix (valid-group advancement, short-gap filtering)
- **Miata 99-05** (edge-counted cam sync, revolution wrap) ⭐ NEW

### Phase 3-6: Planned
- Runtime structure improvements
- Board and comms consistency
- Configuration and observability enhancements
- Teensy 4.1 platform enablement with native peripherals

See [FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md) for complete details.

---

## 🔧 Current Development Phase

**Phase 2: Regression Harness** - Building comprehensive test coverage for critical subsystems

Latest work (2026-03-21):
- Added Miata 99-05 decoder trace replay coverage
- Documented edge-counted cam synchronization behavior
- All 164 decoder tests passing

See [SESSION_HANDOFF_2026-03-21.md](speeduino/SESSION_HANDOFF_2026-03-21.md) for the latest development notes.

---

## 🚀 Quick Start

### Prerequisites

- [PlatformIO Core](https://platformio.org/) or [PlatformIO IDE](https://platformio.org/platformio-ide)
- Git
- (Optional) [TunerStudio](https://www.tunerstudio.com/) for tuning

### Clone the Repository

```bash
git clone https://github.com/BuffJesus/SpeeduinoAIEdition.git
cd SpeeduinoAIEdition
```

---

## 🔨 Building the Firmware

### For Teensy 4.1 (Recommended)

```bash
pio run -e teensy41
```

Output: `.pio/build/teensy41/firmware.hex`

### For Arduino Mega 2560

```bash
pio run -e megaatmega2560
```

### All Supported Platforms

See `platformio.ini` for the complete list of build environments.

---

## 🧪 Running Tests

### Run All Unit Tests

```bash
pio test -e megaatmega2560_sim_unittest
```

### Run Specific Test Suites

```bash
# Decoder tests only
pio test -e megaatmega2560_sim_unittest --filter test_decoders

# Config migration tests
pio test -e megaatmega2560_sim_unittest --filter test_updates

# Math/correction tests
pio test -e megaatmega2560_sim_unittest --filter test_maths
```

### Current Test Status
- **164/164 decoder tests** ✅
- **25/25 config migration tests** ✅
- All test suites passing

---

## 📁 Project Structure

```
SpeeduinoAIEdition/
├── speeduino/                  # Main firmware source
│   ├── decoders.cpp           # Trigger decoder implementations
│   ├── corrections.cpp        # Fueling/ignition corrections
│   ├── updates.cpp            # Config migration logic
│   ├── FIRMWARE_ROADMAP.md    # Development roadmap
│   └── SESSION_HANDOFF_*.md   # Development session notes
├── test/                      # Unit test suite
│   ├── test_decoders/         # Decoder validation tests
│   │   └── traces/            # Externalized trigger trace headers
│   ├── test_updates/          # Config migration tests
│   └── test_maths/            # Math/correction tests
├── release/                   # Release artifacts (.hex, .ini)
├── reference/                 # Hardware designs and documentation
├── platformio.ini             # Build configuration
├── README.md                  # This file
└── LICENSE                    # GPLv3 license
```

---

## 🗺️ Development Roadmap

This fork follows a **structured, phase-based roadmap** prioritizing safety and correctness:

1. **Phase 1: Safety And Correctness** ✅ Complete
2. **Phase 2: Regression Harness** 🔄 In Progress
3. **Phase 3: Runtime Structure** 📋 Planned
4. **Phase 4: Board And Comms Consistency** 📋 Planned
5. **Phase 5: Configuration And Observability** 📋 Planned
6. **Phase 6: Teensy 4.1 Platform Enablement** 📋 Planned

See [FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md) for complete phase descriptions and goals.

---

## 📝 Session Handoffs

Development is tracked through session handoff documents that capture:
- Changes made in each session
- Test verification results
- Notes and insights
- Recommended next steps

Latest handoffs:
- [SESSION_HANDOFF_2026-03-21.md](speeduino/SESSION_HANDOFF_2026-03-21.md) - Miata 99-05 decoder coverage
- [SESSION_HANDOFF_2026-03-20.md](speeduino/SESSION_HANDOFF_2026-03-20.md) - Trace replay harness expansion
- [SESSION_HANDOFF_2026-03-19.md](speeduino/SESSION_HANDOFF_2026-03-19.md) - Config migration tests
- [SESSION_HANDOFF_2026-03-16.md](speeduino/SESSION_HANDOFF_2026-03-16.md) - Decoder state-machine tests

---

## 🤝 Contributing

This is an **experimental research fork** focused on systematic improvement and testing. Contributions are welcome, but please note:

### Contribution Guidelines

1. **Safety First**: All changes must maintain or improve safety/correctness
2. **Test Coverage Required**: New features must include unit tests
3. **Follow the Roadmap**: Align with current phase objectives
4. **Documentation**: Update session handoffs and roadmap as appropriate

### Development Workflow

```bash
# 1. Create a feature branch
git checkout -b feature/your-feature-name

# 2. Make changes and add tests
# ... edit files ...

# 3. Run tests to verify
pio test -e megaatmega2560_sim_unittest

# 4. Build firmware to verify compilation
pio run -e teensy41

# 5. Commit with descriptive messages
git commit -m "Add feature: description"

# 6. Push and create pull request
git push origin feature/your-feature-name
```

See [contributing.md](contributing.md) for more details (inherited from upstream).

---

## ⚠️ Important Warnings

### This is Experimental Software

- **DO NOT USE ON A REAL ENGINE** without extensive bench testing
- This fork contains experimental changes that may not be as well-tested as upstream Speeduino
- Always have a backup ECU and safety plan when testing
- Read and understand the code before deploying

### Compatibility Notes

- This fork is **not guaranteed to be compatible** with upstream Speeduino releases
- INI file format may differ from mainline Speeduino
- Config migrations are tested but may not handle all edge cases
- Teensy 4.1 builds are the primary development target

### Liability

This software is provided "AS IS" without warranty of any kind. The authors and contributors are not responsible for any damage to engines, vehicles, or property resulting from use of this firmware.

---

## 📜 License

This project inherits the **GPLv3** license from upstream Speeduino.

```
Speeduino AI Edition
Copyright (C) 2024-2026 BuffJesus and contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

See [LICENSE](LICENSE) for the full license text.

---

## 🙏 Acknowledgments

### Upstream Speeduino Project
This fork is based on the excellent work of the Speeduino community:
- **Josh Stewart** (noisymime) - Original Speeduino creator and maintainer
- The entire Speeduino community - For years of development and support
- **Official Speeduino**: https://github.com/noisymime/speeduino

### Inspiration and Borrowed Patterns
- **rusEFI Project**: For verification discipline patterns, trigger testing approaches, and config migration architecture
  - Repository: https://github.com/rusefi/rusefi
  - License: GPLv3

### AI-Assisted Development
This fork utilizes AI assistance (Claude Code) for:
- Systematic code review and refactoring
- Test development and coverage expansion
- Documentation generation
- Adherence to safety and correctness principles

### Community Support
- Speeduino Discord, Forum, and Facebook communities
- Open-source embedded systems community

---

## 📞 Contact & Support

- **Repository**: https://github.com/BuffJesus/SpeeduinoAIEdition
- **Issues**: https://github.com/BuffJesus/SpeeduinoAIEdition/issues
- **Upstream Speeduino Support**:
  - Discord: https://discord.gg/YWCEexaNDe
  - Forum: https://speeduino.com/forum
  - Facebook: https://www.facebook.com/groups/191918764521976/

---

<div align="center">

**Built with ❤️ and 🤖 for the Open-Source EMS Community**

*Based on Speeduino 202501.6*

</div>
