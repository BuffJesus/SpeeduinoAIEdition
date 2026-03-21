/**
 * @file test_config_migrations.cpp
 * @brief Unit tests for EEPROM config migration functions (updates.cpp)
 */

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
    memset(cltCalibration_bins, 0, sizeof(cltCalibration_bins));
    memset(cltCalibration_values, 0, sizeof(cltCalibration_values));
    memset(iatCalibration_bins, 0, sizeof(iatCalibration_bins));
    memset(iatCalibration_values, 0, sizeof(iatCalibration_values));
    memset(o2Calibration_bins, 0, sizeof(o2Calibration_bins));
    memset(o2Calibration_values, 0, sizeof(o2Calibration_values));
    updatesTestResetState();
    updatesTestSetInterceptStorageCalls(true);
}

static void seedConfigPage10Bytes() {
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&configPage10);
    for (uint16_t i = 0; i < sizeof(configPage10); ++i) {
        bytes[i] = uint8_t(i);
    }
}

template <typename table3d_t>
static void seedTableAxesAndValues(table3d_t &table, uint16_t xStart, uint16_t yStart, uint8_t valueStart) {
    auto x = table.axisX.begin();
    uint16_t currentX = xStart;
    while (!x.at_end()) {
        *x = currentX++;
        ++x;
    }

    auto y = table.axisY.begin();
    uint16_t currentY = yStart;
    while (!y.at_end()) {
        *y = currentY++;
        ++y;
    }

    auto rows = table.values.begin();
    uint8_t currentValue = valueStart;
    while (!rows.at_end()) {
        auto row = *rows;
        while (!row.at_end()) {
            *row = currentValue++;
            ++row;
        }
        ++rows;
    }
}

template <typename table3d_t>
static void assertAxisUnchanged(table3d_t &table, uint16_t expectedStart) {
    auto axis = table.axisX.begin();
    uint16_t expected = expectedStart;
    while (!axis.at_end()) {
        TEST_ASSERT_EQUAL_UINT16(expected++, *axis);
        ++axis;
    }
}

template <typename table3d_t>
static void assertAxisScaled(table3d_t &table, uint16_t expectedStart, uint8_t scale) {
    auto axis = table.axisY.begin();
    uint16_t expected = expectedStart;
    while (!axis.at_end()) {
        TEST_ASSERT_EQUAL_UINT16(expected * scale, *axis);
        ++axis;
        ++expected;
    }
}

template <typename table3d_t>
static void assertValuesUnchanged(table3d_t &table, uint8_t expectedStart) {
    auto rows = table.values.begin();
    uint8_t expected = expectedStart;
    while (!rows.at_end()) {
        auto row = *rows;
        while (!row.at_end()) {
            TEST_ASSERT_EQUAL_UINT8(expected++, *row);
            ++row;
        }
        ++rows;
    }
}

void test_migratePIDGain_v13_to_v14_normal_range(void) {
    TEST_ASSERT_EQUAL_UINT8(0, migratePIDGain_v13_to_v14(0));
    TEST_ASSERT_EQUAL_UINT8(32, migratePIDGain_v13_to_v14(1));
    TEST_ASSERT_EQUAL_UINT8(64, migratePIDGain_v13_to_v14(2));
    TEST_ASSERT_EQUAL_UINT8(96, migratePIDGain_v13_to_v14(3));
    TEST_ASSERT_EQUAL_UINT8(128, migratePIDGain_v13_to_v14(4));
    TEST_ASSERT_EQUAL_UINT8(160, migratePIDGain_v13_to_v14(5));
    TEST_ASSERT_EQUAL_UINT8(192, migratePIDGain_v13_to_v14(6));
    TEST_ASSERT_EQUAL_UINT8(224, migratePIDGain_v13_to_v14(7));
}

void test_migratePIDGain_v13_to_v14_overflow_protection(void) {
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migratePIDGain_v13_to_v14(8));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migratePIDGain_v13_to_v14(9));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migratePIDGain_v13_to_v14(10));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migratePIDGain_v13_to_v14(255));
}

