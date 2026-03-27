#pragma once
#include <Arduino.h>
#include "pages.h"

/*
 * Calculates and returns the CRC32 value of a given page of memory
 */
uint32_t calculatePageCRC32(byte pageNum /**< [in] The page number to compute CRC for. */);
uint32_t calculatePageCRC32ForMode(byte pageNum, ts_page_serialization_mode mode);
