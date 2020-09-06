#include <Arduino.h>
#include <SPI.h>

#ifndef ARDUINO_IDE
#include "SPIc.h"
#endif

struct slave {
  boolean initialized = false;
  uint8_t id;
  uint8_t pin;
};

struct spicontroller {
  boolean initialized = false;
  uint8_t mosi;
  uint8_t miso;
  uint8_t clock;
  uint32_t speed;
  uint8_t bitorder;
  uint8_t mode; 
  SPIClass spi;
  boolean transmitting = false;
  uint8_t activeSlave = -1;
  //all the slaves that are associated with the spi controller.
  slave slaves[8];
};


//array where all the spicontrollers are saved
spicontroller spicontrollers [4];


void setupSPI(uint8_t spi_id, uint8_t miso, uint8_t mosi, uint8_t clock,  uint32_t speed, uint8_t bitorder, uint8_t mode){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return;
  }


  //check if the spi is already initialized
  if(spicontrollers[spi_id].initialized == true){
    //TODO send error message 
    return;
  }


  //ifdef to run diffrent code depending on the platform 
  #ifdef TARGET_ESP32
    //init the spi for the esp (2 SPIs)
    spicontrollers[spi_id].spi = SPIClass(spi_id);
    spicontrollers[spi_id].spi.begin(clock, miso, mosi, -1);
  #else
    //init the spi for arduino platforms
    spicontrollers[spi_id].spi = SPI;
    spicontrollers[spi_id].spi.begin();
  #endif

  //update the data of the spicontroller struct 
  spicontrollers[spi_id].miso = miso;
  spicontrollers[spi_id].mosi = mosi;
  spicontrollers[spi_id].clock = clock;
  spicontrollers[spi_id].speed = speed;
  spicontrollers[spi_id].bitorder = bitorder;
  spicontrollers[spi_id].mode = mode;
  spicontrollers[spi_id].initialized = true;

}

void setupSlave(uint8_t spi_id, uint8_t slave_id, uint8_t pin){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return;
  }

  //slave_id bounds check
  if(slave_id >= sizeof(spicontrollers[spi_id].slaves) / sizeof(spicontrollers[spi_id].slaves[0])){
    //TODO send error slave_id out of bounds
    return;
  }

  if(spicontrollers[spi_id].initialized == false){
    //TODO return error spi not inited
    return;
  }

  if(spicontrollers[spi_id].slaves[slave_id].initialized == true){
    //TODO return error slave already inited
    return;
  }


  pinMode(pin, OUTPUT);

  //set the pin high so that the slave is not selected
  digitalWrite(pin, HIGH);

  spicontrollers[spi_id].slaves[slave_id].id = slave_id;
  spicontrollers[spi_id].slaves[slave_id].pin = pin;
  spicontrollers[spi_id].slaves[slave_id].initialized = true;
  
}


void updateSpiMode(uint8_t spi_id, uint8_t mode) {

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return;
  }

  //check wether the spi is in a transaction or not
  if(spicontrollers[spi_id].transmitting == true){
    //TODO send error can't change mode while transmitting / in transaction
    return;
  }

  spicontrollers[spi_id].mode = mode;

}


void updateSpiSpeed(uint8_t spi_id, uint32_t speed){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return;
  }

  //check wether the spi is in a transaction or not
  if(spicontrollers[spi_id].transmitting == true){
    //TODO send error can't change speed while transmitting / in transaction
    return;
  }

  spicontrollers[spi_id].speed = speed;
}


//NOTE: if the spi bus is stopped it keeps the config (config will not be reset)
void stopSpi(uint8_t spi_id){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return;
  }  

  if(spicontrollers[spi_id].initialized == false){
    //TODO return error spi already stopped
    return;
  }

  //check wether the spi is in a transaction or not
  if(spicontrollers[spi_id].transmitting == true){
    //TODO send error can't stop spi while transmitting / in transaction
    return;
  }


  spicontrollers[spi_id].initialized = false;
  spicontrollers[spi_id].spi.end();

}