void test_migratePIDGain_v13_to_v14_boundary_values(void) {
    TEST_ASSERT_EQUAL_UINT8(224, migratePIDGain_v13_to_v14(7));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migratePIDGain_v13_to_v14(8));
}

void test_migrateCrankingEnrich_v13_to_v14_normal_range(void) {
    TEST_ASSERT_EQUAL_UINT8(0, migrateCrankingEnrich_v13_to_v14(0));
    TEST_ASSERT_EQUAL_UINT8(20, migrateCrankingEnrich_v13_to_v14(100));
    TEST_ASSERT_EQUAL_UINT8(40, migrateCrankingEnrich_v13_to_v14(200));
    TEST_ASSERT_EQUAL_UINT8(51, migrateCrankingEnrich_v13_to_v14(255));
}

void test_migrateCrankingEnrich_v13_to_v14_division_rounding(void) {
    TEST_ASSERT_EQUAL_UINT8(1, migrateCrankingEnrich_v13_to_v14(5));
    TEST_ASSERT_EQUAL_UINT8(1, migrateCrankingEnrich_v13_to_v14(6));
    TEST_ASSERT_EQUAL_UINT8(1, migrateCrankingEnrich_v13_to_v14(9));
    TEST_ASSERT_EQUAL_UINT8(2, migrateCrankingEnrich_v13_to_v14(10));
}

void test_migrateFlexAdvance_v13_to_v14_normal_range(void) {
    TEST_ASSERT_EQUAL_UINT8(40, migrateFlexAdvance_v13_to_v14(0));
    TEST_ASSERT_EQUAL_UINT8(50, migrateFlexAdvance_v13_to_v14(10));
    TEST_ASSERT_EQUAL_UINT8(80, migrateFlexAdvance_v13_to_v14(40));
    TEST_ASSERT_EQUAL_UINT8(140, migrateFlexAdvance_v13_to_v14(100));
}

void test_migrateFlexAdvance_v13_to_v14_overflow_protection(void) {
    TEST_ASSERT_EQUAL_UINT8(255, migrateFlexAdvance_v13_to_v14(215));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateFlexAdvance_v13_to_v14(216));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateFlexAdvance_v13_to_v14(255));
}

void test_migrateFlexAdvance_v13_to_v14_boundary_values(void) {
    TEST_ASSERT_EQUAL_UINT8(255, migrateFlexAdvance_v13_to_v14(215));
    TEST_ASSERT_EQUAL_UINT8(254, migrateFlexAdvance_v13_to_v14(214));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateFlexAdvance_v13_to_v14(216));
}

void test_migrateCrankingPctToCurve_v4_to_v5_normal_range(void) {
    TEST_ASSERT_EQUAL_UINT8(100, migrateCrankingPctToCurve_v4_to_v5(0));
    TEST_ASSERT_EQUAL_UINT8(110, migrateCrankingPctToCurve_v4_to_v5(10));
    TEST_ASSERT_EQUAL_UINT8(150, migrateCrankingPctToCurve_v4_to_v5(50));
    TEST_ASSERT_EQUAL_UINT8(200, migrateCrankingPctToCurve_v4_to_v5(100));
}

void test_migrateCrankingPctToCurve_v4_to_v5_overflow_protection(void) {
    TEST_ASSERT_EQUAL_UINT8(255, migrateCrankingPctToCurve_v4_to_v5(155));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateCrankingPctToCurve_v4_to_v5(156));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateCrankingPctToCurve_v4_to_v5(255));
}

void test_migrateCrankingPctToCurve_v4_to_v5_boundary_values(void) {
    TEST_ASSERT_EQUAL_UINT8(255, migrateCrankingPctToCurve_v4_to_v5(155));
    TEST_ASSERT_EQUAL_UINT8(254, migrateCrankingPctToCurve_v4_to_v5(154));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateCrankingPctToCurve_v4_to_v5(156));
}

