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


//this ifndef is just here for a script to create a single main file (used by the arduino ide) and to not include non-existend files 
#ifndef ARDUINO_IDE
#include "parser.h"
#else
#include <Arduino.h>
#endif

//--------------Serial Communication Parser-----------------------
// this function splits the data by the given separator and returns it using a String array
void getCommandInfo(String dataIn, char separator, String dataOut[])
{
    int section = 0;
    int lastSeperator = 0;
    for (int i = 0; i <= dataIn.length(); i++) {
        if (dataIn.charAt(i) == separator) {
            dataOut[section] = dataIn.substring(lastSeperator + 1, i);
            lastSeperator = i;
            section++;
        }
    }
    dataOut[section] = (lastSeperator + 1, dataIn.length());
}

//will try to get the command from the data string if any is present
void getCommand(String data){
  
  if(data.length() != 0){
    // check if first character is a C otherwise data is no command
    if(data.charAt(0) == 'C'){ 
      // split command
      String commandinfo[10]; // change here to max number of fields possible
      getCommandInfo(data, ':', commandinfo);

      // parse according to command type
      if(commandinfo[1] == "TX"){
        if(commandinfo[5] != NULL){
          uint8_t spi_id = commandinfo[2].toInt();
          uint8_t slave_id = commandinfo[3].toInt();
          uint8_t bitcount = commandinfo[4].toInt();
          uint32_t txdata = commandinfo[5].toInt();

          
          uint32_t response = transfer(spi_id, slave_id, bitcount, txdata); 
          sendRespone(spi_id, slave_id, bitcount, response, DEC);
          
        }
        else{
            // not enough fields for transfer command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
        }
      }
      else if(commandinfo[1] == "SE"){
        if(commandinfo[2] == "SPI"){
          if(commandinfo[9] != NULL){
            uint8_t spi_id = commandinfo[3].toInt();
            uint8_t miso = commandinfo[4].toInt();
            uint8_t mosi = commandinfo[5].toInt();
            uint8_t clock = commandinfo[6].toInt();
            uint32_t speed = commandinfo[7].toInt();
            uint8_t bitorder = commandinfo[8].toInt();
            uint8_t mode = commandinfo[9].toInt();
            setupSPI(spi_id, miso, mosi, clock, speed, bitorder, mode);
          }
          else{
            // not enough fields for setup spi command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
        }
        else if(commandinfo[2] == "MODE"){
          if(commandinfo[4] != NULL){
            uint8_t spi_id = commandinfo[3].toInt();
            uint8_t mode = commandinfo[4].toInt();
            updateSpiMode(spi_id, mode);
          }
          else{
            // not enough fields for setup mode command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
        }
        else if(commandinfo[2] == "SPEED"){
          if(commandinfo[4] != NULL){
            uint8_t spi_id = commandinfo[3].toInt();
            uint32_t speed = commandinfo[4].toInt();
            updateSpiSpeed(spi_id, speed);
          }
          else{
            // not enough fields for setup speed command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
        }
        else if(commandinfo[2] == "SLAVE"){
          if(commandinfo[5] != NULL){
            uint8_t spi_id = commandinfo[3].toInt();
            uint8_t slave_id = commandinfo[4].toInt();
            uint8_t pin = commandinfo[5].toInt();
            setupSlave(spi_id, slave_id, pin);
          }
          else{
            // not enough fields for setup speed command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
        }
        else{
            // not a valid setup command type
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_INVALID_SETUP_COMMAND_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_INVALID_SETUP_COMMAND_ERROR, PARSER_INVALID_COMMAND_ERROR);
            #endif
        }
      }
      else if(commandinfo[1] == "ST"){
        if(commandinfo[2] != NULL){
          uint8_t spi_id = commandinfo[2].toInt();
          stopSpi(spi_id);
        }
        else{
            // not enough fields for spi stop command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
      }
      else if(commandinfo[1] == "BS"){
        if(commandinfo[3] != NULL){
          uint8_t spi_id = commandinfo[2].toInt();
          uint8_t slave_id = commandinfo[3].toInt();
          startSpiBulkTransfer(spi_id, slave_id);
        }
        else{
            // not enough fields for burst transfer start command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
      }
      else if(commandinfo[1] == "BE"){
        if(commandinfo[2] != NULL){
          uint8_t spi_id = commandinfo[2].toInt();
          stopSpiBulkTransfer(spi_id);
        }
        else{
            // not enough fields for burst transfer start command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
      }
      else if(commandinfo[1] == "BT"){
        if(commandinfo[3] != NULL){
          uint8_t spi_id = commandinfo[2].toInt();
          uint8_t data = commandinfo[3].toInt();
          bulkTransfer(spi_id, data);
        }
        else{
            // not enough fields for burst transfer start command
            #ifndef DEBUG
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR);
            #else
              sendError(PARSER_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR, PARSER_COMMAND_FIELDS_MISSING_ERROR_TEXT);
            #endif
          }
      }
    }
    else{
      // no valid command type
      #ifndef DEBUG
        sendError(PARSER_ERROR, PARSER_INVALID_COMMAND_TYPE_ERROR);
      #else
        sendError(PARSER_ERROR, PARSER_INVALID_COMMAND_TYPE_ERROR, PARSER_INVALID_COMMAND_ERROR);
      #endif
    }
  }
  else{
    // data string empty
    #ifndef DEBUG
      sendError(PARSER_ERROR, PARSER_EMPTY_DATA_ERROR);
    #else
      sendError(PARSER_ERROR, PARSER_EMPTY_DATA_ERROR, PARSER_EMPTY_DATA_ERROR_TEXT);
    #endif
  }
  
}