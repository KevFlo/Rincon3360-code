#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <pthread.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <pthread.h>


std::vector<std::string> binaryMessagePerBitLen(std::string message, int bitLen){
        std::vector<std::string> truncatedMessage;
        while (message.length()>0){
            std::string bin = message.substr(0, bitLen);
            truncatedMessage.push_back(bin);
            message = message.substr(bitLen);
        }
    return truncatedMessage;
}

int main(int argc, char *argv[]){
    std::vector<std::string> decompressArgs;
    std::string compressedMessage;
    int numberOfbits;
    std::getline(std::cin,compressedMessage);
    
    //get the number of bits from the server

    
    
    
    
    
    
    decompressArgs = binaryMessagePerBitLen(compressedMessage, numberOfbits);

}