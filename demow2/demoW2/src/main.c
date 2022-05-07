#include <util/delay.h>
#include <avr/io.h>
#include <paulLib.h>
#include <avr/interrupt.h>
#include <usart.c>
#include <button.h>
#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define LED1 PB2
#define LED2 PB3

/* First, review the code in the main function!
 * This ISR is called when Pin Change Interrupt 1 is triggered (PCINT1_vect)
 * This is the interrupt for PORTC, where the switches/buttons are connected... */
ISR( PCINT1_vect )
{

    // printString(" works!");
    
    // buttonPushed(1);


    // button 1 is pressed (bit is set to 0)?
    if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
    {
      buttonPushed(1);
    }
    if ( bit_is_clear(BUTTON_PIN,PC2))
    {
      buttonPushed(2);
    }
    if (bit_is_clear(BUTTON_PIN,PC3)){
      buttonPushed(3);
    }
    
    //     //We wait 1000 microseconds and check again (debounce!)
        // _delay_us( 1000 );
    //     // button 0 is pressed (bit is set to 0)?
    //     if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
    //     {
    //         if ( bit_is_set( LED_PORT, LED2 ))  // if led2 is off (bit is set to 1)
    //         {
    //             LED_PORT &= ~_BV( LED2 );       // turn led2 on (set bit to 0)
    //             // lightUpOneLed(1);
    //         }
    //         else                                // else
    //         {
    //             LED_PORT |= _BV( LED2 );        // turn led2 off (set bit to 1)
    //             // lightDownOneLed(1);
    //         }
    //     }
    // } 
    // if(bit_is_clear(BUTTON_PIN,PC2) ){
    //     _delay_us( 1000 );
    //     if ( bit_is_clear(BUTTON_PIN,PC2))
    //     {
    //         if ( bit_is_set( LED_PORT, PB4 ))  // if led2 is off (bit is set to 1)
    //         {
    //             LED_PORT &= ~_BV( PB4 );       // turn led2 on (set bit to 0)
    //             // lightUpOneLed(2);
    //         }
    //         else                                // else
    //         {
    //             LED_PORT |= _BV( PB4 );        // turn led2 off (set bit to 1)
    //             // lightDownOneLed(2);
    //         }
    //     }
    // }
    // if(bit_is_clear(BUTTON_PIN,PC3)){
    //     _delay_us( 1000 );
    //     if ( bit_is_clear(BUTTON_PIN,PC3))
    //     {
    //         if ( bit_is_set( LED_PORT, PB5 ))  // if led2 is off (bit is set to 1)
    //         {
    //             LED_PORT &= ~_BV( PB5 );       // turn led2 on (set bit to 0)
    //             // lightUpOneLed(3);
    //         }
    //         else                                // else
    //         {
    //             LED_PORT |= _BV( PB5 );        // turn led2 off (set bit to 1)
    //             // lightDownOneLed(3);
    //         }
    //     }
    // }
}

int main()
{
    initUSART();
    enableAllLeds();
    lightDownAllLeds();
  
    // LED_DDR |= _BV( LED1 ) | _BV( LED2 );   // we'll use led1 and led2
    // LED_PORT |= _BV( LED1 ) | _BV( LED2 );  // turn 2 leds off
    BUTTON_DDR &= ~_BV( BUTTON1 );          // we'll use button1
    BUTTON_DDR &= ~_BV(PC2);
    BUTTON_DDR &= ~_BV(PC3);
    BUTTON_PORT |= _BV( BUTTON1 );          // enable pull-up
    BUTTON_PORT |= _BV(PC2);
    BUTTON_PORT |= _BV(PC3);
    PCICR |= _BV( PCIE1 );  /* in Pin Change Interrupt Control Register: indicate
                             * which interrupt(s) you want to activate (PCIE0: port B,
                             * PCIE1: port C, PCIE2: port D) */
    PCMSK1 |= _BV( BUTTON1 );   /* In the corresponding Pin Change Mask Register: indicate*/
    PCMSK1 |= _BV( PC2 );   /* In the corresponding Pin Change Mask Register: indicate*/
    PCMSK1 |= _BV( PC3 );   /* In the corresponding Pin Change Mask Register: indicate
                                //  * which pin(s) of that port activate the ISR. */
    sei();  // Set Enable Interrupts --> activate the interrupt system globally.
    int i;
    while ( 1 )
    {
        LED_PORT &= ~_BV( LED1 );   // turn led1 on
        _delay_ms( 2 );
        LED_PORT |= _BV( LED1 );    // turn led1 off
        _delay_ms( 8 );
        for(i=1;i<4;i++){
          buttonReleased(i);
        }
    }
    return 0;
}