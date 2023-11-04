//
// Created by Alex on 2023/10/31.
//
#include<iostream>
#include<fstream>
#include<ctime>

#pragma once

class logger {
public:
    void userOperationLog(const char* operation, char* filename, int mode);
    void successLog(char* operation, char* filename, char* mode);
    void failLog(char* operation, char* filename, char* mode);
    void errorLog(const char* operation, char* filename, int errorCode, char* errorMessage);
    void getAckLog(int blockNum);
    void sentDataLog(int blockNum);
    void getDatLog(int blockNum);
    void retransmissionLog();
    void showSpeed(int amount, int time);
    void anyLog(const char *message);
    //get current time
    char* getTime();
    logger(const char* filename);
    ~logger();
private:
    std::ofstream log;
};
