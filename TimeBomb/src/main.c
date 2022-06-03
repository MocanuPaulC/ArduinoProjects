#define __DELAY_BACKWARD_COMPATIBLE__
#include <paulLib.h>
#include <util/delay.h>
#include <avr/io.h>
#include <display.c>
#include <stdio.h>
#include <button.h>
#include <linkedList.h>

#define Echo_InputPin PD5 
#define Trigger_OutputPin PD6 

void initBtns(){
  BUTTON_DDR &= ~_BV( BUTTON1 );
  BUTTON_DDR &= ~_BV( BUTTON2 );
  BUTTON_DDR &= ~_BV( BUTTON3 );
  BUTTON_PORT |= _BV( BUTTON1 ); 
  BUTTON_PORT |= _BV( BUTTON2 );
  BUTTON_PORT |= _BV( BUTTON3 );
//   PCICR |= _BV( PCIE1 );
   PCICR |= _BV( PCIE1 );  /* in Pin Change Interrupt Control Register: indicate
                             * which interrupt(s) you want to activate (PCIE0: port B,
                             * PCIE1: port C, PCIE2: port D) */
    PCMSK1 |= _BV( BUTTON1 );   /* In the corresponding Pin Change Mask Register: indicate*/
    PCMSK1 |= _BV( BUTTON2 );   /* In the corresponding Pin Change Mask Register: indicate*/
    PCMSK1 |= _BV( BUTTON3 );
}

void initADC()
{
    
    ADMUX |= ( 1 << REFS0 );    //Set up of reference voltage. We choose 5V as reference.
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );  //Determine a sample rate by setting a division factor. Used division factor: 128
    ADCSRA |= ( 1 << ADEN ); //Enable the ADC
}

void enableBuzzer()
{
    DDRD |= ( 1 << PD3 );   //Buzzer is connected to PD3
}

void playTone( float frequency, uint32_t duration )
{
    uint32_t periodInMicro = ( uint32_t ) ( 1000000 / frequency );  //Calculate the period in microsecs from the freq
    uint32_t durationInMicro = duration * 1000; //We express duration in microsecs
    for ( uint32_t time = 0; time < durationInMicro; time += periodInMicro ) //See tutorial on Music for more info!
    {
        PORTD &= ~( 1 << PD3 );         //turn the buzzer on
        _delay_us( periodInMicro / 2 ); //Wait for the half of the period
        PORTD |= ( 1 << PD3 );          //Turn the buzzer off
        _delay_us( periodInMicro / 2 ); //Wait again for half of the period
    }
}

// disarm sequence is btn 1, btn 2 , btn 3 , btn 3 
int disarmSequence[4];
int disarm=0;
int indexToCheck=0;
int stop=0;
int freq=100;
long duration=0;
long magnitude=0;
uint8_t oldLeds=0b00000000;
uint8_t newLeds=0b00000000;
int timer;

// interrupt used to disarm the bomb
// it iterates through one index of the disarmSequence array every time a button is pushed
// if the code is correct, the index goes above that of the disarm Sequence and disarms the bomb.
ISR( PCINT1_vect ) {
	
    if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
    {
      _delay_us(15000);
      if ( bit_is_clear( BUTTON_PIN, BUTTON1 )){
		if(disarmSequence[indexToCheck]==1){
			indexToCheck++;
		}
		else{
			indexToCheck=0;
		}
        }
      }
    
    if ( bit_is_clear(BUTTON_PIN,PC2))
    {
      _delay_us(10000);
      if(bit_is_clear(BUTTON_PIN,BUTTON2)){
        if(disarmSequence[indexToCheck]==2){
			indexToCheck++;
		}
		else{
			indexToCheck=0;
		}
        }
      }
    
    if (bit_is_clear(BUTTON_PIN,PC3)){
      _delay_us(10000);
      if(bit_is_clear(BUTTON_PIN,BUTTON3)){
		if(disarmSequence[indexToCheck]==3){
			indexToCheck++;
		}
		else{
			indexToCheck=0;
		}
      }
    }
	if(indexToCheck==4){
		stop=1;
		disarm=1;
	}
  
}

