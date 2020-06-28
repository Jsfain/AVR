/******************************************************************************
 * Author: Joshua Fain
 * Date:   6/23/2020
 * 
 * File: SD_MISC.H
 * 
 * Required by: SD_MISC.C
 *
 * Target: ATmega 1280
 * 
 * Description: 
 * Declares functions defined in SD_MISC.C and defines DataSector struct.
 * ***************************************************************************/

#ifndef SD_MISC_H
#define SD_MISC_H

// Struct to store data from data block returned in response to CMD17 along with the R1 response and CRC.
typedef struct  DataSector { //A block is limited to 512 bytes in this implementation
    uint8_t R1;
    uint8_t ERROR;  // no specific errors defined yet.
    uint8_t data[DATA_BLOCK_LEN];
    uint8_t CRC[2];
} DataSector;



// Reads in a single data secotr from an SD card and returns the data in a 
// DataSector struct.
DataSector sd_ReadSingleDataSector(uint32_t address);



// Calculate and return the memory capacity of the SD Card in Bytes.
uint32_t sd_getMemoryCapacity();



// Print the data in data sector array passed as the argument.
void print_sector(uint8_t *sector);  //only 512 byte sector supported.




#endif // SD_MISC_H