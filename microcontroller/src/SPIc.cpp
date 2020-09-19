//this ifndef is just here for a script to create a single main file (used by the arduino ide) and to not include non-existend files 
#ifndef ARDUINO_IDE
#include "SPIc.h"
#else
#include <Arduino.h>
#include <SPI.h>
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
  slave slaves[NUMBER_OF_SLAVES_PER_SPI];
};


//array where all the spicontrollers are saved
spicontroller spicontrollers [SIZE_OF_SPICONTROLLER_ARRAY];


//---------------------error checks------------------------

bool isSPI_ID_inBounds(uint8_t spi_id){
  //spi_id bounds check (checks if the spi id is not bigger than the spi array, if it is this function will send an error)
  if(spi_id >= sizeof(spicontrollers)/sizeof(spicontrollers[0])){
    
    //send error message SPI id out of bounds
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_ID_OUT_OF_BOUNDS_ERROR);
    #else
      sendError(SPI_ERROR, SPI_ID_OUT_OF_BOUNDS_ERROR, SPI_ID_OUT_OF_BOUNDS_ERROR_TEXT);
    #endif

    return false;
  }else{
    return true;
  }
}

bool isSPI_notInitialized(uint8_t spi_id){
  //check if the spi is already initialized (if it is this function will send a error)
  if(spicontrollers[spi_id].initialized == true){
    
    //send error message SPI is already initialized
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_ALREADY_INITIALIZED);
    #else
      sendError(SPI_ERROR, SPI_ALREADY_INITIALIZED, SPI_ALREADY_INITIALIZED_TEXT);
    #endif

    return false;
  }

  return true;
}

bool isSPI_Initialized(uint8_t spi_id){
  //check if the spi is initialized (if it is not this function will send a error)
  if(spicontrollers[spi_id].initialized == true){
    
    //send error message SPI is already initialized
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_ALREADY_INITIALIZED);
    #else
      sendError(SPI_ERROR, SPI_ALREADY_INITIALIZED, SPI_ALREADY_INITIALIZED_TEXT);
    #endif

    return false;
  }else{
    return true;
  }
}


bool isSPI_Slave_inBounds(uint8_t spi_id, uint8_t slave_id){
  //slave_id bounds check (checks if the slave id is not bigger than the slave array, if it is this function will send an error)
  if(slave_id >= sizeof(spicontrollers[spi_id].slaves) / sizeof(spicontrollers[spi_id].slaves[0])){
    
    //send error message SPI slave id out of bounds
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_SLAVE_ID_OUT_OF_BOUNDS_ERROR);
    #else
      sendError(SPI_ERROR, SPI_SLAVE_ID_OUT_OF_BOUNDS_ERROR, SPI_SLAVE_ID_OUT_OF_BOUNDS_ERROR_TEXT);
    #endif

    return false;
  }else{
    return true;
  }
}

bool isSPI_Slave_notInitialized(uint8_t spi_id, uint8_t slave_id){
  //send error if the spi slave is already initialized
  if(spicontrollers[spi_id].slaves[slave_id].initialized == true){
    
    //send error message SPI slave already initialized
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_SLAVE_ALREADY_INITIALIZED);
    #else
      sendError(SPI_ERROR, SPI_SLAVE_ALREADY_INITIALIZED, SPI_SLAVE_ALREADY_INITIALIZED_TEXT);
    #endif

    return false;
  }else{
    return true;
  }
}

bool isSPI_Slave_Initialized(uint8_t spi_id, uint8_t slave_id){
  //check if the slave is initialized, if not this function will send a error
  if(spicontrollers[spi_id].slaves[slave_id].initialized == false){
    
    //send error message SPI slave is not initialized
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_SLAVE_NOT_INITIALIZED);
    #else
      sendError(SPI_ERROR, SPI_SLAVE_NOT_INITIALIZED, SPI_SLAVE_NOT_INITIALIZED_TEXT);
    #endif

    return false;
  }else{
    return true;
  }
}

bool isSPI_notTransmitting(uint8_t spi_id){
  //check if the spi has no active transaction (if yes (has active transaction) = error)
  if(spicontrollers[spi_id].transmitting == true){
    
    //send error message due to being in a transaction
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_IS_IN_TRANSACTION_ERROR);
    #else
      sendError(SPI_ERROR, SPI_IS_IN_TRANSACTION_ERROR, SPI_IS_IN_TRANSACTION_ERROR_TEXT);
    #endif

    return false;
  }else{
    return true;
  }
}

bool isSPI_Transmitting(uint8_t spi_id){
  //check wether the spi is currently in a transaction (if no = error)
  if(spicontrollers[spi_id].transmitting == false){
   
    //send error there is no active transaction
    #ifndef DEBUG
      sendError(SPI_ERROR, SPI_NO_ACTIVE_TRANSACTION_ERROR);
    #else
      sendError(SPI_ERROR, SPI_NO_ACTIVE_TRANSACTION_ERROR, SPI_NO_ACTIVE_TRANSACTION_ERROR_TEXT);
    #endif

    return false;
  }else{
    return true;
  }
}

