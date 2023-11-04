//
// Created by Alex on 2023/10/31.
//
#include <winsock.h> // For Winsock 2, you should include <winsock2.h>
#include <iostream>
#include <string>
#include <process.h>

//file
#include <fstream>
#include "logger.h"
#include "myTimer.h"
#include <winsock.h>
//#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib to avoid linking errors


#define Time_Out 30      // Set the timeout for retransmission (in microseconds)
#define Max_Try  10

extern const char* Read_File_Path;
extern const char* Write_File_Path;

#define SERVER_PORT 69   // Define the initial port for client connections (pseudo-connection)
#define SERVER_ADDR "127.0.0.1"   // Define the server address

#define File_Name_Size  100   // Set the maximum file name length
#define BUFFER_SIZE 512   // Set the maximum buffer size

class client {
public:
    client(logger *outl);
    ~client();
    int makeRequest(const char* mode, char* filename, int type);
    void readRequest(char* request, int type, int size, char* filename);
    void writeRequest(char* request, int type, int size, char* filename);
    char* requestBuffer;
    void sendAck(int blockNum, SOCKET clientSocket, struct sockaddr_in serverAddr);
    void sendError(SOCKET clientSocket, struct sockaddr_in serverAddr);

private:
    logger *l;
    char* dataBuffer;
    timer *t;
};

