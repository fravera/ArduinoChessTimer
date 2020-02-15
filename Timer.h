#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
    Timer(uint32_t startValue); //seconds
    ~Timer();

    void setTimer(uint32_t startValue); //seconds
    void updateTimer();
    String getTimerString(bool left);
    uint32_t getTimerValue();
    bool isTimeOver();

private:
    uint32_t timerValue_ {0}; //Tens of ms
    uint8_t  fineTimer_  {0};
    char charVector[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
};

Timer::Timer(uint32_t startValue) 
: timerValue_(startValue*10) 
{
}

Timer::~Timer()
{
}

void Timer::setTimer(uint32_t timerValue)
{
    timerValue_ = timerValue * 10;
}

void Timer::updateTimer()
{
    ++fineTimer_;
    if(fineTimer_ == 98)
    {
        fineTimer_ = 0;
        if(timerValue_>0) --timerValue_;
    }
}

bool Timer::isTimeOver()
{
    return timerValue_==0;
}


uint32_t Timer::getTimerValue()
{
    return timerValue_;
}

String Timer::getTimerString(bool left)
{
    char buffer[7];
    uint32_t timerValueCopy = timerValue_;
    if(timerValueCopy>600)
    {
        uint32_t timerInSeconds = timerValueCopy / 10;
        uint8_t  seconds = timerInSeconds % 60;
        uint8_t  minutes = (timerInSeconds / 60) % 60;
        uint8_t  hours   = (timerInSeconds / 3600);
        buffer[0] = charVector[hours];
        buffer[1] = ':';
        buffer[2] = charVector[minutes/10];
        buffer[3] = charVector[minutes%10];
        buffer[4] = ':';
        buffer[5] = charVector[seconds/10];
        buffer[6] = charVector[seconds%10];
    }
    else if(timerValueCopy>0)
    {
        buffer[0] = ' ';
        buffer[1] = ' ';
        buffer[2] = charVector[timerValueCopy/100];
        buffer[3] = charVector[(timerValueCopy/10)%10];
        buffer[4] = '.';
        buffer[5] = charVector[timerValueCopy%10];
        buffer[6] = ' ';
    }
    else
    {
        return "TIME UP";
    }
    
    String timerValue = buffer;
    return timerValue;
}

#endif