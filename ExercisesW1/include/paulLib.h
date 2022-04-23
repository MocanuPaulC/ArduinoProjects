#ifndef PAULLIB
#define PAULLIB
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h>
#include <stdio.h>
#include <time.h>
// #include <Arduino.h>

void enableOneLed(int led){
    DDRB |= (1<< (PB2 + led));
}


void enableMultipleLeds(uint8_t leds){
    uint8_t ledNumber = 0b00000001;
    for(int i = 0 ; i <4;i++){
        if((leds & ledNumber)!=0){
            enableOneLed(i);
        }
        ledNumber=ledNumber<<1;
    }

};


void enableAllLeds(){
    enableMultipleLeds(0b00001111);
}

void lightUpOneLed(int led){
    PORTB &= ~( 1 << (PB2 + led));
}

void lightUpMultipleLeds(uint8_t leds){
    uint8_t ledNumber = 0b00000001;
    for(int i = 0 ; i <4;i++){
        if((leds & ledNumber)!=0){
            lightUpOneLed(i);
        }
        ledNumber=ledNumber<<1;
    }
}

void lightUpAllLeds(){
    lightUpMultipleLeds(0b00001111);
}

void lightDownOneLed(int led){
    PORTB |= ( 1 << ( PB2 + led ));
}

void lightDownMultipleLeds(uint8_t leds){
    uint8_t ledNumber = 0b00000001;
    for(int i = 0 ; i <4;i++){
        if((leds & ledNumber)!=0){
            lightDownOneLed(i);
        }
        ledNumber=ledNumber<<1;
    }
}

void lightDownAllLeds(){
    lightDownMultipleLeds(0b00001111);
}


void dimLed(int ledNumber, int percentage, int duration){

    float toDelay = percentage/100.0;

    switch (percentage)
        {
        case 0 :
            lightDownOneLed(ledNumber);
            _delay_ms(duration);
            // lightUpOneLed(ledNumber);
            // _delay_ms(1000);
            // duration=-1;
            break;
        case 100:
            lightUpOneLed(ledNumber);
            _delay_ms(duration);
            // lightDownOneLed(ledNumber);
            // duration=-1;
            break;
        default:

           while(duration>0){
        // delay(1-toDelay);
            _delay_ms(1-toDelay);
            lightUpOneLed(ledNumber);
            _delay_ms(toDelay);
        // delay(toDelay);
            lightDownOneLed(ledNumber);
            duration-=toDelay;
           }
           break;
    }
}

void fadeInLed(int led, int duration){
    lightDownOneLed(led);
    for(int i = 0 ; i < duration;i++){
        dimLed(led,i/(duration/100),1);
    }
    dimLed(led,100,duration/100);
}
void fadeOutLed(int led, int duration){
    lightUpOneLed(led);
    for(int i = duration; i >= 0;i--){
        dimLed(led,i/(duration/100),1);
    }
    dimLed(led,0,duration/100);
}
    
void ledChaos(){
    
  srand(0);

  int randLed;
  int randDuration;
  int randCommand;
  int randPerc;
  while(1){
    randPerc=rand()%101;
    randCommand=rand()%5;
    randLed=rand() % 4;
    randDuration=(rand() % 901)+100;

    switch (randCommand)
    {
    case 0:
      // lightUpAllLeds();
      break;
    case 1:
      lightDownAllLeds();
      break;
    case 2:
      lightUpOneLed(randLed);
      break;
    case 3:
      lightDownOneLed(randLed);
      break;
    case 4:
      fadeInLed(randLed,randDuration);
      break;
    case 5:
      // fadeOutLed(randLed,randDuration);
      break;
    case 6:
      dimLed(randLed,randPerc,randDuration);
    default:
      break;
    }
    

    // fadeInLed(1,1000);
    // fadeOutLed(1,1000);

  }
}

void lightUpLedsCons(){
    for(int i = 0 ; i < 4;i++){
        fadeInLed(i,300);
    }
}
void lightDownLedsCons(){
    for(int i = 3 ; i >=0;i--){
        fadeOutLed(i,300);
    }
}

void flashLed(int led,int nrOfFlashes){
    for(int i = 0 ; i < nrOfFlashes; i++){
        lightUpOneLed(led);
        _delay_ms(50);
        lightDownOneLed(led);
        _delay_ms(50);
    }
}

void incrementLedBurn(int led){
    for(int i = 10 ; i <=1000; i+=50){
        lightUpOneLed(led);
        _delay_ms(i);
        lightDownOneLed(led);
        _delay_ms(50);
    }
}

void startSequence(){
  lightUpAllLeds();
  _delay_ms(500);
  lightDownAllLeds();
  _delay_ms(500);
  lightUpMultipleLeds(0b00000111);
  _delay_ms(500);
  lightDownAllLeds();
  _delay_ms(500);
  lightUpMultipleLeds(0b00000011);
  _delay_ms(500);
  lightDownAllLeds();
  _delay_ms(500);
  lightUpMultipleLeds(0b00000001);
  _delay_ms(500);
  lightDownAllLeds();
}

void dance(){
    lightUpLedsCons();
    lightDownLedsCons();
    lightUpLedsCons();
    lightDownLedsCons();
    lightUpLedsCons();
    lightDownLedsCons();
    lightUpLedsCons();
    lightDownLedsCons();
}




#endif