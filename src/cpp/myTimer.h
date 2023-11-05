//
// Created by Alex on 2023/10/31.
//
#include <ctime>

class timer {
private:
    clock_t startTime;
    clock_t endTime;
    bool isRunning;

public:
    timer();
    ~timer();

    void start();
    void stop();
    int getElapsedTime();
};

