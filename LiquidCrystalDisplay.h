#ifndef LIQUID_CRYSTAL_DISPLAY_H
#define LIQUID_CRYSTAL_DISPLAY_H

class LiquidCrystalDisplay
{
public:
    LiquidCrystalDisplay(uint8_t registerSelect, uint8_t enable, uint8_t digital4, uint8_t digital5, uint8_t digital6, uint8_t digital7);
    ~LiquidCrystalDisplay();

    void initialize();
    void writeWord(String theWord);
    // void setLineAndPosition(bool line, uint8_t position = 0);
    void appendWord(String theWord);
    void firstLine();
    void secondLine();
    void showCursor();
    void hideCursor();
    uint32_t setTimerSetting(Button& buttonUp, Button& buttonDown, Button& buttonLeft, Button& buttonRight, Button& buttonEnter);
    void shiftCursorToTheRight();
    void shiftCursorToTheLeft();

private:

    void write4bitValue(uint8_t theValue, bool isInstruction = true);
    void write8bitValue(uint8_t theValue, bool isInstruction = true);
    bool updateCursorPosition();

    uint8_t registerSelect_ ;
    uint8_t enable_         ;
    uint8_t digital4_       ;
    uint8_t digital5_       ;
    uint8_t digital6_       ;
    uint8_t digital7_       ;
    uint8_t currentPosition_;
    char charVector[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};



};


LiquidCrystalDisplay::LiquidCrystalDisplay(uint8_t registerSelect, uint8_t enable, uint8_t digital4, uint8_t digital5, uint8_t digital6, uint8_t digital7)
: registerSelect_ (registerSelect)
, enable_         (enable        )
, digital4_       (digital4      )
, digital5_       (digital5      )
, digital6_       (digital6      )
, digital7_       (digital7      )
, currentPosition_(0             )
{
}

LiquidCrystalDisplay::~LiquidCrystalDisplay()
{
}

void LiquidCrystalDisplay::initialize()
{
    pinMode(registerSelect_,OUTPUT);
    pinMode(enable_        ,OUTPUT);
    pinMode(digital4_      ,OUTPUT);
    pinMode(digital5_      ,OUTPUT);
    pinMode(digital6_      ,OUTPUT);
    pinMode(digital7_      ,OUTPUT);

    digitalWrite(registerSelect_, 0 );
    digitalWrite(enable_        , 0 );
    digitalWrite(digital4_      , 0 );
    digitalWrite(digital5_      , 0 );
    digitalWrite(digital6_      , 0 );
    digitalWrite(digital7_      , 0 );

    delay(16);

    write4bitValue(0x3);

    delay(5);

    write4bitValue(0x3);

    delay(1);

    write4bitValue(0x3);

    write4bitValue(0x2); //set LCD to recive 4 bit command

    //set LCD to recive 4 bit command, two line and highest resolution
    write8bitValue(0x2C);

    //clear display
    write8bitValue(0x01);

    //set display off
    write8bitValue(0x08);

    //Set cursor increment to the right, set text tp the left
    write8bitValue(0x06);

    //set display on
    write8bitValue(0x0C);



}

void LiquidCrystalDisplay::write4bitValue(uint8_t theValue, bool isInstruction)
{
    digitalWrite(enable_, 1);
    delay(1);

    digitalWrite(registerSelect_, isInstruction ? 0 : 1 );
    digitalWrite(digital4_      , (theValue >> 0) & 0x1 );
    digitalWrite(digital5_      , (theValue >> 1) & 0x1 );
    digitalWrite(digital6_      , (theValue >> 2) & 0x1 );
    digitalWrite(digital7_      , (theValue >> 3) & 0x1 );

    delay(1);
    digitalWrite(enable_, 0);
    delay(1);
}


void LiquidCrystalDisplay::write8bitValue(uint8_t theValue, bool isInstruction)
{
    write4bitValue((theValue >> 4) & 0xF, isInstruction);
    write4bitValue((theValue >> 0) & 0xF, isInstruction);
}

void LiquidCrystalDisplay::writeWord(String theWord)
{

    for(firstLine(); currentPosition_<theWord.length(); )
    {
        write8bitValue(theWord[currentPosition_], false);
        if(!updateCursorPosition()) break;
    }
}

void LiquidCrystalDisplay::appendWord(String theWord)
{
    for(uint8_t position = 0; position<theWord.length(); ++position)
    {
        write8bitValue(theWord[position], false);
        if(!updateCursorPosition()) break;
    }
}

