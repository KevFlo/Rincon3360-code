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

struct foo
{
    std::string binary, decompressed, val;
};

int port;
std::string hostname;

/// pointerfunction to be used in pthread

void *decompressMsg (void *foo_ptr){
    struct foo *value = (struct foo *)foo_ptr;
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    server = gethostbyname(hostname.c_str());
    if (server == NULL)
        std::cout << "ERR - No hostname found" << std::endl;
    
    // open socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERR - Can't open socket" << std::endl;
    
    // connection data
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // connect to server
    int socket = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (socket < 0)
        std::cout << "ERR - Can't connect" << std::endl;

    // Send the truncated message to the server

    char buffer[1024];
    bzero(buffer, 1024);
    strcpy(buffer, value->val.c_str());
    int w = write(sockfd, buffer, 1024);
    if (w < 0)
        std::cout << "ERR - Can't send data via socket" << std::endl;
    bzero(buffer, 1024);

    // Receive the decompressed message from the server
    int r;

    r = read(sockfd, buffer, 1024);
    if (r < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;
    value->decompressed = buffer;
    bzero(buffer, 1024);

    return NULL;
}

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

    port = atoi(argv[2]);
    hostname = argv[1];
    
    std::getline(std::cin,compressedMessage);

    //from Blackboard thanks Dr.Rincon <3
    int sockfd, protno, n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[1024];
    struct sockaddr_in serv_addr;

    // open socket
    if (sockfd < 0)
        std::cout << "ERR - Can't open socket" << std::endl;

    // connection data
    bzero((char *)&serv_addr, sizeof(serv_addr));
    port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    // connect to server
    int socket = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (socket < 0)
        std::cout << "ERR - Can't connect" << std::endl;

    // get number of symbols from socket
    int numberofSymbols;

    bzero(buffer,1024);
    int r = read(sockfd, buffer, sizeof(1024));
    if (r < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;
    numberOfbits = atol(buffer);
    bzero(buffer,1024);

    int rc;
    rc = close(sockfd);
    
    decompressArgs = binaryMessagePerBitLen(compressedMessage, numberOfbits);

    int THREADS = decompressArgs.size();
    pthread_t tid[THREADS];
    struct foo binparts[THREADS];

    // for loop to cout everything in decompressArgs
    for (int i = 0; i < THREADS; i++){
        binparts[i].val = decompressArgs[i];
    }
    //Pthreads go here
    for (int i = 0; i < THREADS; i++){
            //Will create the threads needed to decompress the message
            if (pthread_create(&tid[i], NULL, decompressMsg, (void *) &binparts[i])) {
                std::cout << "Error:unable to create thread," << std::endl;
                return 1;
        }   
   }
   //join the threads
   for (int j = 0; j < decompressArgs.size(); j++){
       pthread_join(tid[j], NULL);
   }
   rc = close(sockfd);
   for (int i = 0 ; i < THREADS; i++){
       std::cout << binparts[i].decompressed << std::endl;
   }
    return 0;
}