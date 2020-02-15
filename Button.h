#ifndef BUTTON_H_
#define BUTTON_H_

class Button
{
public:
    Button(uint8_t inputPin);
    ~Button();

    bool isPressed();
private:
    uint8_t inputPin_;
    static const uint8_t debounceDelay_ = 10; //ms
    bool previousStatus_ {false};
    bool stateRead_ {false};
    uint8_t timeLastCheck {0};
};

Button::Button(uint8_t inputPin)
: inputPin_(inputPin)
{
    pinMode(inputPin_, INPUT);
}

Button::~Button()
{
}

bool Button::isPressed()
{
    bool inputStatus = (digitalRead(inputPin_) == LOW);

    if(inputStatus != previousStatus_)
    {
        stateRead_ = false;
        previousStatus_ = inputStatus;
        timeLastCheck = millis();
        return false;
    }
    if(stateRead_) return false;
    uint16_t currentTime = millis();
    if(currentTime < timeLastCheck) currentTime + 0xFF;
    if( (currentTime - timeLastCheck) > debounceDelay_)
    {
        stateRead_ = true;
        return inputStatus;
    }

    return false;
}

#endif
