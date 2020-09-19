
#include "unittest_UART_interceptor.h"


String lastErrorCategory = "";
uint8_t lastErrorID = -1;
boolean errorUpdatedStatus = false;
uint8_t updatedCount = 0;

void uinittest_logError(String category, uint8_t id){
    lastErrorCategory = category;
    lastErrorID = id;
    errorUpdatedStatus = true;
    updatedCount++;
}

String uinittest_getLastErrorCategory(){
    return lastErrorCategory;
}


uint8_t uinittest_getLastErrorID(){
    return lastErrorID;
}

void uinittest_resetErrorUpdatedStatus(){
    errorUpdatedStatus = false;
    updatedCount = 0;
}

bool uinittest_wasErrorUpdated(){
    return errorUpdatedStatus;
}

uint8_t uinittest_getErrorUpdatedCount(){
    return updatedCount;
}