class Switch
{
public:
    Switch(uint8_t inputPin);
    ~Switch();

    bool isWhiteTurn();
    void waitForStart();

private:
    uint8_t inputPin_;
};

Switch::Switch(uint8_t inputPin)
: inputPin_(inputPin)
{
}

Switch::~Switch()
{
}

bool Switch::isWhiteTurn()
{
    return digitalRead(PIN_PLAYER_SWITCH) == LOW;
}

void Switch::waitForStart()
{
    while(digitalRead(PIN_PLAYER_SWITCH) == HIGH) {}
    while(digitalRead(PIN_PLAYER_SWITCH) == LOW)  {}
}




