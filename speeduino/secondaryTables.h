#pragma once

#include "table3d_typedefs.h"

table3d_value_t calculateSecondaryFuel(table3d_value_t ve1Runtime);
void calculateSecondarySpark(void);
byte getVE2(void);
table3d_value_t getVE2Runtime(void);
byte getAdvance2(void);