bool isValidBitCount(uint8_t bitcount){

  //check if the bitcount is valid, if not this function will send a error

  #ifdef TARGET_ESP32
    if(bitcount != 8 && bitcount != 16 && bitcount != 32){
      
      //send error message invalid bitcount
      #ifndef DEBUG
        sendError(SPI_ERROR, SPI_INVALID_BITCOUNT_ERROR);
      #else
        sendError(SPI_ERROR, SPI_INVALID_BITCOUNT_ERROR, SPI_INVALID_BITCOUNT_ERROR_TEXT);
      #endif

      return false;
    }else{
      return true;
    }
  #else
    //arduino uno only has a 16 bit spi buffer
    if(bitcount != 8 && bitcount != 16){
      
      //send error message invalid bitcount
      #ifndef DEBUG
        sendError(SPI_ERROR, SPI_INVALID_BITCOUNT_ERROR);
      #else
        sendError(SPI_ERROR, SPI_INVALID_BITCOUNT_ERROR, SPI_INVALID_BITCOUNT_ERROR_TEXT);
      #endif

      return false;
    }else{
      true
    }
  #endif
}


void setupSPI(uint8_t spi_id, uint8_t miso, uint8_t mosi, uint8_t clock,  uint32_t speed, uint8_t bitorder, uint8_t mode){

  
  //spi_id bounds check
  if(isSPI_ID_inBounds(spi_id) == false){
    return;
  }

  //check if the spi is already initialized
  if(isSPI_notInitialized(spi_id) == false){
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
  if(isSPI_ID_inBounds(spi_id) == false){
    return;
  }

  //slave_id bounds check
  if(isSPI_Slave_inBounds(spi_id, slave_id) == false){
    return;
  }

  //check if the spi is initialized
  if(isSPI_Initialized(spi_id) == false){
    return;
  }

  //check if the spi slave is already initialized
  if(isSPI_Slave_notInitialized(spi_id, slave_id) == false){
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
  if(isSPI_ID_inBounds(spi_id) == false){
    return;
  }

  //check if the spi free and not transmitting
  if(isSPI_notTransmitting(spi_id) == false){
    return;
  }

  spicontrollers[spi_id].mode = mode;

}


void updateSpiSpeed(uint8_t spi_id, uint32_t speed){

  //spi_id bounds check
  if(isSPI_ID_inBounds(spi_id) == false){
    return;
  }

  //check if the spi free and not transmitting
  if(isSPI_notTransmitting(spi_id) == false){
    return;
  }

  spicontrollers[spi_id].speed = speed;
}


//NOTE: if the spi bus is stopped it keeps the config (config will not be reset)
void stopSpi(uint8_t spi_id){

  //spi_id bounds check
  if(isSPI_ID_inBounds(spi_id) == false){
    return;
  }

  //check if the spi is initialized
  if(isSPI_Initialized(spi_id) == false){
    return;
  }

  //check if the spi free and not transmitting
  if(isSPI_notTransmitting(spi_id) == false){
    return;
  }


  spicontrollers[spi_id].initialized = false;
  spicontrollers[spi_id].spi.end();

}


uint32_t transfer(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, uint32_t data){

  //spi_id bounds check
  if(isSPI_ID_inBounds(spi_id) == false){
    return 0;
  }

  //slave_id bounds check
  if(isSPI_Slave_inBounds(spi_id, slave_id) == false){
    return 0;
  }

  //check if the spi is initialized
  if(isSPI_Initialized(spi_id) == false){
    return 0;
  }

  //check if the slave is initialized
  if(isSPI_Slave_Initialized(spi_id, slave_id) == false){
    return 0;
  }

  //check if the spi free and not transmitting
  if(isSPI_notTransmitting(spi_id) == false){
    return 0;
  }

  //check if the bitcount is valid
  if(isValidBitCount(bitcount) == false){
    return 0;
  }


  //begin the transmission
  spicontrollers[spi_id].spi.beginTransaction(SPISettings(spicontrollers[spi_id].speed, spicontrollers[spi_id].bitorder, spicontrollers[spi_id].mode));

  spicontrollers[spi_id].transmitting = true;
  

  //activate the slave
  digitalWrite(spicontrollers[spi_id].slaves[slave_id].pin, LOW);

  spicontrollers[spi_id].activeSlave = slave_id;

  

  uint32_t result = 0;

  switch (bitcount) {
    case 8:

    result = spicontrollers[spi_id].spi.transfer((uint8_t) data);
    break;

    case 16:

    result = spicontrollers[spi_id].spi.transfer16((uint16_t) data);
    break;

    //only the esp32 has the function to transmitt 32bit of data 
    #ifdef TARGET_ESP32
      case 32:
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
  if(isSPI_ID_inBounds(spi_id) == false){
    return;
  }

  //slave_id bounds check
  if(isSPI_Slave_inBounds(spi_id, slave_id) == false){
    return;
  }

  //check if the spi is initialized
  if(isSPI_Initialized(spi_id) == false){
    return;
  }

  //check if the slave is initialized
  if(isSPI_Slave_Initialized(spi_id, slave_id) == false){
    return;
  }

  //check if the spi free and not transmitting
  if(isSPI_notTransmitting(spi_id) == false){
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
  if(isSPI_ID_inBounds(spi_id) == false){
    return;
  }

  //check if there is any active transaction
  if(isSPI_Transmitting(spi_id) == false){
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
  if(isSPI_ID_inBounds(spi_id) == false){
    return 0;
  }

  //check if there is any active transaction to end
  if(isSPI_Transmitting(spi_id) == false){
    return 0;
  }

  return spicontrollers[spi_id].spi.transfer(data);

}