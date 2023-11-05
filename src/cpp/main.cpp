//
// Created by Alex on 2023/10/31.
//
#include "main.h"

int main(int argc, char* argv[]) {
//    int argc = 4;
//    char* argv[4] = {"./tftp-client.exe", "read", "ascii", "read.txt"};
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <operation> <mode> <fileName>" << endl;
        return 1;
    }

    const char* operation = argv[1];
    const char* mode = argv[2];
    const char* fileName = argv[3];

    mainProgram m(Log_File);

    if(strcmp(operation, "read")==0){
        if(strcmp(mode, "ascii")==0){
            m.runRead(1, (char*)fileName);
            std::cout<<"Read ASCII"<<std::endl;
        }else if(strcmp(mode, "binary")==0){
            m.runRead(2, (char*)fileName);
            std::cout<<"Read Binary"<<std::endl;
        }
    }else if(strcmp(operation, "write")==0){
        if(strcmp(mode, "ascii")==0){
            m.runWrite(1, (char*)fileName);
            std::cout<<"Write ASCII"<<std::endl;
        }else if(strcmp(mode, "binary")==0){
            m.runWrite(2, (char*)fileName);
            std::cout<<"Write Binary"<<std::endl;
        }
    }else{
        cout<<"Invalid input!"<<endl;
    }

    return 0;
}


void mainProgram::setServerAddr() {
    this->server_addr.sin_family = AF_INET;   // Set the address family to IPv4
    this->server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);   // Set the IP address
    this->server_addr.sin_port = htons(SERVER_PORT);   // Set the port number
}

void mainProgram::init() {
        // Initialize Winsock
        WSADATA wsaData;  // Used for Winsock initialization
        int re = WSAStartup(0x0202, &wsaData);
        if (re) {
            // Winsock initialization failed
            printf("Error Code: %d    ", WSAGetLastError());
            printf("Winsock initialization error\n");
            exit(-1);
        }

        system("cls");// Clear the screen
        server_addr.sin_port = htons(SERVER_PORT);
}


int mainProgram::runRead(int type, char* filename) {
    int byteCount = 0;
    byteCount = c->makeRequest("read", filename, type);
    if(byteCount==-1){
        std::cout << "Failed to open the file\n";
        return -1;
        //this->ui();
    }
    c->readRequest(c->requestBuffer, type, byteCount, filename);
    //this->ui();
    return 0;
}

int mainProgram::runWrite(int type, char* filename) {
    int byteCount = 0;
    byteCount = c->makeRequest("write", filename, type);
    if(byteCount==-1){
        std::cout << "Failed to open the file\n";
        return -1;
    }
    c->writeRequest(c->requestBuffer, type, byteCount, filename);
    //this->ui();
    return 0;
}

void mainProgram::close() {
    WSACleanup();
    exit(0);
}

mainProgram::mainProgram(const char* log_file) {
    this->init();
    l = new logger(log_file);
    c = new client(l);
    this->setServerAddr();
}

mainProgram::~mainProgram() {
    this->close();
    delete l;
    delete c;
}