//
// Created by Alex on 2023/10/31.
//

#include "myTimer.h"

timer::timer() {
    isRunning = false;
}

timer::~timer() {
    // 析构函数
}

void timer::start() {
    if (!isRunning) {
        startTime = clock();
        isRunning = true;
    }
}

void timer::stop() {
    if (isRunning) {
        endTime = clock();
        isRunning = false;
    }
}

int timer::getElapsedTime() {
    if (isRunning) {
        stop();
    }

    return static_cast<int>(((endTime - startTime) * 1000) / CLOCKS_PER_SEC);
}