uint32_t transfer(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, uint32_t data){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return 0;
  }

  //slave_id bounds check
  if(slave_id >= sizeof(spicontrollers[spi_id].slaves) / sizeof(spicontrollers[spi_id].slaves[0])){
    //TODO send error slave_id out of bounds
    return 0;
  }

  //check if the spi controller is initialized
  if(spicontrollers[spi_id].initialized == false){
    //TODO send error spi not inited
    return 0;
  }

  //check if the slave is initialized
  if(spicontrollers[spi_id].slaves[slave_id].initialized == false){
    //TODO send error slave not ready
    return 0;
  }


  //check wether the spi is in a transaction or not
  if(spicontrollers[spi_id].transmitting == true){
    //TODO send error can't start tranfer while already transmitting / in transaction
    return 0;
  }


  #ifdef TARGET_ESP32
    if(bitcount != 8 || bitcount != 16 || bitcount != 32){
      //TODO send error not legimate bitcount
      return 0;
    }
  #else
    //arduino uno only has a 16 bit spi buffer
    if(bitcount != 8 || bitcount != 16){
      //TODO send error not legimate bitcount
      return 0;
    }
  #endif


  //begin the transmission
  spicontrollers[spi_id].spi.beginTransaction(SPISettings(spicontrollers[spi_id].speed, spicontrollers[spi_id].bitorder, spicontrollers[spi_id].mode));

  spicontrollers[spi_id].transmitting = true;
  

  //activate the slave
  digitalWrite(spicontrollers[spi_id].slaves[slave_id].pin, LOW);

  spicontrollers[spi_id].activeSlave = slave_id;

  

  uint32_t result = 0;

  switch (bitcount) {
    case 8:

    //to eliminate all other bits (>8)
    data = data & 0xF;
    result = spicontrollers[spi_id].spi.transfer((uint8_t) data);

    break;
    case 16:

    //to eliminate all other bits (>16)
    data = data & 0xFF;
    result = spicontrollers[spi_id].spi.transfer16((uint16_t) data);

    break;

    //only the esp32 has the function to transmitt 32bit of data 
    #ifdef TARGET_ESP32
      case 32:
      //to eliminate all other bits (>32)
      data = data & 0xFFFF;
      result = spicontrollers[spi_id].spi.transfer32((uint32_t) data);
      break;
    #endif
  }

  

  //disable the slave
  digitalWrite(spicontrollers[spi_id].slaves[slave_id].pin, HIGH);
  
  spicontrollers[spi_id].activeSlave = -1;


  //end the transmission
  spicontrollers[spi_id].spi.endTransaction();

  spicontrollers[spi_id].transmitting = false;

  //return the result
  return result;
  
}

void startSpiBulkTransfer(uint8_t spi_id, uint8_t slave_id){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return;
  }

  //slave_id bounds check
  if(slave_id >= sizeof(spicontrollers[spi_id].slaves) / sizeof(spicontrollers[spi_id].slaves[0])){
    //TODO send error slave_id out of bounds
    return;
  }

  //check if the spi controller is initialized
  if(spicontrollers[spi_id].initialized == false){
    //TODO send error spi not inited
    return;
  }

  //check if the slave is initialized
  if(spicontrollers[spi_id].slaves[slave_id].initialized == false){
    //TODO send error slave not ready
    return;
  }

  //check wether the spi is in a transaction or not
  if(spicontrollers[spi_id].transmitting == true){
    //TODO send error can't start tranfer while already transmitting / in transaction
    return;
  }


  //begin the transmission
  spicontrollers[spi_id].spi.beginTransaction(SPISettings(spicontrollers[spi_id].speed, spicontrollers[spi_id].bitorder, spicontrollers[spi_id].mode));

  spicontrollers[spi_id].transmitting = true;

  //activate the slave
  digitalWrite(spicontrollers[spi_id].slaves[slave_id].pin, LOW);

  spicontrollers[spi_id].activeSlave = slave_id;
}



void stopSpiBulkTransfer(uint8_t spi_id){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return;
  }

  //check wether the spi is in a transaction or not
  if(spicontrollers[spi_id].transmitting == false){
    //TODO send error no transaction to end
    return;
  }

  //deactivate the slave
  digitalWrite(spicontrollers[spi_id].slaves[spicontrollers[spi_id].activeSlave].pin, HIGH);

  spicontrollers[spi_id].activeSlave = -1;

  //end the transmission
  spicontrollers[spi_id].spi.endTransaction();

  spicontrollers[spi_id].transmitting = false;

}


uint8_t bulkTransfer(uint8_t spi_id, uint8_t data){

  //spi_id bounds check
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    //TODO send error spi_id out of bounds
    return 0;
  }

  //check wether the spi is in a transaction or not
  if(spicontrollers[spi_id].transmitting == false){
    //TODO send error no active transaction
    return 0;
  }

  return spicontrollers[spi_id].spi.transfer(data);

}