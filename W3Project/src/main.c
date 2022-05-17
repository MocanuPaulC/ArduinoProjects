#include <util/delay.h>
#include <avr/io.h>
#include <usart.c>
#include <display.c>
#include <button.h>
#define STARTNUMBER 21
#define MAXNUMBER 3


void initADC()
{
    
    ADMUX |= ( 1 << REFS0 );    //Set up of reference voltage. We choose 5V as reference.
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );  //Determine a sample rate by setting a division factor. Used division factor: 128
    ADCSRA |= ( 1 << ADEN ); //Enable the ADC
}



void initBtns(){
  BUTTON_DDR &= ~_BV( BUTTON1 );
  BUTTON_DDR &= ~_BV( BUTTON2 );
  BUTTON_DDR &= ~_BV( BUTTON3 );
  BUTTON_PORT |= _BV( BUTTON1 ); 
  BUTTON_PORT |= _BV( BUTTON2 );
  BUTTON_PORT |= _BV( BUTTON3 );
  PCICR |= _BV( PCIE1 );
  PCMSK1 |= _BV( BUTTON1 );   
  PCMSK1 |= _BV( BUTTON2 );   
  PCMSK1 |= _BV( BUTTON3 );   
  sei();


}
int* choice;
char currentPlayer;
int* availableCards=STARTNUMBER;
char* finalMsg;
int* nrToSubtract;

ISR( PCINT1_vect )
{
   if ( bit_is_clear( BUTTON_PIN, BUTTON1)){
        _delay_us(15000);
        if ( bit_is_clear( BUTTON_PIN, BUTTON1)){
          if(*choice>1)*choice-=1;
        }
   }
      if ( bit_is_clear( BUTTON_PIN, BUTTON2)){
        _delay_us(15000);
        if ( bit_is_clear( BUTTON_PIN, BUTTON2)){
          *availableCards-=*choice;
          printf("Player took %d matches, there are %d available matches\n",*choice,*availableCards);
          currentPlayer='C';
        }
      }
      if ( bit_is_clear( BUTTON_PIN, BUTTON3)){
        
        _delay_us(15000);
        if ( bit_is_clear( BUTTON_PIN, BUTTON3)){
          if(*choice<3)*choice+=1;
        }
      }
}



int main(){
  initUSART();
  choice=malloc(1);
  availableCards=malloc(1);

  int seed;
  enableAllButtons();
  initBtns();
  initADC();
  initDisplay();
  while ( 1 )
    {
        ADCSRA |= ( 1 << ADSC ); 
        loop_until_bit_is_clear( ADCSRA, ADSC );    
        uint16_t value = ADC;   
        value*=9;
        value%=10000;
        writeNumber(value);
        if ( bit_is_clear( BUTTON_PIN, BUTTON1)){
          seed=value;
          break;
        }
    }
  srand(seed);
  int firstPlayer=rand()%2;
  *choice=1;
  *availableCards=21;

  
  if(firstPlayer){
    currentPlayer='C';
  }
  else{
    currentPlayer='P';
  }

  while(*availableCards>1){

    if(currentPlayer=='C'){
    for (int i = 0; i < 3000 / 20; i++) {
    writeNumberToSegment(2,*availableCards/10);
    _delay_ms(5);
    writeNumberToSegment(3,*availableCards%10);
    _delay_ms(5);
    writeNumberToSegment(0,*choice);
    _delay_ms(5);
    if((i>10&&i<25)||(i>45&&i<75)||(i>95&&i<115)){
    writeCharToSegment(1,currentPlayer);
    }
    _delay_ms(5);
    }
    nrToSubtract=malloc(1);
    *nrToSubtract=(*availableCards-1)%(MAXNUMBER+1);
    if(*nrToSubtract==0)*nrToSubtract=rand()%3+1;
    *availableCards-=*nrToSubtract;
    currentPlayer='P';
    printf("Computer took %d matches, there are %d available matches\n",*nrToSubtract,*availableCards);
    free(nrToSubtract);
    }
    else{
      for (int i = 0; i < 3000 / 20; i++) {
    writeNumberToSegment(2,*availableCards/10);
    _delay_ms(5);
    writeNumberToSegment(3,*availableCards%10);
    _delay_ms(5);
    writeNumberToSegment(0,*choice);
    _delay_ms(5);
    if((i>10&&i<25)||(i>45&&i<75)||(i>95&&i<115)){
    writeCharToSegment(1,currentPlayer);
    }
    _delay_ms(5);
    }
    }
    
  }

  if(currentPlayer=='P'){
    finalMsg=writeLinesOnHeap("ggez");
    printString("The computer has won!");
  }
  else{
    printString("The player has won!");
    finalMsg=writeLinesOnHeap("ggwp");
  }
  
  while (1)
  {
    writeString(finalMsg);
  }
  
}