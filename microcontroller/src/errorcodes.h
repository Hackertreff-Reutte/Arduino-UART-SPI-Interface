#ifndef ERRORCODES_H
#define ERRORCODES_H

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


//spi error Strings
#ifdef DEBUG
static const String SPI_ID_OUT_OF_BOUNDS_ERROR_TEXT = "SPI ID is out of bounds";

#endif


//sdcard error IDs


//sdcard error Strings
#ifdef DEBUG


#endif



#endif