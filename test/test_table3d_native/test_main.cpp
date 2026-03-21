#include <unity.h>
#include "table3d_interpolate.cpp"
#include "..\test_tables\tests_tables.cpp"

void setUp(void) {}

void tearDown(void) {}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  
  testTables();
  RUN_TEST(test_all_incrementing);

  UNITY_END();

    return 0;
}
