#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h>
#include <stdlib.h>
#include <paulLib.h>


int main()
{
  enableAllLeds();
  lightDownAllLeds();

  while (1)
  {
  
  fadeInLed(1,500);
  fadeOutLed(1,500);

  }
  return 0;
}