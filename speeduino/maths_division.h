#ifndef MATHS_DIVISION_H
#define MATHS_DIVISION_H

#include <stdint.h>

#if defined(CORE_AVR) || defined(ARDUINO_ARCH_AVR)

static inline bool udiv_is16bit_result(uint32_t dividend, uint16_t divisor) {
  return divisor > (uint16_t)(dividend >> 16U);
}

#endif

/**
 * @brief Optimised division: uint32_t/uint16_t => uint16_t
 *
 * Optimised division of unsigned 32-bit by unsigned 16-bit when it is known
 * that the result fits into unsigned 16-bit.
 *
 * ~60% quicker than raw 32/32 => 32 division on ATMega
 *
 * @note Bad things will likely happen if the result doesn't fit into 16-bits.
 * @note Copied from https://stackoverflow.com/a/66593564
 *
 * @param dividend The dividend (numerator)
 * @param divisor The divisor (denominator)
 * @return uint16_t
 */
static inline uint16_t udiv_32_16(uint32_t dividend, uint16_t divisor)
{
#if defined(CORE_AVR) || defined(ARDUINO_ARCH_AVR)

    if (divisor == 0U || !udiv_is16bit_result(dividend, divisor)) { return UINT16_MAX; }

    #define INDEX_REG "r16"

    asm(
        "    ldi " INDEX_REG ", 16 ; bits = 16\n\t"
        "0:\n\t"
        "    lsl  %A0     ; shift\n\t"
        "    rol  %B0     ;  rem:quot\n\t"
        "    rol  %C0     ;   left\n\t"
        "    rol  %D0     ;    by 1\n\t"
        "    brcs 1f     ; if carry out, rem > divisor\n\t"
        "    cp   %C0, %A1 ; is rem less\n\t"
        "    cpc  %D0, %B1 ;  than divisor ?\n\t"
        "    brcs 2f     ; yes, when carry out\n\t"
        "1:\n\t"
        "    sub  %C0, %A1 ; compute\n\t"
        "    sbc  %D0, %B1 ;  rem -= divisor\n\t"
        "    ori  %A0, 1  ; record quotient bit as 1\n\t"
        "2:\n\t"
        "    dec  " INDEX_REG "     ; bits--\n\t"
        "    brne 0b     ; until bits == 0"
        : "=d" (dividend)
        : "d" (divisor), "0" (dividend)
        : INDEX_REG
    );

    // Lower word contains the quotient, upper word contains the remainder.
    return dividend & 0xFFFFU;
#else
    // The non-AVR platforms are all fast enough (or have built in hardware dividers)
    // so just fall back to regular 32-bit division.
    return dividend / divisor;
#endif
}

/**
 * @brief Same as udiv_32_16(), except this will round to nearest integer
 * instead of truncating.
 *
 * Minor performance drop compared to non-rounding version.
 **/
static inline uint16_t udiv_32_16_closest(uint32_t dividend, uint16_t divisor)
{
#if defined(CORE_AVR) || defined(ARDUINO_ARCH_AVR)
    dividend = dividend + (uint32_t)(divisor >> 1U);
    return udiv_32_16(dividend, divisor);
#else
    return (uint16_t)((dividend + (uint32_t)(divisor >> 1U)) / divisor);
#endif
}

#endif
