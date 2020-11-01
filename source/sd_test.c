/*
***********************************************************************************************************************
*                                                   AVR-SD CARD MODULE
*
* File    : SD_TEST.C
* Version : ?
* Author  : Joshua Fain
* Target  : ATMega1280
*
*
* DESCRIPTION:
* Contains main() and includes some examples to test the AVR-SD Card Module
* 
*
*                                                 
*                                                       MIT LICENSE
*
* Copyright (c) 2020 Joshua Fain
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
* documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
* permit ersons to whom the Software is furnished to do so, subject to the following conditions: The above copyright 
* notice and this permission notice shall be included in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************************************
*/

#include <stdint.h>
#include <avr/io.h>
#include "../includes/usart.h"
#include "../includes/spi.h"
#include "../includes/prints.h"
#include "../includes/sd_spi_base.h"
#include "../includes/sd_spi_data_access.h"
#include "../includes/sd_spi_misc.h"


// local function
uint32_t enterBlockNumber();


int main(void)
{
  USART_Init();
  SPI_MasterInit();


  // ******************* SD CARD INITILIAIZATION ****************
  //
  // Initializing ctv. These members will be set by the SD card's
  // initialization routine. They should only be set there.
  
  CTV ctv;

  uint32_t initResponse;

  // Attempt, up to 10 times, to initialize the SD card.
  for (uint8_t i = 0; i < 10; i++)
    {
      print_str ("\n\n\r SD Card Initialization Attempt # "); print_dec(i);
      initResponse = SD_InitializeSPImode(&ctv);
      if (((initResponse & 0xFF) != 0) && ((initResponse & 0xFFF00) != 0))
        {    
          print_str ("\n\n\r FAILED INITIALIZING SD CARD");
          print_str ("\n\r Initialization Error Response: "); 
          SD_PrintInitError (initResponse);
          print_str ("\n\r R1 Response: "); SD_PrintR1 (initResponse);
        }
      else
        {   
          print_str ("\n\r SUCCESSFULLY INITIALIZED SD CARD");
          break;
        }
    }


  // initialization successful
  if (initResponse == 0)
    {      
      // Some variables for addressing
      // in the subsequent functions
      uint32_t startBlockNumber;
      uint32_t endBlockNumber;
      uint32_t blockNumber;
      uint32_t numberOfBlocks;

      uint8_t  blockArr[512];
      uint16_t err16; // 16-bit returned errors
      

      // ****************** TEST: SD_ReadSingleBlock() *******************
      // 
      // Read in a single block from the SD card at location specified by
      // blockNumber into the array, blockArr. This also demonstrates how
      // to use SD_PrintSingleBlock() to print the block.
/*
      blockNumber = 8192;

      if (ctv.type == SDHC) 
        err16 = SD_ReadSingleBlock (blockNumber, blockArr);
      else // SDSC
        err16 = SD_ReadSingleBlock (blockNumber * BLOCK_LEN, blockArr);
      
      if (err16 != READ_SUCCESS)
        { 
          print_str ("\n\r >> SD_ReadSingleBlock() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }
          else 
            { 
              print_str (" error "); 
              SD_PrintReadError (err16);
            }
        }

      // print single data block just loaded into the array.
      else
        SD_PrintSingleBlock();

      // *******************************************************************
*/


      // ****************** TEST: SD_PrintMultipleBlocks() *****************
      //
      // Use this function to print multiple, consecutive SD card blocks
      // beginning at blockNumber. The number of blocks printed is specified
      // by numberOfBlocks. The function calls the READ_MULTIPLE_BLOCKS SD 
      // card command.
/*      
      numberOfBlocks = 3;
      
      blockNumber = 20;
      
      if (ctv.type == SDHC) 
        err16 = SD_PrintMultipleBlocks (blockNumber,numberOfBlocks);
      else  // SDSC
        err16 = SD_PrintMultipleBlocks (blockNumber * BLOCK_LEN, numberOfBlocks);
      
      if (err16 != READ_SUCCESS)
        { 
          print_str ("\n\r >> SD_PrintMultipleBlock() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }
          else 
            { 
              print_str (" error "); 
              SD_PrintReadError (err16);
            }
        }
      // ********************************************************************
*/


      // ****************** TEST: SD_WriteSingleBlock() **********************
      //
      // Use this function to write to a single block on the SD card specified
      // by blockNumber. This demo will first erase a single block using 
      // SD_EraseBlocks(), read and print the block to confirm it is erased
      // using SD_ReadSingleBlock and SD_PrintSingleBlock(). This will then
      // call the SD_WriteSingleBlock() function, and reads and prints the 
      // block again to confirm the write was successful.
/*

      // data to write to block
      uint8_t dataArr[BLOCK_LEN] = "Howdy Globe......"; 
  
      blockNumber = 20;

      // ERASE single block (erase START block = erase END block)
      print_str("\n\r Erasing Block "); print_dec(blockNumber);
      if (ctv.type == SDHC)
        err16 = SD_EraseBlocks (blockNumber, blockNumber);
      else // SDSC
        err16 = SD_EraseBlocks (blockNumber * BLOCK_LEN, blockNumber * BLOCK_LEN);
      
      if (err16 != ERASE_SUCCESSFUL)
        {
          print_str ("\n\r >> SD_EraseBlocks() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }

          print_str (" error "); 
          SD_PrintEraseError (err16);
        }


      // READ and PRINT
      print_str("\n\r Read and Print Block "); print_dec(blockNumber); print_str(" after erasing.");
      if (ctv.type == SDHC) 
        err16 = SD_ReadSingleBlock (blockNumber, blockArr);
      else //SDSC
        err16 = SD_ReadSingleBlock (blockNumber * BLOCK_LEN, blockArr);

      if (err16 != READ_SUCCESS)
        { 
          print_str ("\n\r >> SD_ReadSingleBlock() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }
          else 
            { 
              print_str (" error "); 
              SD_PrintReadError (err16);
            }
        }
      // print the data block just loaded into the array.
      else
        SD_PrintSingleBlock(blockArr);

      // WRITE
      print_str("\n\r Write to block "); print_dec(blockNumber);
      err16 = SD_WriteSingleBlock (blockNumber, dataArr);
      if (err16 != DATA_ACCEPTED_TOKEN_RECEIVED)
        { 
          print_str ("\n\r >> SD_WriteSingleBlock() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }
          else 
            { 
              print_str (" error "); 
              SD_PrintWriteError (err16);

              // Get the R2 (SEND_STATUS) response if the Write Error Token
              // was returned by the card while writing to the block.
              // May convert this to a function. 
              if (( err16 & WRITE_ERROR_TOKEN_RECEIVED) == WRITE_ERROR_TOKEN_RECEIVED)
                {
                  print_str ("\n\r WRITE ERROR TOKEN returned. Getting R2 response.");
                  
                  uint16_t r2;
                  CS_LOW;             
                  SD_SendCommand (SEND_STATUS,0);
                  r2 = SD_GetR1(); // The first byte of R2 is R1
                  r2 <<= 8;
                  r2 |= SD_ReceiveByteSPI();
                  CS_HIGH;
                  print_str ("\n\r R2 Response = ");
                  print_dec (r2);
                }
            }
        }

      // READ amd PRINT
      else // Verify data has been written to block
        {
          print_str("\n\r Read and Print Block "); print_dec(blockNumber); print_str(" after writing");

          if (ctv.type == SDHC)
            err16 = SD_ReadSingleBlock (blockNumber, blockArr);
          else // SDSC
            err16 = SD_ReadSingleBlock (blockNumber * BLOCK_LEN, blockArr);

          if (err16 != READ_SUCCESS)
            { 
              print_str ("\n\r >> SD_ReadSingleBlock() returned ");
              if (err16 & R1_ERROR)
                {
                  print_str ("R1 error: ");
                  SD_PrintR1 (err16);
                }
              else 
                { 
                  print_str (" error "); 
                  SD_PrintReadError (err16);
                }
            }
          else// print the single data block that was read just in.
            SD_PrintSingleBlock (blockArr);
        }
      // *******************************************************************
*/



      // ****************** TEST: SD_WriteMultipleBlocks() *****************
      //
      // Use this function to write to multiple blocks of the SD card. This
      // function will write the same data to the blocks specified by 
      // startBlockNumber and numberOfBlocks. This function is not really
      // useful as it writes the same data to every block, but is used to 
      // demonstrate the WRITE_MULTIPLE_BLOCK command. This block of code
      // will first erase the multiple blocks using SD_EraseBlocks(), then
      // read-in and print the same blocks using SD_PrintMultipleBlocks().
      // Then it will write to the multiple blocks. It should then do some
      // error handling if the WRITE_BLOCK_TOKEN_RECIEVED is returned, but I
      // haven't really been able to test this out. It will then read-in and
      // print the blocks again to show that the write was successful. 


      // data to be written to the blocks
      uint8_t dataArr2[BLOCK_LEN] = "Well Hello There! I see you brought a PIE!!";

      startBlockNumber = 20;
      endBlockNumber   = 24; // used for erasing
      numberOfBlocks   = 4;  // used for writing

      // ERASE multiple blocks
      print_str("\n\r ***** ERASING BLOCKS ***** ");
      if (ctv.type == SDHC)
        err16 = SD_EraseBlocks (startBlockNumber, endBlockNumber);
      else // SDSC
        err16 = SD_EraseBlocks (startBlockNumber * BLOCK_LEN, endBlockNumber * BLOCK_LEN);
      
      if(err16 != ERASE_SUCCESSFUL)
        {
          print_str ("\n\r >> SD_EraseBlocks() returned ");
          if (err16 & R1_ERROR)
            {
              print_str("R1 error: ");
              SD_PrintR1(err16);
            }
          print_str(" error "); 
          SD_PrintEraseError(err16);
        }

      // Print Multiple Blocks
      print_str ("\n\r ***** PRINTING BLOCKS BEFORE WRITE ***** ");
      if (ctv.type == SDHC)
        err16 = SD_PrintMultipleBlocks (startBlockNumber, numberOfBlocks);
      else // SDSC
        err16 = SD_PrintMultipleBlocks (startBlockNumber * BLOCK_LEN, numberOfBlocks);
      
      if (err16 != READ_SUCCESS)
        { 
          print_str ("\n\r >> SD_PrintMultipleBlocks() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }
          else 
            { 
              print_str (" error "); 
              SD_PrintReadError (err16);
            }
        }

      // Write Multiple Blocks
      print_str ("\n\r ***** WRITING BLOCKS ***** ");
      err16 = SD_WriteMultipleBlocks (startBlockNumber, numberOfBlocks, dataArr2);
      if (err16 != DATA_ACCEPTED_TOKEN_RECEIVED)
        { 
          print_str ("\n\r >> SD_WriteMultipleBlocks() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }
          else 
            { 
              print_str (" error "); 
              SD_PrintWriteError (err16);

              // Get the R2 (SEND_STATUS) response if the Write Error Token
              // was returned by the card while writing to the block.
              // May convert this to a function. 
                
              if (( err16 & WRITE_ERROR_TOKEN_RECEIVED) == WRITE_ERROR_TOKEN_RECEIVED)
                {
                  // Getting R2.  May make this into a function.
                  print_str ("\n\r WRITE_ERROR_TOKEN set.");
                  print_str ("\n\r Getting STATUS (R2) response.");
                  uint16_t r2;
                  CS_LOW;             
                  SD_SendCommand (SEND_STATUS,0);
                  r2 = SD_GetR1(); // The first byte of R2 is R1
                  r2 <<= 8;
                  r2 |= SD_ReceiveByteSPI();
                  CS_HIGH;
                  print_str ("\n\r R2 Response = ");
                  print_dec (r2);

                  // Number of Well Written Blocks
                  print_str ("\n\r Getting Number of Well Written Blocks.");
                  uint32_t nwwb;
                  err16 = SD_GetNumberOfWellWrittenBlocks (&nwwb);
                  if (err16 != READ_SUCCESS)
                    { 
                      print_str("\n\r >> SD_GetNumberOfWellWritteBlocks() returned ");
                      if(err16 & R1_ERROR)
                        {
                          print_str("R1 error: ");
                          SD_PrintR1(err16);
                        }
                      else 
                        { 
                          print_str(" error "); 
                          SD_PrintReadError(err16);
                        }
                      print_str("\n\r Number of well written write blocks = ");
                      print_dec(nwwb);
                    }
                }
            }
        }


      print_str("\n\r ***** PRINTING BLOCKS AFTER WRITE ***** ");
      // READ amd PRINT post write

      if (ctv.type == SDHC)
        err16 = SD_PrintMultipleBlocks (startBlockNumber, numberOfBlocks);
      else // SDSC
        err16 = SD_PrintMultipleBlocks (startBlockNumber * BLOCK_LEN, numberOfBlocks);
      
      if (err16 != READ_SUCCESS)
        { 
          print_str ("\n\r >> SD_PrintMultipleBlocks() returned ");
          if (err16 & R1_ERROR)
            {
              print_str ("R1 error: ");
              SD_PrintR1 (err16);
            }
          else 
            { 
              print_str (" error "); 
              SD_PrintReadError (err16);
            }
        }
      // ********************************************************************



      // ************************ User Input Section ************************
      //
      // This section allows a user to interact with the the function 
      // SD_ReadMultipleBlocks() by entering the block number to begin the 
      // multile block read at and then setting the number of blocks to read.

      uint8_t  answer;
      do
        {
          do
            {
              print_str ("\n\n\n\rEnter Start Block\n\r");
              startBlockNumber = enterBlockNumber();
              print_str ("\n\rHow many blocks do you want to print?\n\r");
              numberOfBlocks = enterBlockNumber();
              print_str ("\n\rYou have selected to print "); print_dec(numberOfBlocks);
              print_str (" blocks beginning at block number "); print_dec(startBlockNumber);
              print_str ("\n\rIs this correct? (y/n)");
              answer = USART_Receive();
              USART_Transmit (answer);
              print_str ("\n\r");
            }
          while (answer != 'y');

          // READ amd PRINT post write

          // SDHC is block addressable
          if (ctv.type == SDHC) 
            err16 = SD_PrintMultipleBlocks (startBlockNumber,numberOfBlocks);
          // SDSC is byte addressable
          else // (SDSC)
            err16 = SD_PrintMultipleBlocks (startBlockNumber * BLOCK_LEN, numberOfBlocks);
          
          if (err16 != READ_SUCCESS)
            { 
              print_str ("\n\r >> SD_PrintMultipleBlocks() returned ");
              if (err16 & R1_ERROR)
                {
                  print_str ("R1 error: ");
                  SD_PrintR1 (err16);
                }
              else 
                { 
                  print_str (" error "); 
                  SD_PrintReadError (err16);
                }
            }

          print_str ("\n\rPress 'q' to quit: ");
          answer = USART_Receive();
          USART_Transmit (answer);
        }
      while (answer != 'q');
      // ********************************************************************



      // ********************************************************************
      //                          SD_SPI_MISC FUNCTIONS
      // ********************************************************************


      // *******  SD_GetMemoryCapacityHC/SC *******
      //
      // Get SD Card's memory capacity and print it
      // to the screen. Still testing these....
      print_str ("\n\n\n\r Memory capacity = ");
      if (ctv.type == SDHC) 
        print_dec (SD_GetMemoryCapacityHC());
      else // SDSC
        print_dec (SD_GetMemoryCapacitySC());
      print_str( " Bytes");
      // ******************************************
      


      // ***************** SD_FindNonZeroBlockNumbers() ******************
      //
      // print the block numbers of those block entries that have non-zero
      // entries. I made this to help locate raw data on the SD card.
      // This is not fast, so don't use it over a large range of blocks.
      print_str ("\n\n\r\r ***** SD_FindNonZeroBlockNumbers() ******\n\r");
      startBlockNumber = 5;
      endBlockNumber = 30;

      // SDHC is block addressable
      if (ctv.type == SDHC) 
        SD_FindNonZeroBlockNumbers(startBlockNumber, endBlockNumber);
      // SDSC is block addressable
      else // SDSC
        SD_FindNonZeroBlockNumbers (startBlockNumber * BLOCK_LEN, endBlockNumber * BLOCK_LEN);
      print_str("\n\r Done\n\r");
      // *****************************************************************
  }



  // Something to do after SD card testing has completed.
  while(1)
    USART_Transmit(USART_Receive());
  return 0;
}





// local function for taking user input to specify a block
// number. If nothing is entered then the block number is 0.
uint32_t enterBlockNumber()
{
  uint8_t x;
  uint8_t c;
  uint32_t blockNumber = 0;

  c = USART_Receive();
  
  while (c != '\r')
    {
      if ((c >= '0') && (c <= '9'))
        {
          x = c - '0';
          blockNumber = blockNumber * 10;
          blockNumber += x;
        }
      else if (c == 127) // backspace
        {
          print_str ("\b ");
          blockNumber = blockNumber / 10;
        }

      print_str ("\r");
      print_dec (blockNumber);
      
      if (blockNumber >= 4194304)
        {
          blockNumber = 0;
          print_str("\n\rblock number is too large.");
          print_str("\n\rEnter value < 4194304\n\r");
        }
      c = USART_Receive();
    }
  return blockNumber;
}