void test_migrateTPSResolution_v18_to_v19_normal_range(void) {
    TEST_ASSERT_EQUAL_UINT8(0, migrateTPSResolution_v18_to_v19(0));
    TEST_ASSERT_EQUAL_UINT8(20, migrateTPSResolution_v18_to_v19(10));
    TEST_ASSERT_EQUAL_UINT8(50, migrateTPSResolution_v18_to_v19(25));
    TEST_ASSERT_EQUAL_UINT8(100, migrateTPSResolution_v18_to_v19(50));
    TEST_ASSERT_EQUAL_UINT8(200, migrateTPSResolution_v18_to_v19(100));
}

void test_migrateTPSResolution_v18_to_v19_overflow_protection(void) {
    TEST_ASSERT_EQUAL_UINT8(254, migrateTPSResolution_v18_to_v19(127));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateTPSResolution_v18_to_v19(128));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateTPSResolution_v18_to_v19(255));
}

void test_migrateTPSResolution_v18_to_v19_boundary_values(void) {
    TEST_ASSERT_EQUAL_UINT8(254, migrateTPSResolution_v18_to_v19(127));
    TEST_ASSERT_EQUAL_UINT8(252, migrateTPSResolution_v18_to_v19(126));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, migrateTPSResolution_v18_to_v19(128));
}

void test_multiplyTableLoad_scales_only_load_axis(void) {
    seedTableAxesAndValues(fuelTable, 1000U, 10U, 1U);

    multiplyTableLoad(&fuelTable, fuelTable.type_key, 4U);

    assertAxisUnchanged(fuelTable, 1000U);
    assertAxisScaled(fuelTable, 10U, 4U);
    assertValuesUnchanged(fuelTable, 1U);
}

void test_divideTableLoad_scales_only_load_axis(void) {
    seedTableAxesAndValues(vvtTable, 2000U, 20U, 21U);

    divideTableLoad(&vvtTable, vvtTable.type_key, 2U);

    assertAxisUnchanged(vvtTable, 2000U);

    auto axis = vvtTable.axisY.begin();
    uint16_t expected = 20U;
    while (!axis.at_end()) {
        TEST_ASSERT_EQUAL_UINT16(expected / 2U, *axis);
        ++axis;
        ++expected;
    }

    assertValuesUnchanged(vvtTable, 21U);
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

void test_multiplyTableValue_scales_entire_page(void) {
    const uint16_t touched = seedMappedPageValues(wmiMapPage, 20U);

    multiplyTableValue(wmiMapPage, 2U);

    uint16_t verified = 0U;
    for (page_iterator_t it = page_begin(wmiMapPage); it.type != End; it = advance(it)) {
        for (uint16_t offset = it.start; offset < (it.start + it.size); offset++) {
            TEST_ASSERT_EQUAL_UINT8(uint8_t((offset % 20U) * 2U), getPageValue(wmiMapPage, offset));
            verified++;
        }
    }
    TEST_ASSERT_EQUAL_UINT16(touched, verified);
}

void test_divideTableValue_scales_entire_page(void) {
    const uint16_t touched = seedMappedPageValues(wmiMapPage, 25U, 4U);

    divideTableValue(wmiMapPage, 4U);

    uint16_t verified = 0U;
    for (page_iterator_t it = page_begin(wmiMapPage); it.type != End; it = advance(it)) {
        for (uint16_t offset = it.start; offset < (it.start + it.size); offset++) {
            TEST_ASSERT_EQUAL_UINT8(uint8_t(offset % 25U), getPageValue(wmiMapPage, offset));
            verified++;
        }
    }
    TEST_ASSERT_EQUAL_UINT16(touched, verified);
}

void test_doUpdates_v19_to_v20_sets_boost_lookup_defaults_and_afr_protection(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(19U);
    updatesTestSetStopAfterStore(true);

    configPage2.nCylinders = 4U;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage4.inj4cylPairing = 3U;
    configPage9.hardRevMode = 0U;
    configPage6.tachoMode = 1U;
    configPage4.CANBroadcastProtocol = CAN_BROADCAST_PROTOCOL_VAG;
    configPage15.boostDCWhenDisabled = 55U;
    configPage15.boostControlEnable = EN_BOOST_CONTROL_FIXED;
    configPage9.afrProtectEnabled = 2U;
    configPage9.afrProtectMinMAP = 0U;
    configPage9.afrProtectMinRPM = 0U;
    configPage9.afrProtectMinTPS = 0U;
    configPage9.afrProtectDeviation = 0U;
    seedTableAxesAndValues(boostTableLookupDuty, 10U, 20U, 1U);

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(20U, state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(20U, state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(1U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);

    TEST_ASSERT_EQUAL_UINT8(INJ_PAIR_13_24, configPage4.inj4cylPairing);
    TEST_ASSERT_EQUAL_UINT8(1U, configPage9.hardRevMode);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage6.tachoMode);
    TEST_ASSERT_EQUAL_UINT8(CAN_BROADCAST_PROTOCOL_OFF, configPage4.CANBroadcastProtocol);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage15.boostDCWhenDisabled);
    TEST_ASSERT_EQUAL_UINT8(EN_BOOST_CONTROL_BARO, configPage15.boostControlEnable);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage9.afrProtectEnabled);
    TEST_ASSERT_EQUAL_UINT8(90U, configPage9.afrProtectMinMAP);
    TEST_ASSERT_EQUAL_UINT8(40U, configPage9.afrProtectMinRPM);
    TEST_ASSERT_EQUAL_UINT8(160U, configPage9.afrProtectMinTPS);
    TEST_ASSERT_EQUAL_UINT8(14U, configPage9.afrProtectDeviation);

    auto x = boostTableLookupDuty.axisX.begin();
    uint16_t expectedRpm = 1500U;
    while (!x.at_end()) {
        TEST_ASSERT_EQUAL_UINT16(expectedRpm, *x);
        expectedRpm += 500U;
        ++x;
    }

    auto y = boostTableLookupDuty.axisY.begin();
    uint16_t expectedBoost = 130U;
    while (!y.at_end()) {
        TEST_ASSERT_EQUAL_UINT16(expectedBoost, *y);
        expectedBoost += 10U;
        ++y;
    }

    auto rows = boostTableLookupDuty.values.begin();
    while (!rows.at_end()) {
        auto row = *rows;
        while (!row.at_end()) {
            TEST_ASSERT_EQUAL_UINT8(100U, *row);
            ++row;
        }
        ++rows;
    }
}