// void LiquidCrystalDisplay::setLineAndPosition(bool line, uint8_t position)
// {
//     currentPosition_ = (line ? 1<<6 : 0) | (position & 0xF);
//     write8bitValue(0x80 || currentPosition_);
// }

void LiquidCrystalDisplay::firstLine()
{
    write8bitValue(0x02);
    currentPosition_ = 0;
}

void LiquidCrystalDisplay::secondLine()
{
    write8bitValue(0xC0);
    currentPosition_ = 0x10;
}


bool LiquidCrystalDisplay::updateCursorPosition()
{
    ++currentPosition_;
    if(currentPosition_ == 0x10) secondLine();
    if(currentPosition_ >= 0x20)
    {
        firstLine();
        return false;
    }
    return true;
}

void LiquidCrystalDisplay::showCursor()
{
    write8bitValue(0x0E);
}

void LiquidCrystalDisplay::hideCursor()
{
    write8bitValue(0x0C);
}

uint32_t LiquidCrystalDisplay::setTimerSetting(Button& buttonUp, Button& buttonDown, Button& buttonLeft, Button& buttonRight, Button& buttonEnter)
{
    uint8_t maxVector[16] = {0,0,0,0,0,9,0,5,9,0,5,9,0,0,0,0};
    String timerString = "     0:00:00    ";
    firstLine();
    appendWord (timerString);
    secondLine();
    appendWord ("    Set timer   ");
    showCursor();
    uint8_t currentCursorPosition = 11;
    uint8_t minCursorPosition = 5;
    uint8_t maxCursorPosition = currentCursorPosition;
    for(uint8_t i=0; i<currentCursorPosition; ++i) shiftCursorToTheRight();
    do
    {
        if(buttonRight.isPressed())
        {
            if(currentCursorPosition != maxCursorPosition)
            {
                ++currentCursorPosition;
                shiftCursorToTheRight();
                if(timerString[currentCursorPosition] == ':')
                {
                    shiftCursorToTheRight();
                    ++currentCursorPosition;
                }
                // delay(1000);
            }
        }
        if(buttonLeft.isPressed())
        {
            if(currentCursorPosition != minCursorPosition)
            {
                --currentCursorPosition;
                shiftCursorToTheLeft();
                if(timerString[currentCursorPosition] == ':')
                {
                    shiftCursorToTheLeft();
                    --currentCursorPosition;
                }
                // delay(1000);
            }
        }
        if(buttonUp.isPressed())
        {
            uint8_t time = atoi(String(timerString[currentCursorPosition]).c_str());
            ++time;
            if(time > maxVector[currentCursorPosition]) time = 0;
            timerString[currentCursorPosition] = charVector[time];
            // appendWord (String(timerString[currentCursorPosition]));
            // shiftCursorToTheLeft();
            firstLine();
            appendWord(timerString);
            firstLine();
            for(uint8_t i=0; i<currentCursorPosition; ++i) shiftCursorToTheRight();
            // delay(1000);
        }
        if(buttonDown.isPressed())
        {
            uint8_t time = atoi(String(timerString[currentCursorPosition]).c_str());
            if(time == 0) time = maxVector[currentCursorPosition];
            else --time;
            timerString[currentCursorPosition] = charVector[time];
            // appendWord (String(timerString[currentCursorPosition]));
            // shiftCursorToTheLeft();
            firstLine();
            appendWord(timerString);
            firstLine();
            for(uint8_t i=0; i<currentCursorPosition; ++i) shiftCursorToTheRight();
            // delay(1000);
        }

    } while (!buttonEnter.isPressed());
    
    hideCursor();

    secondLine();
    appendWord (" White    Black ");
    uint32_t timerInSeconds = atoi(String(timerString[11]).c_str()) + atoi(String(timerString[10]).c_str()) * 10 + atoi(String(timerString[8]).c_str()) * 60 + atoi(String(timerString[7]).c_str()) * 600 + atoi(String(timerString[5]).c_str()) * 3600;
    return timerInSeconds;
}

void LiquidCrystalDisplay::shiftCursorToTheRight()
{
    // write8bitValue(0x06);
    write8bitValue(0x14);
}

void LiquidCrystalDisplay::shiftCursorToTheLeft()
{
    // write8bitValue(0x04);
    write8bitValue(0x10);
}




#endif
