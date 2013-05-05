#include "./unity/unity.h"
#include "unit-test-1.h"
#include "unit-test-2.h"

#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);extern void tearDown(void);

static void runTest(UnityTestFunction test)
{
  if (TEST_PROTECT())
  {
      setUp();
      test();
  }
  if (TEST_PROTECT() && !TEST_IS_IGNORED)
  {
    tearDown();
  }
}
void resetTest()
{
  tearDown();
  setUp();
}


void testMain(void)
{
  Unity.TestFile = "Hypervisor.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_initialState0, 0);
  RUN_TEST(test_initialState1, 0);
 // RUN_TEST(test_initialState2, 0);
 // RUN_TEST(test_initialState3, 0);
 // RUN_TEST(test_initialState4, 0);
 // RUN_TEST(test_initialState5, 0);
 // RUN_TEST(test_initialState6, 0);

  //RUN_TEST(test_stateTransition0, 0);
  //RUN_TEST(test_stateTransition1, 0);
  //RUN_TEST(test_stateTransition2, 0);
  //RUN_TEST(test_stateTransition3, 0);
  //RUN_TEST(test_stateTransition4, 0);
  //RUN_TEST(test_stateTransition5, 0);
  //RUN_TEST(test_stateTransition6, 0);
  //RUN_TEST(test_stateTransition7, 0);

  UnityEnd();
  //return 0;
}

