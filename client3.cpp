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
    int port;
    std::string pattern, server;
    std::string pattenr_location;
    struct hostent *hostname;
};

/// pointerfunction to be used in pthread
struct hostent *server;

void *decompressMsg (void *foo_ptr){
    struct foo *value = (struct foo *)foo_ptr;
    int sockfd;
    struct sockaddr_in serv_addr;

    int port = value->port;

    
    if (value->hostname == NULL)
        std::cout << "ERR - No hostname found" << std::endl;
    
    // open socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERR - Can't open socket" << std::endl;
    
    // connection data
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bcopy((char *)value->hostname->h_addr, (char *)&serv_addr.sin_addr.s_addr, value->hostname->h_length);

    // connect to server
    int socket = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (socket < 0)
        std::cout << "ERR - Can't connect" << std::endl;

    // Send the pattern message to the server

    char buffer[256];
    bzero(buffer, 256);
    strcpy(buffer, value->pattern.c_str());
    int w = write(sockfd, buffer, 256);
    if (w < 0)
        std::cout << "ERR - Can't send data via socket" << std::endl;
    bzero(buffer, 256);

    // Receive the position vector from the server
    int r = read(sockfd, buffer, 256);
    if (r < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;
    value->pattenr_location = buffer;
    bzero(buffer, 256);

    // close socket
    close(sockfd);

    return NULL;
}


int main(int argc, char *argv[]){
    int port;
    struct hostent *hostname;

    std::vector<std::string> patterns;
    std::string pattern;
    int numberOfbits;

    port = atoi(argv[2]);
    hostname = gethostbyname(argv[1]);
    //this will keep getting the patterns from clinet input file and stores them in a vector
    while (std::cin >> pattern){
        patterns.push_back(pattern);
    }

    //from Blackboard thanks Dr.Rincon <3
    ///                               this is connecting to the server to get the bit lenght then disconnecting to 
    ///                               use the bit lenght to get the binary message and then use the binary message truncaktion
    ///                               to pass into the threaded function which then starts a new connection to the server
    ///                               and sends the binary message to the server and then receives the decompressed message from the server
    // open socket
    int sockfd, protno, n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[256];
    struct sockaddr_in serv_addr;

    
    if (sockfd < 0)
        std::cout << "ERR - Can't open socket" << std::endl;

    // connection data
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)hostname->h_addr,(char *)&serv_addr.sin_addr.s_addr,
    hostname->h_length);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    // connect to server
    int socket = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (socket < 0)
        std::cout << "ERR - Can't connect" << std::endl;


    bzero(buffer,256);
    int r = read(sockfd, buffer, sizeof(256));
    if (r < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;


    // setting up the POSTIX thread variables
    int THREADS = patterns.size();
    pthread_t tid[THREADS];
    struct foo pattern_holder[THREADS];

    // for loop to cout everything in decompressArgs
    for (int i = 0; i < THREADS; i++){
        pattern_holder[i].pattern = patterns[i];
        pattern_holder[i].port = port;
        pattern_holder[i].hostname = hostname;
    }
    //Pthreads go here
    for (int i = 0; i < THREADS; i++){
            //Will create the threads needed to decompress the message
            if (pthread_create(&tid[i], NULL, decompressMsg, (void *) &pattern_holder[i])) {
                std::cout << "Error:unable to create thread," << std::endl;
                return 1;
        }   
   }
   //join the threads
   for (int j = 0; j < THREADS; j++){
       pthread_join(tid[j], NULL);
   }
    //print the decompressed message
   std::string message;
   for (int i = 0 ; i < THREADS; i++){
       message += pattern_holder[i].decompressed;
   }
    std::cout << "Decompressed message: " << message << std::endl;
    return 0;
}