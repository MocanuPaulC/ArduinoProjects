#include "display.h"
#include <stdio.h>
#include <ctype.h>
#include <avr/io.h>
#include <util/delay.h>
// #include <usart.c>
#define SPACE 0xFF

/* Segment byte maps for numbers 0 to 9 */
// CHANGED -0x92 to 0x48 bcs 5 was showing as 9
// 
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                               0xFF, 0x9C, 0xF8, 0X80, 0X90};

/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};



void initDisplay() {
  sbi(DDRD, LATCH_DIO);
  sbi(DDRD, CLK_DIO);
  sbi(DDRB, DATA_DIO);
}
//01011100

const uint8_t ALPHABET_MAP[] = {0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xC2,
                                0x89, 0xCF, 0xE1, 0x8A, 0xC7, 0xEA, 0xC8,
                                0xC0, 0x8C, 0x4A, 0xCC, 0x92, 0x87, 0xC1,
                                0xC1, 0xD5, 0x89, 0x91, 0xA4};


void writeCharToSegment(uint8_t segment, char character){
  if((int)character>=97&&(int)character<=122)character-=32;
  if((int)character<65||(int)character>90)character=' ';
  uint8_t value;
  switch (character)
  {
  case 'A':
      value=ALPHABET_MAP[0];
      break;
    case 'B':
      value=ALPHABET_MAP[1];
      break;
    case 'C':
      value=ALPHABET_MAP[2];
      break;
    case 'D':
      value=ALPHABET_MAP[3];
      break;
    case 'E':
      value=ALPHABET_MAP[4];
      break;
    case 'F':
      value=ALPHABET_MAP[5];
      break;
    case 'G':
      value=ALPHABET_MAP[6];
      break;
    case 'H':
      value=ALPHABET_MAP[7];
      break;
    case 'I':
      value=ALPHABET_MAP[8];
      break;
    case 'J':
      value=ALPHABET_MAP[9];
      break;
    case 'K':
      value=ALPHABET_MAP[10];
      break;
    case 'L':
      value=ALPHABET_MAP[11];
      break;
    case 'M':
      value=ALPHABET_MAP[12];
      break;
    case 'N':
      value=ALPHABET_MAP[13];
      break;
    case 'O':
      value=ALPHABET_MAP[14];
      break;
    case 'P':
      value=ALPHABET_MAP[15];
      break;
    case 'Q':
      value=ALPHABET_MAP[16];
      break;
    case 'R':
      value=ALPHABET_MAP[17];
      break;
    case 'S':
      value=ALPHABET_MAP[18];
      break;
    case 'T':
      value=ALPHABET_MAP[19];
      break;
    case 'U':
      value=ALPHABET_MAP[20];
      break;
    case 'V':
      value=ALPHABET_MAP[21];
      break;
    case 'W':
      value=ALPHABET_MAP[22];
      break;
    case 'X':
      value=ALPHABET_MAP[23];
      break;
    case 'Y':
      value=ALPHABET_MAP[24];
      break;
    case 'Z':
      value=ALPHABET_MAP[25];
      break;
    default:
      value=SPACE;
      break;
    }

  // printf("%d\n",value);
  cbi(PORTD, LATCH_DIO);
  shift(value, MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);

}


// loop through seven segments of LED display and shift the correct bits in the
// data register
void shift(uint8_t val, uint8_t bitorder) {
  uint8_t bit;

  for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    if (bitorder == LSBFIRST) {
      bit = !!(val & (1 << i));
    } else {
      bit = !!(val & (1 << (7 - i)));
    }
    // write bit to register
    if (bit == HIGH)
      sbi(PORTB, DATA_DIO);
    else
      cbi(PORTB, DATA_DIO);
    // if(i==2&&val==5){
    // cbi(PORTB,DATA_DIO);
    // }
    // Trigger the clock pin so the display updates
    sbi(PORTD, CLK_DIO);
    cbi(PORTD, CLK_DIO);
  }
}

//Writes a digit to a certain segment. Segment 0 is the leftmost.
void writeNumberToSegment(uint8_t segment, uint8_t value) {
  cbi(PORTD, LATCH_DIO);
  shift(SEGMENT_MAP[value], MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

void writeString(char* str){
  writeCharToSegment(0,str[0]);
  writeCharToSegment(1,str[1]);
  writeCharToSegment(2,str[2]);
  writeCharToSegment(3,str[3]);
}

void writeStringAndWait(char* str, int delay){
  for (int i = 0; i < delay / 20; i++) {
    writeCharToSegment(0, str[0]);
    _delay_ms(5);
    writeCharToSegment(1, str[1]);
    _delay_ms(5);
    writeCharToSegment(2, str[2]);
    _delay_ms(5);
    writeCharToSegment(3, str[3]);
    _delay_ms(5);
  }
}

//Writes a nuber between 0 and 9999 to the display. To be used in a loop...
void writeNumber(int number) {
  if (number < 0 || number > 9999) return;
  writeNumberToSegment(0, number / 1000);
  writeNumberToSegment(1, (number / 100) % 10);
  writeNumberToSegment(2, (number / 10) % 10);
  writeNumberToSegment(3, number % 10);
}

//Writes a number between 0 and 9999 to the display and makes sure that it stays there a certain number of millisecs.
//Note: the timing is approximate; the amount of time writeNumberToSegment takes is not accounted for...
void writeNumberAndWait(int number, int delay) {
  if (number < 0 || number > 9999) return;
  for (int i = 0; i < delay / 20; i++) {
    writeNumberToSegment(0, number / 1000);
    _delay_ms(5);
    writeNumberToSegment(1, (number / 100) % 10);
    _delay_ms(5);
    writeNumberToSegment(2, (number / 10) % 10);
    _delay_ms(5);
    writeNumberToSegment(3, number % 10);
    _delay_ms(5);
  }
}