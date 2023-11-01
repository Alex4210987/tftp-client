//
// Created by Alex on 2023/10/31.
//

#include "logger.h"

void logger::userOperationLog(const char *operation, char *filename, int mode) {
    log << "User operation: " << operation << " " << filename << " " << mode << " " << this->getTime() << std::endl;
    std::cout<<"User operation: "<<operation<<" "<<filename<<" "<<mode<<" "<<this->getTime()<<std::endl;
    log.close();
}

void logger::successLog(char *operation, char *filename, char *mode) {
    log << "Success: " << operation << " " << filename << " " << mode << " " << this->getTime() << std::endl;
    std::cout<<"Success: "<<operation<<" "<<filename<<" "<<mode<<" "<<this->getTime()<<std::endl;
    log.close();
}

void logger::failLog(char *operation, char *filename, char *mode){
    log << "Fail: " << operation << " " << filename << " " << mode << " " << this->getTime() << std::endl;
    std::cout<<"Fail: "<<operation<<" "<<filename<<" "<<mode<<" "<<this->getTime()<<std::endl;
    log.close();
}

void logger::errorLog(const char *operation, char *filename, int errorCode, char *errorMessage) {
    log << "Error: " << operation << " " << filename << " " << this->getTime() << std::endl;
    switch (errorCode) {
        case 0:
            //delete first 4 char of errorMessage
            errorMessage += 4;
            log << "Error code: 0\tError message: " << errorMessage << std::endl;
            std::cout<< "Error code: 0\tError message: " << errorMessage << std::endl;
            break;
        case 1:
            log << "Error code: 1\tError message: File not found" << std::endl;
            std::cout<< "Error code: 1\tError message: File not found" << std::endl;
            break;
        case 2:
            log << "Error code: 2\tError message: Access violation" << std::endl;
            std::cout<< "Error code: 2\tError message: Access violation" << std::endl;
        case 3:
            log << "Error code: 3\tError message: Disk full or allocation exceeded" << std::endl;
            std::cout<< "Error code: 3\tError message: Disk full or allocation exceeded" << std::endl;
            break;
        case 4:
            log << "Error code: 4\tError message: Illegal TFTP operation" << std::endl;
            std::cout<< "Error code: 4\tError message: Illegal TFTP operation" << std::endl;
            break;
        case 5:
            log << "Error code: 5\tError message: ID Unknown transfer ID" << std::endl;
            std::cout<< "Error code: 5\tError message: ID Unknown transfer ID" << std::endl;
            break;
        case 6:
            log << "Error code: 6\tError message: File already exists" << std::endl;
            std::cout<< "Error code: 6\tError message: File already exists" << std::endl;
            break;
        case 7:
            log << "Error code: 7\tError message: No such user" << std::endl;
            std::cout<< "Error code: 7\tError message: No such user" << std::endl;
            break;
    }
    log.close();
}

char *logger::getTime() {
    time_t now = time(0);
    char *dt = ctime(&now);
    return dt;
}

void logger::retransmissionLog() {
    log << "Retransmission: " << this->getTime() << std::endl;
    std::cout<<"Retransmission: "<<std::endl;
    log.close();
}

void logger::getAckLog(int blockNum) {
    log << "Get ACK: " << blockNum << " " << this->getTime() << std::endl;
    std::cout<<"Get ACK: "<<blockNum<<std::endl;
    log.close();
}

void logger::sentDataLog(int blockNum) {
    log << "Sent Data: " << blockNum << " " << this->getTime() << std::endl;
    std::cout<<"Sent Data: "<<blockNum<<std::endl;
    log.close();
}

void logger::getDatLog(int blockNum) {
    log << "Get Data: " << blockNum << " " << this->getTime() << std::endl;
    std::cout<<"Get Data: "<<blockNum<<std::endl;
    log.close();
}

void logger::showSpeed(int amount, int time) {
    log << "Speed: " << amount*1000.0 / time*1024.0 << "KB/s" << this->getTime() << std::endl;
    std::cout<<"Speed: "<<amount*1000.0 / time*1024.0<<" KB/s"<<std::endl;
    log.close();
}

logger::logger(const char *filename) {
    log.open(filename, std::ios::app);
    log << "File: " << filename << " " << this->getTime() << std::endl;
    std::cout<<"File: "<<filename<<std::endl;
    log.close();
}

logger::~logger() {
    log << "End " << this->getTime() << std::endl;
    std::cout<<"End "<<std::endl;
    log.close();
}

void logger::anyLog(const char *message) {
    log << message << std::endl;
    std::cout<<message<<std::endl;
    log.close();
}