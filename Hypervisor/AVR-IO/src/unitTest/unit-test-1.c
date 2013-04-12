#include "./unity/unity.h"
#include "unit-test-1.h"
#include "StateFunctions.h"
#include "HyperVisor.h"

unsigned char State;
 
void setUp(void)
{
}

void tearDown(void)
{
}

void test_initialState0(void)
{
  //All of these should pass
  State = getInitialState();
  TEST_ASSERT_EQUAL(POWEROFF,State);
}

void test_initialState1 (void)
{
    State = Ubuntu_State[State]();
    TEST_ASSERT_EQUAL(POWEROFF,State);
}

void test_initialState2(void)
{
    State = Ubuntu_State[BOOTING]();
    TEST_ASSERT_EQUAL(BOOTING,State);
}

void test_initialState3 (void)
{
    State = Ubuntu_State[RUN]();
    TEST_ASSERT_EQUAL(RUN,State);
}

void test_initialState4(void)
{
    State = Ubuntu_State[SUSPEND]();
    TEST_ASSERT_EQUAL(SUSPEND,State);
}

void test_initialState5(void)
{
    //setPowerStatus(ON);
    State = Ubuntu_State[BEFOREHIBERNATE]();
    TEST_ASSERT_EQUAL(BEFOREHIBERNATE,State);
    //setPowerStatus(OFF);
}

void test_initialState6 (void)
{
    State = Ubuntu_State[HIBERNATE]();
    TEST_ASSERT_EQUAL(HIBERNATE,State);
}

