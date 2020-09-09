#include <Arduino.h>

#ifndef ARDUINO_IDE
#include "SPIc.h"
#endif

//----------------Serial Communication Functions-------------------

//this functions will send an information message
void sendInformation(String info) {
  Serial.println("I:" + info);
}


//this functions will send a warning message
void sendWarning(String text) {
  Serial.println("W:" + text);
}


//this function will send an error message with a given category and id
//for the categories and ids please look at the doc
void  sendError(uint8_t category, uint8_t id, String text) {
  Serial.println("E:" + String(category, DEC) + ":" + String(id, DEC) + ":" + text);
}

//this function is used to send back the response you get from the spi controller / slave
void sendRespone(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, String data){
  Serial.println("R:" + String(spi_id, DEC) + ":" + String(slave_id, DEC) + ":" + String(bitcount, DEC) + ":" + data);
}

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
  
  // check if first character is a C otherwise data is no command
  if(preprocessed.charAt(0) == 'C'){  
      }
  else{
    // no valid command
    //sendError("parser", idfornovalidcommand, "Data is not a valid command" )
  }
}

void setup() {

  //init the serial communication
  Serial.begin(115200);

  //wait until the serial communication is ready (tty connected)
  while (!Serial)
  {
    delay(1000);
  }

}

void loop() {
  
}