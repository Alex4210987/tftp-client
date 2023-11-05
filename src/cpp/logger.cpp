//
// Created by Alex on 2023/10/31.
//

#include "logger.h"

void logger::userOperationLog(const char *operation, char *filename, int mode) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "User operation: " << operation << " " << filename << " " << mode << " " << this->getTime() << std::endl;
    std::cout<<"User operation: "<<operation<<" "<<filename<<" "<<mode<<" "<<this->getTime()<<std::endl;
    fprintf(pipe, "User operation: %s %s %d %s\n", operation, filename, mode, this->getTime());
    log.close();
    _pclose(pipe);
}

void logger::successLog(char *operation, char *filename, char *mode) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "Success: " << operation << " " << filename << " " << mode << " " << this->getTime() << std::endl;
    std::cout<<"Success: "<<operation<<" "<<filename<<" "<<mode<<" "<<this->getTime()<<std::endl;
    fprintf(pipe, "Success: %s %s %s %s\n", operation, filename, mode, this->getTime());
    log.close();
    _pclose(pipe);
}

void logger::failLog(char *operation, char *filename, char *mode){
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "Fail: " << operation << " " << filename << " " << mode << " " << this->getTime() << std::endl;
    std::cout<<"Fail: "<<operation<<" "<<filename<<" "<<mode<<" "<<this->getTime()<<std::endl;
    fprintf(pipe, "Fail: %s %s %s %s\n", operation, filename, mode, this->getTime());
    log.close();
    _pclose(pipe);
}

void logger::errorLog(const char *operation, char *filename, int errorCode, char *errorMessage) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
     switch (errorCode) {
        case 0:
            //delete first 4 char of errorMessage
            errorMessage += 4;
            fprintf(pipe, "Error code: 0\tError message: %s\n", errorMessage);
            break;
        case 1:
            fprintf(pipe, "Error code: 1\tError message: File not found\n");
            break;
        case 2:
            fprintf(pipe, "Error code: 2\tError message: Access violation\n");
            break;
        case 3:
            fprintf(pipe, "Error code: 3\tError message: Disk full or allocation exceeded\n");
            break;
        case 4:
            fprintf(pipe, "Error code: 4\tError message: Illegal TFTP operation\n");
            break;
        case 5:
            fprintf(pipe, "Error code: 5\tError message: ID Unknown transfer ID\n");
            break;
        case 6:
            fprintf(pipe, "Error code: 6\tError message: File already exists\n");
            break;
        case 7:
            fprintf(pipe, "Error code: 7\tError message: No such user\n");
            break;
    }
    _pclose(pipe);
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
            break;
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
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "Retransmission: " << this->getTime() << std::endl;
    std::cout<<"Retransmission: "<<std::endl;
    fprintf(pipe, "Retransmission: %s\n", this->getTime());
    log.close();
    _pclose(pipe);
}

void logger::getAckLog(int blockNum, char *filename) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }

    fprintf(pipe, "Get ACK: %d %s %s\n", blockNum, filename, this->getTime());

    _pclose(pipe);
    log << "Get ACK: " << blockNum << " " << this->getTime() << std::endl;
    std::cout<<"Get ACK: "<<blockNum<<std::endl;
    log.close();
}

void logger::sentDataLog(int blockNum) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "Sent Data: " << blockNum << " " << this->getTime() << std::endl;
    std::cout<<"Sent Data: "<<blockNum<<std::endl;
    fprintf(pipe, "Sent Data: %d %s\n", blockNum, this->getTime());
    log.close();
    _pclose(pipe);
}

void logger::getDatLog(int blockNum) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "Get Data: " << blockNum << " " << this->getTime() << std::endl;
    std::cout<<"Get Data: "<<blockNum<<std::endl;
    fprintf(pipe, "Get Data: %d %s\n", blockNum, this->getTime());
    log.close();
    _pclose(pipe);
}

void logger::showSpeed(int amount, int time) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "Speed: " << amount*1000.0 / time*1024.0 << "KB/s" << this->getTime() << std::endl;
    std::cout<<"Speed: "<<amount*1000.0 / time*1024.0<<" KB/s"<<std::endl;
    fprintf(pipe, "Speed: %f KB/s %s\n", amount*1000.0 / time*1024.0, this->getTime());
    log.close();
    _pclose(pipe);
}

logger::logger(const char *filename) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log.open(filename, std::ios::app);
    log<<"Start "<<this->getTime()<<std::endl;
    std::cout<<"Start "<<std::endl;
    fprintf(pipe, "Start %s\n", this->getTime());
    _pclose(pipe);
    log.close();
}

logger::~logger() {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    log << "End " << this->getTime() << std::endl;
    std::cout<<"End "<<std::endl;
    fprintf(pipe, "End %s\n", this->getTime());
    log.close();
    _pclose(pipe);
}

void logger::anyLog(const char *message) {
    pipe = _popen("curl -X POST -d @- http://127.0.0.1:8081/log", "wt");
    if (pipe == nullptr) {
        std::cerr << "Error opening pipe: " << strerror(errno) << std::endl;
        exit(1);
    }
    std::cout<<this->getTime()<<" "<<message<<std::endl;
    fprintf(pipe, "%s %s\n", this->getTime(), message);
    _pclose(pipe);
    log << this->getTime() << " " << message << std::endl;
    log.close();
}