#include <unity.h>
#include "../../speeduino/updates.h"
#include "../../speeduino/globals.h"
#include "../../speeduino/pages.h"
#include "../../speeduino/storage.h"
#include "../../speeduino/sensors.h"
#include "../../speeduino/comms_CAN.h"

static void resetMigrationState() {
    resetConfigPages();
    memset(&configPage2, 0, sizeof(configPage2));
    memset(&configPage4, 0, sizeof(configPage4));
    memset(&configPage6, 0, sizeof(configPage6));
    memset(&configPage9, 0, sizeof(configPage9));
    memset(&configPage10, 0, sizeof(configPage10));
    memset(&configPage13, 0, sizeof(configPage13));
    memset(&configPage15, 0, sizeof(configPage15));
    updatesTestResetState();
    updatesTestSetInterceptStorageCalls(true);
}

static void seedConfigPage10Bytes() {
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&configPage10);
    for (uint16_t i = 0; i < sizeof(configPage10); ++i) {
        bytes[i] = uint8_t(i);
    }
}

static uint16_t seedMappedPageValues(byte pageNum, uint8_t modulus, uint8_t scale = 1U) {
    uint16_t touched = 0U;
    for (page_iterator_t it = page_begin(pageNum); it.type != End; it = advance(it)) {
        for (uint16_t offset = it.start; offset < (it.start + it.size); offset++) {
            setPageValue(pageNum, offset, uint8_t((offset % modulus) * scale));
            touched++;
        }
    }
    return touched;
}

void test_doUpdates_v21_to_v22_sets_rolling_cut_defaults_and_halves_dfco_hysteresis(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(21U);
    updatesTestSetStopAfterStore(true);

    configPage4.dfcoHyster = 18U;
    memset(configPage15.rollingProtRPMDelta, 0, sizeof(configPage15.rollingProtRPMDelta));
    memset(configPage15.rollingProtCutPercent, 0, sizeof(configPage15.rollingProtCutPercent));

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(22U, state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(22U, state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(1U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);

    TEST_ASSERT_EQUAL_UINT8(9U, configPage4.dfcoHyster);
    TEST_ASSERT_EQUAL_INT8(-30, configPage15.rollingProtRPMDelta[0]);
    TEST_ASSERT_EQUAL_INT8(-20, configPage15.rollingProtRPMDelta[1]);
    TEST_ASSERT_EQUAL_INT8(-10, configPage15.rollingProtRPMDelta[2]);
    TEST_ASSERT_EQUAL_INT8(-5, configPage15.rollingProtRPMDelta[3]);
    TEST_ASSERT_EQUAL_UINT8(50U, configPage15.rollingProtCutPercent[0]);
    TEST_ASSERT_EQUAL_UINT8(65U, configPage15.rollingProtCutPercent[1]);
    TEST_ASSERT_EQUAL_UINT8(80U, configPage15.rollingProtCutPercent[2]);
    TEST_ASSERT_EQUAL_UINT8(95U, configPage15.rollingProtCutPercent[3]);
}

void test_doUpdates_v22_to_v23_scales_wmi_map_and_sets_new_defaults(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(22U);
    updatesTestSetStopAfterStore(true);

    seedMappedPageValues(wmiMapPage, 20U);
    configPage10.wmiMode = WMI_MODE_OPENLOOP;
    configPage13.hwTestInjDuration = 1U;
    configPage13.hwTestIgnDuration = 1U;
    configPage9.dfcoTaperEnable = 1U;
    configPage9.dfcoTaperTime = 0U;
    configPage9.dfcoTaperFuel = 0U;
    configPage9.dfcoTaperAdvance = 0U;
    configPage9.egoMAPMax = 12U;
    configPage9.egoMAPMin = 99U;
    configPage2.canWBO = 3U;

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(23U, state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(23U, state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(1U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);

    for (page_iterator_t it = page_begin(wmiMapPage); it.type != End; it = advance(it)) {
        for (uint16_t offset = it.start; offset < (it.start + it.size); offset++) {
            TEST_ASSERT_EQUAL_UINT8(uint8_t((offset % 20U) * 2U), getPageValue(wmiMapPage, offset));
        }
    }

    TEST_ASSERT_EQUAL_UINT8(8U, configPage13.hwTestInjDuration);
    TEST_ASSERT_EQUAL_UINT8(4U, configPage13.hwTestIgnDuration);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage9.dfcoTaperEnable);
    TEST_ASSERT_EQUAL_UINT8(10U, configPage9.dfcoTaperTime);
    TEST_ASSERT_EQUAL_UINT8(100U, configPage9.dfcoTaperFuel);
    TEST_ASSERT_EQUAL_UINT8(20U, configPage9.dfcoTaperAdvance);
    TEST_ASSERT_EQUAL_UINT8(255U, configPage9.egoMAPMax);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage9.egoMAPMin);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage2.canWBO);
}

void test_doUpdates_v23_to_v24_migrates_can_knock_and_page10_alignment(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(23U);

    configPage2.unused1_126_1 = false;
    configPage2.unused1_126_2 = true;
    configPage4.CANBroadcastProtocol = CAN_BROADCAST_PROTOCOL_OFF;
    configPage2.flexFreqLow = 1U;
    configPage2.flexFreqHigh = 2U;
    seedConfigPage10Bytes();

    doUpdates();

    const updates_test_state state = updatesTestGetState();
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&configPage10);

    TEST_ASSERT_EQUAL_UINT8(updatesTestGetCurrentDataVersion(), state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(24U, state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(1U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);

    TEST_ASSERT_EQUAL_UINT8(KNOCK_MODE_OFF, configPage10.knock_mode);
    TEST_ASSERT_EQUAL_UINT8(CAN_BROADCAST_PROTOCOL_VAG, configPage4.CANBroadcastProtocol);
    TEST_ASSERT_EQUAL_UINT8(255U, configPage10.lnchCtrlVss);
    TEST_ASSERT_EQUAL_UINT8(50U, configPage2.flexFreqLow);
    TEST_ASSERT_EQUAL_UINT8(150U, configPage2.flexFreqHigh);

    TEST_ASSERT_EQUAL_UINT8(27U, bytes[25]);
    TEST_ASSERT_EQUAL_UINT8(25U, bytes[26]);
    TEST_ASSERT_EQUAL_UINT8(26U, bytes[27]);
    for (uint8_t index = 32U; index < 74U; ++index) {
        TEST_ASSERT_EQUAL_UINT8(uint8_t(index + 1U), bytes[index]);
    }
    TEST_ASSERT_EQUAL_UINT8(32U, bytes[74]);
}

void test_doUpdates_new_eeprom_initializes_defaults_and_current_version(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(255U);

    configPage9.true_address = 0U;
    configPage4.FILTER_FLEX = 0U;
    for (uint8_t i = 0; i < 8U; ++i) {
        configPage13.outputPin[i] = uint8_t(i + 10U);
    }

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(updatesTestGetCurrentDataVersion(), state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(updatesTestGetCurrentDataVersion(), state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);
    TEST_ASSERT_EQUAL_UINT16(0x200U, configPage9.true_address);
    TEST_ASSERT_EQUAL_UINT8(FILTER_FLEX_DEFAULT, configPage4.FILTER_FLEX);
    for (uint8_t i = 0; i < 8U; ++i) {
        TEST_ASSERT_EQUAL_UINT8(0U, configPage13.outputPin[i]);
    }
}

void test_doUpdates_future_version_clamps_to_current(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(uint8_t(updatesTestGetCurrentDataVersion() + 3U));

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(updatesTestGetCurrentDataVersion(), state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(updatesTestGetCurrentDataVersion(), state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);
}

void testConfigMigrations_group3b1(void) {
    RUN_TEST(test_doUpdates_v21_to_v22_sets_rolling_cut_defaults_and_halves_dfco_hysteresis);
    RUN_TEST(test_doUpdates_v22_to_v23_scales_wmi_map_and_sets_new_defaults);
}

void testConfigMigrations_group3b2(void) {
    RUN_TEST(test_doUpdates_v23_to_v24_migrates_can_knock_and_page10_alignment);
    RUN_TEST(test_doUpdates_new_eeprom_initializes_defaults_and_current_version);
    RUN_TEST(test_doUpdates_future_version_clamps_to_current);
}

void test_migrateVVTTableEntry_v17_to_v18_normal_range(void) {
    TEST_ASSERT_EQUAL_UINT8(0, migrateVVTTableEntry_v17_to_v18(0));
    TEST_ASSERT_EQUAL_UINT8(2, migrateVVTTableEntry_v17_to_v18(1));
    TEST_ASSERT_EQUAL_UINT8(100, migrateVVTTableEntry_v17_to_v18(50));
    TEST_ASSERT_EQUAL_UINT8(200, migrateVVTTableEntry_v17_to_v18(100));
    TEST_ASSERT_EQUAL_UINT8(254, migrateVVTTableEntry_v17_to_v18(127));
}

void test_migrateVVTTableEntry_v17_to_v18_overflow_protection(void) {
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateVVTTableEntry_v17_to_v18(128));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateVVTTableEntry_v17_to_v18(200));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateVVTTableEntry_v17_to_v18(255));
}

void test_migrateVVTTableEntry_v17_to_v18_boundary_values(void) {
    TEST_ASSERT_EQUAL_UINT8(254, migrateVVTTableEntry_v17_to_v18(127));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateVVTTableEntry_v17_to_v18(128));
}

