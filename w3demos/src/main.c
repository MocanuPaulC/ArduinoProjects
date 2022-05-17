#include <util/delay.h>
#include <avr/io.h>
// #include <usart.c>
#include <display.c>
#include <button.h>
#define A 2020
#define B 2021
#define C 2022

void initADC()
{
    
    ADMUX |= ( 1 << REFS0 );    //Set up of reference voltage. We choose 5V as reference.
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );  //Determine a sample rate by setting a division factor. Used division factor: 128
    ADCSRA |= ( 1 << ADEN ); //Enable the ADC
}


int writeLinesOnHeap(char sentence[]){
    char *copy;
    // int size=strlen(sentence)+1;
    copy=malloc(strlen(sentence)+1);
    strcpy(copy,sentence);
    printf("\"%s\" is on heap\n", sentence);
    return copy;
}

int main()
{
    char message[]="I am not allowed to speak in class";
    int spaceUsed=0;
    initUSART();
    initADC();
    initDisplay();
    enableAllButtons();
    BUTTON_DDR &= ~_BV( BUTTON1 );          // we'll use button1
    BUTTON_DDR &= ~_BV( BUTTON2 );
    BUTTON_DDR &= ~_BV( BUTTON3 );
    BUTTON_PORT |= _BV( BUTTON1 );          // enable pull-up
    BUTTON_PORT |= _BV( BUTTON2 );
    BUTTON_PORT |= _BV( BUTTON3 );

    while ( 1 )
    {
      if ( bit_is_clear( BUTTON_PIN, BUTTON1)|| bit_is_clear(BUTTON_PIN,BUTTON2)|| bit_is_clear(BUTTON_PIN,BUTTON3))
      {
        break;
      }
    }
    for(int i = 1; i <101;i++){
      spaceUsed=writeLinesOnHeap(message);
      printf("%d: %s\n",i,message);
      printf("%d bytes are now occupied on heap\n",spaceUsed);
      }
      
    

    return 0;

}