void test_doUpdates_v18_to_v19_scales_tps_inputs_and_tps_based_tables(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(18U);
    updatesTestSetStopAfterStore(true);

    configPage6.fanUnused = 1U;
    configPage2.idleAdvTPS = 12U;
    configPage2.iacTPSlimit = 17U;
    configPage4.floodClear = 20U;
    configPage4.dfcoTPSThresh = 21U;
    configPage6.egoTPSMax = 22U;
    configPage10.lnchCtrlTPS = 23U;
    configPage10.wmiTPS = 24U;
    configPage10.n2o_minTPS = 25U;
    configPage10.fuel2SwitchVariable = FUEL2_CONDITION_TPS;
    configPage10.fuel2SwitchValue = 30U;
    configPage10.spark2SwitchVariable = SPARK2_CONDITION_TPS;
    configPage10.spark2SwitchValue = 31U;

    configPage2.fuelAlgorithm = LOAD_SOURCE_TPS;
    configPage2.ignAlgorithm = LOAD_SOURCE_TPS;
    configPage10.fuel2Algorithm = LOAD_SOURCE_TPS;
    configPage10.spark2Algorithm = LOAD_SOURCE_TPS;
    configPage6.vvtLoadSource = VVT_LOAD_TPS;
    configPage4.sparkMode = IGN_MODE_ROTARY;
    for (uint8_t i = 0; i < 8U; ++i) {
        configPage10.rotarySplitBins[i] = uint8_t(10U + i);
    }

    seedTableAxesAndValues(fuelTable, 1000U, 10U, 1U);
    seedTableAxesAndValues(afrTable, 1100U, 20U, 11U);
    seedTableAxesAndValues(trim1Table, 1200U, 30U, 21U);
    seedTableAxesAndValues(trim2Table, 1300U, 40U, 31U);
    seedTableAxesAndValues(trim3Table, 1400U, 50U, 41U);
    seedTableAxesAndValues(trim4Table, 1500U, 60U, 51U);
    seedTableAxesAndValues(trim5Table, 1600U, 70U, 61U);
    seedTableAxesAndValues(trim6Table, 1700U, 80U, 71U);
    seedTableAxesAndValues(trim7Table, 1800U, 90U, 81U);
    seedTableAxesAndValues(trim8Table, 1900U, 100U, 91U);
    seedTableAxesAndValues(ignitionTable, 2000U, 110U, 101U);
    seedTableAxesAndValues(fuelTable2, 2100U, 120U, 111U);
    seedTableAxesAndValues(ignitionTable2, 2200U, 130U, 121U);
    seedTableAxesAndValues(boostTable, 2300U, 140U, 131U);
    seedTableAxesAndValues(vvtTable, 2400U, 150U, 141U);
    seedTableAxesAndValues(vvt2Table, 2500U, 160U, 151U);

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(19U, state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(19U, state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(1U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);

    TEST_ASSERT_EQUAL_UINT8(1U, configPage2.fanEnable);
    TEST_ASSERT_EQUAL_UINT8(24U, configPage2.idleAdvTPS);
    TEST_ASSERT_EQUAL_UINT8(34U, configPage2.iacTPSlimit);
    TEST_ASSERT_EQUAL_UINT8(40U, configPage4.floodClear);
    TEST_ASSERT_EQUAL_UINT8(42U, configPage4.dfcoTPSThresh);
    TEST_ASSERT_EQUAL_UINT8(44U, configPage6.egoTPSMax);
    TEST_ASSERT_EQUAL_UINT8(46U, configPage10.lnchCtrlTPS);
    TEST_ASSERT_EQUAL_UINT8(48U, configPage10.wmiTPS);
    TEST_ASSERT_EQUAL_UINT8(50U, configPage10.n2o_minTPS);
    TEST_ASSERT_EQUAL_UINT16(60U, configPage10.fuel2SwitchValue);
    TEST_ASSERT_EQUAL_UINT8(SPARK2_CONDITION_TPS, configPage10.spark2SwitchVariable);
    TEST_ASSERT_EQUAL_UINT16(62U, configPage10.spark2SwitchValue);

    assertAxisScaled(fuelTable, 10U, 4U);
    assertAxisScaled(afrTable, 20U, 4U);
    assertAxisScaled(ignitionTable, 110U, 4U);
    assertAxisScaled(fuelTable2, 120U, 4U);
    assertAxisScaled(ignitionTable2, 130U, 4U);
    assertAxisScaled(boostTable, 140U, 2U);
    assertAxisScaled(vvtTable, 150U, 2U);
    assertAxisScaled(vvt2Table, 160U, 2U);
    assertValuesUnchanged(fuelTable, 1U);
    assertValuesUnchanged(ignitionTable2, 121U);

    for (uint8_t i = 0; i < 8U; ++i) {
        TEST_ASSERT_EQUAL_UINT8(uint8_t((10U + i) * 2U), configPage10.rotarySplitBins[i]);
    }
}

void test_doUpdates_v18_to_v19_divides_non_tps_vvt_tables_and_resets_logging_defaults(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(18U);
    updatesTestSetStopAfterStore(true);

    configPage6.fanUnused = 0U;
    configPage6.vvtLoadSource = VVT_LOAD_MAP;
    seedTableAxesAndValues(vvtTable, 2600U, 200U, 1U);
    seedTableAxesAndValues(vvt2Table, 2700U, 220U, 21U);

    configPage13.onboard_log_csv_separator = 1U;
    configPage13.onboard_log_file_style = 1U;
    configPage13.onboard_log_file_rate = 1U;
    configPage13.onboard_log_filenaming = 1U;
    configPage13.onboard_log_storage = 1U;
    configPage13.onboard_log_trigger_boot = 1U;
    configPage13.onboard_log_trigger_RPM = 1U;
    configPage13.onboard_log_trigger_prot = 1U;
    configPage13.onboard_log_trigger_Vbat = 1U;
    configPage13.onboard_log_trigger_Epin = 1U;
    configPage13.onboard_log_tr1_duration = 1U;
    configPage13.onboard_log_tr2_thr_on = 1U;
    configPage13.onboard_log_tr2_thr_off = 1U;
    configPage13.onboard_log_tr3_thr_RPM = 1U;
    configPage13.onboard_log_tr3_thr_MAP = 1U;
    configPage13.onboard_log_tr3_thr_Oil = 1U;
    configPage13.onboard_log_tr3_thr_AFR = 1U;
    configPage13.onboard_log_tr4_thr_on = 1U;
    configPage13.onboard_log_tr4_thr_off = 1U;
    configPage13.onboard_log_tr5_Epin_pin = 1U;

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(19U, state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(19U, state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(1U, state.writeAllConfigCalls);

    auto axis = vvtTable.axisY.begin();
    uint16_t expected = 200U;
    while (!axis.at_end()) {
        TEST_ASSERT_EQUAL_UINT16(expected / 2U, *axis);
        ++axis;
        ++expected;
    }

    auto axis2 = vvt2Table.axisY.begin();
    expected = 220U;
    while (!axis2.at_end()) {
        TEST_ASSERT_EQUAL_UINT16(expected / 2U, *axis2);
        ++axis2;
        ++expected;
    }

    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_csv_separator);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_file_style);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_file_rate);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_filenaming);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_storage);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_trigger_boot);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_trigger_RPM);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_trigger_prot);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_trigger_Vbat);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_trigger_Epin);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr1_duration);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr2_thr_on);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr2_thr_off);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr3_thr_RPM);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr3_thr_MAP);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr3_thr_Oil);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr3_thr_AFR);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr4_thr_on);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr4_thr_off);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage13.onboard_log_tr5_Epin_pin);
}

