#include <util/delay.h>
#include <avr/io.h>
#include <paulLib.h>
#include <avr/interrupt.h>
#include <usart.c>
#include <button.h>
#include <stdio.h>
#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED4 PB5
#define TRUE 1
#define FALSE 0
#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
// #define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3
#define DELAY 500

uint8_t randomnr[10];
// uint8_t pressednr[10];


int i;
int startGame=1;
int count=1;
int correct=1;
ISR( PCINT1_vect )
{

    // printString(" works!");
    
    // buttonPushed(1);


    // button 1 is pressed (bit is set to 0)?
    if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
    {
      _delay_us(10000);
      if ( bit_is_clear( BUTTON_PIN, BUTTON1 )){
        if(startGame){
          startGame=0;
        }
        else{
          if((int)randomnr[i]+1==1){
            printString("You pressed button 1, correct!");
          }
          else{
            printString("You pressed button 1, wrong!");
            correct=0;
          }
          i=i+1;
        }
      }
    }
    if ( bit_is_clear(BUTTON_PIN,PC2))
    {
      _delay_us(10000);
      if(bit_is_clear(BUTTON_PIN,BUTTON2)){
        if(!startGame){
          if((int)randomnr[i]+1==2){
            printString("You pressed button 2, correct!");
          }
          else{
            printString("You pressed button 2, wrong!");
            correct=0;
          }
          i=i+1;
        }
      }
    }
    if (bit_is_clear(BUTTON_PIN,PC3)){
      _delay_us(10000);
      if(bit_is_clear(BUTTON_PIN,BUTTON3)){
        if(!startGame){
          if((int)randomnr[i]+1==3){
            printString("You pressed button 3, correct!");
          }
          else{
            printString("You pressed button 3, wrong!");
            correct=0;
          }
          i=i+1;
        }
      }
    }
  
}
int main()
{
    initUSART();
    enableAllLeds();
    lightDownAllLeds();
    srand(1); 

    // flashLed(1, 20);
    // LED_DDR |= _BV( LED1 ) | _BV( LED2 );   // we'll use led1 and led2
    // LED_PORT |= _BV( LED1 ) | _BV( LED2 );  // turn 2 leds off
    BUTTON_DDR &= ~_BV( BUTTON1 );          // we'll use button1
    BUTTON_DDR &= ~_BV( BUTTON2 );
    BUTTON_DDR &= ~_BV( BUTTON3 );
    BUTTON_PORT |= _BV( BUTTON1 );          // enable pull-up
    BUTTON_PORT |= _BV( BUTTON2 );
    BUTTON_PORT |= _BV( BUTTON3 );
    PCICR |= _BV( PCIE1 );  /* in Pin Change Interrupt Control Register: indicate
                             * which interrupt(s) you want to activate (PCIE0: port B,
                             * PCIE1: port C, PCIE2: port D) */
    PCMSK1 |= _BV( BUTTON1 );   /* In the corresponding Pin Change Mask Register: indicate*/
    PCMSK1 |= _BV( BUTTON2 );   /* In the corresponding Pin Change Mask Register: indicate*/
    PCMSK1 |= _BV( BUTTON3 );   /* In the corresponding Pin Change Mask Register: indicate
                                //  * which pin(s) of that port activate the ISR. */
    sei();  // Set Enable Interrupts --> activate the interrupt system globally.

    
    i=0;
    while (startGame)
    {
        LED_PORT &= ~_BV( LED4 );   // turn led1 on
        _delay_ms( 500 );
        LED_PORT |= _BV( LED4 );    // turn led1 off
        _delay_ms( 500 );
    }
    while(!startGame&&count<10){
      i=0;
      generatePuzzle(count);
      playPuzzle(randomnr,count);

      while(i<count){
        _delay_ms(2000);
        printByte(i);
        printString(" is i and count is ");
        printByte(count);
        printString("\n");
        if(!correct)break;
      }
      if(!correct){
        printString("Wrong, the correct patter was: ");
        printPuzzle(count);
        printString("\n");
        break;
      }
      else{
        flashLed(3,20);
        printString("correct, WE GO TO level ");
        printByte(i);
        printString("\n");
        // printPuzzle(count);
      }
      count++;
      
    }
    if(count>=10){
      printString("Congratulations,you are the Simon Master!");
    }

    return 0;
}



void printPuzzle(int count){
  printString("[ ");
  for(int i = 0 ; i <count;i++){
          printByte(randomnr[i]+1);
          printString(" ");
          // printString("\n");
        }
        printString(" ]\n");
        
}

void generatePuzzle(int count){
  for(int i = 0; i <count;i++){
    randomnr[i]=(rand()%3);
  }
}


void playPuzzle(uint8_t arr[], int count){
  for(int i = 0; i <count;i++){
    // printString((int)arr[i]);
    lightUpOneLed((int)arr[i]);
    _delay_ms(DELAY);
    lightDownOneLed((int)arr[i]);
    _delay_ms(DELAY);

    
  }
}