/*
 * TEJ4M mainLineFollow1.c
 * Author: Daniel Liang
 * Date: 2016/11/22
 * Purpose: The first line following course
 * PIC used: PIC24F32KA302 operating at 8MHz
 * I/O ports used and hardware attached:
 * RB15 connected to blue LED
 * RA0 connected to pushbutton
 * RA0 connected to first IR sensor
 * RA1 connected to second IR sensor
 * L293:
 * pin 2 to RB15
 * pin 7 to RB14
 * pin 10 to RB13
 * pin 13 to RB12
*/

//Include Files
#include "p24F32KA302.h"
#include "configBits.h"
#include "delay.h"

//constants for pins
const int LF = 9; //Left red 1A
const int LR = 8; //left green 2A
const int RF = 6; //Right red 3A
const int RR = 7; //Right green 4A
const int left = 0; //left sensor
const int right = 1; //right sensor
const int indicator = 15; //indicator of robot

//constants for direction of robot
const int FWD = 0; //go forward
const int LEFT = 1; //turn left
const int RIGHT = 2; //turn right
const int CW = 3; //turn clockwise
const int CCW = 4; //turn counterclockwise
const int BWD = 5; //reverse
const int STOP = 6; //stop the motors
const int RLEFT = 7; //reverse left
const int RRIGHT = 8; //reverse right

//Local Function Prototypes
void initTimer (void);
void delay (unsigned long milli);

//main function
int main ()
{
    //initialization process
    initTimer(); //initialize peripherals
    TRISB=0; //PORTB all outputs
    LATB=0; // initialize PORTB
    TRISA=0xFF; //PORTA all [digital] inputs
    ANSA=0; //all digital inputs

    //local variables
    int finish = 0; //determine far line (finish) or starting line
    int pressed = 0; //used for positive edge
    int unpressed = 0; //used for negative edge toggle on/off the motors
    int start = 0; //start line indicator

    while(1){//repeat forever and read inputs
        //if the switch was pressed and motors are off
        if(digitalRead(2) && !pressed && !unpressed){
            pressed = 1; //positive edge (wait)
            finish = 0; //reset finish (far) line indicator
            start = 0; //reset start line indicator
        }

        //when the switch is released, negative edge trigger
        if(!digitalRead(2) && pressed){
            unpressed = 1; //motors are running
            digitalWrite(indicator, 1); //indicating motors are ready

            //move from start line
            if(start == 0){
                while(!digitalRead(left) && !digitalRead(right) && !digitalRead(2)){
                    drive(FWD);
                }
                start = 1; //start line cleared
            }

            //If robot on line, move forward
            if(digitalRead(left) && digitalRead(right)) drive(FWD);

            //If robot right of line, turn left
            else if(!digitalRead(left) && digitalRead(right)) drive(CCW);

            //If robot left of line, turn right
            else if(digitalRead(left) && !digitalRead(right)) drive(CW);

            //If robot reaches far line, turn around
            else if(!digitalRead(left) && !digitalRead(right) && !finish){
                //keep turning ccs while on the black line
                while(!digitalRead(left) && !digitalRead(right) && !digitalRead(2)){
                    drive(CCW);
                }
                //keep turning while the left sensor is off the line
                while(digitalRead(left) && !digitalRead(right) && !digitalRead(2)){
                    drive(CCW);
                }
                //keep turning while both sensors are in white area
                while(digitalRead(left) && digitalRead(right) && !digitalRead(2)){
                    drive(CCW);
                }
                finish = 1; //turn complete
            }

            //If robot returns to start line, exit program
            else if(!digitalRead(left) && !digitalRead(right) && finish){
                drive(STOP);
                pressed = 0;
            }

        }//end if (motor sequence)

        //if the switch is pressed and motors are on
        if(digitalRead(2) && unpressed){
            pressed = 0; //positive edge (wait)
        }

        //when the switch is released, negative edge trigger or program is finished
        if(!digitalRead(2) && !pressed){
            drive(STOP);
            unpressed = 0; //motors are off
            finish = 0; //reset finish (far) line indicator
            start = 0; //reset start line indicator
            digitalWrite(indicator, 0); //incating motors are off
        }
    }//end while
}//end main

//drives the robot in a direction
void drive(int direction){
    if(direction == FWD){
        digitalWrite(LF, 1);
        digitalWrite(LR, 0);
        digitalWrite(RF, 1);
        digitalWrite(RR, 0);
    }
    else if(direction == LEFT){
        digitalWrite(LF, 0);
        digitalWrite(LR, 0);
        digitalWrite(RF, 1);
        digitalWrite(RR, 0);
    }
    else if(direction == RIGHT){
        digitalWrite(LF, 1);
        digitalWrite(LR, 0);
        digitalWrite(RF, 0);
        digitalWrite(RR, 0);
    }
    else if(direction == CW){
        digitalWrite(LF, 1);
        digitalWrite(LR, 0);
        digitalWrite(RF, 0);
        digitalWrite(RR, 1);
    }
    else if(direction == CCW){
        digitalWrite(LF, 0);
        digitalWrite(LR, 1);
        digitalWrite(RF, 1);
        digitalWrite(RR, 0);
    }
    else if(direction == BWD){
        digitalWrite(LF, 0);
        digitalWrite(LR, 1);
        digitalWrite(RF, 0);
        digitalWrite(RR, 1);
    }
    else if(direction == STOP){
        digitalWrite(LF, 0);
        digitalWrite(LR, 0);
        digitalWrite(RF, 0);
        digitalWrite(RR, 0);
    }
    else if(direction == RLEFT){
        digitalWrite(LF, 0);
        digitalWrite(LR, 0);
        digitalWrite(RF, 0);
        digitalWrite(RR, 1);
    }
    else if(direction == RRIGHT){
        digitalWrite(LF, 0);
        digitalWrite(LR, 1);
        digitalWrite(RF, 0);
        digitalWrite(RR, 0);
    }
}//end drive

//This function assigns a state to pins RB6 to RB15
void digitalWrite(int pin, int power){
    switch(pin){
        case 6:
            LATBbits.LATB6 = power;
        break;
        case 7:
            LATBbits.LATB7 = power;
        break;
        case 8:
            LATBbits.LATB8 = power;
        break;
        case 9:
            LATBbits.LATB9 = power;
        break;
        case 10:
            LATBbits.LATB10 = power;
        break;
        case 11:
            LATBbits.LATB11 = power;
        break;
        case 12:
            LATBbits.LATB12 = power;
        break;
        case 13:
            LATBbits.LATB13 = power;
        break;
        case 14:
            LATBbits.LATB14 = power;
        break;
        case 15:
            LATBbits.LATB15 = power;
        break;
    }//end case
}//end digitalWrite

//This function reads the state of the input pins RA0 to RA4
int digitalRead(int pin){
    switch(pin){
        case 0:
            return PORTAbits.RA0;
        break;
        case 1:
            return PORTAbits.RA1;
        break;
        case 2:
            return PORTAbits.RA2;
        break;
        case 3:
            return PORTAbits.RA3;
        break;
        case 4:
            return PORTAbits.RA4;
        break;
    }//end case
}//end digitalRead