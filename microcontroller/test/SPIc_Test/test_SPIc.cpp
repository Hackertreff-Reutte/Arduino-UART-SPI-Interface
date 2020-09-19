

#include "unity.h"
#include "unittest_UART_interceptor.h"
#include "errorcodes.h"
#include "Arduino.h"
#include "SPIc.h"


//helpful: https://github.com/ThrowTheSwitch/Unity#unity-test-api 
// https://docs.platformio.org/en/latest/plus/unit-testing.html


//for testing error messages 
// 1) reset the error status -> uinittest_resetErrorUpdatedStatus();
// 2) call the function that will produce the error
// 3) check if a error was created -> uinittest_wasErrorUpdated()  ex. TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
// 4) check if one or more errors where created  -> uinittest_getErrorUpdatedCount() ex. (if only one) TEST_ASSERT_EQUAL_INT8(1, uinittest_getErrorUpdatedCount());
// 5) check if it is the right category -> uinittest_getLastErrorCategory()  ex. TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
// 6) check if it is the right id -> uinittest_getLastErrorID() ex. TEST_ASSERT_EQUAL_INT8(SPI_ID_OUT_OF_BOUNDS_ERROR, uinittest_getLastErrorID());
// bonus: you can append _MESSAGE to any of the macros to make them take an additional argument. This argument is a string that will be printed at the end of the failure strings. This is useful for specifying more information about the problem.
// (this helps to clarify where the error occured)
 



void test_SPI_BoundsAndBitcountChecks(){
    //is SPI ID bound checks
    uinittest_resetErrorUpdatedStatus();
    TEST_ASSERT_TRUE(isSPI_ID_inBounds(SIZE_OF_SPICONTROLLER_ARRAY - 1));
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    TEST_ASSERT_FALSE(isSPI_ID_inBounds(SIZE_OF_SPICONTROLLER_ARRAY)); //error should be produced
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_INT8(1, uinittest_getErrorUpdatedCount()); //only one error should be here
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_ID_OUT_OF_BOUNDS_ERROR, uinittest_getLastErrorID());


    //is SPI SLAVE ID bound checks
    uinittest_resetErrorUpdatedStatus();
    TEST_ASSERT_TRUE(isSPI_Slave_inBounds(SIZE_OF_SPICONTROLLER_ARRAY - 1, NUMBER_OF_SLAVES_PER_SPI - 1));
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    TEST_ASSERT_FALSE(isSPI_Slave_inBounds(SIZE_OF_SPICONTROLLER_ARRAY - 1, NUMBER_OF_SLAVES_PER_SPI)); //error should be produced
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_INT8(1, uinittest_getErrorUpdatedCount()); //only one error should be here
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_SLAVE_ID_OUT_OF_BOUNDS_ERROR, uinittest_getLastErrorID());

    //is SPI SLAVE ID bound checks
    uinittest_resetErrorUpdatedStatus();
    TEST_ASSERT_TRUE(isValidBitCount(8));
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    TEST_ASSERT_FALSE(isValidBitCount(7)); //error should be produced
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_INT8(1, uinittest_getErrorUpdatedCount()); //only one error should be here
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_INVALID_BITCOUNT_ERROR, uinittest_getLastErrorID());

    
}




void setup(){
    delay(2000);
    UNITY_BEGIN();    // IMPORTANT LINE!

    RUN_TEST(test_SPI_BoundsAndBitcountChecks);
    

    UNITY_END(); // stop unit testing
}


void loop(){

}