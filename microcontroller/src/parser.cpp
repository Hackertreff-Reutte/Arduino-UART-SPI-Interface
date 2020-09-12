//this ifndef is just here for a script to create a single main file (used by the arduino ide) and to not include non-existend files 
#ifndef ARDUINO_IDE
#include "parser.h"
#else
#include <Arduino.h>
#endif


//--------------Serial Communication Parser-----------------------
// this function defines what is considered a whitespace
bool isSpace(char c) {
 	return (c == ' ' || c == '\n' || c == '\r' ||
		   c == '\t' || c == '\v' || c == '\f');
}

// this function removes all whitespaces in front of another charecter
String trimWhitespaces(String data){
   String temp = data;
 
   if(!temp.length() == 0){
     for (int i = 0; i < temp.length(); i++)
     {
       if(!isSpace(temp.charAt(0))){
         return "";
       }
       else{
         temp.remove(0);
       }
     }
   }
   return temp;
}

// this function splits the data by the given separator and returns it using a String array
String* getCommandInfo(String data, char separator)
{
    String info[10]; // changes here to max number of seperated fields that are allowed

    int section = 0;
    int lastSeperator = 0;
    for (int i = 0; i <= data.length(); i++) {
        if (data.charAt(i) == separator) {
            info[section] = data.substring(lastSeperator + 1, i);
            lastSeperator = i;
        }
    }
    return info;
}


//will try to get the command from the data string if any is present
void getCommand(String data){

  // remove whitespaces infront of other characters 
  String preprocessed = trimWhitespaces(data);
  
  if(preprocessed.length() != 0){
    // check if first character is a C otherwise data is no command
    if(preprocessed.charAt(0) == 'C'){ 
      // split command
      String * commandinfo = getCommandInfo(preprocessed, ':');

      // parse according to command type
      if(commandinfo[1] == "TX"){
        if(commandinfo[5] != NULL){
          uint32_t spi_id = commandinfo[2].toInt();
          uint32_t slave_id = commandinfo[3].toInt();
          uint32_t bitcount = commandinfo[4].toInt();
          uint32_t txdata = commandinfo[5].toInt();

          if(txdata == bitcount){
            transfer(spi_id, slave_id, bitcount, txdata);
          }
          else{
            // sendError("parser", "P3", "Bitcount does not match data length" );
          }
        }
        else{
          // sendError("parser", "P2", "Data does not contain enough fields for valid TX command");
        }
      }
      else if("SE"){
        if(commandinfo[2] == "SPI"){
          if(commandinfo[9] != NULL){
            uint32_t spi_id = commandinfo[3].toInt();
            uint32_t miso = commandinfo[4].toInt();
            uint32_t mosi = commandinfo[5].toInt();
            uint32_t clock = commandinfo[6].toInt();
            uint32_t speed = commandinfo[7].toInt();
            uint32_t bitorder = commandinfo[8].toInt();
            uint32_t mode = commandinfo[9].toInt();
            setupSPI(spi_id, miso, mosi, clock, speed, bitorder, mode);
          }
          else{
            // sendError("parser", "P2, "Data does not contain enough fields for valid SE:SPI command");
          }
        }
        else if(commandinfo[2] == "MODE"){
          if(commandinfo[4] != NULL){
            uint32_t spi_id = commandinfo[3].toInt();
            uint32_t mode = commandinfo[4].toInt();
            updateSpiMode(spi_id, mode);
          }
          else{
            // sendError("parser", "P2", "Data does not contain enough fields for valid SE:MODE command");
          }
        }
        else if(commandinfo[2] == "SPEED"){
          if(commandinfo[4] != NULL){
            uint32_t spi_id = commandinfo[3].toInt();
            uint32_t speed = commandinfo[4].toInt();
            updateSpiSpeed(spi_id, speed);
          }
          else{
            // sendError("parser", "P2", "Data does not contain enough fields for valid SE:SPEED command");
          }
        }
        else if(commandinfo[2] == "SLAVE"){
          if(commandinfo[5] != NULL){
            uint32_t spi_id = commandinfo[3].toInt();
            uint32_t slave_id = commandinfo[4].toInt();
            uint32_t pin = commandinfo[5].toInt();
            setupSlave(spi_id, slave_id, pin);
          }
          else{
            // sendError("parser", "P2", "Data does not contain enough fields for valid SE:SLAVE command");
          }
        }
        else{
        // sendError("parser", "P4", "Invalid setup command");
        }
      }
      else if("ST"){
      Serial.write("Call st function");
      }
      else if("BS"){
      Serial.write("Call bs function");
      }
      else if("BE"){
      Serial.write("Call be function");
      }
      else if("BT"){
      Serial.write("Call bt function");
      }
    }
    else{
      // no valid command
      //sendError("parser", "P1", "Data is not a valid command");
    }
  }
  else{
    //sendError("parser", "P0", "Empty data");
  }
  
}