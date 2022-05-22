#define __DELAY_BACKWARD_COMPATIBLE__
#include <paulLib.h>
#include <util/delay.h>
#include <avr/io.h>
#include <display.c>
#include <stdio.h>
#include <button.h>
#include <linkedList.h>
#define GRAVITY 1.622
#define QUARTERTANK 375

//FREQUENCIES OF THE NOTES
#define C5  523.250
#define D5  587.330
#define E5  659.250
#define F5  698.460
#define G5  783.990
#define A5  880.00
#define B5  987.770
#define C6  1046.500


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


void initBtns(){
  BUTTON_DDR &= ~_BV( BUTTON2 );
  BUTTON_PORT |= _BV( BUTTON2 );
  PCICR |= _BV( PCIE1 );
  // PCMSK1 |= _BV( BUTTON2 );   
  sei();


}

int second=0;
float currentSpeed=100;
int distance=9999;
node_t *head;
int fuelReserve=1500;
uint8_t ledsToDisplay=0b00000000;
uint8_t ledToFlash=0b00001111;
int burst=0;
int sem2=0;

ISR(TIMER1_OVF_vect){
  second+=1;
  burst=0;
  while(bit_is_clear( BUTTON_PIN, BUTTON2)){
    if(burst<50){
    burst+=5;
    }
    else{
      break;
    }
    writeNumberAndWait(burst,100);
    // _delay_ms(300);

  }
  int nrOfLeds;
  ledsToDisplay=0b00000000;
  for(int i = 1;i<5;i++){
    if(fuelReserve>=i*QUARTERTANK){
      nrOfLeds=i;
      // printf(" fuel reserve > %d \n",i*QUARTERTANK);
    }
  }
  for(int i=0;i<nrOfLeds;i++){
      ledsToDisplay=ledsToDisplay<<1;
      ledsToDisplay|=0b00000001;
      // printBinaryByte(ledsToDisplay);
      // printf("\n");
    }
    distance-=currentSpeed;
    currentSpeed+=GRAVITY-burst/5;
    fuelReserve-=burst;
    log_t val;
    val.burst=burst;
    val.currentSpeed=currentSpeed;
    val.distance=distance;
    val.fuelReserve=fuelReserve;
    val.second=second;
    push(head,val);


    // printf("%d is fuel reserver\n",fuelReserve);
    TCNT1=49025; // one second interrupt
}


void initLinkedList(){
  head=NULL;
  head = (node_t *) malloc(sizeof(node_t));
  if(head==NULL){
    return 1;
  }

  head->val.second=0;
  head->val.burst=0;
  head->val.currentSpeed=currentSpeed;
  head->val.fuelReserve=fuelReserve;
  head->val.distance=distance;
  head->next=NULL;
}

int main(){
  
  initUSART();
  enableBuzzer();
  playTone(1,100);
  initDisplay();
  enableAllLeds();
  lightDownAllLeds();
  enableAllButtons();
  initBtns();
  initLinkedList();


  TCNT1=49025;
  TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);  // Timer mode with 1024 prescler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei(); 

  while (distance>3)
  {
    showParameters();
    if(fuelReserve<0){
    playTone(C5,100);
    playTone(D5,100);
  }
  }
  cli();
  
  if(currentSpeed<=5){
    printf("congratulation!\n");
    // playTone(C6,300);
    sem2=1;
  }
  else{
    printf("you crashed!\n");
    playTone(C5,300);
    _delay_ms(100);
    playTone(D5,300);
    _delay_ms(100);
    playTone(E5,300);
  }

  print_list(head);

  return 0;
}
// 1500/4 = 375

void showParameters(){
  ledToFlash=ledsToDisplay<<1;
  ledToFlash|=0b00000001;
  ledToFlash^=ledsToDisplay;
  int delay=(fuelReserve%375)/2+40;
  lightDownAllLeds();
  lightUpMultipleLeds(ledsToDisplay);
  lightUpMultipleLeds(ledToFlash);
  writeNumberAndWait(distance,delay);
  lightDownMultipleLeds(ledToFlash);
  writeNumberAndWait(distance,delay);


}