void test_doUpdates_v20_to_v21_updates_programmable_outputs_and_defaults(void) {
    resetMigrationState();
    updatesTestSetInitialVersion(20U);
    updatesTestSetStopAfterStore(true);

    configPage2.taeMinChange = 0U;
    configPage2.maeMinChange = 0U;
    configPage2.decelAmount = 0U;
    configPage10.oilPressureProtTime = 9U;
    configPage9.iacStepperPower = 1U;
    configPage15.airConEnable = 1U;

    configPage13.firstDataIn[0] = 22U;
    configPage13.firstDataIn[1] = 23U;
    configPage13.firstDataIn[2] = 92U;
    configPage13.firstDataIn[3] = 93U;
    configPage13.firstDataIn[4] = 239U;
    configPage13.firstDataIn[5] = 240U;

    configPage13.secondDataIn[0] = 22U;
    configPage13.secondDataIn[1] = 23U;
    configPage13.secondDataIn[2] = 92U;
    configPage13.secondDataIn[3] = 93U;
    configPage13.secondDataIn[4] = 239U;
    configPage13.secondDataIn[5] = 240U;

    doUpdates();

    const updates_test_state state = updatesTestGetState();

    TEST_ASSERT_EQUAL_UINT8(21U, state.eepromVersion);
    TEST_ASSERT_EQUAL_UINT8(21U, state.lastStoredVersion);
    TEST_ASSERT_EQUAL_UINT8(1U, state.storeVersionCalls);
    TEST_ASSERT_EQUAL_UINT8(1U, state.writeAllConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.loadConfigCalls);
    TEST_ASSERT_EQUAL_UINT8(0U, state.writeCalibrationCalls);

    TEST_ASSERT_EQUAL_UINT8(4U, configPage2.taeMinChange);
    TEST_ASSERT_EQUAL_UINT8(2U, configPage2.maeMinChange);
    TEST_ASSERT_EQUAL_UINT8(100U, configPage2.decelAmount);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage10.oilPressureProtTime);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage9.iacStepperPower);
    TEST_ASSERT_EQUAL_UINT8(0U, configPage15.airConEnable);

    TEST_ASSERT_EQUAL_UINT8(22U, configPage13.firstDataIn[0]);
    TEST_ASSERT_EQUAL_UINT8(24U, configPage13.firstDataIn[1]);
    TEST_ASSERT_EQUAL_UINT8(94U, configPage13.firstDataIn[2]);
    TEST_ASSERT_EQUAL_UINT8(95U, configPage13.firstDataIn[3]);
    TEST_ASSERT_EQUAL_UINT8(240U, configPage13.firstDataIn[4]);
    TEST_ASSERT_EQUAL_UINT8(240U, configPage13.firstDataIn[5]);

    TEST_ASSERT_EQUAL_UINT8(22U, configPage13.secondDataIn[0]);
    TEST_ASSERT_EQUAL_UINT8(24U, configPage13.secondDataIn[1]);
    TEST_ASSERT_EQUAL_UINT8(94U, configPage13.secondDataIn[2]);
    TEST_ASSERT_EQUAL_UINT8(95U, configPage13.secondDataIn[3]);
    TEST_ASSERT_EQUAL_UINT8(240U, configPage13.secondDataIn[4]);
    TEST_ASSERT_EQUAL_UINT8(240U, configPage13.secondDataIn[5]);
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

