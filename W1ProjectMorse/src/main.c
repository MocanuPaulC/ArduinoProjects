#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h>
#include <stdlib.h>
#include <paulLib.h>
#include <string.h>
#include <ctype.h>
int letters[36][5];

int main()
{

  // I haven't been able to figure out how to use the serial monitor
  // only works with letters, numbers and spaces
  enableAllLeds();
  lightDownAllLeds();
  initialiseLetters();
  startSequence();

  char message[] = "Hello World";
  
  int stringToInt[strlen(message)];
  for(int i = 0 ; i <strlen(message);i++){
  message[i]=tolower((unsigned char)message[i]);
    switch (message[i])
    {
    case ' ':
      stringToInt[i]=-1;
      break;
    case '0':
      stringToInt[i]=0;
      break;
    case '1':
      stringToInt[i]=1;
      break;
    case '2':
      stringToInt[i]=2;
      break;
    case '3':
      stringToInt[i]=3;
      break;
    case '4':
      stringToInt[i]=4;
      break;
    case '5':
      stringToInt[i]=5;
      break;
    case '6':
      stringToInt[i]=6;
      break;
    case '7':
      stringToInt[i]=7;
      break;
    case '8':
      stringToInt[i]=8;
      break;
    case '9':
      stringToInt[i]=9;
      break;
    case 'a':
      stringToInt[i]=10;
      break;
    case 'b':
      stringToInt[i]=11;
      break;
    case 'c':
      stringToInt[i]=12;
      break;
    case 'd':
      stringToInt[i]=13;
      break;
    case 'e':
      stringToInt[i]=14;
      break;
    case 'f':
      stringToInt[i]=15;
      break;
    case 'g':
      stringToInt[i]=16;
      break;
    case 'h':
      stringToInt[i]=17;
      break;
    case 'i':
      stringToInt[i]=18;
      break;
    case 'j':
      stringToInt[i]=19;
      break;
    case 'k':
      stringToInt[i]=20;
      break;
    case 'l':
      stringToInt[i]=21;
      break;
    case 'm':
      stringToInt[i]=22;
      break;
    case 'n':
      stringToInt[i]=23;
      break;
    case 'o':
      stringToInt[i]=24;
      break;
    case 'p':
      stringToInt[i]=25;
      break;
    case 'q':
      stringToInt[i]=26;
      break;
    case 'r':
      stringToInt[i]=27;
      break;
    case 's':
      stringToInt[i]=28;
      break;
    case 't':
      stringToInt[i]=29;
      break;
    case 'u':
      stringToInt[i]=30;
      break;
    case 'v':
      stringToInt[i]=31;
      break;
    case 'w':
      stringToInt[i]=32;
      break;
    case 'x':
      stringToInt[i]=33;
      break;
    case 'y':
      stringToInt[i]=34;
      break;
    case 'z':
      stringToInt[i]=35;
      break;
    default:
      break;
    }
    
  }


  for(int i = 0 ; i <strlen(message);i++){
    if(stringToInt[i]==-1){
      _delay_ms(2800);
      break;
    }
    else{
    _delay_ms(1200);
    }
    for (int j = 0 ; j <5;j++){
      if(letters[stringToInt[i]][j]==2)break;
      
      switch (letters[stringToInt[i]][j])
      {
      case 0:
        lightUpAllLeds();
        _delay_ms(400);
        lightDownAllLeds();
        _delay_ms(400);
        break;
      case 1:
        lightUpAllLeds();
        _delay_ms(1200);
        lightDownAllLeds();
        _delay_ms(400);
        break;
      default:
        break;
      }
    }
  }

  dance();
 
}
// A=65 Z=90, a=97, z=122, 0=48, 9=57
// 0 means short unit, 1 means long unit, 2 means quit

//for (size_t i = 0; i < strlen(str); ++i) {
     //   printf("%c", tolower((unsigned char) str[i]));
  //}
//





