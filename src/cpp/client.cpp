//
// Created by Alex on 2023/10/31.
//

#include <sstream>
#include "client.h"

client::client(logger *outl) {
    l = outl;
    requestBuffer = new char[BUFFER_SIZE];
    dataBuffer = new char[BUFFER_SIZE];
    t = new timer();
}

client::~client() {
    delete l;
    delete requestBuffer;
    delete dataBuffer;
    delete t;
}

int client::makeRequest(const char* mode, char* filename, int type){
    char fullFileName[File_Name_Size];
    int byteCount = 0;
    memset(fullFileName, 0, sizeof(fullFileName));
    memset(this->requestBuffer, 0, sizeof(this->requestBuffer));
    requestBuffer[0] = 0;
    if(strcmp(mode, "read")==0){
        strcpy(fullFileName, Read_File_Path);
        requestBuffer[1] = 1;   // Set Opcode to 1
    }else{
        strcpy(fullFileName, Write_File_Path);
        requestBuffer[1] = 2;   // Set Opcode to 2
    }
    strcat(fullFileName, filename);
    byteCount += 2;
    memcpy(requestBuffer + byteCount, filename, strlen(filename));
    byteCount += strlen(filename);
    requestBuffer[byteCount] = 0; // Append 0 to indicate the end of the filename
    byteCount++;
    if(type==1){
        strcpy(requestBuffer + byteCount, "netascii");
        byteCount += strlen("netascii") + 1;
    }else{
        strcpy(requestBuffer + byteCount, "octet");
        byteCount += strlen("octet") + 1;
    }
    FILE* fp;
    fp = fopen(fullFileName, "r");
    std::cout<<"fullFileName: "<<fullFileName<<std::endl;
    if(fp==NULL&&strcmp(mode, "write")==0){
        return -1;
    }
    if(strcmp(mode, "read")==0){
        if(type==1){
            fp = fopen(fullFileName, "w+b");//w+b means write and read
        }else{
            fp = fopen(fullFileName, "wb");//wb means write binary
        }
    }else{
        if(type==1){
            fp = fopen(fullFileName, "r+b");//r+b means read and write
        }else{
            fp = fopen(fullFileName, "rb");//rb means read binary
        }
    }

    return byteCount;
}

void client::readRequest(char* request, int type, int size, char* filename) {
    struct sockaddr_in clientAddr;
    struct sockaddr_in serverAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(0);
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        l->anyLog("Error: socket() failed");
        WSACleanup();
        return;
    }

    char* filePath = new char[File_Name_Size];
    memset(filePath, 0, sizeof(filePath));
    strcpy(filePath, Read_File_Path);
    strcat(filePath, filename);
    std::cout<<"filePath: "<<filePath<<std::endl;
    FILE* fp;
    if (type == 1) {
        fp = fopen(filePath, "w+b");  // w+b means write and read
    } else {
        fp = fopen(filePath, "wb");  // wb means write binary
    }

    // set non-blocking
    unsigned long ul = 1;
    ioctlsocket(clientSocket, FIONBIO, &ul);

    // bind
    if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
        l->anyLog("Error: bind() failed");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    long long int totalSend = 0;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int byteCount = size;
    t->start();

    if (sendto(clientSocket, request, byteCount, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        l->anyLog("Error: sendto() failed");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // Receive and process data
    long long int totalReceived = 0;
    int bytesReceived;
    int blockNum; // Initial block number
    int retransmissionCount = 0;
    int addrLen = sizeof(serverAddr);

    while (true) {
        // Receive data
        memset(buffer, 0, sizeof(buffer));
        bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, &addrLen);
        if (buffer[1] == 5) {
            retransmissionCount = 0;
            l->errorLog("Read", filename, buffer[3], buffer);
            break;
        }
        if (buffer[1] == 3){
            //write and ack
            blockNum=(((buffer[2] & 0xff) << 8) + (buffer[3] & 0xff)) & 0xffff;
            if(bytesReceived<0){
                fwrite(buffer+4, 1, 512, fp);
                totalReceived += 512;
            }else{
                fwrite(buffer+4, 1, bytesReceived-4, fp);
                totalReceived += bytesReceived;
            }
            //l->anyLog("Received data packet%d", blockNum,"for", filename);
            sendAck(blockNum, clientSocket, serverAddr);
                l->getAckLog(blockNum, filename);
            retransmissionCount = 0;
            if (bytesReceived < 516&&bytesReceived>4) {
                break;
            }
        }
        retransmissionCount++;
        Sleep(Time_Out);
        if(retransmissionCount==Max_Try){
            sendError(clientSocket, serverAddr);
            return;
        }else{
            continue;
        }
    }
    l->anyLog("Read operation completed successfully");
    t->stop();
    long long int elapsedTime = t->getElapsedTime();
    char logMessage[256];
    //calculate speed in kpB/s
    long double speed = (long double)totalReceived*1000.0 / (elapsedTime*1024.0);
    sprintf(logMessage, "Sent %d bytes in %lld milliseconds, speed: %Lf kB/s", totalReceived, elapsedTime, speed);
    l->anyLog(logMessage);
    closesocket(clientSocket);
    WSACleanup();
}

