#include "Button.h"
#include "Definitions.h"
#include "LiquidCrystalDisplay.h"
#include "Timer.h"
#include "Switch.h"
#include <stdio.h>
#include <stdlib.h>

LiquidCrystalDisplay theLCD(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
Timer timerWhite(10);
Timer timerBlack(10);
Button buttonUp    (PIN_BUTTON_UP   );
Button buttonDown  (PIN_BUTTON_DOWN );
Button buttonLeft  (PIN_BUTTON_LEFT );
Button buttonRight (PIN_BUTTON_RIGHT);
Button buttonEnter (PIN_BUTTON_ENTER);
Switch playerSwitch(PIN_PLAYER_SWITCH);
StateMachineStates theState = StateMachineStates::INITIALIZING;

uint32_t timerLenght = 0;

void waitForAnyBottomToBePressed()
{
    while(!buttonEnter.isPressed() && !buttonUp.isPressed() && !buttonDown.isPressed() && !buttonLeft.isPressed() && !buttonRight.isPressed()) {}
}
void waitForBottomToBePressed(Button theBottom)
{
    while(!theBottom.isPressed()) {}
}


void setup() 
{

    // code interrupt generated every TIMER0_COMPA reaches 0xAF
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);


}

SIGNAL(TIMER0_COMPA_vect) 
{
    if(theState == StateMachineStates::RUNNING)
    {
        if(playerSwitch.isWhiteTurn()) timerWhite.updateTimer();
        else                           timerBlack.updateTimer();
    }
}


void loop() 
{
    switch(theState)
    {
        case StateMachineStates::INITIALIZING:
        {
            theLCD.initialize();
            theLCD.firstLine();
            theLCD.appendWord ("  Chess  Timer  ");
            theLCD.secondLine();
            theLCD.appendWord (" press a button ");
            waitForAnyBottomToBePressed();
            theState = StateMachineStates::CONFIGURING;
            break;
        }

        case StateMachineStates::CONFIGURING:
        {
            uint32_t theTimerLength = theLCD.setTimerSetting(buttonUp, buttonDown, buttonLeft, buttonRight, buttonEnter);
            if(theTimerLength == 0)
            {
                theLCD.secondLine();
                theLCD.appendWord ("Error, timer = 0");
                delay(3000);
                break;
            } 
            timerWhite.setTimer(theTimerLength);
            timerBlack.setTimer(theTimerLength);
            theLCD.writeWord (timerWhite.getTimerString(true ));
            theLCD.appendWord( "  " );
            theLCD.appendWord(timerBlack.getTimerString(false));
            playerSwitch.waitForStart();
            theState = StateMachineStates::RUNNING;
            break;
        }

        case StateMachineStates::RUNNING:
        {
            if(timerWhite.isTimeOver() || timerBlack.isTimeOver()) theState = StateMachineStates::STOPPING;
            if(buttonEnter.isPressed()) theState = StateMachineStates::PAUSING;
            theLCD.writeWord (timerWhite.getTimerString(true ));
            theLCD.appendWord( "  " );
            theLCD.appendWord(timerBlack.getTimerString(false));
            // if(timerWhite.isTimeOver() || timerBlack.isTimeOver()) theState = StateMachineStates::STOPPING;
            break;
        }

        case StateMachineStates::PAUSING:
        {
            theLCD.secondLine();
            theLCD.appendWord ("< resume  stop >");
            bool isLeftPressed = false;
            bool isRightPressed = false;
            do{
                isLeftPressed=buttonLeft.isPressed(); 
                isRightPressed=buttonRight.isPressed(); 
            }while(!isLeftPressed && !isRightPressed);
            if(isLeftPressed) theState = StateMachineStates::RESUMING;
            else              theState = StateMachineStates::STOPPING;
            break;
        }

        case StateMachineStates::RESUMING:
        {
            theLCD.secondLine();
            theLCD.appendWord ("White      Black");
            theState = StateMachineStates::RUNNING;
            break;
        }

        case StateMachineStates::STOPPING:
        {
            theLCD.secondLine();
            theLCD.appendWord (" press a button ");
            waitForAnyBottomToBePressed();
            theState = StateMachineStates::CONFIGURING;
            break;
        }

        default:
        {
            theLCD.writeWord ("Error");
            theLCD.appendWord (" press a button ");
            waitForAnyBottomToBePressed();
            theState = StateMachineStates::INITIALIZING;
            break;
        }
    }
}
