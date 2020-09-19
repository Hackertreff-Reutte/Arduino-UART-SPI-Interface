#ifndef UNITTEST_UART_INTERCEPTOR_H
#define UNITTEST_UART_INTERCEPTOR_H

//change this add some point (testing without Arduino.h dependency)
#include <Arduino.h>

void uinittest_logError(String category, uint8_t id);
String uinittest_getLastErrorCategory();
uint8_t uinittest_getLastErrorID();
void uinittest_resetErrorUpdatedStatus();
bool uinittest_wasErrorUpdated();
uint8_t uinittest_getErrorUpdatedCount();

#endif