// An interrupt triggered by a timer
// This interrupt happens aproximately around 1 second
ISR(TIMER1_OVF_vect){
	timer--;
	printf("time left: %d seconds\n",timer);
	// If statement for increase of frequency in arming stage
	if(timer<5)freq*=2;
	// constant 50 hz frequency after the bomb exploded
	if(timer<1){
		freq=50;
		oldLeds=0b00001111;
	}
	playTone(freq,100);
	TCNT1=49025;
}



void setup() {
	
	initADC();
	initUSART();
  	enableBuzzer();
  	playTone(1000,100);
  	initDisplay();
  	enableAllLeds();
  	lightDownAllLeds();
  	initBtns();
	disarmSequence[0]=1;
	disarmSequence[1]=2;
	disarmSequence[2]=3;
	disarmSequence[3]=3;
	DDRD |= ( 1 << Trigger_OutputPin);
	DDRD &= ~( 1 << Echo_InputPin );
	TCNT1=49025;   // ~1 second timer
 	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);  // Timer mode with 1024 prescler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}





int main()
{
	setup();
	while(1){
		// get the seconds from the potentiometer
		ADCSRA |= ( 1 << ADSC ); 
        loop_until_bit_is_clear( ADCSRA, ADSC );    
        uint16_t value = ADC;   
        value%=231;
		value+=10;
		writeNumber(value);
        if ( bit_is_clear( BUTTON_PIN, BUTTON2)){
			_delay_us(15000);
			if(bit_is_clear(BUTTON_PIN,BUTTON2)){
         	timer=value;
		  	break;
			}

          
        }
		

	}
	sei(); 
	// tick down bomb
	while(timer>4){
	duration=0;
	// get the value from the sensor
	PORTD |= ( 1 << Trigger_OutputPin); 
	_delay_us(2);
	PORTD &= ~( 1 << Trigger_OutputPin); 
	_delay_us(10);
	PORTD |= ( 1 << Trigger_OutputPin); 
	while(bit_is_clear( PIND, Echo_InputPin));
	while(bit_is_set( PIND, Echo_InputPin)){
		_delay_us(1);
		duration++;
	}

	writeNumberAndWait(timer, 100);
	// based on the duration, we light the leds accordingly
	// 0b0000 0000 means no leds are on
	// 0b0000 0100 means the third led is on
	if(duration>3000){
		newLeds=0b00000000;
	}
	else if (duration>2000)
	{
		newLeds=0b00000001;
	}
	else if (duration>1000)
	{
		newLeds=0b00000011;
	}else if (duration>200)
	{
		newLeds=0b00000111;
	}else
	// if the duration is smaller than 200, the proximity is triggered and the bomb explodes.
	{
		newLeds=0b00001111;
		timer=0;
		stop=1;
	}
	if(newLeds!=oldLeds){
		oldLeds=newLeds;
		lightDownAllLeds();
	}
	lightUpMultipleLeds(oldLeds);
	if(stop)break;

	}
	freq=500;
	oldLeds=0b00000000;
	while(timer>0){

	if(timer==3){
		oldLeds=0b00001000;
	}
	else if (timer==2)
	{
		oldLeds=0b00001100;
	}
	else if(timer==1)
	{
		oldLeds=0b00001110;
	}
	lightUpMultipleLeds(oldLeds);
	writeNumber(timer);

	if(stop)break;
	
	}
	lightUpAllLeds();
	
	if(!disarm){
	printf("Bomb explodes");
	while(timer>-5){
		uint32_t periodInMicro = ( uint32_t ) ( 1000000 / 50 );  //Calculate the period in microsecs from the freq
        PORTD &= ~( 1 << PD3 );         //turn the buzzer on
        _delay_us( periodInMicro / 2 ); //Wait for the half of the period
        PORTD |= ( 1 << PD3 );          //Turn the buzzer off
        _delay_us( periodInMicro / 2 ); //Wait again for half of the period

		displayExplosion(timer);

	}
	}
	cli();
	dance();
	
}

