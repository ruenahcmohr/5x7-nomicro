/***********************************************************************************

Inspried by PHPPLD.
2021 Rue Mohr.
This generic ROM-generator framework was customized for:

twitter demo

This is set up to take a 1-of-9 input and convert them to an ascii value.

We will generate an image for a 16k ROM (use 28C16)?
note this is too large a memory chip, but the system will automatically
deregard the state of unused inputs.

WIRING
 - switch 0 to A0
 - switch 1 to A1
 - switch 2 to A2
 - switch 3 to A3 
 - switch 4 to A4
 - switch 5 to A5 
 - switch 6 to A6
 - switch 7 to A7 
 - switch 8 to A8
 
 - Q0 to D0 of ascii display
 - Q1 to D1 of ascii display
 - Q2 to D2 of ascii display
 - Q3 to D3 of ascii display
 - Q4 to D4 of ascii display
 - Q5 to D5 of ascii display
 - Q6 to D6 of ascii display 
 
 - Q7 to valid LED.
  
 
INPUTS
 - switches, 9 bits.

OUTPUTS
 - value 7 bits ascii
 - valid, not an illegal input.
 
FEEDBACK
 N/A



Address bits      8 bit rom size

       -- no parallel roms available --
     8                  2 k
     9                  4 k
     10                 8 k
     
       -- eeproms available from here --
     11                 16 k  (28C16)
     12                 32 k  (28C32)
     
       -- eprom practical sizes from here --
     13                 64 k  (2764)
     14                 128 k (27128)
     15                 256 k 
     16                 512 k
     17                 1 M  (27010)
     18                 2 M
     19                 4 M
     20                 8 M

       -- flash from here up --



**************************************************************************************/


#include <stdio.h>
#include <stdint.h>
#include "ROMLib.h"
#include "font.h"


// the number of address lines you need !!!???!!!
// this is 11 bits for a 16kbit ROM
#define InputBits 11

// the output data size, 8 or 16
#define OutputBits 8


int main(void) {

  // CREATE YOUR INPUT VALUES HERE
  uint16_t codeI, scanI;                             // (custom) values are 16 bits      
  
  // CREATE YOUR OUTPUT VALUES HERE
  uint16_t rowsO, resetO ;              



  uint32_t out;  // leave it alone!
  setup();       // open output file.    
  // this will loop thru each address
  for( A=0; A<(1<<InputBits); A++) { // A is a bitfield (a mash-up of the vars we want)
       
       
                     
       
     // RESET YOUR VARS
     rowsO      = 0;
     resetO     = 0;
     
     // build input values from the bit field that represents a given input
     // create one of these calls for each variable you need to create from the input bits.
     spliceValueFromField( &codeI,            A,  8,   0,1,2,3,4,5,6,7);        // switchI is made of 9 bits of the input, which are bits 0,1,2...
     spliceValueFromField( &scanI,            A,  3,   8,9,10);        // switchI is made of 9 bits of the input, which are bits 0,1,2...

     // do task
       
     if (scanI >=5) {                           // blank overscan
       rowsO  = 0;  
       resetO = 1;
     }  else {
       if (codeI < 0x20) codeI = 0x20;           // change invalid codes to space.
       if (codeI > 127)  codeI = 0x20;     
       rowsO = Font5x7[((codeI-0x20)*5)+(4-scanI)];  // character offset is code*5, but starts at what should be 0x20.  Scan is reversed, so line = 4-scan
     }
                  
     rowsO ^=0xFF;  // invert becasue led polarity arg
     
     

     // reconstitute the output
     // reset output  
     out = 0;
     spliceFieldFromValue( &out, rowsO,     7,  0,1,2,3,4,5,6); // insert 7 bits from codeO, that go in bit positions 0,1,2,3,4,5,6 of the output
     spliceFieldFromValue( &out, resetO,    1,  7);             // insert 1 bits from validO, that goes in bit position 7 of the output
     
     // submit entry to output file
     write(fd, &out, OutputBits>>3);  // >>3 converts to bytes, leave it!
  }
  
  cleanup(); // close file
  
  return 0;
}






