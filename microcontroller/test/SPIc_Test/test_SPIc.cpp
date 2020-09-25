/*
        UART-SPI Interface to easily debug or script SPI devices with a PC / UART-Interface. 
        Copyright (C) 2020  Alex Hold (UnHold)
        Copyright (C) 2020  Alina Schärmer (tabaluga34 / ina06)

        This program is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


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


void test_SPI_InitializedChecks(){

    //this function will the the spi initialized check with the help 
    //of the SPIsetup() function

    if(SIZE_OF_SPICONTROLLER_ARRAY < 3){
       TEST_FAIL_MESSAGE("SIZE_OF_SPICONTROLLER_ARRAY have to be 3 or bigger");
    }

    //make sure the spi 2 is stopped
    stopSpi(2);
    uinittest_resetErrorUpdatedStatus();

    //TODO check if this works
    //should work (no error)
    setupSPI(2,0,0,0,0,0,0);
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    setupSPI(2,0,0,0,0,0,0);
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_ALREADY_INITIALIZED_ERROR, uinittest_getLastErrorID());

}


void test_SPI_Slave_InitializedChecks(){

    //this function will the the spi slave initialized check with the help 
    //of the SPIsetup() and setupSlave() and transfer() functions

    if(SIZE_OF_SPICONTROLLER_ARRAY < 3){
       TEST_FAIL_MESSAGE("SIZE_OF_SPICONTROLLER_ARRAY have to be 3 or bigger");
    }

    if(NUMBER_OF_SLAVES_PER_SPI < 3){
       TEST_FAIL_MESSAGE("NUMBER_OF_SLAVES_PER_SPI have to be 3 or bigger");
    }

     //make sure the spi 2 is stopped
    stopSpi(2);
    uinittest_resetErrorUpdatedStatus();
    setupSPI(2,0,0,0,0,0,0);
    transfer(2,2,8,0);
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_SLAVE_NOT_INITIALIZED, uinittest_getLastErrorID());
    uinittest_resetErrorUpdatedStatus();
    setupSlave(2,2,0);
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    transfer(2,2,8,0);
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    setupSlave(2,2,0);
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_SLAVE_ALREADY_INITIALIZED, uinittest_getLastErrorID());

    stopSpi(1);
    uinittest_resetErrorUpdatedStatus();

    TEST_ASSERT_TRUE(isSPI_notInitialized(1));
    TEST_ASSERT_TRUE(isSPI_Initialized(2));
    TEST_ASSERT_TRUE(isSPI_Slave_notInitialized(2,0));
    TEST_ASSERT_TRUE(isSPI_Slave_Initialized(2,2));

}


void test_SPI_transmittingChecks(){

    //this function will the the spi transfer check with the help 
    //of the SPIsetup(), setupSlave(), startSpiBulkTransfer(), stopSpiBulkTransfer(), transfer functions

    if(SIZE_OF_SPICONTROLLER_ARRAY < 3){
       TEST_FAIL_MESSAGE("SIZE_OF_SPICONTROLLER_ARRAY have to be 3 or bigger");
    }

    if(NUMBER_OF_SLAVES_PER_SPI < 3){
       TEST_FAIL_MESSAGE("NUMBER_OF_SLAVES_PER_SPI have to be 3 or bigger");
    }

    setupSPI(2,0,0,0,0,0,0);
    setupSlave(2,2,0);
    stopSpiBulkTransfer(2);
    uinittest_resetErrorUpdatedStatus();

    TEST_ASSERT_TRUE(isSPI_notTransmitting(2));
    startSpiBulkTransfer(2,2);
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    TEST_ASSERT_TRUE(isSPI_Transmitting(2));
    startSpiBulkTransfer(2,2);
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_IS_IN_TRANSACTION_ERROR, uinittest_getLastErrorID());

    uinittest_resetErrorUpdatedStatus();
    transfer(2,2,8,0);
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_IS_IN_TRANSACTION_ERROR, uinittest_getLastErrorID());

    uinittest_resetErrorUpdatedStatus();

    bulkTransfer(2,0);
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());

    stopSpiBulkTransfer(2);
    TEST_ASSERT_FALSE(uinittest_wasErrorUpdated());
    stopSpiBulkTransfer(2);
    TEST_ASSERT_TRUE(uinittest_wasErrorUpdated());
    TEST_ASSERT_EQUAL_STRING(SPI_ERROR, uinittest_getLastErrorCategory().c_str());
    TEST_ASSERT_EQUAL_INT8(SPI_NO_ACTIVE_TRANSACTION_ERROR, uinittest_getLastErrorID());
}



void setup(){
    delay(2000);
    UNITY_BEGIN();    // IMPORTANT LINE!

    RUN_TEST(test_SPI_BoundsAndBitcountChecks);
    RUN_TEST(test_SPI_InitializedChecks);
    RUN_TEST(test_SPI_Slave_InitializedChecks);
    RUN_TEST(test_SPI_transmittingChecks);
    
    //TODO: add more checks if necessary

    UNITY_END(); // stop unit testing
}


void loop(){

}