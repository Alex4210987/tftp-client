//
// Created by Alex on 2023/10/31.
//
#ifndef main_h
#define main_h

#include "logger.h"
#include "client.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h> // For Winsock 2, you should include <winsock2.h>
#include <iostream>
#include <cstring>
#include <ctime>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib to avoid linking errors
using namespace std;

extern const char* Log_File;

class mainProgram{
public:
    mainProgram(const char* log_file);
    ~mainProgram();
    void setServerAddr();
    int runRead(int type, char* filename);
    int runWrite(int type, char* filename);
    void close();
    void init();
private:
    logger* l;
    client* c;
    struct sockaddr_in server_addr; // Define the server address structure
};

#endif