void client::writeRequest(char* request, int type, int size, char* filename) {
    struct sockaddr_in clientAddr;
    struct sockaddr_in serverAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(0);
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        l->anyLog("Error: socket() failed");
        WSACleanup();
        return;
    }

    // 设置非阻塞
    unsigned long ul = 1;
    ioctlsocket(clientSocket, FIONBIO, &ul);

    // 绑定
    if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
        l->anyLog("Error: bind() failed");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    char* filePath = new char[File_Name_Size];
    memset(filePath, 0, sizeof(filePath));
    strcpy(filePath, Write_File_Path);
    strcat(filePath, filename);
    std::cout<<"filePath: "<<filePath<<std::endl;
    FILE* fp;
    if (type == 1) {
        fp = fopen(filePath, "r+b");  // r+b means read and write
    } else {
        fp = fopen(filePath, "rb");  // rb means read binary
    }

    //if cannot open file
    if(fp==NULL){
        l->anyLog("Error: cannot open file");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    //calculate the size of file
    fseek(fp, 0, SEEK_END);
    int dataSize = ftell(fp);

    // 发送请求 and read ack
    int byteCount = size;
    t->start();
    if (sendto(clientSocket, request, byteCount, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        l->anyLog("Error: sendto() failed");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    int blockNum = 1;
    int totalSent = 0;
    int bytesSent;
    int ackBlockNum = 0;
    int addrLen = sizeof(serverAddr);
    int retransmissionCount = 0;

    while (totalSent < dataSize) {
        int bytesToSend = std::min(dataSize - totalSent, BUFFER_SIZE); // 每个数据包最大为BUFFER_SIZE
        char dataPacket[BUFFER_SIZE + 4]; // 数据包大小包括操作码、块号和数据
        dataPacket[0] = 0x00; // Opcode for DATA (0x03)
        dataPacket[1] = 0x03;
        dataPacket[2] = (blockNum >> 8) & 0xFF;
        dataPacket[3] = blockNum & 0xFF;
        // 读取文件数据
        fseek(fp, totalSent, SEEK_SET);
        fread(dataPacket + 4, 1, bytesToSend, fp);
        bytesSent = sendto(clientSocket, dataPacket, bytesToSend + 4, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent == SOCKET_ERROR) {
            l->anyLog("Error: sendto() failed");
            closesocket(clientSocket);
            WSACleanup();
            return;
        }

        // 接收ACK确认
        char ackPacket[4]; // ACK包大小为4
        memset(ackPacket, 0, sizeof(ackPacket));
        while (true) {
            int bytesReceived = recvfrom(clientSocket, ackPacket, sizeof(ackPacket), 0, (struct sockaddr*)&serverAddr, &addrLen);
            if (bytesReceived == SOCKET_ERROR) {
                if (retransmissionCount < Max_Try) {
                    // 重传数据包
                    sendto(clientSocket, dataPacket, bytesToSend + 4, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
                    retransmissionCount++;
                    Sleep(Time_Out);
                } else {
                    l->anyLog("Error: Max_Try reached");
                    closesocket(clientSocket);
                    WSACleanup();
                    return;
                }
            } else {
                int receivedBlockNum = (((ackPacket[2] & 0xff) << 8) + (ackPacket[3] & 0xff)) & 0xffff;
                if (ackPacket[1] == 4 && receivedBlockNum == blockNum) {
                    // 成功收到ACK，增加块号并发送下一个数据包
                    retransmissionCount = 0;
                    ackBlockNum = receivedBlockNum;
                    blockNum++;
                    totalSent += bytesToSend;
                    l->getAckLog(ackBlockNum, filename);
                    break;
                } else {
                    // 收到无效ACK，需要重传数据包
                    sendto(clientSocket, dataPacket, bytesToSend + 4, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
                    retransmissionCount++;
                    Sleep(Time_Out);
                }
            }
        }
    }

    l->anyLog("Write operation completed successfully");
    t->stop();
    long long int elapsedTime = t->getElapsedTime();
    char logMessage[256];
    //calculate speed in kpB/s
    long double speed = (long double)totalSent*1000.0 / (elapsedTime*1024.0);
    sprintf(logMessage, "Sent %d bytes in %lld milliseconds, speed: %Lf kB/s", totalSent, elapsedTime, speed);
    l->anyLog(logMessage);
    closesocket(clientSocket);
    WSACleanup();
}

void client::sendAck(int blockNum, SOCKET clientSocket, struct sockaddr_in serverAddr) {
    char ack[4];
    ack[0] = 0;
    ack[1] = 4;
    ack[2] = blockNum / 256;
    ack[3] = blockNum % 256;
    //send
    if(sendto(clientSocket, ack, 4, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr))==SOCKET_ERROR){
        l->anyLog("Error: sendto() failed");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
}

void client::sendError(SOCKET clientSocket, struct sockaddr_in serverAddr){
    char error[512];
    error[0] = 0;
    error[1] = 5;
    //send
    if(sendto(clientSocket, error, 512, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr))==SOCKET_ERROR){
        l->anyLog("Error: sendto() failed");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
    l->anyLog("Send error to server");
}