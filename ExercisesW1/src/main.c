#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h>
#include <stdlib.h>
#include <paulLib.h>


int main()
{
  enableAllLeds();
  lightDownAllLeds();

  incrementLedBurn(0);
  return 0;
}