void testConfigMigrations(void) {
    RUN_TEST(test_migratePIDGain_v13_to_v14_normal_range);
    RUN_TEST(test_migratePIDGain_v13_to_v14_overflow_protection);
    RUN_TEST(test_migratePIDGain_v13_to_v14_boundary_values);
    RUN_TEST(test_migrateCrankingEnrich_v13_to_v14_normal_range);
    RUN_TEST(test_migrateCrankingEnrich_v13_to_v14_division_rounding);
    RUN_TEST(test_migrateFlexAdvance_v13_to_v14_normal_range);
    RUN_TEST(test_migrateFlexAdvance_v13_to_v14_overflow_protection);
    RUN_TEST(test_migrateFlexAdvance_v13_to_v14_boundary_values);
    RUN_TEST(test_migrateCrankingPctToCurve_v4_to_v5_normal_range);
    RUN_TEST(test_migrateCrankingPctToCurve_v4_to_v5_overflow_protection);
    RUN_TEST(test_migrateCrankingPctToCurve_v4_to_v5_boundary_values);
    RUN_TEST(test_migrateTPSResolution_v18_to_v19_normal_range);
    RUN_TEST(test_migrateTPSResolution_v18_to_v19_overflow_protection);
    RUN_TEST(test_migrateTPSResolution_v18_to_v19_boundary_values);
    RUN_TEST(test_multiplyTableLoad_scales_only_load_axis);
    RUN_TEST(test_divideTableLoad_scales_only_load_axis);
    RUN_TEST(test_multiplyTableValue_scales_entire_page);
    RUN_TEST(test_divideTableValue_scales_entire_page);
    RUN_TEST(test_doUpdates_v18_to_v19_scales_tps_inputs_and_tps_based_tables);
    RUN_TEST(test_doUpdates_v18_to_v19_divides_non_tps_vvt_tables_and_resets_logging_defaults);
    RUN_TEST(test_doUpdates_v19_to_v20_sets_boost_lookup_defaults_and_afr_protection);
    RUN_TEST(test_doUpdates_v20_to_v21_updates_programmable_outputs_and_defaults);
    RUN_TEST(test_doUpdates_v21_to_v22_sets_rolling_cut_defaults_and_halves_dfco_hysteresis);
    RUN_TEST(test_doUpdates_v22_to_v23_scales_wmi_map_and_sets_new_defaults);
    RUN_TEST(test_doUpdates_v23_to_v24_migrates_can_knock_and_page10_alignment);
    RUN_TEST(test_doUpdates_new_eeprom_initializes_defaults_and_current_version);
    RUN_TEST(test_doUpdates_future_version_clamps_to_current);
}