void initialiseLetters(){
   //zero
  letters[0][0]=1;
  letters[0][1]=1;
  letters[0][2]=1;
  letters[0][3]=1;
  letters[0][4]=1;
  //one
  letters[1][0]=0;
  letters[1][1]=1;
  letters[1][2]=1;
  letters[1][3]=1;
  letters[1][4]=1;
  //two
  letters[2][0]=0;
  letters[2][1]=0;
  letters[2][2]=1;
  letters[2][3]=1;
  letters[2][4]=1;
  //three
  letters[3][0]=0;
  letters[3][1]=0;
  letters[3][2]=0;
  letters[3][3]=1;
  letters[3][4]=1;
  //four
  letters[4][0]=0;
  letters[4][1]=0;
  letters[4][2]=0;
  letters[4][3]=0;
  letters[4][4]=1;
  //five
  letters[5][0]=0;
  letters[5][1]=0;
  letters[5][2]=0;
  letters[5][3]=0;
  letters[5][4]=0;
  //six
  letters[6][0]=1;
  letters[6][1]=0;
  letters[6][2]=0;
  letters[6][3]=0;
  letters[6][4]=0;
  //seven
  letters[7][0]=1;
  letters[7][1]=1;
  letters[7][2]=0;
  letters[7][3]=0;
  letters[7][4]=0;
  //eight
  letters[8][0]=1;
  letters[8][1]=1;
  letters[8][2]=1;
  letters[8][3]=0;
  letters[8][4]=0;
  //nine
  letters[9][0]=1;
  letters[9][1]=1;
  letters[9][2]=1;
  letters[9][3]=1;
  letters[9][4]=0;
  //a
  letters[10][0]=0;
  letters[10][1]=1;
  letters[10][2]=2;
  //b
  letters[11][0]=1;
  letters[11][1]=0;
  letters[11][2]=0;
  letters[11][3]=0;
  letters[11][4]=2;
  //c
  letters[12][0]=1;
  letters[12][1]=0;
  letters[12][2]=1;
  letters[12][3]=0;
  letters[12][4]=2;
  //d
  letters[13][0]=1;
  letters[13][1]=0;
  letters[13][2]=0;
  letters[13][3]=2;
  //e
  letters[14][0]=0;
  letters[14][1]=2;
  //f
  letters[15][0]=0;
  letters[15][1]=0;
  letters[15][2]=1;
  letters[15][3]=0;
  letters[15][4]=2;
  //g
  letters[16][0]=1;
  letters[16][1]=1;
  letters[16][2]=0;
  letters[16][3]=2;
  //h
  letters[17][0]=0;
  letters[17][1]=0;
  letters[17][2]=0;
  letters[17][3]=0;
  letters[17][4]=2;
  //i
  letters[18][0]=0;
  letters[18][1]=0;
  letters[18][2]=2;
  //j
  letters[19][0]=0;
  letters[19][1]=1;
  letters[19][2]=1;
  letters[19][3]=1;
  letters[19][4]=2;
  //k
  letters[20][0]=1;
  letters[20][1]=0;
  letters[20][2]=1;
  letters[20][3]=2;
  //l
  letters[21][0]=0;
  letters[21][1]=1;
  letters[21][2]=0;
  letters[21][3]=0;
  letters[21][4]=2;
  //m
  letters[22][0]=1;
  letters[22][1]=1;
  letters[22][2]=2;
  //n
  letters[23][0]=1;
  letters[23][1]=0;
  letters[23][2]=2;
  //o
  letters[24][0]=1;
  letters[24][1]=1;
  letters[24][2]=1;
  letters[24][3]=2;
  //p
  letters[25][0]=0;
  letters[25][1]=1;
  letters[25][2]=1;
  letters[25][3]=0;
  letters[25][4]=2;
  //q
  letters[26][0]=1;
  letters[26][1]=1;
  letters[26][2]=0;
  letters[26][3]=1;
  letters[26][4]=2;
  //r
  letters[27][0]=0;
  letters[27][1]=1;
  letters[27][2]=0;
  letters[27][3]=2;
  //s
  letters[28][0]=0;
  letters[28][1]=0;
  letters[28][2]=0;
  letters[28][3]=2;
  //t
  letters[29][0]=1;
  letters[29][1]=2;
  //u
  letters[30][0]=0;
  letters[30][1]=0;
  letters[30][2]=1;
  letters[30][3]=1;
  letters[30][4]=2;
  //v
  letters[31][0]=0;
  letters[31][1]=0;
  letters[31][2]=0;
  letters[31][3]=1;
  letters[31][4]=2;
  //w
  letters[32][0]=0;
  letters[32][1]=1;
  letters[32][2]=1;
  letters[32][3]=2;
  //x
  letters[33][0]=1;
  letters[33][1]=0;
  letters[33][2]=0;
  letters[33][3]=1;
  letters[33][4]=2;
  //y
  letters[34][0]=1;
  letters[34][1]=0;
  letters[34][2]=1;
  letters[34][3]=1;
  letters[34][4]=2;
  //z
  letters[35][0]=1;
  letters[35][1]=1;
  letters[35][2]=0;
  letters[35][3]=0;
  letters[35][4]=2;

}