void test_migrateIdleAdvDelay_v17_to_v18_normal_range(void) {
    TEST_ASSERT_EQUAL_UINT8(0, migrateIdleAdvDelay_v17_to_v18(0));
    TEST_ASSERT_EQUAL_UINT8(2, migrateIdleAdvDelay_v17_to_v18(1));
    TEST_ASSERT_EQUAL_UINT8(20, migrateIdleAdvDelay_v17_to_v18(10));
    TEST_ASSERT_EQUAL_UINT8(50, migrateIdleAdvDelay_v17_to_v18(25));
    TEST_ASSERT_EQUAL_UINT8(254, migrateIdleAdvDelay_v17_to_v18(127));
}

void test_migrateIdleAdvDelay_v17_to_v18_overflow_protection(void) {
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateIdleAdvDelay_v17_to_v18(128));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateIdleAdvDelay_v17_to_v18(200));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateIdleAdvDelay_v17_to_v18(255));
}

void test_migrateIdleAdvDelay_v17_to_v18_boundary_values(void) {
    TEST_ASSERT_EQUAL_UINT8(254, migrateIdleAdvDelay_v17_to_v18(127));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateIdleAdvDelay_v17_to_v18(128));
}

void testConfigMigrations_group1b(void) {
    // v17→v18 migration helpers (Phase 5)
    RUN_TEST(test_migrateVVTTableEntry_v17_to_v18_normal_range);
    RUN_TEST(test_migrateVVTTableEntry_v17_to_v18_overflow_protection);
    RUN_TEST(test_migrateVVTTableEntry_v17_to_v18_boundary_values);
    RUN_TEST(test_migrateIdleAdvDelay_v17_to_v18_normal_range);
    RUN_TEST(test_migrateIdleAdvDelay_v17_to_v18_overflow_protection);
    RUN_TEST(test_migrateIdleAdvDelay_v17_to_v18_boundary_values);
}
