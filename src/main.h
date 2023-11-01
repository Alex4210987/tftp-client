//
// Created by Alex on 2023/10/31.
//
#include "logger.h"
#include "client.h"

#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h> // For Winsock 2, you should include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctime>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib to avoid linking errors
using namespace std;



extern const char* Log_File;
WSADATA wsaData;  // Used for Winsock initialization

class mainProgram{
public:
    mainProgram(const char* log_file);
    ~mainProgram();
    void setServerAddr();
    void ui();
    void runRead(int type, char* filename);
    void runWrite(int type, char* filename);
    void close();
    void init();
private:
    logger* l;
    client* c;
    struct sockaddr_in server_addr; // Define the server address structure
};

