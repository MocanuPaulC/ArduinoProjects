#ifndef BUTTON
#define BUTTON

#include <util/delay.h>
#include <avr/io.h>
#include <paulLib.h>
#include <avr/interrupt.h>
// #include <usart.c>

int btn1=0;
int btn2=0;
int btn3=0;


void enableButton(int button){
    int btnToEnable=PC1;
    if(button==1){
        btnToEnable=PC1;
    }
    else if (button==2)
    {
        btnToEnable=PC2;
    }
    else{
        btnToEnable=PC3;
    }
    
    DDRC &=  ~ _BV(btnToEnable);   
    // BUTTON_DDR &= ~_BV(PC3);
    PORTC |= _BV(btnToEnable);
}


void buttonReleased(int button){
    if(button==1){
        if(bit_is_set(PINC,PC1)&&btn1==1){
            btn1=0;
            printString("button1 released!");
        }
    }
    else if(button==2){
        if(bit_is_set(PINC,PC2)&&btn2==1){
            btn2=0;
            printString("button2 released!");
        }
    }
    else if(button==3){
        if(bit_is_set(PINC,PC3)&&btn3==1){
            btn3=0;
            printString("button3 released!");
        }
    }
}

void buttonPushed(int button){
    if (bit_is_clear(PINC, PC1)){
    //     //We wait 1000 microseconds and check again (debounce!)
        _delay_us( 5000 );
    if(button==1){
        if(bit_is_clear(PINC,PC1)){
            if(btn1==0){
                btn1=1;
                printString("button1 pushed");
            }
        }
    }
    }
    if(bit_is_clear(PINC,PC2)){
    if (button==2)
    {
        if(bit_is_clear(PINC,PC2)){
            if(btn2==0){
                btn2=1;
                printString("button2 pushed");
            }
        }

    }
    }
    if (bit_is_clear(PINC,PC3))
    {
        if(bit_is_clear(PINC,PC3)){
            if(btn3==0){
                btn3=1;
                printString("button3 pushed");
            }
        }
        
    }
    // button=-1;
    return;
}

#endif