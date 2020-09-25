#ifndef ERRORCODES_H
#define ERRORCODES_H

#include "settings.h"

//error categories 
#define SYSTEM_ERROR "SYSTEM"
#define PARSER_ERROR "PARSER"
#define SPI_ERROR "SPI"
#define SDCARD_ERROR "SDCARD" 



//system error IDs


//system error Strings
#ifdef DEBUG


#endif


//parser error IDs


//parser error Strings
#ifdef DEBUG


#endif


//spi error IDs
#define SPI_ID_OUT_OF_BOUNDS_ERROR 0
#define SPI_SLAVE_ID_OUT_OF_BOUNDS_ERROR 1
#define SPI_ALREADY_INITIALIZED_ERROR 2
#define SPI_NOT_INITIALIZED_ERROR 3
#define SPI_SLAVE_NOT_INITIALIZED 4
#define SPI_SLAVE_ALREADY_INITIALIZED 5
#define SPI_IS_IN_TRANSACTION_ERROR 6
#define SPI_NO_ACTIVE_TRANSACTION_ERROR 7
#define SPI_INVALID_BITCOUNT_ERROR 8


//spi error Strings
#ifdef DEBUG
static const String SPI_ID_OUT_OF_BOUNDS_ERROR_TEXT = "SPI ID is out of bounds";
static const String SPI_SLAVE_ID_OUT_OF_BOUNDS_ERROR_TEXT = "SPI Slave ID is out of bounds";
static const String SPI_ALREADY_INITIALIZED_TEXT = "SPI already initialized";
static const String SPI_NOT_INITIALIZED_TEXT = "SPI not initialized";
static const String SPI_SLAVE_NOT_INITIALIZED_TEXT = "SPI slave not initialized";
static const String SPI_SLAVE_ALREADY_INITIALIZED_TEXT = "SPI slave already initialized";
static const String SPI_IS_IN_TRANSACTION_ERROR_TEXT = "Error due to the SPI being in a transaction";
static const String SPI_NO_ACTIVE_TRANSACTION_ERROR_TEXT = "Error there is no active transaction";
static const String SPI_INVALID_BITCOUNT_ERROR_TEXT = "Error not a vaild bitcount";

#endif


//sdcard error IDs


//sdcard error Strings
#ifdef DEBUG


#endif



#endif