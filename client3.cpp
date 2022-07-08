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
    std::string output_s;
    std::string found;
    std::string notfound;
    struct hostent *hostname;
};

/// pointerfunction to be used in pthread
struct hostent *server;

void *patternMatching (void *foo_ptr){
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
    // args->subMsg.copy(buffer, sizeof(args->subMsg.length()));
    value->pattern.copy(buffer, sizeof(value->pattern.length()));
    int w = write(sockfd, buffer, 256);
    if (w < 0)
        std::cout << "ERR - Can't send data via socket" << std::endl;
    
    
    char buffer2[256];
    bzero(buffer, 256);

    // Receive the position vector from the server
    int r = read(sockfd, buffer, 256);
    if (r < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;
    value->found = buffer;
    bzero(buffer, 256);

    // Receive the position vector from the server
    int k = read(sockfd, buffer, 256);
    if (k < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;
    value->notfound = buffer;
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
    
    std::string line;
    while (std::getline(std::cin, line))
    {
        patterns.push_back(line);
    }
    //from Blackboard thanks Dr.Rincon <3
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

    // setting up the POSTIX thread variables
    int THREADS = patterns.size();
    pthread_t *tid = new pthread_t[THREADS];
    foo *pattern_holder = new foo[THREADS];

    // for loop to populate the thres structs with the correct data
    for (int i = 0; i < THREADS; i++){
        pattern_holder[i].pattern = patterns[i];
        pattern_holder[i].port = port;
        pattern_holder[i].hostname = hostname;
    }
    //Pthreads go here
    for (int i = 0; i < THREADS; i++){
            //Will create the threads needed to decompress the message
            if (pthread_create(&tid[i], NULL, patternMatching, (void *) &pattern_holder[i])) {
                std::cout << "Error:unable to create thread," << std::endl;
                return 1;
        }   
   }
    //Joining the threads
    for (int i = 0; i < THREADS; i++){
        pthread_join(tid[i], NULL);
    }
    //Closing the socket
    close(sockfd);
    

    //Printing found and not found strings
    std::cout << "SEARCH RESULTS:" << "\n" << std::endl;

    //for each pattern in patterns vector, print the found and not found strings
    for (int i = 0; i < THREADS; i++){
        std::cout << "Pattern: " << pattern_holder[i].pattern << "\n" << std::endl;
        std::cout << "Found: " << pattern_holder[i].found << "\n" << std::endl;
        std::cout << "Not Found: " << pattern_holder[i].notfound << "\n" << std::endl;
    }
    // return 0;


    // for (int i = 0; i < THREADS; i++){
    //     std::cout << pattern_holder[i].found  << std::endl;
    // }
    // for (int i = 0; i < patterns.size(); i++){
    //     if (pattern_holder[i].found.find(patterns[i]) != std::string::npos) {
    //         std::cout << pattern_holder[i].found << std::endl;
    //     }
    //     std::cout << "\n" << std::endl;

        
    //     std::cout << pattern_holder[i].notfound << std::endl;
    // }


    return 0;
}
