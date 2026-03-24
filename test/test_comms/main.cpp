/**
 * @file main.cpp
 * @brief Test harness entry point for comms integration tests
 */

#include <unity.h>

// Forward declarations of test suites
extern void test_setup();

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  test_setup();
  UNITY_END();

  return 0;
}
