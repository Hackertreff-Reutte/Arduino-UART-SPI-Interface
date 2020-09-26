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

#include <Arduino.h>

//put here the custom headerfiles (not arduino standard / own header files)
#ifndef ARDUINO_IDE
#include "SPIc.h"
#include "UART.h"
#include "parser.h"
#endif




void pullCommands(){
        String data = pullUART();
        if(data.equalsIgnoreCase("")){
                //no new data was found
                return;
        }else{
                //this will parse the command
                getCommand(data);
        }
}


//this is an importanted guard for the unit testing framework (otherwise multiple definitions)
#ifndef UNIT_TEST 

void setup() {

        uartSetup();

}

void loop() {

        delay(100);
        pullCommands();
  
}

#endif