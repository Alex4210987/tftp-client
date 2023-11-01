//
// Created by Alex on 2023/10/31.
//
#include "main.h"

int main(){
    mainProgram m(Log_File);
    m.setServerAddr();
    while(TRUE){
        m.init();
        m.ui();
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

void mainProgram::ui() {
    int choice, type;
    char filename[File_Name_Size];
    cout<< "Welcome to the TFTP mainProgram!"<<endl;
    cout<<"What do you want to do?"<<endl;
    cout<<"1. Read"<<endl;
    cout<<"2. Write"<<endl;
    cout<<"3. Exit"<<endl;
    cin>> choice;
    cout<<"Which type do you want to use?"<<endl;
    cout<<"1. ASCII"<<endl;
    cout<<"2. Binary"<<endl;
    cin>>type;
    cout<<"Please input the filename:"<<endl;
    cin>>filename;
    switch (choice) {
        case 1:
            this->runRead(type, filename);
            l->userOperationLog("Read", filename, type);
            break;
        case 2:
            this->runWrite(type, filename);
            l->userOperationLog("Write", filename, type);
            break;
        case 3:
            this->close();
            break;
        default:
            cout<<"Invalid input!"<<endl;
            break;
    }
}

void mainProgram::runRead(int type, char* filename) {
    int byteCount = 0;
    byteCount = c->makeRequest("read", filename, type);
    if(byteCount==-1){
        std::cout << "\t\t\t\tFailed to open the file\n";
        this->ui();
    }
    c->readRequest(c->requestBuffer, type, byteCount, filename);
    this->ui();
}

void mainProgram::runWrite(int type, char* filename) {
    int byteCount = 0;
    byteCount = c->makeRequest("write", filename, type);
    if(byteCount==-1){
        std::cout << "\t\t\t\tFailed to open the file\n";
        this->ui();
    }
    c->writeRequest(c->requestBuffer, type, byteCount, filename);
    this->ui();
}

void mainProgram::close() {
    WSACleanup();
    exit(0);
}

mainProgram::mainProgram(const char* log_file) {
    this->init();
    l = new logger(log_file);
    c = new client(l);
}

mainProgram::~mainProgram() {
    this->close();
    delete l;
    delete c;
}